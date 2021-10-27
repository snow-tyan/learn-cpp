### 封装线程

#### 面向对象的方法
> 采用继承的方式，覆盖纯虚函数run方法。  
> 在threadFunc函数中需要一个对象指针调用run方法

![](https://gitee.com/snow-tyan/learn-cpp/raw/master/Figure/Thread_OO.jpg)

#### 基于对象的方法
> 相比面向对象方法，更加灵活，回调函数作为Thread类的一个数据成员，  
> Thread类通过传回调函数构造类。注册回调函数function+bind  
> 同样threadFunc中需要一个对象指针调用传过来的回调函数

![](https://gitee.com/snow-tyan/learn-cpp/raw/master/Figure/Thread_OB.jpg)

