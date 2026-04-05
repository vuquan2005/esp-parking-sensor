#include "IRSensorManager.h"
#include <Arduino.h>

// Sử dụng các GPIO an toàn cho ESP32 (tránh GPIO 6-11, 1-3, và các pin flash/boot không phù hợp)
const int irPinsSC[] = {2, 4, 5, 16, 17, 18};
const int NUM_SC = sizeof(irPinsSC) / sizeof(irPinsSC[0]);

const int irPinsSM[] = {19, 21, 22, 23, 25, 26};
const int NUM_SM = sizeof(irPinsSM) / sizeof(irPinsSM[0]);

const int irPinsSE[] = {27, 32, 33, 34, 35, 36};
const int NUM_SE = sizeof(irPinsSE) / sizeof(irPinsSE[0]);

const int irPinsSL[] = {39, 13, 14, 15, 12, 0};
const int NUM_SL = sizeof(irPinsSL) / sizeof(irPinsSL[0]);
// Test 2, 34, 35 không nhận

// Khởi tạo 4 đối tượng quản lý cảm biến
// SC: Pallet có xe không
IRSensorManager SCar("SC", irPinsSC, NUM_SC);

// SH: Pallet có ở home không
IRSensorManager SMoveHome("SH", irPinsSM, NUM_SM);

// SE: Pallet có ở end không
IRSensorManager SMoveEnd("SE", irPinsSE, NUM_SE);

// SL: Có chạm limit không
IRSensorManager SLimit("SL", irPinsSL, NUM_SL);

void setup() {
    Serial.begin(115200);

    // Khởi tạo các chân INPUT cho cả 3 cụm
    SCar.begin();
    SMoveHome.begin();
    SMoveEnd.begin();
    SLimit.begin();

    Serial.println("He thong SS san sang!");
}

void loop() {
    // 1. Quét cảm biến liên tục cho 3 cụm
    SCar.update();
    SMoveHome.update();
    SMoveEnd.update();
    SLimit.update();

    // 2. Nhận lệnh UART (VD: gõ "SC", "SM", "SL" từ Serial Monitor)
    if (Serial.available()) {
        String cmd = Serial.readStringUntil('\n');

        SCar.processCommand(cmd);
        SMoveHome.processCommand(cmd);
        SMoveEnd.processCommand(cmd);
        SLimit.processCommand(cmd);

        // SS
        if (cmd.equals("SS")) {
            // Gửi SC và SL dưới dạng SS <SC> <SL>
            Serial.print("SS ");
            Serial.print(SCar.getAllStates());
            Serial.print(" ");
            Serial.println(SLimit.getAllStates());
        }
    }
}