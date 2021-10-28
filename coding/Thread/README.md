### 封装线程

Linux下POSIX线程库

#### 面向对象的方法
> 采用继承的方式，覆盖纯虚函数run方法。  
> 在threadFunc函数中需要一个对象指针调用run方法

![](https://gitee.com/snow-tyan/learn-cpp/raw/master/Figure/OO_Thread.png)

#### 线程为什么要封装成这样？

线程需要完成某个任务，但是又不知道具体任务是什么，所以设置成纯虚函数，对外提供一个接口。  
`run()`方法是由子线程运行，所以它是在线程执行体`threadFunc`中调用（私有），不能直接通过**线程对象**调用（只能在`threadFunc`内部，通过传本对象的`this`指针调用`run`方法）。  
线程对象在Main线程下创建，只有调用了`thread->start()`方法，子线程才被创建出来。  
而线程入口函数必须是`void *threadFunc(void *arg)`形式，所以要设成static干掉this指针。

#### 基于对象的方法
> 对面向对象而言，最终要执行的就是`void run()`方法，基于对象通过回调函数完成。  
> 相比面向对象方法，更加灵活，回调函数作为Thread类的一个数据成员，  
> Thread类通过传回调函数**构造类**。注册回调函数function+bind  
> 同样threadFunc中需要一个对象指针调用传过来的回调函数

![](https://gitee.com/snow-tyan/learn-cpp/raw/master/Figure/OB_Thread.png)

