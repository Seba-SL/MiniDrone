#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <math.h>
#include "motores.h"
// ============================================================
// WIFI
// ============================================================

const char* ssid = "DronESP";
const char* password = "12345678";

WebServer server(80);

// ============================================================
// MPU6050
// ============================================================

#define I2C_SDA 8
#define I2C_SCL 9

Adafruit_MPU6050 mpu;

bool control_activo = false;

float roll = 0;
float pitch = 0;
float yaw = 0;

float acc_roll_offset = 0;
float acc_pitch_offset = 0;

float gyro_roll_offset = 0;
float gyro_pitch_offset = 0;
float gyro_yaw_offset = 0;

unsigned long lastTime = 0;

const float alpha = 0.98;


// ============================================================
// PAGINA WEB
// ============================================================

void handleRoot() {

  String estadoMPU =
    control_activo ? "MPU ON" : "MPU OFF";

  String estadoMotor1 =
    motor1_activo ? "MOTOR 1 ON" : "MOTOR 1 OFF";

  String estadoMotor2 =
    motor2_activo ? "MOTOR 2 ON" : "MOTOR 2 OFF";

  String estadoMotor3 =
    motor3_activo ? "MOTOR 3 ON" : "MOTOR 3 OFF";
  
  String estadoMotor4 =
    motor4_activo ? "MOTOR 4 ON" : "MOTOR 4 OFF";


  String html =
    "<!DOCTYPE html>"
    "<html>"
    "<head>"
    "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"

    "<style>"

    "body {"
    "  font-family: Arial, sans-serif;"
    "  text-align: center;"
    "  background: #111;"
    "  color: white;"
    "  margin: 0;"
    "  padding: 20px;"
    "}"

    "h2 {"
    "  margin-bottom: 25px;"
    "}"

    "button {"
    "  font-size: 18px;"
    "  padding: 12px 25px;"
    "  margin: 8px;"
    "  border-radius: 8px;"
    "  border: none;"
    "}"

    ".datos {"
    "  display: flex;"
    "  justify-content: center;"
    "  gap: 15px;"
    "  flex-wrap: wrap;"
    "}"

    ".dato {"
    "  background: #222;"
    "  border-radius: 10px;"
    "  padding: 15px;"
    "  width: 90px;"
    "}"

    ".titulo {"
    "  font-size: 14px;"
    "  color: #aaa;"
    "}"

    ".valor {"
    "  font-size: 25px;"
    "  margin-top: 8px;"
    "}"

    ".motor {"
    "  background: #222;"
    "  padding: 20px;"
    "  margin: 20px auto;"
    "  border-radius: 10px;"
    "  max-width: 400px;"
    "}"



    "</style>"
    "</head>"

    "<body>"

    "<h2>DRON ESP32-C3</h2>"

    "<button onclick=\"fetch('/stop')\" "
    "style=\"background:red;color:white;font-size:20px;padding:15px;\">"
    "🛑 PARAR TODOS LOS MOTORES"
    "</button>"
    // ========================================================
    // MOTOR 1
    // ========================================================

    "<div class=\"motor\">"

    "<h3>MOTOR 1</h3>"

    "<p id=\"motor1Estado\">"
    + estadoMotor1 +
    "</p>"

    "<button onclick=\"toggleMotor1()\">"
    "MOTOR 1 ON / OFF"
    "</button>"

    "<p>GPIO4 - PWM: "
    + String(motor1_pwm) +
    "/255"
    "</p>"

    "</div>"

    // ========================================================
    // MOTOR 2
    // ========================================================

    "<div class=\"motor\">"

    "<h3>MOTOR 2</h3>"

    "<p id=\"motor2Estado\">"
    + estadoMotor2 +
    "</p>"

    "<button onclick=\"toggleMotor2()\">"
    "MOTOR 2 ON / OFF"
    "</button>"

    "<p>GPIO3 - PWM: "
    + String(motor2_pwm) +
    "/255"
    "</p>"

    "</div>"


    // ========================================================
    // MOTOR 3
    // ========================================================

    "<div class=\"motor\">"

    "<h3>MOTOR 3</h3>"

    "<p id=\"motor3Estado\">"
    + estadoMotor3 +
    "</p>"

    "<button onclick=\"toggleMotor3()\">"
    "MOTOR 3 ON / OFF"
    "</button>"

    "<p>GPIO2 - PWM: "
    + String(motor3_pwm) +
    "/255"
    "</p>"

    "</div>"

   
    // ========================================================
    // MOTOR 4
    // ========================================================


     "<div class=\"motor\">"

    "<h3>MOTOR 4</h3>"

    "<p id=\"motor4Estado\">"
    + estadoMotor4 +
    "</p>"

    "<button onclick=\"toggleMotor4()\">"
    "MOTOR 4 ON / OFF"
    "</button>"

    "<p>GPIO1 - PWM: "
    + String(motor4_pwm) +
    "/255"
    "</p>"

    "</div>"

    // ========================================================
    // MPU
    // ========================================================

    "<div>"
    "<p id=\"estado\">"
    + estadoMPU +
    "</p>"

    "<button onclick=\"toggleMPU()\">"
    "ACTIVAR / DESACTIVAR MPU"
    "</button>"
    "</div>"

    "<br>"

    "<div class=\"datos\">"

    "<div class=\"dato\">"
    "<div class=\"titulo\">ROLL</div>"
    "<div class=\"valor\" id=\"roll\">--</div>"
    "</div>"

    "<div class=\"dato\">"
    "<div class=\"titulo\">PITCH</div>"
    "<div class=\"valor\" id=\"pitch\">--</div>"
    "</div>"

    "<div class=\"dato\">"
    "<div class=\"titulo\">YAW</div>"
    "<div class=\"valor\" id=\"yaw\">--</div>"
    "</div>"

    "</div>"

    "<br>"

    "<div class=\"datos\">"

    "<div class=\"dato\">"
    "<div class=\"titulo\">ACC X</div>"
    "<div class=\"valor\" id=\"ax\">--</div>"
    "</div>"

    "<div class=\"dato\">"
    "<div class=\"titulo\">ACC Y</div>"
    "<div class=\"valor\" id=\"ay\">--</div>"
    "</div>"

    "<div class=\"dato\">"
    "<div class=\"titulo\">ACC Z</div>"
    "<div class=\"valor\" id=\"az\">--</div>"
    "</div>"

    "</div>"

    "<br>"

    "<div class=\"datos\">"

    "<div class=\"dato\">"
    "<div class=\"titulo\">GYRO X</div>"
    "<div class=\"valor\" id=\"gx\">--</div>"
    "</div>"

    "<div class=\"dato\">"
    "<div class=\"titulo\">GYRO Y</div>"
    "<div class=\"valor\" id=\"gy\">--</div>"
    "</div>"

    "<div class=\"dato\">"
    "<div class=\"titulo\">GYRO Z</div>"
    "<div class=\"valor\" id=\"gz\">--</div>"
    "</div>"

    "</div>"

    "<br>"

    "<div>"
    "Temperatura: <span id=\"temp\">--</span> °C"
    "</div>"

    // ========================================================
    // JAVASCRIPT
    // ========================================================

    "<script>"

    "function actualizar() {"

    "fetch('/datos')"
    ".then(r => r.json())"
    ".then(d => {"

    "document.getElementById('roll').innerText = d.roll.toFixed(2);"
    "document.getElementById('pitch').innerText = d.pitch.toFixed(2);"
    "document.getElementById('yaw').innerText = d.yaw.toFixed(2);"

    "document.getElementById('ax').innerText = d.ax.toFixed(2);"
    "document.getElementById('ay').innerText = d.ay.toFixed(2);"
    "document.getElementById('az').innerText = d.az.toFixed(2);"

    "document.getElementById('gx').innerText = d.gx.toFixed(2);"
    "document.getElementById('gy').innerText = d.gy.toFixed(2);"
    "document.getElementById('gz').innerText = d.gz.toFixed(2);"

    "document.getElementById('temp').innerText = d.temp.toFixed(1);"

    "document.getElementById('estado').innerText = "
    "d.activo ? 'MPU ON' : 'MPU OFF';"

    "document.getElementById('motor1Estado').innerText = "
    "d.motor1 ? 'MOTOR 1 ON' : 'MOTOR 1 OFF';"

    "document.getElementById('motor2Estado').innerText = "
    "d.motor2 ? 'MOTOR 2 ON' : 'MOTOR 2 OFF';"

    "document.getElementById('motor3Estado').innerText = "
    "d.motor3 ? 'MOTOR 3 ON' : 'MOTOR 3 OFF';"
    

    "document.getElementById('motor4Estado').innerText = "
    "d.motor4 ? 'MOTOR 4 ON' : 'MOTOR 4 OFF';"
    
    "});"
    "}"

    "function toggleMotor1() {"
    "fetch('/motor1');"
    "}"

    "function toggleMotor2() {"
    "fetch('/motor2');"
    "}"

     "function toggleMotor3() {"
    "fetch('/motor3');"
    "}"

         "function toggleMotor4() {"
    "fetch('/motor4');"
    "}"


    "function toggleMPU() {"
    "fetch('/control');"
    "}"

    "setInterval(actualizar, 100);"

    "actualizar();"

    "</script>"

    "</body>"
    "</html>";

  server.send(200, "text/html", html);
}


