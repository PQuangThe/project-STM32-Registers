#Giao tiếp với bộ nhớ flash w25q qua spi. Giao tiếp qua UART để tương tác với flash.

##Lệnh:
    `ri`                  : đọc JEDEC ID của w25q
    `r0 <x> <y>`          : đọc dữ liệu trong w25q (x địa chỉ bắt đầu, y là số byte cần đọc)
    `wp <x> <y1> <y2> ... : Ghi dữ liệu vào w25q (x là địa chỉ bắt đầu ghi, y123... là dữ liệu ghi)

