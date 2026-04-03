#include <Arduino.h>
#include "IRSensorManager.h"

// Gán tạm các chân cắm cảm biến cho 3 cụm. (Thay đổi theo mạch thực tế)
const int irPinsSC[] = {2, 3, 4, 5, 6, 7};
const int NUM_SC = sizeof(irPinsSC) / sizeof(irPinsSC[0]);

const int irPinsSM[] = {8, 9, 10, 11, 12, 13};
const int NUM_SM = sizeof(irPinsSM) / sizeof(irPinsSM[0]);

const int irPinsSL[] = {14, 15, 16, 17, 18, 19};
const int NUM_SL = sizeof(irPinsSL) / sizeof(irPinsSL[0]);

// Khởi tạo 3 đối tượng quản lý cảm biến
IRSensorManager SCar("SC", irPinsSC, NUM_SC);
IRSensorManager SMove("SM", irPinsSM, NUM_SM);
IRSensorManager SLimit("SL", irPinsSL, NUM_SL);

void setup()
{
    Serial.begin(115200);

    // Khởi tạo các chân INPUT cho cả 3 cụm
    SCar.begin();
    SMove.begin();
    SLimit.begin();

    Serial.println("He thong SS san sang!");
}

void loop()
{
    // 1. Quét cảm biến liên tục cho 3 cụm
    SCar.update();
    SMove.update();
    SLimit.update();

    // 2. Nhận lệnh UART (VD: gõ "SC", "SM", "SL" từ Serial Monitor)
    if (Serial.available())
    {
        String cmd = Serial.readStringUntil('\n');

        SCar.processCommand(cmd);
        SMove.processCommand(cmd);
        SLimit.processCommand(cmd);
    }
}