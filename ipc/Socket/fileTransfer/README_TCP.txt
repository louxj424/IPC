最近在写Linux网络方面的demo,用TCP实现了一个简单的传输文件程序，适用于网卡设备及TCP/IP协议栈及网络环境测试时使用。用法容易，代码简洁，已经测试成功。其功能是使用TCP协议从client端向server端传输文件，用法如下:

编译：

client:gcc -o client client.c

server:gcc -o server server.c

运行：

client端：./client <server IP> <端口号> <上传文件名>

server端：./server <端口号> <保存为文件名>

其中，server端先运行，client端与server端的端口号必须一致并且不能与已知端口冲突（例如8888即可）


