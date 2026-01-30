#ifndef FLOAT_MANAGER_H
#define FLOAT_MANAGER_H

#include "Config.h"
#include "SensorDriver.h"
#include "MotorDriver.h"
#include "CommsDriver.h"
#include "StorageDriver.h"

class FloatManager {
public:
    // 构造函数：注入所有依赖的模块
    FloatManager(SensorDriver* s, MotorDriver* m, CommsDriver* c, StorageDriver* st) 
        : sensor(s), motor(m), comms(c), storage(st) {}

    void init() {
        // 可以在这里做一些逻辑上的复位
        currentState = IDLE;
    }

    // 这个函数会被主循环不断调用
    void update() {
        unsigned long currentMillis = millis();

        // --- 状态机逻辑 ---
        switch (currentState) {
            case IDLE:
                // 逻辑：如果在水面检测到入水(深度 > 0.1m)，或者收到指令
                // 动作：发送“我活着”的包，然后开始下潜
                comms->sendRealtimeData(currentMillis, sensor->getDepth(), 0);
                // 模拟跳转
                // if (sensor->getDepth() > 0.1) currentState = DIVE_1;
                break;

            case DIVE_1:
                // 逻辑：PID控制电机去 TARGET_DEPTH_DEEP
                // 记录数据：if (currentMillis - lastLogTime > LOG_INTERVAL) { ... }
                // 检查：如果到达深度并稳定，进入 HOVER_1
                break;

            case HOVER_1:
                // 逻辑：维持深度，计时 30秒
                // 计时结束 -> ASCEND_1
                break;

            case ASCEND_1:
                // 逻辑：PID控制去 TARGET_DEPTH_SHALLOW
                break;
            
            // ... 重复 Dive 2 等状态 ...

            case RECOVERY:
                motor->stop(); // 或者保持最大浮力
                // 逻辑：等待岸上指令，或者自动开始传数据
                comms->dumpHistoryData();
                break;
        }
    }

private:
    // 引用其他模块
    SensorDriver* sensor;
    MotorDriver* motor;
    CommsDriver* comms;
    StorageDriver* storage;

    // 定义状态枚举
    enum State { 
        IDLE, 
        DIVE_1, HOVER_1, ASCEND_1, HOVER_SHALLOW_1,
        DIVE_2, HOVER_2, ASCEND_2, HOVER_SHALLOW_2,
        RECOVERY 
    };
    State currentState = IDLE;

    unsigned long lastLogTime = 0; // 用于控制记录频率
    unsigned long stateStartTime = 0; // 用于计算悬停时间
};
#endif