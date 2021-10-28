### 线程池

#### 为什么要用线程池？

大并发下，线程的频繁创建和销毁对象很费时  
为避免反复创建和销毁对象带来的性能开销  
一个想法是对象池的思想：  
先预开N个线程，池内存放众多线程，池中线程由池管理器管理。  
当由任务来临时，从池中取线程，任务完成，归还线程入池。  
本质是生产者消费者模型，有任务就取任务，没任务就等着...

#### 基本组件
+ 线程池管理器 ThreadPool
+ 工作线程 WorkThread
+ 任务接口 Task
+ 任务队列 TaskQueue

#### 面向对象

![](https://gitee.com/snow-tyan/learn-cpp/raw/master/Figure/OO_ThreadPool.png)

#### 基于对象

![](https://gitee.com/snow-tyan/learn-cpp/raw/master/Figure/OB_ThreadPool.png)
