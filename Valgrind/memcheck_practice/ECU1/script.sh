#Cau hinh CAN cho ECU1 truoc khi chay

sudo config-pin P9_24 can
sudo config-pin P9_26 can

# Gỡ cấu hình nếu đang bật
sudo ip link set can1 down 2>/dev/null

# Thiết lập lại với tốc độ mong muốn
sudo ip link set can1 type can bitrate 500000

# Bật giao diện
sudo ip link set can1 up

# Hiển thị trạng thái
ip -details link show can1
