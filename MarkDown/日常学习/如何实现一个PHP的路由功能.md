# 如何实现一个PHP的路由功能

目的：实现类似于CodeIgniter 这样的路由功能，可以通过`/controller/function/arr`这样的形式去访问特定的方法

## PHP中单双引号的区别

- 单引号把内容当作纯文本，不会经过服务器翻译。而双引号则与此相反。里面的内容会经过服务器处理

  ```php
  $foo = "data";
  echo '$foo';		//单引号输出 $foo
  echo "$foo";		//双引号输出 data
  ```

- 单引号不能处理变量和转义字符

  ```php
  除了 \\ 和 \' 
  ```

- 要输出 `what's your name?` 这个字符串，有以下两种方式

  ```php
  echo "what's your name?"		//最简单的方式，单引号和双引号不会产生冲突
  echo 'what\'s your name?'		//用斜杠转义
  ```


