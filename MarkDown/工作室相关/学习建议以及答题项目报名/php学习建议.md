# 前言

开启了PHP的学习，实际上也就是开启了后端的学习。一入后端深似海，后端的知识太过繁重，需要花很多的时间去学习和研究的。

# PHP学习建议

### 学习来源

+ 通过视频学习，虽然学习起来比较花时间，但是通过直观的演示能跳过许多的坑
  + 中国网站：[it慕课网](https://www.imooc.com/)，[网易云课堂](https://study.163.com/)，[哔哩哔哩](https://www.bilibili.com/)
  + 国外网站：[Udemy](https://www.udemy.com/)，[YouTube](https://www.youtube.com/)
+ 通过相关书籍学习，工作室已经有很多的书了，自己可以找来看一看，或者根据自己的需要自己购买
  + 语言学习
  + 实际应用
  + 架构设计
+ 通过查看相关文档学习，技术文档是最为权威的，遇到问题，首先就是查找一下文档

### 系统环境

> 环境的搭建是入门的一大坑。开始学习的时候，让自己的程序跑起来就算很成功了。等到自己学习一段时间，用的比较熟悉了，在反过来研究运行过程和原理

+ 集成开发环境
  + WampServer
  + XAMPP
  + phpStudy
+ 分模块安装
  + php
  + nginx/apache
  + Mysql/MariaDB
+ 重量级本地开发环境
  + Vagrant盒子 => Laravel Homestead
+ 轻量级的独立虚拟机
  + docker

### PHP基础

> PHP总体来说中规中矩，与其他c系的语言有相通之处，入门也较为简单。需要花一点功夫的也就是需要把数组和字符串相关的函数操作给记一下

### SQL

> 后台大部分时间都在与数据库打交道

+ 最基本的sql语句，比如建库、建表、增删改查、复杂的SQL查询（多表关联，分组查询，排序等）
+ 数据库设计，数据库范式
+ 数据库优化
  + 是否需要优化？慢查询，需要即时响应，高并发
  + 针对什么操作优化？增、删、改、查
  + 使用什么来优化？表字段结构、索引、分页、缓存
  + 规避一些效率低的操作

### HTTP协议

> 前后端通讯规范

+ HTTP报文格式
+ CORS跨域请求的限制与解决
+ cookie和session
+ HTTP长连接
+ HTTPS
+ HTTP2

### 开发工具的使用

> 磨刀不费砍柴功

+ git，版本控制器，在代码编写、团队合作中发挥了很重要的作用，是软件开发必备技能
+ composer，PHP的一个依赖管理工具，避免造重复的轮子，很方便的安装和使用第三方库
+ phpMyAdmin、mysql workbench、navicat，数据库图形化管理工具
+ sublime、vscode、vim、PhpStorm，都是优秀的编辑器，根据自己习惯选择

### 框架的学习使用

> 如果开始使用和学习框架了，后台开发就算是彻底入门了

1. 学习框架之前确保自己已经掌握PHP基本的语法，面向对象的设计实现，SQL操作，异常处理

2. 绝大多数的框架都实现了 MVC（Model-View-Controller）设计模式并基于该模式组织代码，所以MVC设计模式一定要熟记于心。

3. 一般来说框架会在php MySQL、php MySQLi、php PDO之上建立数据访问层

   1. 实现构造器来替代原生的sql语句，比如说

      ```php
      $affectedRow = $db->insert('tableName',['key1'=>'value1','key2'=>'value2']);
      $data = $db->select('field1,field2')->from('tableName')->get();
      ```

   2. 实现ORM来替代原生的sql语句，比如说

      ```php
      class User extends Model {
      }
      //Retrieving All Records
      $users = User::all();
      //Retrieving A Record By Primary Key
      $user = User::find(1);
      var_dump($user->name);
      ```

4. 在视图层，框架也会对渲染、数据获取提供许多便利的方法

5. 依赖注入也是框架中一个很重要的概念，是控制反转的一种实现

### 架构设计

> 作为程序猿还是要紧跟时代的潮流，需要了解架构的发展

+ SOA(面向服务的架构)
+ 微服务

### 分布式与集群

+ 负载均衡
+ CDN
+ MQ
  + Apache ActiveMQ
  + RabbitMQ
  + Artemis