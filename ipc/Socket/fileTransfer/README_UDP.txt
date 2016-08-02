最近在写Linux网络方面的demo,用UDP实现了一个简单的传输文件程序，适用于网卡设备及TCP/IP协议栈及网络环境测试时使用。当然这里要说的是，一般通过网络传输文件不会选择UDP协议，因为UPD提供不可靠的传输，文件传输过程中会出现丢包现象而导致文件传输错误。这里要实现UDP传输文件的目的是为了测试网卡的丢包率，然后与正常网络的丢包率进行比较，从而可以检验网卡及网络环境的质量。本文仅将其雏形写出。其功能是使用UDP协议从client端向server端传输文件，用法如下:


编译：

client:gcc -o client client.c

server:gcc -o server server.c

运行：

client端：./client <server IP> <端口号> <上传文件名>

server端：./server <端口号> <保存为文件名>

其中，server端先运行，client端与server端的端口号必须一致并且不能与已知端口冲突（例如8888即可）


TODO:
	1.the buf is too limited and print so many #,the user experence is very tedious.
	2.the server may don't what file qu receive,how to determine what's file name ?
	3.once the file is receive is done ,the transmission is finsihed?the server should not stop and run forever until the process is killed 
	4.show help when the user add an '-h' option
