#include "motores.h"




void incializacion_motores(int pwm , bool activos)
{

    bool motor1_activo = false;
    bool motor2_activo = false;
    bool motor3_activo = false;
    bool motor4_activo = false;

    int motor1_pwm = 255;
    int motor2_pwm = 255;
    int motor3_pwm = 255;
    int motor4_pwm = 255;
    pinMode(MOTOR1_PIN, OUTPUT);
    pinMode(MOTOR2_PIN, OUTPUT);
    pinMode(MOTOR3_PIN, OUTPUT);
    pinMode(MOTOR4_PIN, OUTPUT);

    ledcAttach(MOTOR1_PIN, 20000, 8);
    ledcAttach(MOTOR2_PIN, 20000, 8);
    ledcAttach(MOTOR3_PIN, 20000, 8);
    ledcAttach(MOTOR4_PIN, 20000, 8);

    ledcWrite(MOTOR1_PIN, 0);
    ledcWrite(MOTOR2_PIN, 0);
    ledcWrite(MOTOR3_PIN, 0);
    ledcWrite(MOTOR4_PIN, 0);
}