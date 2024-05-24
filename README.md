# Ezreal Mirror Shooting
 Phiên bản gốc của Game có tên là Ezreal Mirror Match 2 và được phát hành trên CH Play.

## Screenshots
<img width="1440" alt="MainMenu" src="https://github.com/VietThang5605/gameProject/assets/67374734/35462a99-5ce2-4c55-9174-c448c407804a">
<img width="400" alt="GamePlay1" src="https://github.com/VietThang5605/gameProject/assets/67374734/4fb36802-6d16-4798-8f44-50ca1a7c52ff">
<img width="400" alt="GamePlay2" src="https://github.com/VietThang5605/gameProject/assets/67374734/639da24b-d5d0-46eb-9147-09caa1ce4005">
<img width="400" alt="GamePlay3" src="https://github.com/VietThang5605/gameProject/assets/67374734/753c29e8-863f-48d7-b416-650114b447d0">
<img width="400" alt="GamePlay4" src="https://github.com/VietThang5605/gameProject/assets/67374734/98fbe048-5494-4754-ac76-a10a0e53980a">
<img width="400" alt="VideoGamePlay1" src="https://github.com/VietThang5605/gameProject/assets/67374734/e5583968-4e11-4e8d-b0d1-f20dd3f1bae8">
<img width="400" alt="VideoGamePlay2" src="https://github.com/VietThang5605/gameProject/assets/67374734/46d5223b-7dfa-4521-b407-02a38c1b8339">

## Thông tin cá nhân
- Họ và tên: Nguyễn Việt Thắng
- Mã sinh viên: 23021723
- Khóa K68 - CA1
- Ngành Khoa học máy tính

## Giới thiệu về Game
 Ezreal Mirror Shooting là một game đối kháng hai người chơi. Sử dụng các kỹ năng, đọc trước nước đi của đối thủ và vận dụng chiến thuật của riêng bạn để hạ gục đối thủ và dành chiến thắng. Trò chơi có 2 chế độ là VS AI và VS Player nơi bạn có thể khiêu chiến với bạn bè của mình. Trò chơi có thể chơi trên trình duyệt web tại [đây](https://vietthang5605.github.io/gameProject/).

## Ngưỡng điểm mong muốn: 9 - 10
  ### Các kỹ thuật cơ bản:
  - Xử lý thao tác sử dụng các phím kỹ năng và chuột của người chơi.
  - Sử dụng đồ họa, hiệu ứng âm thanh, text.
  - Có thể kéo thả để tùy chỉnh kích thước cửa sổ.
  - Tạo Menu và các phím chức năng.
  - Sử dụng các kiến thức về toán học để điều chỉnh vận tốc và hướng đi của các kỹ năng.
  - Xử lý các channels của mixer để tạm dừng hiệu ứng âm thanh của các kỹ năng khi pause game và tiếp tục phát khi hết tín hiệu thời gian chờ.

  ### Các kỹ thuật nổi bật:
  - Lập trình hướng đối tượng
  - Sử dụng thuật toán **SAT** kiểm tra va chạm giữa các hình đã bị xoay lệch khỏi góc ban đầu.
  - Áp dụng thuật toán **Minimax** và cắt tỉa **Alpha - Beta** cùng hàm **Heuristic** cho mô hình AI.
  - Sử dụng Emscripten complier để biên dịch sang WebAssembly để có thể chơi trên web.

## Kết luận
  - Đây là project đầu tiên của em nên vẫn còn rất nhiều sai sót. Qua dự án này, em đã được biết thêm các kiến thức về 
  lập trình hướng đối tượng, cách quản lý file trong project, rèn luyện tư duy và phong cách lập trình rõ ràng, dễ hiểu. 
  Đồng thời được thử sức tự tạo một mô hình AI trong trò chơi đối kháng.

## Nguồn tài liệu nghiên cứu và tham khảo
  ### Collision
  - Rotating a point around another point:
    - [Step-by-step prove](https://www.youtube.com/watch?v=EZufiIwwqFA&ab_channel=PenandPaperScience)
  - Convex polygon collisions (using SAT): 
    - [Basic concept](https://www.youtube.com/watch?v=dn0hUgsok9M) 
    - [Explanation and Examples](https://www.youtube.com/watch?v=7Ik2vowGcU0&ab_channel=javidx9)

  ### AI Model
  - Basic concept:
    - [Adversarial search - Game playing (Lecturer: Nguyễn Văn Hòa, Faculty of Information Technology - An Giang University)](https://staff.agu.edu.vn/nvhoa/AI/lecture4.pdf)
    - [Adversarial Search in Artificial Intelligence](https://www.almabetter.com/bytes/tutorials/artificial-intelligence/minimax-algorithm-in-ai)
    - [Heuristic Function in AI (Artificial Intelligence)](https://www.almabetter.com/bytes/tutorials/artificial-intelligence/heuristic-function-in-ai)

  - Heuristic evaluation for AI
    - [What are some heuristics for quickly evaluating chess positions?](https://www.quora.com/What-are-some-heuristics-for-quickly-evaluating-chess-positions)
    - [Better Heuristic function for a game (AI Minimax)](https://stackoverflow.com/questions/33644353/better-heuristic-function-for-a-game-ai-minimax)
    - [Heuristic function for a strategic board game AI](https://boardgamegeek.com/thread/2419449/heuristic-function-for-a-strategic-board-game-ai)
  
  ### SDL2 and Programming style
  - [SDL2 Wiki](https://wiki.libsdl.org/SDL2/FrontPage)
  - [codergopher](https://www.youtube.com/playlist?list=PL2RPjWnJduNmXHRYwdtublIPdlqocBoLS)
  - [PolyMars](https://www.youtube.com/@PolyMars)
  - [LazyFoo](https://lazyfoo.net/tutorials/SDL/)

  ### Emscripten
  - [Emscripten.org](https://emscripten.org/)
  - [Programming With Nick](https://www.youtube.com/watch?v=j6akryezlzc&t=803s&ab_channel=ProgrammingWithNick)
  - [javidx9](https://www.youtube.com/watch?v=MrI5kkVY6zk&t=1030s&ab_channel=javidx9)

  ### Images
  - [Ezreal (by serklaz)](https://www.deviantart.com/serklaz/art/Ezreal-League-of-Legends-Pixel-art-PixeLoL-548352903)
  - [Background tilesets (by Cup Nooble)](https://cupnooble.itch.io/sprout-lands-asset-pack)