#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <math.h>
#include <WiFi.h>
#include <WebServer.h>
#include <array>
#include <stdint.h>

//Librerias Propias
#include "mpu.h"
#include "web.h"
#include "motores.h"



bool motor1_activo;
bool motor2_activo;
bool motor3_activo;
bool motor4_activo;

int motor1_pwm;
int motor2_pwm;
int motor3_pwm;
int motor4_pwm;

// ============================================================
// SETUP
// ============================================================

void setup() {

  Serial.begin(115200);

  delay(1000);

  Serial.println();
  Serial.println("==============================");
  Serial.println(" MINI DRONE ESP32-C3");
  Serial.println("==============================");

  // ----------------------------------------------------------
  // MOTORES
  // ----------------------------------------------------------

  incializacion_motores(50,false);

  // PWM:
  // GPIO4 -> Motor 1
  // GPIO3 -> Motor 2
  // GPIO2 -> Motor 3
  // GPIO1 -> Motor 4
  // 20 kHz
  // 8 bits


  Serial.println("Motor 1 configurado en GPIO4");
  Serial.println("Motor 2 configurado en GPIO3");
  Serial.println("Motor 3 configurado en GPIO2");
  Serial.println("Motor 4 configurado en GPIO2");


  // ----------------------------------------------------------
  // I2C
  // ----------------------------------------------------------

  Serial.println("Iniciando I2C...");

  Wire.begin(I2C_SDA, I2C_SCL);

  Serial.print("SDA = GPIO");
  Serial.println(I2C_SDA);

  Serial.print("SCL = GPIO");
  Serial.println(I2C_SCL);

  // ----------------------------------------------------------
  // MPU6050
  // ----------------------------------------------------------

  Serial.println("Buscando MPU6050...");

  if (!mpu.begin(0x68, &Wire)) {

    Serial.println("ERROR: MPU6050 no detectado");

    while (1) {
      delay(1000);
    }
  }

  Serial.println("MPU6050 encontrado!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);

  mpu.setGyroRange(MPU6050_RANGE_500_DEG);

  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  // ----------------------------------------------------------
  // WIFI
  // ----------------------------------------------------------

  WiFi.softAP(ssid, password);

  Serial.println();
  Serial.println("WiFi AP creado");

  Serial.print("SSID: ");
  Serial.println(ssid);

  Serial.print("IP: ");
  Serial.println(WiFi.softAPIP());

  // ----------------------------------------------------------
  // SERVIDOR
  // ----------------------------------------------------------

  server.on("/", handleRoot);

  server.on("/datos", handleDatos);

  server.on("/control", handleControl);

  server.on("/motor1", handleMotor1);

  server.on("/motor2", handleMotor2);

  server.on("/motor3", handleMotor3);

  server.on("/motor4", handleMotor4);

  server.on("/motores/on", handleEncenderMotores);

  server.on("/stop", []() {apagarTodosLosMotores();server.send(200, "text/plain", "Motores apagados");});

  server.begin();

  Serial.println("Servidor web iniciado");

  lastTime = millis();
}


// ============================================================
// LOOP
// ============================================================

void loop() {

  server.handleClient();

  // ----------------------------------------------------------
  // MPU
  // ----------------------------------------------------------

  sensors_event_t a;
  sensors_event_t g;
  sensors_event_t temp;

  mpu.getEvent(&a, &g, &temp);

  // ----------------------------------------------------------
  // TIEMPO
  // ----------------------------------------------------------

  unsigned long currentTime = millis();

  float dt =
    (currentTime - lastTime) / 1000.0;

  lastTime = currentTime;

  if (dt <= 0 || dt > 0.1) {
    dt = 0.01;
  }

  // ----------------------------------------------------------
  // ACELEROMETRO
  // ----------------------------------------------------------

  float ax = -a.acceleration.x;
  float ay = -a.acceleration.y;
  float az = -a.acceleration.z;

  float acc_pitch =
    atan2(
      -ax,
      sqrt(ay * ay + az * az)
    )
    * 180.0 / PI;

  float acc_roll =
    atan2(
      ay,
      sqrt(ax * ax + az * az)
    )
    * 180.0 / PI;

  // ----------------------------------------------------------
  // GIROSCOPIO
  // ----------------------------------------------------------

  float gyro_x =
    g.gyro.x - gyro_roll_offset;

  float gyro_y =
    g.gyro.y - gyro_pitch_offset;

  float gyro_z =
    g.gyro.z - gyro_yaw_offset;

  // ----------------------------------------------------------
  // FILTRO COMPLEMENTARIO
  // ----------------------------------------------------------

  if (control_activo) {

    pitch =
      alpha *
      (pitch + gyro_y * dt * 180.0 / PI)
      +
      (1.0 - alpha) *
      (acc_pitch - acc_pitch_offset);

    roll =
      alpha *
      (roll + gyro_x * dt * 180.0 / PI)
      +
      (1.0 - alpha) *
      (acc_roll - acc_roll_offset);

    yaw +=
      gyro_z *
      dt *
      180.0 / PI;
  }

  // ----------------------------------------------------------
  // MONITOR SERIE
  // ----------------------------------------------------------

  static unsigned long lastSerial = 0;

  if (millis() - lastSerial >= 100) {

    lastSerial = millis();


    uint8_t estado = estado_motores();

    std::array<int, 4> pwm_m = pwm_motores();

    Serial.printf(
      "Roll: %7.2f | Pitch: %7.2f | Yaw: %7.2f | "
      "GYRO: %6.2f %6.2f %6.2f | "
      "M1: %s PWM=%d | M2: %s PWM=%d | M3: %s PWM=%d, | M4: %s PWM=%d\n",

      roll,
      pitch,
      yaw,

      g.gyro.x,
      g.gyro.y,
      g.gyro.z,
     
      estado & (1 << 0) ? "ON" : "OFF",
      estado & (1 << 0) ? pwm_m[0] : 0,

      estado & (1 << 1) ? "ON" : "OFF",
      estado & (1 << 1) ? pwm_m[1] : 0,


      estado & (1 << 2) ? "ON" : "OFF",
      estado & (1 << 2) ? pwm_m[2] : 0,


       estado & (1 << 3) ? "ON" : "OFF",
       estado & (1 << 3) ? pwm_m[3] : 0
    );
  }

  delay(5);
}
