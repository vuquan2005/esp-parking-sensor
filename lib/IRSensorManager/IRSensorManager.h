#ifndef IR_SENSOR_MANAGER_H
#define IR_SENSOR_MANAGER_H

#include <Arduino.h>

// Bạn có thể tùy chỉnh thời gian debounce (ms)
#define DEBOUNCE_DELAY 20

// Mặc định cho cảm biến IR obstacle: HIGH = SENSOR_CLEAR, LOW = SENSOR_DETECTED
#ifndef SENSOR_CLEAR
#define SENSOR_CLEAR HIGH
#endif

#ifndef SENSOR_DETECTED
#define SENSOR_DETECTED LOW
#endif

class IRSensorManager {
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

    // Trả về chuỗi trạng thái của tất cả cảm biến (VD: "10110011") mà không gửi qua UART
    String getAllStates();

    // Lấy trạng thái của một cảm biến cụ thể (trả về true nếu phát hiện - SENSOR_DETECTED)
    bool getSensorState(int index);

    // Lấy trạng thái "thô" (chưa qua debounce) của một cảm biến cụ thể
    bool getRawSensorState(int index);

    // Truy cập trực tiếp vào mảng dữ liệu gốc
    const bool *getValidatedStatesArray() const {
        return validatedState;
    }
    const bool *getRawStatesArray() const {
        return lastReadState;
    }

    // Gọi để gửi toàn bộ trạng thái cảm biến
    void sendAllStates();

    // Xử lý chuỗi lệnh từ UART (VD: "SC", "SM", "SL")
    void processCommand(String command);
};

#endif