// ============================================================
// DATOS
// ============================================================
void handleDatos() {

  sensors_event_t a;
  sensors_event_t g;
  sensors_event_t temp;

  mpu.getEvent(&a, &g, &temp);

  String json = "{";

  json += "\"roll\":" + String(roll, 2) + ",";
  json += "\"pitch\":" + String(pitch, 2) + ",";
  json += "\"yaw\":" + String(yaw, 2) + ",";

  json += "\"ax\":" + String(a.acceleration.x, 2) + ",";
  json += "\"ay\":" + String(a.acceleration.y, 2) + ",";
  json += "\"az\":" + String(a.acceleration.z, 2) + ",";

  json += "\"gx\":" + String(g.gyro.x, 2) + ",";
  json += "\"gy\":" + String(g.gyro.y, 2) + ",";
  json += "\"gz\":" + String(g.gyro.z, 2) + ",";

  json += "\"temp\":" + String(temp.temperature, 1) + ",";

  json += "\"activo\":"
       + String(control_activo ? "true" : "false")
       + ",";

  json += "\"motor1\":"
       + String(motor1_activo ? "true" : "false")
       + ",";

  json += "\"motor2\":"
       + String(motor2_activo ? "true" : "false")
       + ",";

  json += "\"motor3\":"
       + String(motor3_activo ? "true" : "false")
       + ",";

  json += "\"motor4\":"
       + String(motor4_activo ? "true" : "false");

  json += "}";

  server.send(200, "application/json", json);
}


