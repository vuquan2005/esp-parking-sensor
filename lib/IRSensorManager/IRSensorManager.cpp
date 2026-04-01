#include "IRSensorManager.h"

IRSensorManager::IRSensorManager(const int pins[], int count)
{
    numSensors = (count <= MAX_SENSORS) ? count : MAX_SENSORS;

    for (int i = 0; i < numSensors; i++)
    {
        sensorPins[i] = pins[i];
        lastReadState[i] = SENSOR_CLEAR;
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
    // Format lệnh mong đợi: "S {sensor_number}" (VD: "S 1" để yêu cầu trạng thái cảm biến 1)
    command.trim();
    command.toUpperCase();

    if (command.startsWith("S ") && command.length() > 2)
    {
        String numStr = command.substring(2); // Cắt bỏ "S " và lấy phần số
        int sensorNumber = numStr.toInt();

        int index = sensorNumber - 1; // Người dùng nhập S1 -> index = 0

        if (index >= 0 && index < numSensors)
        {
            sendSensorState(index);
        }
        else
        {
            // Phản hồi ERROR
            Serial.println("S E");
        }
    }
}

void IRSensorManager::sendSensorState(int index)
{
    // Dùng định nghĩa chung để linh động với nhiều loại cảm biến
    // SENSOR_CLEAR = trạng thái bình thường, SENSOR_DETECTED = phát hiện vật (hoặc sự kiện)
    String stateStr = (validatedState[index] == SENSOR_DETECTED) ? "1" : "0";

    // Format: "S{sensor_number} {state}\n" (VD: "S1 1\n" nghĩa là cảm biến 1 phát hiện vật)
    Serial.print("S ");
    Serial.print(index + 1);
    Serial.print(" ");
    Serial.println(stateStr);
}