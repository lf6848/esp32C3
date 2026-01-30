#ifndef SENSOR_DRIVER_H
#define SENSOR_DRIVER_H

#include <Arduino.h>
#include <Wire.h>
#include "MS5837.h"
#include "Config.h"

class SensorDriver {
public:
    // 构造函数
    SensorDriver() {}

    void init() {
        Serial.println("Sensor: Initializing...");

        // --- 关键点 A: 启动 I2C ---
        // ESP32 可以自定义 I2C 引脚。
        // 根据 Config.h，我们将 SDA 接 D4(GPIO6), SCL 接 D5(GPIO7)
        Wire.begin(PIN_SDA, PIN_SCL);

        // --- 关键点 B: 初始化传感器 ---
        // 库会自动发送 PDF 第8页提到的 Reset 命令
        if (!sensor.init()) {
            Serial.println("Sensor: Failed to init! Checking connection...");
            // 如果卡在这里，通常是线没接好，或者 SDA/SCL 接反了
            // 可以加入一个死循环闪灯报警，或者 retry 逻辑
            return; 
        }

        // --- 关键点 C: 设置流体密度 ---
        // MATE 比赛的冰池/海池密度大约是 1025 kg/m^3
        // 设置错了会导致深度读数有 2-3% 的误差
        sensor.setFluidDensity(FLUID_DENSITY); 

        // 验证模型 (可选)
        // PDF 第6页提到这个传感器是 30BA (300米量程)
        // 这个库支持 30BA 和 02BA，通常会自动识别
        Serial.println("Sensor: Init Success!");
    }

    void update() {
        // --- 关键点 D: 读取数据 ---
        // 这一步库会自动发送 PDF 第9页的转换命令 (D1, D2)
        // 并读取 ADC 结果，计算温度补偿
        sensor.read();

        // 获取结果
        currentDepth = sensor.depth();       // 单位: m (米)
        currentPressure = sensor.pressure(); // 单位: mbar (毫巴)
        currentTemp = sensor.temperature();  // 单位: C (摄氏度)
        
        // 简单的调试打印 (不要在高速循环里一直开)
        // Serial.print("Depth: "); Serial.println(currentDepth);
    }

    float getDepth() {
        return currentDepth;
    }

    float getPressure() {
        return currentPressure;
    }
    
    float getTemp() {
        return currentTemp;
    }

private:
    MS5837 sensor; // 实例化库对象
    float currentDepth = 0.0;
    float currentPressure = 0.0;
    float currentTemp = 0.0;
};

#endif