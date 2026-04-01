# 📋 Project: Smart Parking System - Motor Control Dashboard

## 🔴 ƯU TIÊN 1: BẢO VỆ PHẦN CỨNG (Cần làm ngay)
*Ngăn chặn hỏng hóc vật lý cho ESP32 và đảm bảo an toàn vận hành.*

- [ ] **Hạ áp tín hiệu 5V TTL về 3.3V cho GPIO ESP32**
    - **Vấn đề:** Các cảm biến/limit switch dùng nguồn 5V sẽ làm cháy GPIO của ESP32 (chỉ chịu được 3.3V).
    - **Phương án 1 (Khuyên dùng):** Dùng trở 1kΩ nối tiếp + Zener 3.3V (ghim áp) + tụ 104 (lọc nhiễu).
    - **Phương án 2:** Cầu phân áp (Trở 1kΩ và 2kΩ).
- [ ] **Lắp Diode chống dòng ngược (Back-EMF)**
    - **Vị trí:** Mắc song song với mỗi motor (4 motor = 4 diode).
    - **Linh kiện:** Diode 1N4007.
    - **Cách mắc:** Cực Cathode (có vạch trắng) nối cực Dương (+), Anode nối cực Âm (-) của motor.

---

## 🟡 ƯU TIÊN 2: CHỐNG NHIỄU & ỔN ĐỊNH NGUỒN
*Đảm bảo hệ thống không bị treo hoặc nhảy lệnh sai khi motor khởi động.*

- [ ] **Thêm tụ lọc nguồn tại chân vào L298N**
    - **Linh kiện:** 01 tụ hóa 100µF/16V (lọc thấp) + 01 tụ gốm 100nF (lọc cao/nhiễu kim) tại mỗi module driver.
- [ ] **Bọc nhiễu (Shielding) cho dây tín hiệu**
    - **Thực hiện:** Quấn giấy bạc quanh bó dây tín hiệu.
    - **Lưu ý quan trọng:** Chỉ nối 1 đầu giấy bạc vào điểm tiếp địa (GND Star Point) phía tủ điện. Đầu phía motor để hở.
- [ ] **Tối ưu hóa layout dây dẫn**
    - Duy trì khoảng cách ít nhất 1-2cm giữa dây động lực (12V) và dây tín hiệu (UART/GPIO).

---

## 🔵 ƯU TIÊN 3: TỐI ƯU PHẦN MỀM (FIRMWARE)
*Tăng tính thông minh và khả năng tự phục hồi khi có lỗi truyền thông.*

- [ ] **Triển khai Heartbeat / Safety Timeout**
    - **Logic:** Slave tự động ngắt toàn bộ motor nếu sau 500ms không nhận được "nhịp đập" từ Master.
- [ ] **Xử lý Debounce (Chống rung phím) cho Limit Switch**
    - Sử dụng kỹ thuật `millis()` để kiểm tra trạng thái switch ổn định trong ít nhất 20-50ms trước khi xác nhận dừng motor.
- [ ] **Thêm Checksum cho gói tin UART**
    - **Phương án:** Thêm 1 byte CRC đơn giản vào cuối mỗi chuỗi lệnh gửi đi. Slave tính toán lại, nếu khớp mới thực thi.
- [ ] **Kích hoạt Watchdog Timer (WDT)**
    - Cấu hình WDT trên ESP32 để tự khởi động lại (Reset) nếu code bị kẹt trong vòng lặp vô hạn do nhiễu EMI cực mạnh.

---

## ✅ CÁC VIỆC ĐÃ HOÀN THÀNH (STABLE)
*Các hạng mục đã kiểm chứng và hoạt động tốt.*

- [x] **Giải pháp Shielding:** Đã xác định phương án dùng giấy bạc nối PE 1 đầu.
- [x] **Xử lý GND:** Đã cấu hình GND chung (Common Ground) để tránh lệch mức logic digital.
- [x] **Truyền thông:** UART được xác định là đủ khả năng chịu nhiễu trong môi trường này.
- [x] **Quy trình vận hành:** Logic chạy 1 motor tại một thời điểm giúp giảm tổng nhiễu EMI xuống mức thấp nhất.
- [x] **Lọc nhiễu trực tiếp:** Đã gắn tụ 104 trên thân motor để triệt tiêu tia lửa điện tại chổi than.

---

### Ghi chú kỹ thuật cho VuQuan:
> *Hãy ưu tiên làm mạch Level Shifter (5V -> 3.3V) trước khi cấp điện chạy thật. ESP32 rất nhạy cảm với quá áp, chỉ cần một spike 5V từ limit switch cũng có thể làm hỏng chân GPIO hoặc toàn bộ chip.*