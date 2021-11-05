### IPC之管道

> 管道：半双工  
> 管道空时，read管道会阻塞

#### 死锁问题

`./chat1 1.pipe 2.pipe`  
`./chat2 2.pipe 1.pipe`  
死锁了，`chat1`打开一号管道读端，等着读  
`chat2`打开二号管道读端，等着读，就死锁了。

正确做法是：打开同一个管道，一个读一个写。


#### 即时聊天（1）
> 局限性：只能chat1先说，因为chat2在等待读管道  
> 而且是回合制，一人说一句话，说两次对面只读一次。
```c
// chat1
while(1){
    读取标准输入
    写入管道fdw
    读取管道fdr
    打印显示
}

// chat2
while(1){
    读取管道fdr
    打印显示
    读取标准输入
    写入管道fdw    
}
```

#### 即时聊天（2）
> 加入select系统调用，IO多路复用  
> 解决（1）中的局限性

+ 管道读写端：写端关闭，读端返回0，描述符被标识为可读；
  
+ 读端不读，写端会写满管道，读端关闭，写端崩溃（13号信号）。