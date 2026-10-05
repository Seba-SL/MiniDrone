#include "motores.h"


extern bool motor1_activo;
extern bool motor2_activo;
extern bool motor3_activo;
extern bool motor4_activo;

extern int motor1_pwm;
extern int motor2_pwm;
extern int motor3_pwm;
extern int motor4_pwm;

int u_control_m[4] = {30, 0, 0, 60}; // Ej, enciende el m1 y el m4 a 30 y 60 pwm.

void incializacion_motores(int pwm, bool activos)
{
    pinMode(MOTOR1_PIN, OUTPUT);
    pinMode(MOTOR2_PIN, OUTPUT);
    pinMode(MOTOR3_PIN, OUTPUT);
    pinMode(MOTOR4_PIN, OUTPUT);

    ledcAttach(MOTOR1_PIN, 20000, 8);
    ledcAttach(MOTOR2_PIN, 20000, 8);
    ledcAttach(MOTOR3_PIN, 20000, 8);
    ledcAttach(MOTOR4_PIN, 20000, 8);

    motor1_pwm = pwm;
    motor2_pwm = pwm;
    motor3_pwm = pwm;
    motor4_pwm = pwm;

    motor1_activo = activos;
    motor2_activo = activos;
    motor3_activo = activos;
    motor4_activo = activos;

    ledcWrite(MOTOR1_PIN, activos ? pwm : 0);
    ledcWrite(MOTOR2_PIN, activos ? pwm : 0);
    ledcWrite(MOTOR3_PIN, activos ? pwm : 0);
    ledcWrite(MOTOR4_PIN, activos ? pwm : 0);
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
// Apagado de motores
// ============================================================

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
// Encendido de motores
// ============================================================


void encender_motores()
{
    motor1_activo = true;
    motor2_activo = true;
    motor3_activo = true;
    motor4_activo = true;

    ledcWrite(MOTOR1_PIN, motor1_pwm);
    ledcWrite(MOTOR2_PIN, motor2_pwm);
    ledcWrite(MOTOR3_PIN, motor3_pwm);
    ledcWrite(MOTOR4_PIN, motor4_pwm);
}

// ============================================================
// Control de motores
// ============================================================


void control_motores(int u_control_m[4])
{
    motor1_pwm = u_control_m[0];
    motor2_pwm = u_control_m[1];
    motor3_pwm = u_control_m[2];
    motor4_pwm = u_control_m[3];

    ledcWrite(MOTOR1_PIN, u_control_m[0]);
    ledcWrite(MOTOR2_PIN, u_control_m[1]);
    ledcWrite(MOTOR3_PIN, u_control_m[2]);
    ledcWrite(MOTOR4_PIN, u_control_m[3]);
}


// ============================================================
// Estado de motores
// ============================================================

uint8_t estado_motores()
{
    uint8_t estado = 0;

    if (motor1_activo) estado |= (1 << 0);
    if (motor2_activo) estado |= (1 << 1);
    if (motor3_activo) estado |= (1 << 2);
    if (motor4_activo) estado |= (1 << 3);

    return estado;
}


std::array<int, 4> pwm_motores()
{
    return {
        motor1_pwm,
        motor2_pwm,
        motor3_pwm,
        motor4_pwm
    };
}