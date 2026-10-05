#ifndef MOTORES_H
#define MOTORES_H


// ============================================================
// MOTORES DE PRUEBA
// ============================================================


#define MOTOR1_PIN 4
#define MOTOR2_PIN 3
#define MOTOR3_PIN 2
#define MOTOR4_PIN 1

extern bool motor1_activo;
extern bool motor2_activo;
extern bool motor3_activo;
extern bool motor4_activo;

extern int motor1_pwm;
extern int motor2_pwm;
extern int motor3_pwm;
extern int motor4_pwm;



void incializacion_motores(int pwm , bool activos );


#endif