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

        // 1. 启动 I2C
        Wire.begin(PIN_SDA, PIN_SCL);

        // 2. 初始化传感器
        if (!sensor.init()) {
            Serial.println("Sensor: Failed to init! Checking connection...");
            return; 
        }
        sensor.setModel(MS5837::MS5837_02BA);

        // 3. 设置流体密度
        sensor.setFluidDensity(FLUID_DENSITY); 

        Serial.println("Sensor: Init Success! (Model forced to 30BA)");
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