// ============================================================
// MOTOR 1 - GPIO4
// ============================================================

void handleMotor1() {

  motor1_activo = !motor1_activo;

  if (motor1_activo) {

    ledcWrite(MOTOR1_PIN, motor1_pwm);

    Serial.print("MOTOR 1 ON - GPIO4 - PWM = ");
    Serial.println(motor1_pwm);

  } else {

    ledcWrite(MOTOR1_PIN, 0);

    Serial.println("MOTOR 1 OFF");
  }

  server.send(
    200,
    "text/plain",
    motor1_activo ? "MOTOR 1 ON" : "MOTOR 1 OFF"
  );
}


// ============================================================
// MOTOR 2 - GPIO3
// ============================================================

void handleMotor2() {

  motor2_activo = !motor2_activo;

  if (motor2_activo) {

    ledcWrite(MOTOR2_PIN, motor2_pwm);

    Serial.print("MOTOR 2 ON - GPIO3 - PWM = ");
    Serial.println(motor2_pwm);

  } else {

    ledcWrite(MOTOR2_PIN, 0);

    Serial.println("MOTOR 2 OFF");
  }

  server.send(
    200,
    "text/plain",
    motor2_activo ? "MOTOR 2 ON" : "MOTOR 2 OFF"
  );
}

// ============================================================
// MOTOR 3 - GPIO2
// ============================================================


