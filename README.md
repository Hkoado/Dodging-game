# Họ và tên: ĐỖ ANH KHOA
# MSV: 24022805
# SDL Project: Dodging Game

## Giới thiệu về game
Dodging game là game phát triển bằng ngôn ngữ lập trình C++ sử dụng thư viện SDL2, được lấy ý tưởng từ Dinosaur Game của Google nhưng có 1 vài cải tiến trong gameplay( có thể di chuyển trái và phải để né mũi tên từ trên xuống).

---

## Cấu trúc dự án
### Tệp mã nguồn chính
- `Main.cpp`: Tệp chính chứa vòng lặp chính trò chơi, màn hình bắt đầu/ kết thúc, các tính năng và clear tài nguyên.
- `Player.cpp` và `Player.h`: Định nghĩa và triển khai nhân vật.
- `arrow.cpp` và `arrow.h`: Định nghĩa và triển khai các mũi tên trong trò chơi.
- `constants.h`: Chứa các hằng số màn hình và nhân vật trong trò chơi.

### Tài nguyên
- `arrow.png`: Hình ảnh mũi tên.
- `background.png`: Hình nền khi chơi.
- `player.png`: Hình ảnh nhân vật người chơi.
- `start_screen.png`: Màn hình bắt đầu trò chơi.
- `game_over_screen.png`: Màn hình kết thúc trò chơi.

### Âm thanh và nhạc
- `game_music.mp3`: Nhạc nền của trò chơi.
- `game_over_music.mp3`: Nhạc khi thua.
- `jump_sound.wav`: Hiệu ứng âm thanh khi nhân vật nhảy.
- `start_music.mp3`: Nhạc nền khi bắt đầu trò chơi.

### Thư viện và tệp liên quan
- `SDL2.dll`, `SDL2_image.dll`, `SDL2_mixer.dll`, `SDL2_ttf.dll`, `smpeg2.dll`: Các tệp thư viện SDL cần thiết để chạy trò chơi.
- `score.ttf`: Font chữ hiển thị điểm số trong trò chơi.


## **Tính năng chính và gameplay của game**

- **Điều khiển đơn giản:**  
  + Dùng phím mũi tên trái/ phải để di chuyển, space để nhảy lên.

- **Né các mũi tên bay đến:**  
  + Các mũi tên bay đến từ bên phải và phía trên.

- **Tính điểm theo thời gian:**  
  + Điểm số sẽ được tính theo thời gian bạn sinh tồn được.

  - **Highscore**
    + Lưu lại điểm số cao nhất mà người chơi đã đạt được.

- **Độ khó tăng dần theo thời gian:**
  + Sống sót được càng lâu, số mũi tên xuất hiện càng nhiều, đòi hỏi người chơi phải né khéo léo hơn ==> Tránh gây nhàm chán theo thời gian.
    
- **Âm thanh & giao diện:**
  + Hình ảnh bắt đầu, kết thúc và background game.
  + Âm thanh mở đầu, ingame và kết thúc.
  + Hiệu ứng âm thanh khi nhân vật nhảy
  
 - **Tính năng tắt âm thanh:**
   + Tính năng mute khi bấm "m" được thêm vào màn hình bắt đầu và kết thúc để tắt âm thanh.
   
---

## Nguồn tham khảo

- **Sự kiện bàn phím, logic va chạm, vật lý:**  
  -Code mẫu, 1 số logic và vật lý học từ Lazy Foo và youtube

- **Cài đặt SDL2 và các thư viện SDL khác**  
  -Hướng dẫn trên lớp và trên youtube.
  
- **Tài nguyên đồ họa:**  
  -Tự vẽ và tìm trên mạng

- **Âm thanh:**  
  -Nhạc trên youtube và hiệu ứng trên mạng.
---

## Video giới thiệu
https://youtu.be/fA5WXHdkJPY

## File zip bao gồm các tài nguyên, thư viện SDL2 và file exe để tải và chơi
https://drive.google.com/file/d/1WQqbSb5PGTiHHCT1cpKmGzDKw1pLgnP_/view?usp=sharing


---

## Ngưỡng điểm mong muốn 7.5 - 8
- Lý do em muốn được ngưỡng điểm này là vì:
1. **Gameplay quen thuộc nhưng có cải tiến (8.5/10)**:  
   Game được lấy ý tưởng từ game khủng long chơi khi không có internet của google nhưng cải tiến về gameplay khi không chỉ nhảy để né như game gốc mà có thể di chuyển tuỳ ý.

2. **Đồ họa và âm thanh ổn (8/10)**:  
   Đồ hoạ pixel đơn giản, dễ nhìn; có nhạc mở đầu, thua cuộc và khi chơi kiểu darkfantasy và hiệu ứng âm thanh khi nhân vật nhảy lên.

3. **Các tính năng (7.5/10)**:  
   Các tính năng như **mute**, **high score** tuy không nhiều nhưng đủ để người chơi sử dụng.

4. **Sự thử thách (8.5/10)**:  
   Khi người chơi đạt điểm là bội số của 100, sẽ có thêm 1 mũi tên nữa xuất hiện buộc người chơi phải tránh. Điều này giúp game không nhàm chán và làm cho người chơi muốn chơi lại khi thua để vượt qua highscore cũ.
