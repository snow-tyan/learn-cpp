### 生产者消费者问题

> 最核心是TaskQueue的封装  
> 面向对象/基于对象改起来还是很容易的

#### 面向对象

![](https://gitee.com/snow-tyan/learn-cpp/raw/master/Figure/OO_PC.png)

#### 基于对象

TaskQueue 与Mutex/Condition是紧耦合无法改变

但是Thread和Producer/Consumer可以通过基于对象的方式解耦合

![](https://gitee.com/snow-tyan/learn-cpp/raw/master/Figure/OB_PC.png)