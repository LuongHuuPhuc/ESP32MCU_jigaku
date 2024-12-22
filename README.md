# **Lập trình vi điều khiển Esp32 sử dụng framework Esp-Idf** #
## Esp32 Dev Board Datasheet ##
![image](https://github.com/user-attachments/assets/78d933e0-ab94-4725-8c80-3256b728e818)
## **Cài đặt phần mềm** ##
* Download phiên bản mới nhất của Espressif qua gitHub (cho Windows)"
[Espressif gitHub](https://github.com/espressif/esp-idf)
* Sau khi cài đặt phần mềm sẽ tạo ra cho chúng ra 2 trình command prompt để hiển thị mỗi khi lập trình cho Esp32:
  ![image](https://github.com/user-attachments/assets/81a37dcb-e5cb-4623-b240-5a10183e09de)
> Tốt nhất khi cài đặt hãy tạo riêng 1 folder cho phần mềm để đễ dàng kiểm soát
  Trong framework tải về nhà phát hành đã có sẵn những project nhỏ bên trong để chúng ta thực hành
## **Thực hiện cấu hình và build project blink đầu tiên** ##
1. Vào trong folder của Esp32 đã cài đặt -> vào thư mục framworks -> `esp-idf` -> `examples` -> `get started`
2. Copy và Paste chương trình `blink` bên trong ra ngoài song song với folder chính của Esp32:
   ![image](https://github.com/user-attachments/assets/1e24fe1c-2282-41a1-b7f6-aedcffd56039)
3. Mở command prompt của Esp32, sau đó cd vào folder blink (chúng ta có thể dùng Visual Studio Code hoặc bất cứ trình editor code nào để mở tất cả các file code trong folder này lên), ngoài ra nếu muốn quay lại folder cũ thì gõ `cd..`
4. Vào lại command prompt và gõ `ls` để xem toàn bộ danh sách file có trong thư mục blink
5. Tiếp theo dùng lệnh `idf.py menuconfig` để chạy giao điện cấu hình cho Esp32
> idf.py: Đây là một công cụ dòng lệnh giúp quản lý toàn bộ quá trình phát triển, từ khởi tạo dự án, cấu hình, biên dịch, flash mã, đến theo dõi nhật ký từ ESP32.
6. Sau khi chạy xong toàn bộ, màn hình command sẽ hiển thị giao diện như sau: 
![image](https://github.com/user-attachments/assets/840fe240-b7dc-4f69-b536-853400eaa8e4)
7. Trong mục "Serial flasher config", chỉnh "flash size" thành 4MB. Trong "Example configuration" chuyển chân Blink GPIO thành chân 2 (lý do vì trong bản diagram của Esp32 được thiết lập sẵn chân 2 là chân led blink)
8. Sau đó thoát giao diện config bằng nút "esc" trong bàn phím
9. Sau đó dùng lệnh `idf.py build` để thực hiện build dự án blink
> Bản chất viêc idf.py build trên command của Esp32 là máy sẽ chạy "Makefile" bên trong folder blink
![image](https://github.com/user-attachments/assets/0fb86ab0-1f1b-42ce-b797-72e2b0049cf2)
> Ảnh trên cho thấy, nó build ra các file bootloader.bin, partition_table.bin, blink.bin(đây chính là cái app của chúng ta). Những cái vùng flash 0x1000,... như chúng ta thấy bên trên là các vùng bộ nhớ mà các file được nạp vào.
10. Bước quan trọng nhất để nạp chương trình vào vi điều khiển là dùng lệnh `idf.py -p COM(SỐ..)flash monitor `, chương trình sẽ ngay lập tức nạp vào bộ nhớ flash của mcu, vừa nạp vừa giữ nút Boot trên vi điều khiển:
![image](https://github.com/user-attachments/assets/1bc6a018-3c25-4228-93d7-d2be24d1e4b6)
* Sau khi màn hình hiển thị ra dòng như sau:
  ![image](https://github.com/user-attachments/assets/bb562041-4b73-4902-a94a-514e729c436a)
### Như vậy là đèn đã blink thành công ###
* Để thoát chương trình trên command, ta dùng tổ hợp phím "Ctrl + }" để thoát
### **Xóa các file đã build trong quá trình biên dịch trước đó** ###
* Dùng lệnh `idf.py fullclean`
* Khi đó terminal sẽ ra lệnh xóa toàn bộ dữ liệu cũ và đưa dự án về trạng thái ban đầu
### **Đưa Esp32 về trạng thái ban đầu khi chưa nạp code** ###
* Dùng lệnh `idf.py erase_flash`
* Quá trình này sẽ xóa sạch toàn bộ bộ nhớ flash, bao gồm bootloader, firmware, và các phần dữ liệu khác.
***
## *Cách để thêm thư viện ngoài (external) vào esp-idf* ##
1. Sử dụng Git Submodule
   - Nếu thư viện bạn muốn thêm vào có sẵn trên Github hoặc một repo Git khác, các bạn có thể sử dụng Git Submodule để tự động tải thư viện vào dự án của mình
   - Các bước:
    * Thêm Submodule vào dự án của bạn: `git submodule add <URL của repo Git> components/ten_thu_vien`
      >ví dụ, bạn muốn thêm thư viện ssd1306 từ gitHub, bạn sẽ làm như sau:
      `git submodule add https://github.com/ThingPulse/esp8266-oled-ssd1306.git components/ssd1306`
    * Cập nhật submodule sau khi clone dự án: `git submodule update --init --recursive`
   - Cấu hình lại **CMakeLists.txt** (nếu cần thiết): Nếu thư viện không tự động thêm vào đúng chỗ, bạn sẽ cần cấu hình lại **CMakeLists.txt** của dự án để tham chiếu thư viện
     > Lưu ý là nên tạo 1 folder `**components**` riêng bên trong dự án của bạn chứ không nên tạo vào folder `**components**` thư viện chuẩn của esp-idf. Bởi vì:
     > - Cập nhật Esp-idf có thể ghi đè lên thư mục hoặc làm xung đột với thư viện bạn thêm vào.
     > - Giúp tách biệt mã nguồn của esp-idf với các thư viện bên ngoài, và giúp bạn dễ dàng quản lý.

2. Sử dụng `idf.py` với `components`
   - Esp-idf có một cơ chế đơn giản để tự động tải thư viện ngoài bằng cách sử dụng `idf.py` kết hợp với hệ thống quản lý component của esp-idf
   - Các bước:
     * Tạo một thư mục con trong `components` (nếu thư viện không có sẵn trong esp-idf, bạn có thể tải về thư viện của mình vào thư mục `components`)
     * Sử dụng lệnh (command-line tool) `idf.py` để tải và quản lý thư viện
     > Ví dụ nếu bạn muốn thêm thư viện `**Esp32 Oled Ssd1306**` bạn có thể sử dụng một thư viện như sau:
     > `idf.py add -dependency <URL của thư viện>`
   Lệnh trên sẽ tự động tải về và cấu hình thư viện theo cách tương tự như cách bạn thêm submodule trong Git.

3. Sử dụng platformIO (dùng Visual Code Studio)
   - PlatformIO là một công cụ mạnh mẽ để phát triển với Esp-idf và hỗ trựo dễ dàng việc thêm thư viện ngoài dự án
   - Các bước:
     * Tạo hoặc mở dự án esp-idf trong platformIo
     * Thêm thư viện vào `platformio.ini`
     Mở file `platformio.ini` và thêm thư viện vào mục `lib_deps`:
```
[env:esp32]
platform = espressif32
framework = espidf
lib_deps =
    ThingPulse/SSD1306@^2.0.0
```
   * Build lại dự án: `platform run`. PlatformIO sẽ tự động tải và thêm thư viện vào dự án của bạn

