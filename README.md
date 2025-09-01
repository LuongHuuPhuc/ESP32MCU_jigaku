# **Lập trình vi điều khiển ESP32 với framework ESP-IDF** #
## ESP32 Dev Board Datasheet ##
![image](https://github.com/user-attachments/assets/78d933e0-ab94-4725-8c80-3256b728e818)
## **Cài đặt phần mềm** ##
* Download phiên bản mới nhất của Espressif qua gitHub (cho Windows)"
[Espressif gitHub](https://github.com/espressif/esp-idf)
* Sau khi cài đặt phần mềm sẽ tạo ra cho chúng ra 2 trình command prompt để hiển thị mỗi khi lập trình cho Esp32: <br>
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
### 1. Sử dụng Git Submodule (Recommend) ###
- Nếu thư viện bạn muốn thêm vào có sẵn trên Github hoặc một repo Git của người khác, các bạn có thể sử dụng Git Submodule để tự động tải thư viện vào dự án của mình
- Các bước:
    * Thêm Submodule vào dự án của bạn:
      - Lệnh này sẽ tạo ra file `.gitmodules` đồng thời tự động clone repo con từ url 
      ``` bash
      git submodule add <URL của repo Git> components/ten_thu_vien`
      ```
      > ví dụ, bạn muốn thêm thư viện ssd1306 từ gitHub, bạn sẽ làm như sau:
      `git submodule add https://github.com/ThingPulse/esp8266-oled-ssd1306.git components/ssd1306`
    * Cập nhật submodule sau khi clone dự án (nếu submodule có thay đổi):
      - Dùng để đồng bộ với những thay đổi commit trong repo của người khác với con trỏ hiện tại trong repo của bạn. 
      ``` bash
      git submodule update --init --recursive`
      ```
- Cấu hình lại **CMakeLists.txt** (nếu cần thiết): Nếu thư viện không tự động thêm vào đúng chỗ, bạn sẽ cần cấu hình lại **CMakeLists.txt** của dự án để tham chiếu thư viện
     > Lưu ý là nên tạo 1 folder `**components**` riêng bên trong dự án của bạn chứ không nên tạo vào folder `**components**` thư viện chuẩn của esp-idf. Bởi vì:
     > - Cập nhật Esp-idf có thể ghi đè lên thư mục hoặc làm xung đột với thư viện bạn thêm vào.
     > - Giúp tách biệt mã nguồn của esp-idf với các thư viện bên ngoài, và giúp bạn dễ dàng quản lý.
### 2. Sử dụng `idf.py` với `components` ###
   - Esp-idf có một cơ chế đơn giản để tự động tải thư viện ngoài bằng cách sử dụng `idf.py` kết hợp với hệ thống quản lý component của esp-idf
   - Các bước:
     * Tạo một thư mục con trong `components` (nếu thư viện không có sẵn trong esp-idf, bạn có thể tải về thư viện của mình vào thư mục `components`)
     * Sử dụng lệnh (command-line tool) `idf.py` để tải và quản lý thư viện
     > Ví dụ nếu bạn muốn thêm thư viện `**Esp32 Oled Ssd1306**` bạn có thể sử dụng một thư viện như sau:
     > `idf.py add -dependency <URL của thư viện>`
   Lệnh trên sẽ tự động tải về và cấu hình thư viện theo cách tương tự như cách bạn thêm submodule trong Git.

### 3. Sử dụng platformIO (dùng Visual Code Studio) ###
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

 ***
  ### *Xóa Submodule ra khỏi dự án của bạn* ###
  - Quá trình xóa Submodule cần xóa dữ liệu ở nhiều nơi, bao gồm:
    1. File `.gitmodule`
    2. File trong `.git\config`
    3. Thư mục `.git\modules`
    4. Xóa thư mục submodule trong dự án của bạn
***

## **Cách làm việc với `git submodule`** ##
`git submodule` là lệnh giúp bạn nhúng một repo Git bên trong một repo khác. Điều này rất hữu ích khi bạn muốn sử dụng thư viện bên ngoài (ví dụ như driver hay SDK,...) nhưng vẫn giữ liên kết với nguồn gốc của thư viện đó (repo chính chủ ) <br>
### 1. **Thêm submodule** ###
   ```bash
   git submodule add <repo_url> <path>
   ```
   Lệnh này sẽ tự động clone repo vào thư mục bạn chỉ định, ghi link vào `.gitmodules`
   > ví dụ: `git submodule add https://github.com/lexus2k/ssd1306 components/SSD1306`
### 2. **Cập nhật submodule** ###
   ```bash
   git submodule update --init --recursive 
   ```
   > `--init`: Init nếu submodule chưa được clone <br>
   > `--recursive` Áp dụng cho cả submodule lồng trong <br>
   > Dùng sau khi clone về lần đầu hoặc khi người khác gửi bạn repo có submodule

   - Lệnh này được dùng để đồng bộ code trong repo con (hay thư mục submodule) về đúng commit mà repo cha đang tham chiếu đến (được lưu trong file `.gitmodules` và commit commit repo cha)
   - Nó không tự động `fetch` commit mới nhất từ remote của repo con submodule mà chỉ `checkout` đúng commit đã được ghi nhận.
   - Đơn giản: Repo con (submodule) khi thay đổi -> Sinh ra commit hash mới -> repo cha muốn đồng bộ thì cần phải update con trỏ để trỏ đến đúng commit mới nhât của repo bằng lệnh trên !

### 3. **Cập nhật submodule từ chính chủ (upstream)** ###
Bạn cần:
 - Thêm remote `upstream` vào bên trong submodule:
   ```bash
   cd <path>
   git remote add upstream https://github...<repo chính chủ>
   ```
- Lấy các update từ chính chủ (update từ remote về nhưng không merge và không ảnh hưởng đến code hiện tại ở local)
  ```bash
  git fetch upstream
  ```
- Gộp hoặc rebasing
  ```bash
  git merge upstream/master
  # hoặc
  git rebase upstream/master
  ```
### 4. **Tùy chỉnh submodule để sửa đổi** <br> ###
   Nếu bạn muốn lưu submodule về máy của hay local repo của bạn để:
   - Có thể sửa mã mà không ảnh hưởng đến repo chính chủ
   - Dễ push code lên remote repo riêng của bạn
   - Vẫn có thể cập nhật và update những thay đổi mới từ repo chính chủ
 
### Cách 1: Fork Repo gốc từ chính chủ ###
👉Làm như sau:
* Bước 1: Lên GitHub, vào repo người khác -> bấm fork để tạo bản sao về gitHub của bạn. Repo fork này hoàn toàn thuộc quyền của bạn, commit tự do
* Bước 2: Clone repo đã fork đó về máy bạn (thành local repo)
* Bước 3: Thêm remote `upstream` trỏ về repo gốc
  
  ```bash
  git clone https://github.com/<your-username>/<forked-repo>.git
  cd <forked-repo>
  ```
* Bước 3: Thêm remote `upstream` trỏ về repo gốc (chính chủ, dùng cho mục đích cập nhật thông tin, thay đổi sau này nếu có)
  ```bash
  git remote add upstream https://github.com/<original-owner>/<original-repo>.git
  ```
  Kiểm tra bằng: `git remote -v` sẽ thấy có 2 remote:
  - `origin` -> repo fork của bạn (push code ở đây)
  - `upstream` -> repo gốc chính chủ (chỉ kéo update về)
* Bước 4: Đồng bộ với repo gốc khi người ta update
  ```bash
  git fetch upstream
  git checkout main
  git merge upstream/main
  ```
  hoặc nhanh hơn với rebase
  ``` bash
  git pull upstream main --rebase
  ```
* Bước 5: Push lên repo fork của bạn
  ```bash
  git push origin main
  ``` 

### Cách 2: Tạo 1 repo rỗng (khi này repo của bạn trên gitHub sẽ không có chữ `forked`) - thủ công ###
👉Làm như sau:
 * Bước 1: Tạo 1 remote repo để chứa thư viện bạn cần fork trên gitHub (repo rỗng)
 * Bước 2: Thêm repo gốc đó thành submodule: 
   ```bash
   git submodule add <repo_url> <path>
   ```
   ✅ Khi đó, `git clone` về sẽ tự động tải repo gốc đó về vị trí `<path>` trong máy của bạn, đồng thời tạo file `.gitmodules`
 * Bước 3: Chuyển `url` submodule của repo chính chủ mà bạn đã `git submodule add` từ đầu sang link repo mà bạn vừa tạo.
   ```bash
   git config -f .gitmodules submodule.<path/to/your/library>.url + <link repo của bạn>
   git submodule sync #Dùng để đồng bộ URL sau khi sửa file .gitmodules
   ```   
* Bước 4: Khi này folder chứa repo submodule mà bạn vừa tải về vẫn đang liên kết `remote` với repo chính chủ, thế nên bạn cần thay đổi nó với link repo chứa thư viện bạn cần fork mà bạn đã tạo lúc nãy.
  ```bash
  git remote set-url origin + <link repo của bạn> # Lúc này origin sẽ lấy bạn làm mặc định
  ```
  Tiếp theo sử dụng lệnh dưới để add lại link tới repo của chính chủ:
  ```bash
  git remote add upstream + <path repo chính chủ> #Thay vì origin thì đặt là upstream để làm tên cho repo chính chủ khi cần pull về 
  ```
  Việc làm trên sẽ giúp bạn vừa có thể thay đổi mã trong repo bản sao của chính chủ ở local ở repo riêng biệt của bạn và vừa có thể cập nhật những update mới từ repo của chính chủ.
* Bước 5: Sau khi làm xong những bước trên thì bạn chỉ cần dùng lệnh `pull` để đẩy bản sao của repo chính chủ lên repo mà bạn đã tạo trước đó. Và những thay đổi mà bạn làm cho repo đó (như sửa code,..) thì đều không ảnh hưởng đến repo chính chủ.
  ``` bash
  git push origin main/master`
  ```
  - Nếu bạn cần cập nhật lại phiên bản mới nhất từ repo chính chủ thì có thể dùng lệnh:
   ``` bash
   git submodule update
   ```
  hoặc 
  ``` bash 
  git pull upstream master/main (theo nhánh của repo chính chủ)
  ```
  hoặc 
  ``` bash
  git fetch upstream` + `git merge upstream/master` 
  ```

## **Quy trình merge từ upstream về repo riêng** ##
Merge 
### Bước 1: Kiểm tra ###
```bash
git remote -v
```
Đảm bảo thấy:
``` scss
origin    git@github.com:yourname/your-repo.git
upstream  git@github.com:original-author/original-repo.git
```
### Bước 2: Fetch dữ liệu từ upstream ###
```bash
git fetch upstream
```
### Bước 3: Kiểm tra branch trên upstream ###
```bash
git branch -r
```
Ví dụ bạn thấy: 
```bash
upstream/master
origin/master
```
### Bước 4: Merge branch upstream vào branch local ###
```bash
git checkout main     # hoặc master
git merge upstream/main
```
Nếu muốn lịch sử commit sạch: 
```bash
git rebase upstream/main
```
### Bước 5: Xử lý conflict (nếu có) ###
* Mở các file bị conflict (ví dụ trên vscode)
* Chọn:
   - `Accept Current` -> Giữ code như của bạn
   - `Accept Incoming` -> Nhận thay đổi từ upstream
   - `Accept Combination` -> Chấp nhận cả 2

 ✔️ Sau khi sửa xong: 
 ```bash
git add <file>
git commit
```
### Bước 6: Đẩy lên repo của bạn ###
```bash
git push origin master/main
```
### ⚠ Lưu ý ###
 Trong trường hợp bạn repo thư viện đó là repo con của 1 repo cha khác, để thay đổi bắt buộc bạn phải `cd` đến folder chứa repo submodule đó để thực hiện các thao tác như `fetch`, `merge`, `pull`,...hoàn chỉnh rồi sau đó mới `cd..` ngược về repo cha để `git add` những thay đổi ở repo con đó, sau đó mới `push` lên repo remote trên gitHub vì repo con (submodule) đó vẫn là 1 repo độc lập !
 <img width="1231" height="360" alt="image" src="https://github.com/user-attachments/assets/b628cf83-5db5-43f6-a4b5-b473e764ad1e" />
 Repo cha sẽ chỉ hiển các repo con ở dạng các con trỏ commit hash được trỏ bên trong file `.submodules` + index. Chỉ lưu hash commit của submodule, không lưu branch 
 ```bash
TenSubmodule @ commit Hash
```

😅 Nhược điểm:
  * Phức tạp hơn `git clone`
  * Phải nhớ từng bước sync/fetch riêng
  * Gây rối nếu không dùng đúng quy trình <br>

😝 Ưu điểm
 * Quản lý theo phiên bản repo (Mỗi submodule gắn với 1 commit cụ thể)
 * Repo chính sẽ tự ghi nhớ URL và commit cụ thể của từng thư viện:
  ```css
    components/
  ├── MAX30100/   ← submodule trỏ đến 1 commit của repo riêng
  ├── SSD1306/
  └── VL53L0X/
  
  .gitmodules
  ```
  * Có `.gitmodules` theo dõi
  * Khi người khác clone project của bạn bằng máy khác để dùng thì có thể tự động clone luôn submodule (thư viện) kèm theo `git clone --recurse-submodules` trong khi đó `git clone` chỉ clone mã nguồn chính, các thư viện sẽ bị thiếu -> Bạn phải clone lại từng cái
  * Tự động cập nhật thư viện: Dễ update theo remote thư viện gốc hoặc fork
  ***
  ### Cách loại bỏ folder `build` khi up lên gitHub ###
  Folder `build` được tạo ra trong qua trình biên dịch dự án Esp-idf, đây là thư mục tạm, chứa tất cả các file và dữ liệu cần thiết để biên dịch, liên kết và tạo firmware cuối cùng.
  
#### Nội dung chính của thư mục `build` ####
1. File nhị phân (Binary)
   * `firmware.bin` hoặc `project_name.bin`: File firmware cuối cùng để nạp vào Esp32
   * `bootloader.bin`: Bootloader khởi động chip Esp32.
   * `partition-table.bin`: Bảng phân vùng xác định các khu vực bộ nhớ
2. File liên quan đến quá trình biên dịch
   * Object file (`*.o`): File đối tượng được tạo ra từ các file mã nguồn (`.c`, `.cpp`) sau khi được biên dịch
   * File liên kết (`*.elf`): File liên kết (Executable and Linkable Format) là sản phẩm trung gian trước khi tạo file nhị phân (`.bin`)
   * File bản đồ (`*.map`): File cung cấp thông tin về cách mã chương trình và dữ liệu được sắp xếp trong bộ nhớ.
3. Cấu hình dự án:
   * `config/sdkconfig`: File cấu hình SDK dựa trên những gì bạn đã chọn trong `menuconfig`.
4. Log và Metadata
   * `build.ninja` hoặc `Makefile`: Các file dùng bởi hệ thống build để kiểm soát quy trình biên dịch
   * Log files: Ghi lại chi tiết quá trình biên dịch, rất hữu ích khi debug
5. Các file phụ trợ:
   * Generated headers (`*.h`): Các file tiêu đề được tạo tự động từ quá trình biên dịch, ví dụ, file liên quan đến bảng phân dùng hoặc cấu hình
   * Flash script: Các script hỗ trợ nào vào firmware
**** 
#### Cách loại bỏ folder này khi up lên gitHub ####
Do dung lượng lớn và không cần thiết khi show lên các project nên ta loại bỏ:
1. Tạo hoặc chỉnh sửa file `.gitignore`
   * Nếu bạn chưa có file `.gitignore`, tạo 1 file trong thư mục gốc cuae repo:
   ```
   touch .gitignore
   ```
   * Mở file `.gitignore` và thêm dòng sau để bỏ qua tất cả các folder `build` trong mọi thư mụcL
   ```
   **/build/
   ```
2. Xác minh rằng folder `build` sẽ bị bỏ qua
   * Kiểm tra trạng thái Git để xác minh rằng các folder `build` không được thêm vào staged files:
   ```
   git status
   ```
3. Nếu folder `build` được thêm trước đó
   * Nếu folder `build` được thêm vào repo và bạn muốn xóa nó ra khỏi repo nhưng vẫn giữ chúng trong local, thực hiện các bước sau: <br>
   - Bước 1: Xóa folder `build` khỏi repo (không xóa khỏi local), sử dụng lệnh sau:
   ```
   git rm -r --cached **/build/
   ```
   - Bước 2: Commit thay đổi.<br>
   - Bước 3: Push lên remote repo
