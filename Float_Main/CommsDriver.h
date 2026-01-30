#ifndef COMMS_DRIVER_H
#define COMMS_DRIVER_H
#include <Arduino.h>

class CommsDriver {
public:
    void init() {
        // TODO: 初始化 WiFi 或 ESP-NOW
        Serial.println("Comms Init...");
    }

    // 发送单个实时数据包 (用于阶段一：入水测试)
    bool sendRealtimeData(float time, float depth, float pressure) {
        // TODO: 格式化字符串 "ID, Time, Depth..." 并发送
        return true; // 发送成功返回 true
    }

    // 发送存储的历史数据 (用于阶段三：回收)
    void dumpHistoryData() {
        // TODO: 读取 Flash 中的文件，批量发送
    }
};
#endif