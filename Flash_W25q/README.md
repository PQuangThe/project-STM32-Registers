Giao tiếp với bộ nhớ flash w25q qua spi. Giao tiếp qua UART để tương tác với flash.

Lệnh:<br>
    `ri`                   : đọc JEDEC ID của w25q<br>
    `r0 <x> <y>`           : đọc dữ liệu trong w25q (x địa chỉ bắt đầu, y là số byte cần đọc)<br>
    `wp <x> <y1> <y2> ...` : Ghi dữ liệu vào w25q (x là địa chỉ bắt đầu ghi, y123... là dữ liệu ghi)<br>

![image](https://github.com/user-attachments/assets/d0075a19-9d3c-4a07-9bd8-273d61c5d28a)
