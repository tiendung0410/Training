# Socket Programming Training

## Giới thiệu
Thư mục này chứa các ví dụ thực hành về lập trình socket trên Linux, bao gồm các loại socket phổ biến (Internet, Unix, Raw) và các cơ chế xử lý kết nối (Blocking, Non-blocking, Poll, Select). Ngoài ra còn có bài tập mô phỏng trao đổi bảng định tuyến giữa hai thiết bị.

## Cấu trúc thư mục



- **TypesOfSocket/**  
  Các ví dụ về các loại socket:
  - `InternetSocket/`: Socket qua mạng TCP/IP (StreamSocket, DatagramSocket).
  - `UnixSocket/`: Socket nội bộ hệ thống (StreamSocket, DatagramSocket).
  - `RawSocket/`: Socket thô, thao tác trực tiếp với gói tin IP.

- **Assignment/**  
  Bài tập mô phỏng trao đổi bảng định tuyến giữa hai thiết bị (deviceA, deviceB) sử dụng UDP socket.

- **Mechanisms/**  
  Các cơ chế xử lý kết nối:
  - `NonBlockingSocket/`: Ví dụ về socket không chặn (non-blocking).
  - `Poll/`: Sử dụng hàm poll để xử lý nhiều kết nối, thực hành triển khai group chat.
  - `Select/`: Sử dụng hàm select để xử lý nhiều kết nối, thực hành triển khai group chat.

## Hướng dẫn build & chạy

Các thư mục đều có file `Makefile` để biên dịch:
```sh
make           # Biên dịch tất cả các file .c thành file .out
make clean     # Xóa các file .out