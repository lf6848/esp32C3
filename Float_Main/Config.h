#ifndef CONFIG_H
#define CONFIG_H

// 硬件引脚定义 (根据你的PCB修改)
#define PIN_SDA 6
#define PIN_SCL 7
#define PIN_MOTOR_A 2
#define PIN_MOTOR_B 3
#define PIN_BATTERY_ADC 4

// 任务参数
#define TARGET_DEPTH_DEEP 2.5f    // 目标深度1: 2.5米
#define TARGET_DEPTH_SHALLOW 0.4f // 目标深度2: 0.4米
#define HOVER_DURATION 30000      // 悬停时间: 30秒 (毫秒)
#define LOG_INTERVAL 1000         // 数据记录间隔: 1秒

// 参数常量
#define FLUID_DENSITY 1025

// 身份信息
#define COMPANY_ID "PIONEER_01"   // 你的队伍编号 未定

#endif