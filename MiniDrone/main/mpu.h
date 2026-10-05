#ifndef MPU_H
#define MPU_H


// ============================================================
// MPU6050
// ============================================================

#define I2C_SDA 8
#define I2C_SCL 9

bool control_activo = false;
Adafruit_MPU6050 mpu;

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


#endif
