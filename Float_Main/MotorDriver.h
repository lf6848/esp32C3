#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H
#include <Arduino.h>

class MotorDriver {
public:
    void init() {
        // TODO: 初始化 PWM 引脚
        Serial.println("Motor Init...");
    }

    // power 范围: -1.0 (全速吸水/下潜) 到 1.0 (全速排水/上浮)
    void setThrust(float power) {
        // TODO: 将 float 转换为 PWM 信号驱动电机
        // 限制 power 在 -1.0 到 1.0 之间
    }

    void stop() {
        setThrust(0);
    }
};
#endif