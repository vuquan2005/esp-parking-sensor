#include <Arduino.h>
#include "IRSensorManager.h"

// Khai báo mảng chứa các chân cắm cảm biến (Thay đổi theo mạch thực tế của bạn)
const int irPins[] = {2, 3, 4, 5, 6, 7, 8, 9 /* ... thêm đủ 30 chân */};
const int NUM_SENSORS = sizeof(irPins) / sizeof(irPins[0]);

// Khởi tạo đối tượng quản lý cảm biến
IRSensorManager irManager(irPins, NUM_SENSORS);

void setup()
{
    Serial.begin(115200);

    // Gọi begin() trong setup() để khởi tạo các chân INPUT
    irManager.begin();

    Serial.println("He thong SS san sang!");
}

void loop()
{
    // 1. Quét cảm biến liên tục: Xử lý debounce và tự động gửi nếu có thay đổi
    irManager.update();

    // 2. Nhận lệnh UART (VD: gõ "S1", "S2" từ Serial Monitor)
    if (Serial.available())
    {
        String cmd = Serial.readStringUntil('\n');
        irManager.processCommand(cmd);
    }
}