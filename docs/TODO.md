# TODO

- Play audio from difference partitipant in different stream.

    => Hiện tại chỉ nhận audio và gửi vào một stream để chạy ra loa.
    (xem /src/services/stream/audio_stream_service)
    
    Khi cần nói chuyện nhiều người cần khởi tạo nhiều stream khác nhau và gửi các audio tương ứng vào các stream đó (server sẽ gửi source participant để xác định audio đó thuộc nguồn nào).

- Ghép ảnh participant trên server: 

    => Hiện tại server lấy ảnh của một người và gửi cho tất cả những người còn lại. Việc tiếp theo là implement việc ghép ảnh để server chỉ gửi về một hình ảnh chứa tất cả mọi người.

- Implement Audio/Image sequence id (to sort wrong order frames because of UDP).

    => Khi gửi frame sẽ gửi kèm một số thứ tự, dùng để sắp xếp lại thứ tự frame. Sẽ có delay khoảng 5 frame để xếp lại.

- Implement quality control.

    => Server/client dựa vào Image sequence id để xác định xem có bị mất frame không, frame có bị xếp sai thứ tự không, có bị chậm trong quá trình nhận frame không, để chỉnh lại chất lượng hình muốn nhận (xem slide về chất lượng hình).

- Implement AccessDenied message (see slides).

- Implement config server. 

    => Thiết kế server để sinh file cấu hình và chạy stream server. Server này sẽ cung cấp giao diện cho admin để quản lý các conference và giao diện cho client để tải file cấu hình.

- Implement thêm một số phương thức nén dòng stream (đang ở dạng unsigned char): 

    => http://users.cs.cf.ac.uk/Dave.Marshall/Multimedia/node207.html