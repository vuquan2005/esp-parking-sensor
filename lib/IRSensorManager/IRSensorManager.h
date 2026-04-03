#ifndef IR_SENSOR_MANAGER_H
#define IR_SENSOR_MANAGER_H

#include <Arduino.h>

// Bạn có thể tùy chỉnh thời gian debounce (ms)
#define DEBOUNCE_DELAY 50

// Mặc định cho cảm biến IR obstacle: HIGH = SENSOR_CLEAR, LOW = SENSOR_DETECTED
#ifndef SENSOR_CLEAR
#define SENSOR_CLEAR HIGH
#endif

#ifndef SENSOR_DETECTED
#define SENSOR_DETECTED LOW
#endif

class IRSensorManager
{
private:
    static const int MAX_SENSORS = 16; // Điều chỉnh tùy nhu cầu thực tế

    String instanceCode;                         // Mã định danh instance (VD: "SC", "SM", "SL")
    int sensorPins[MAX_SENSORS];                 // Mảng các chân cảm biến
    int numSensors;                              // Số lượng cảm biến
    bool lastReadState[MAX_SENSORS];             // Trạng thái đọc thô ở chu kỳ trước
    bool validatedState[MAX_SENSORS];            // Trạng thái đã qua xử lý debounce
    unsigned long lastDebounceTime[MAX_SENSORS]; // Thời điểm trạng thái thay đổi cuối cùng

    void sendSensorState(int index);

public:
    // Khởi tạo class truyền vào mã instance, mảng chân và số lượng
    IRSensorManager(String code, const int pins[], int count);

    // Gọi trong setup()
    void begin();

    // Gọi trong loop()
    void update();

    // Gọi để gửi toàn bộ trạng thái cảm biến 
    void sendAllStates();

    // Xử lý chuỗi lệnh từ UART (VD: "SC", "SM", "SL")
    void processCommand(String command);
};

#endif