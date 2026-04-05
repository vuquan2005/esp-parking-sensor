#include "IRSensorManager.h"

IRSensorManager::IRSensorManager(String code, const int pins[], int count) {
    instanceCode = code;
    numSensors = (count <= MAX_SENSORS) ? count : MAX_SENSORS;

    for (int i = 0; i < numSensors; i++) {
        sensorPins[i] = pins[i];
        lastReadState[i] = SENSOR_CLEAR;
        validatedState[i] = SENSOR_CLEAR;
        lastDebounceTime[i] = 0;
    }
}

void IRSensorManager::begin() {
    for (int i = 0; i < numSensors; i++) {
        pinMode(sensorPins[i], INPUT_PULLUP);
        lastReadState[i] = digitalRead(sensorPins[i]);
        validatedState[i] = lastReadState[i];
    }
}

void IRSensorManager::update() {
    unsigned long currentMillis = millis();

    for (int i = 0; i < numSensors; i++) {
        bool currentState = digitalRead(sensorPins[i]);

        // Nếu trạng thái thô thay đổi so với lần đọc trước, reset timer
        if (currentState != lastReadState[i]) {
            lastDebounceTime[i] = currentMillis;
        }

        // Nếu trạng thái thô giữ nguyên đủ lâu (vượt qua DEBOUNCE_DELAY)
        if ((currentMillis - lastDebounceTime[i]) > DEBOUNCE_DELAY) {
            // Và nếu nó khác với trạng thái chính thức đang lưu
            if (currentState != validatedState[i]) {
                validatedState[i] = currentState;

                // Trạng thái BẤT KỲ thay đổi -> Tự động gửi qua UART theo format: <Mã> <Index> <Giá
                // trị>
                sendSensorState(i);
            }
        }

        lastReadState[i] = currentState;
    }
}

void IRSensorManager::processCommand(String command) {
    command.trim();
    command.toUpperCase();

    // Nếu lệnh trùng khớp với mã của instance này (VD: "SC"), in toàn bộ chuỗi bit
    if (command.equals(instanceCode)) {
        sendAllStates();
    }
}

void IRSensorManager::sendAllStates() {
    String stateString = getAllStates();

    // Gửi gộp lại trên một dòng. VD: "SC 10110011"
    Serial.print(instanceCode);
    Serial.print(" ");
    Serial.println(stateString);
}

String IRSensorManager::getAllStates() {
    String stateString = "";
    for (int i = 0; i < numSensors; i++) {
        stateString += (validatedState[i] == SENSOR_DETECTED) ? "1" : "0";
    }
    return stateString;
}

bool IRSensorManager::getSensorState(int index) {
    if (index >= 0 && index < numSensors) {
        return validatedState[index] == SENSOR_DETECTED;
    }
    return false;
}

bool IRSensorManager::getRawSensorState(int index) {
    if (index >= 0 && index < numSensors) {
        // Trả về trạng thái "đọc thô" từ chu kỳ trước
        return lastReadState[index] == SENSOR_DETECTED;
    }
    return false;
}

void IRSensorManager::sendSensorState(int index) {
    // Dùng định nghĩa chung để linh động với nhiều loại cảm biến
    // SENSOR_CLEAR = trạng thái bình thường, SENSOR_DETECTED = phát hiện vật (hoặc sự kiện)
    String stateStr = (validatedState[index] == SENSOR_DETECTED) ? "1" : "0";

    // Format cập nhật 1 cảm biến: <Mã> <Index> <Giá trị> (VD: "SC 1 1")
    Serial.print(instanceCode);
    Serial.print(" ");
    Serial.print(index + 1);
    Serial.print(" ");
    Serial.println(stateStr);
}