void handleMotor3() {

  motor3_activo = !motor3_activo;

  if (motor3_activo) {

    ledcWrite(MOTOR3_PIN, motor3_pwm);

    Serial.print("MOTOR 3 ON - GPIO2 - PWM = ");
    Serial.println(motor3_pwm);

  } else {

    ledcWrite(MOTOR3_PIN, 0);

    Serial.println("MOTOR 3 OFF");
  }

  server.send(
    200,
    "text/plain",
    motor3_activo ? "MOTOR 3 ON" : "MOTOR 3 OFF"
  );
}



// ============================================================
// MOTOR 4 - GPIO1
// ============================================================


void handleMotor4() {

  motor4_activo = !motor4_activo;

  if (motor4_activo) {

    ledcWrite(MOTOR4_PIN, motor4_pwm);

    Serial.print("MOTOR 4 ON - GPIO1 - PWM = ");
    Serial.println(motor4_pwm);

  } else {

    ledcWrite(MOTOR4_PIN, 0);

    Serial.println("MOTOR 4 OFF");
  }

  server.send(
    200,
    "text/plain",
    motor4_activo ? "MOTOR 4 ON" : "MOTOR 4 OFF"
  );
}


// ============================================================
// MPU ON / OFF
// ============================================================

void handleControl() {

  control_activo = !control_activo;

  if (control_activo) {

    sensors_event_t a;
    sensors_event_t g;
    sensors_event_t temp;

    mpu.getEvent(&a, &g, &temp);

    float ax = -a.acceleration.x;
    float ay = -a.acceleration.y;
    float az = -a.acceleration.z;

    acc_roll_offset =
      atan2(
        ay,
        sqrt(ax * ax + az * az)
      )
      * 180.0 / PI;

    acc_pitch_offset =
      atan2(
        -ax,
        sqrt(ay * ay + az * az)
      )
      * 180.0 / PI;

    gyro_roll_offset = g.gyro.x;
    gyro_pitch_offset = g.gyro.y;
    gyro_yaw_offset = g.gyro.z;

    roll = 0;
    pitch = 0;
    yaw = 0;

    lastTime = millis();

    Serial.println("MPU ACTIVADO");
    Serial.println("Calibracion realizada");

  } else {

    Serial.println("MPU DESACTIVADO");
  }

  server.send(
    200,
    "text/plain",
    control_activo ? "MPU ON" : "MPU OFF"
  );
}


// Apagado de motores
void apagarTodosLosMotores() {
  motor1_activo = false;
  motor2_activo = false;
  motor3_activo = false;
  motor4_activo = false;

  ledcWrite(MOTOR1_PIN, 0);
  ledcWrite(MOTOR2_PIN, 0);
  ledcWrite(MOTOR3_PIN, 0);
  ledcWrite(MOTOR4_PIN, 0);

  Serial.println("!!! PARADA: TODOS LOS MOTORES APAGADOS !!!");
}


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

      motor1_activo ? "ON" : "OFF",
      motor1_activo ? motor1_pwm : 0,

      motor2_activo ? "ON" : "OFF",
      motor2_activo ? motor2_pwm : 0,


      motor3_activo ? "ON" : "OFF",
      motor3_activo ? motor3_pwm : 0,


      motor4_activo ? "ON" : "OFF",
      motor4_activo ? motor4_pwm : 0
    );
  }

  delay(5);
}
