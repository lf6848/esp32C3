#include "Config.h"
#include "SensorDriver.h"
#include "MotorDriver.h"
#include "CommsDriver.h"
#include "StorageDriver.h"
#include "FloatManager.h"

// 1. 实例化所有硬件驱动
SensorDriver  mySensor;
MotorDriver   myMotor;
CommsDriver   myComms;
StorageDriver myStorage;

// 2. 实例化管理器，并将驱动传给它
FloatManager  myManager(&mySensor, &myMotor, &myComms, &myStorage);

void setup() {
    Serial.begin(115200);
    delay(1000); // 等待串口稳定

    // 初始化硬件
    mySensor.init();
    myMotor.init();
    myComms.init();
    myStorage.init();

    // 初始化逻辑
    myManager.init();
}

void loop() {
    //获取当前时间
    unsigned long currentMillis=millis();

    // 1. 更新感知 (读取所有传感器)
    mySensor.update();

    //获取当前水压计数据
    float depth = mySensor.getDepth();
    float pressure = mySensor.getPressure();
    float temp = mySensor.getTemp();

    //临时测试代码
    // Serial.print("[Time: ");
    // Serial.print(currentMillis / 1000.0); // 显示秒数
    // Serial.print("s] ");

    // Serial.print("Depth: ");
    // Serial.print(depth, 3); // 保留3位小数
    // Serial.print(" m  |  ");

    // Serial.print("Pressure: ");
    // Serial.print(pressure, 1);
    // Serial.print(" mbar  |  ");

    // Serial.print("Temp: ");
    // Serial.print(temp, 2);
    // Serial.println(" C");

    // 2. 更新决策 (计算状态机、PID、处理数据记录)
    myManager.update();

    // 3. (可选) 如果MotorDriver需要平滑控制，也可以在这里加 update
    // myMotor.update();

    // 4. 控制频率 (简单的延时，或者用 millis 控制固定周期)
    delay(10); 
}