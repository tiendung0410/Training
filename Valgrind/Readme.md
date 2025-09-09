# Valgrind Demo: Memcheck, Massif, Helgrind

Repository này chứa các ví dụ thực hành với 3 công cụ chính của [Valgrind](https://valgrind.org/):

- **Memcheck**: Phát hiện lỗi bộ nhớ động (memory errors) như leak, truy cập sai, giải phóng sai, dùng dữ liệu chưa khởi tạo, v.v.
- **Massif**: Phân tích và đo lường việc sử dụng heap (heap profiler).
- **Helgrind**: Phát hiện lỗi race condition trong chương trình đa luồng.

## Cấu trúc thư mục

```
Memcheck/
    accessInvalidMemory/
    alignmentError/
    IllegalFree/
    InvalidMemoryArgument/
    memoryLeak/
    mismatchMemoryManage/
    overlapMemoryArgument/
    useUninitializedData/
Massif/
    main.c
    massif.out.*
Helgrind/
    main.c
    valgrind.log
```

## Hướng dẫn sử dụng

### 1. Memcheck

Các ví dụ trong thư mục `Memcheck` minh họa các lỗi phổ biến về quản lý bộ nhớ:

- **accessInvalidMemory**: Truy cập vùng nhớ không hợp lệ (out-of-bounds, use-after-free).
- **alignmentError**: Lỗi căn chỉnh bộ nhớ khi cấp phát.
- **IllegalFree**: Giải phóng bộ nhớ sai (double free, free không đúng con trỏ gốc).
- **InvalidMemoryArgument**: Đối số không hợp lệ cho các hàm cấp phát bộ nhớ.
- **memoryLeak**: Các trường hợp rò rỉ bộ nhớ (definitely, indirectly, possibly lost, still reachable).
- **mismatchMemoryManage**: Quản lý bộ nhớ không khớp (new/delete, malloc/free).
- **overlapMemoryArgument**: Đối số bộ nhớ bị chồng lấn (overlapping memcpy).
- **useUninitializedData**: Sử dụng dữ liệu chưa khởi tạo.

**Chạy Memcheck:**
```sh
valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=valgrind.log ./your_program
```
- `--leak-check=full`: Kiểm tra chi tiết các trường hợp rò rỉ bộ nhớ.
- `--show-leak-kinds=all`: Hiển thị tất cả các loại leak.
- `--track-origins=yes`: Truy vết nguồn gốc lỗi bộ nhớ.
- `--log-file=valgrind.log`: Ghi kết quả kiểm tra vào file log.

Xem file `valgrind.log` trong từng thư mục để biết kết quả.

### 2. Massif

Ví dụ trong [Massif/main.c](Massif/main.c) dùng để phân tích việc sử dụng heap.

**Chạy Massif:**
```sh
valgrind --tool=massif ./your_program
ms_print massif.out.<pid>
```
Xem file [massif.out.108784](Massif/massif.out.108784) để tham khảo kết quả.

### 3. Helgrind

Ví dụ trong [Helgrind/main.c](Helgrind/main.c) minh họa lỗi race condition với biến toàn cục.

**Chạy Helgrind:**
```sh
valgrind --tool=helgrind ./your_program
```
Xem file [valgrind.log](Helgrind/valgrind.log) để biết kết quả.

## Yêu cầu

- GCC để biên dịch các chương trình C/C++
- Valgrind đã cài đặt trên hệ thống

## Tài liệu tham khảo

- [Valgrind Documentation](https://valgrind.org/docs/manual/manual.html)
- [Valgrind Quick Start](https://valgrind.org/docs/manual/quick-start.html)

---

