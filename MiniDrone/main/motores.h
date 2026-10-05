#ifndef MOTORES_H
#define MOTORES_H


// ============================================================
// Definiciones
// ============================================================
#include <stdint.h>

#define MOTOR1_PIN 4
#define MOTOR2_PIN 3
#define MOTOR3_PIN 2
#define MOTOR4_PIN 1



// ============================================================
// Funciones
// ============================================================


void incializacion_motores(int pwm , bool activos );
void encender_todos_motores();
void handleMotor1(void);
void handleMotor2(void);
void handleMotor3(void);
void handleMotor4(void);
void apagarTodosLosMotores(void);
void control_motores(int u_control_m[4]);

uint8_t estado_motores();
std::array<int, 4> pwm_motores();

#endif