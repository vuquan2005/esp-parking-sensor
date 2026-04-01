#include "IRSensorManager.h"

IRSensorManager::IRSensorManager(const int pins[], int count)
{
    numSensors = (count <= MAX_SENSORS) ? count : MAX_SENSORS;

    for (int i = 0; i < numSensors; i++)
    {
        sensorPins[i] = pins[i];
        lastReadState[i] = SENSOR_CLEAR; // Mặc định trạng thái không phát hiện vật
        validatedState[i] = SENSOR_CLEAR;
        lastDebounceTime[i] = 0;
    }
}

void IRSensorManager::begin()
{
    for (int i = 0; i < numSensors; i++)
    {
        pinMode(sensorPins[i], INPUT);
        lastReadState[i] = digitalRead(sensorPins[i]);
        validatedState[i] = lastReadState[i];
    }
}

void IRSensorManager::update()
{
    unsigned long currentMillis = millis();

    for (int i = 0; i < numSensors; i++)
    {
        bool currentState = digitalRead(sensorPins[i]);

        // Nếu trạng thái thô thay đổi so với lần đọc trước, reset timer
        if (currentState != lastReadState[i])
        {
            lastDebounceTime[i] = currentMillis;
        }

        // Nếu trạng thái thô giữ nguyên đủ lâu (vượt qua DEBOUNCE_DELAY)
        if ((currentMillis - lastDebounceTime[i]) > DEBOUNCE_DELAY)
        {
            // Và nếu nó khác với trạng thái chính thức đang lưu
            if (currentState != validatedState[i])
            {
                validatedState[i] = currentState;

                // Trạng thái BẤT KỲ thay đổi -> Tự động gửi qua UART
                sendSensorState(i);
            }
        }

        lastReadState[i] = currentState;
    }
}

void IRSensorManager::processCommand(String command)
{
    command.trim();        // Xóa khoảng trắng thừa hoặc ký tự \n \r
    command.toUpperCase(); // Chuẩn hóa thành chữ hoa

    if (command.startsWith("S"))
    {
        // Cắt bỏ chữ 'S', lấy phần số
        String numStr = command.substring(1);
        int sensorNumber = numStr.toInt();

        int index = sensorNumber - 1; // Người dùng nhập S1 -> index = 0

        if (index >= 0 && index < numSensors)
        {
            sendSensorState(index);
        }
        else
        {
            Serial.println("Loi: Cam bien khong ton tai!");
        }
    }
}

void IRSensorManager::sendSensorState(int index)
{
    // Dùng định nghĩa chung để linh động với nhiều loại cảm biến
    // SENSOR_CLEAR = trạng thái bình thường, SENSOR_DETECTED = phát hiện vật (hoặc sự kiện)
    String stateStr = (validatedState[index] == SENSOR_DETECTED) ? "1" : "0";

    Serial.print("S");
    Serial.print(index + 1); // Trả về dạng S1, S2...
    Serial.print(":");
    Serial.println(stateStr);
}