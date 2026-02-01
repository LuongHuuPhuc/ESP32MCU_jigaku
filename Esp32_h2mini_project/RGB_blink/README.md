# BLINK RGB LED WS2812 (Neopixel) #
- Rất nhiều board ESP32-C3/S3,... mini gắn sẵn 1 con LED RGB WS2812 địa chỉ 
- Con này cần 1 protocol đặc biệt, không điều khiển được bằng PWM bình thường
- Trong ESP-IDF, Espressif có sẵn RMT driver (Remote Control Transceiver) + LED strip library đề điều khiển
  * Ban đầu, Espressif làm để thu phát tín hiệu hồng ngoại (IR), vốn có dạng "pulses" theo thời gian chính xác cao (NEC, RC5, Sony IR,...)

  * Về sau, vì nó rất linh hoạt trong việc phát/nhận chuỗi xung vuông có độ rộng/thời gian chính xác (us hoặc ns), nên RMT được dùng cho: 
    - LED RGB WS2812/Neopixel (**protocol 1-wire timing-based**)
    - Giao tiếp điều khiển remote
    - DHT11/DHT22 (đọc sensor 1-wire dạng bit-timing)
    - IR Transmitter/Receiver 
    - Mấy giao thức tự chế timing-based khác

## CƠ CHẾ HOẠT ĐỘNG CỦA RMT ##
- RMT là một bộ tạo/ghi waveform dựa trên thời gian, bạn chỉ cần "dịch dữ liệu" sang các symbol, còn chuyện giữ timing đã có hardware lo.

- RMT có channel riêng, mỗi channel có FIFO để lưu **"items (symbol)"**, mỗi item mô tả một xung gồm 2 pha:
  - `level0` và `duration0` -> Mức điện áp + thời gian giữ mức này
  - `level1` và `duration1` -> Mức điện áp + thời gian giữ mức nay
- **Cách biểu diễn dữ liệu:** Muốn gửi một chuỗi dữ liệu (ví dụ 24bit màu cho LED), ta phải chuyển từng bit thành các symbol RMT
  - Ví dụ: Để tạo ra 1 bit "0" trong WS2812, bạn cần phải phát ~400ns HIGH + ~850ns LOW. Nếu dùng `gpio_set_level` thì CPU không thể chính xác được, nhưng RMT thì làm "cực mượt"

|Bit | HIGH time (T0H/T1H) | LOW time (T0L/T1L) |
|----|---------------------|--------------------|
| 0 | ~0.4us | ~0.85us |
| 1 | ~0.8us | ~0.45us |

- Như vậy: 
  * Bit `0` -> HIGH 0.4us + LOW 0.85us
  * Bit `1` -> HIGH 0.8us + LOW 0.45us

```c
rmt_symbol_word_t symbol = {
  .level0 = 1, .duration0 = 400, // HIGH trong 400ns
  .level1 = 0, .duration1 = 850 //LOW trong 850s
};
```
👉 Đây chính là 1 bit “0” theo chuẩn WS2812.

### Điểm mạnh của RMT ###
- Khi bạn nạp dãy items vào RMT, phần cứng sẽ tự động xuất hiện tín hiệu với timing chính xác, không cần CPU bận rộn
- Cho phép encode phức tạp (ví dụ: NEC IR, WS2812)
- Hỗ trợ cả TX (phát) và RX (thu)


## TRONG ESP-IDF ##
* Bạn có RMT driver API: 
  - `rmt_config()`: Cấu hình channel (TX/RX, GPIO, clock driver)
  - `rmt_driver_install()`: Khởi tạo driver 
  - `rmt_driver_items()`: Ghi dãy items để phát
  - `rmt_rx_start()`: Bắt đầu nhận tín hiệu
* Ngoài ra, có library cao hơn như `led_strip` đã wrap sẵn RMT cho WS2812, bạn chỉ cần việc gọi `set_pixel()` thay vì tự tạo items
* Thư viện `led_strip`, trước đây (bản ESP-IDF 4.x) Espressif có ví dụ demo WS2812 bằng RMT, nhưng không đóng gói thành component chuẩn
* Từ ESP-IDF 5.x trở đi, Espressif tách nhiều "component phụ" ra khỏi core ESP-IDF và đưa lên ESP Component Registry (https://components.espressif.com), và `led_strip()` là một trong các component ngoài core, nên phải khai báo trong `idf_component.yml` để tải 