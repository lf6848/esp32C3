#ifndef STORAGE_DRIVER_H
#define STORAGE_DRIVER_H
#include <Arduino.h>
// #include <LittleFS.h> 

class StorageDriver {
public:
    void init() {
        // TODO: 挂载 LittleFS 文件系统
        Serial.println("Storage Init...");
    }

    void logPoint(float time, float depth) {
        // TODO: 打开文件，追加写入一行数据
    }

    void clearLogs() {
        // TODO: 删除旧的日志文件，防止空间满了
    }
};
#endif