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
// SC: Pallet có xe không
IRSensorManager SCar("SC", irPinsSC, NUM_SC);

// SH: Pallet có ở home không
IRSensorManager SMoveHome("SH", irPinsSM, NUM_SM);

// SE: Pallet có ở end không
IRSensorManager SMoveEnd("SE", irPinsSM, NUM_SM);

// SL: Có chạm limit không
IRSensorManager SLimit("SL", irPinsSL, NUM_SL);

void setup()
{
    Serial.begin(115200);

    // Khởi tạo các chân INPUT cho cả 3 cụm
    SCar.begin();
    SMoveHome.begin();
    SMoveEnd.begin();
    SLimit.begin();

    Serial.println("He thong SS san sang!");
}

void loop()
{
    // 1. Quét cảm biến liên tục cho 3 cụm
    SCar.update();
    SMoveHome.update();
    SMoveEnd.update();
    SLimit.update();

    // 2. Nhận lệnh UART (VD: gõ "SC", "SM", "SL" từ Serial Monitor)
    if (Serial.available())
    {
        String cmd = Serial.readStringUntil('\n');

        SCar.processCommand(cmd);
        SMoveHome.processCommand(cmd);
        SMoveEnd.processCommand(cmd);
        SLimit.processCommand(cmd);
    }
}