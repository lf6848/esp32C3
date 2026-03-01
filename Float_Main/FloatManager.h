#ifndef FLOAT_MANAGER_H
#define FLOAT_MANAGER_H

#include <Arduino.h> 
#include "Config.h"
#include "SensorDriver.h"
#include "MotorDriver.h"
#include "CommsDriver.h"
#include "StorageDriver.h"

class FloatManager {
public:
    // 构造函数
    FloatManager(SensorDriver* s, MotorDriver* m, CommsDriver* c, StorageDriver* st) 
        : sensor(s), motor(m), comms(c), storage(st) {}

    void init() {
        currentState = IDLE;
        Serial.println("Manager: Init Complete. State = IDLE");
    }

    void update() {
        unsigned long currentMillis = millis();
        float currentDepth = sensor->getDepth(); 

        switch (currentState) {
            case IDLE:
                // 假设我们通过某种方式触发任务开始（比如收到WiFi指令，或者检测到入水）
                // 这里为了演示，假设初始化时就清空一次
                if (currentMillis < 5000) { // 开机前5秒清空
                     // 实际逻辑建议加一个标志位，只清空一次
                     // storage->clearLogs(); 
                }
                
                // 发送心跳包 (之前的逻辑)
                if (currentMillis - lastLogTime > 5000) {
                    lastLogTime = currentMillis; 
                    comms->sendRealtimeData(currentDepth);
                    // 假设这里触发下潜：
                    // currentState = DIVE_1;
                    // storage->clearLogs(); // 进入下潜前务必清空旧数据！
                }
                break;

            case DIVE_1:
            case HOVER_1:
            case ASCEND_1:
                // 1秒记录一次 (1000ms)
                if (currentMillis - lastLogTime > 1000) {
                    lastLogTime = currentMillis; 
                    
                    // 写入 Flash
                    storage->logData(currentMillis, currentDepth);
                    
                    // 注意：水下不要调用 comms->sendRealtimeData
                }
                
                // 这里还要写你的 PID 控制逻辑 motor->setThrust(...)
                
                // 模拟任务完成，跳转到回收状态
                // if (任务完成) currentState = RECOVERY;
                break;

            case RECOVERY:
                motor->stop();
                
                // 只有当 WiFi 连上时才发
                if (currentMillis - lastLogTime > 5000) { // 每5秒尝试一次批量发送
                     lastLogTime = currentMillis;
                     
                     Serial.println("Manager: Uploading History Data...");
                     
                     // 1. 获取文件句柄
                     File file = storage->openFileForRead();
                     if (!file) {
                         Serial.println("Manager: No logs found!");
                         break;
                     }
                     
                     // 2. 逐行读取并发送
                     while (file.available()) {
                         String line = file.readStringUntil('\n'); // 读一行 "12500,2.54"
                         // 调用通信驱动发送这一行 (需要稍后在 CommsDriver 加这个函数)
                         comms->sendHistoryLine(line); 
                         delay(50); // 发慢点，防止堵塞网络
                     }
                     file.close();
                     Serial.println("Manager: Upload Complete!");
                     
                     // 发完可以停在某个状态，防止无限重发
                }
                break;
        }
    }

private:
    SensorDriver* sensor;
    MotorDriver* motor;
    CommsDriver* comms;
    StorageDriver* storage;

    enum State { 
        IDLE, 
        DIVE_1, HOVER_1, ASCEND_1, HOVER_SHALLOW_1,
        DIVE_2, HOVER_2, ASCEND_2, HOVER_SHALLOW_2,
        RECOVERY 
    };
    State currentState = IDLE;

    unsigned long lastLogTime = 0; 
    unsigned long stateStartTime = 0; 
};
#endif