#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#include <Arduino.h>
#include "Config.h"

class MotorDriver {
public:
    void init() {
        Serial.print("Motor: Initializing Brushless Pump...");
        
        // 1. 初始化 PWM 引脚 (白线)
        pinMode(PIN_PUMP_PWM, OUTPUT);
        
        // 2. 初始化 方向 引脚 (绿线)
        pinMode(PIN_PUMP_DIR, OUTPUT);
        
        // 3. 初始状态：停止
        stop();
        // 4.修改频率
        analogWriteFrequency(20000); 
        Serial.println("Success!");
    }

    /**
     * 控制推力 (用于 PID 控制)
     * @param power: 范围 -1.0 (全速吸水) 到 1.0 (全速排水)
     *               正数 = 正转 (Green 接 GND)
     *               负数 = 反转 (Green 悬空)
     */
    void setThrust(float power) {
        // 1. 限制范围
        if (power > 1.0f) power = 1.0f;
        if (power < -1.0f) power = -1.0f;

        // 2. 处理死区 (Deadzone)
        // 说明书说 0-10% 占空比不转。
        // 为了让 PID 更好工作，如果 power 很小，直接给 0
        if (abs(power) < 0.1f) {
            stop();
            return;
        }

        // 3. 设置方向 (绿线逻辑)
        if (power > 0) {
            // 正转：绿线接电源负极 -> ESP32 输出 LOW
            pinMode(PIN_PUMP_DIR, OUTPUT);
            digitalWrite(PIN_PUMP_DIR, LOW); 
        } else {
            // 反转：绿线悬空 -> ESP32 设为输入模式 (高阻态)
            pinMode(PIN_PUMP_DIR, INPUT); 
        }

        // 4. 设置速度 (白线 PWM)
        // 将 0.0 - 1.0 映射到 0 - 255
        int pwmValue = abs(power) * 255;
        
        // 确保 PWM 频率符合泵的要求 (20kHz)
        // analogWrite 在 ESP32 上默认频率可能只有 1kHz
        analogWrite(PIN_PUMP_PWM, pwmValue);
    }

    void stop() {
        analogWrite(PIN_PUMP_PWM, 0); // PWM = 0
        // 方向无所谓
    }
};
#endif