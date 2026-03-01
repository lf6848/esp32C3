#ifndef COMMS_DRIVER_H
#define COMMS_DRIVER_H

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "Config.h"

class CommsDriver {
public:
    void init() {
        Serial.print("Comms: Connecting to WiFi");
        WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
        
        // 尝试连接 10 秒，连不上就不等了，以免卡死整个系统
        int timeout = 0;
        while (WiFi.status() != WL_CONNECTED && timeout < 20) {
            delay(500);
            Serial.print(".");
            timeout++;
        }

        if (WiFi.status() == WL_CONNECTED) {
            Serial.println("\nComms: WiFi Connected!");
            Serial.print("IP Address: ");
            Serial.println(WiFi.localIP());
        } else {
            Serial.println("\nComms: WiFi Connection Failed (Offline Mode)");
        }
    }

    // 发送数据函数：接收当前深度作为参数
    bool sendRealtimeData(float depth) {
        // 1. 检查网络状态
        if (WiFi.status() != WL_CONNECTED) {
            // 尝试重连（可选，或者直接返回失败）
            return false;
        }

        WiFiClient client;
        HTTPClient http;
        bool success = false;

        // 2. 开始连接
        http.begin(client, SERVER_URL);
        http.addHeader("Content-Type", "application/json");

        // 3. 构建动态 JSON 字符串
        // 目标格式: {"name": "Pioneer_Float", "value": 1.25}
        String jsonPayload = "{\"name\":\"Pioneer_Float\",\"value\":";
        jsonPayload += String(depth, 2); // 将 float 转为字符串，保留2位小数
        jsonPayload += "}";

        // 4. 发送 POST 请求
        int httpResponseCode = http.POST(jsonPayload);

        // 5. 检查结果
        if (httpResponseCode > 0) {
            Serial.print("Comms: Sent Data > ");
            Serial.println(jsonPayload);
            Serial.print("Comms: Server Response > ");
            Serial.println(httpResponseCode); // 200 表示成功
            success = true;
        } else {
            Serial.print("Comms: Error code: ");
            Serial.println(httpResponseCode);
        }

        // 6. 释放资源
        http.end();
        return success;
    }
    // 发送历史数据行
    // dataLine 格式如: "12500,2.54"
    void sendHistoryLine(String dataLine) {
        if (WiFi.status() != WL_CONNECTED) return;

        // 1. 数据清洗（去掉首尾空格和换行符）
        dataLine.trim();
        
        // 2. 安全检查：如果行是空的，直接跳过
        if (dataLine.length() == 0) return;

        // 3. 解析 CSV 格式
        int commaIndex = dataLine.indexOf(',');
        
        // 如果没找到逗号，说明数据格式不对，直接放弃这一行，防止崩溃
        if (commaIndex == -1) {
            Serial.print("Comms: Bad data format -> ");
            Serial.println(dataLine);
            return;
        }

        String timeStr = dataLine.substring(0, commaIndex);
        String depthStr = dataLine.substring(commaIndex + 1);

        WiFiClient client;
        HTTPClient http;

        http.begin(client, SERVER_URL);
        http.addHeader("Content-Type", "application/json");

        // 构建 JSON: {"name": "History", "time": 12500, "value": 2.54}
        // 注意：value 后面不需要加引号，因为它应该是数字
        String jsonPayload = "{\"name\":\"History\",\"time\":";
        jsonPayload += timeStr;
        jsonPayload += ",\"value\":";
        jsonPayload += depthStr;
        jsonPayload += "}";

        int code = http.POST(jsonPayload);
        
        // 只有发送成功才打印，避免刷屏
        if (code > 0) {
            Serial.print("Comms: History Packet Sent > ");
            Serial.println(jsonPayload);
        } else {
            Serial.print("Comms: Upload Fail code: ");
            Serial.println(code);
        }
        
        http.end();
    }
};
#endif