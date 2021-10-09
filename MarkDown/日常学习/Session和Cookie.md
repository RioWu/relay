# Session和Cookie

## 会话控制

HTTP是无状态的协议，所以不能维护两个事务之间的状态。但我们一般希望用户在请求一个页面之后再请求另外一个页面能够让服务器明白这是同一个用户。因此我们需要会话控制。一般PHP有三种方法去实现：

- 使用超链接或者header()函数等重定向的方式，通过在URL的GET请求中附加参数的形式，将数据从一个页面转向另一个PHP脚本中。也可以通过网页中的各种**隐藏表单**来储存使用者的资料，并将这些信息在提交表单时传递给服务器中的PHP脚本使用。

- 使用Cookie将用户的状态信息，存放在客户端的计算机之中。让其他程序能通过存取用户端计算机的Cookie，来存取目前的使用者资料信息。
- 相对于Cookie还可以使用Session，将访问者的状态信息存放于服务器之中，让其他程序能够透过服务器中的文件或者数据库，来存取使用者的信息。

### Cookie

- 向客户段计算机中设置Cookie

```php
bool setcookie(string $name [,string $value [,int $expire [,string $path [, string $domain [,bool $secure]]]]])
/*
$name Cookie的识别名称
$value Cookie的值，可以为数值或者字符串形态，此值保存在客户端，不要用来保存敏感数据
$expire Cookie的生存期限
$path Cookie在服务器端的指定路径，当设定此值时，服务器中只有指定路径下的网页或程序可以存取此Cookie*/
```

- 在PHP脚本中读取Cookie 的资料内容

```php
print_r($_COOKIE);
```

- 数组形态的Cookie应用,下面的程序建立了一个标识名称为“user”的Cookie，但其中包含了三个数据，这样就形成了Cookie的关联数组形式。

```php
<?php
  setcookie("user[username]","riochen");
	setcookie("user[password]".md5"(123456)");
	setcookie("user[mail]","283489710@qq.com");
?>
```

- 删除Cookie

```php
<?php
	setcookie("account");//通过同名来覆盖之前的Cookie
	setcookie("islogin","",time()-1);//通过设置有效时间来删除Cookie
?>
```

### Session

- 因为Cookie 的信息存放于客户端，而客户有权利禁用Cookie ，因此，出现了一种将用户信息保存在服务器端的技术，也就是Session。
  - Session一般有两种用法：
    - 通过Cookie传递Session ID：如果客户端没有禁用Cookie，则在PHP脚本中通过session_start()函数进行初始化后，服务器会自动发送HTTP标头将Session ID保存到客户端计算机的Cookie中。
    - 通过URL传递Session ID：如果客户端禁止Cookie的使用，浏览器中就不存在作为Cookie的Session ID，因此在客户请求中不包含Cookie信息。这种情况下，只能通过把Session ID添加到URL信息里的方式去传递Session ID，但工作量比较大，因此不建议这种方式。

- Session的声明与使用

```php
bool session_start(void) //创建Session，开始一个会话，进行Session初始化
```

这个函数没有参数，且返回值均为TRUE。有两个主要作用，一是开始一个会话，二是返回已经存在的会话。

1. 第一次访问网站时，session_start()函数会创建一个唯一的Session ID，并自动通过HTTP的响应头，将这个Session ID 保存到客户端的Cookie中。同时，也在服务器端创建一个以这个Session ID命名的文件，用于保存这个用户的会话信息。
2. 当同一个用户再次访问这个网站时，也会自动通过HTTP的请求头将客户端Cookie中保存的Session ID再携带过来，这时session_start()函数就不会再去分配一个新的Session ID,而是在服务器的硬盘中去寻找和这个Session ID同名的Session文件，将之前为这个用户保存的会话信息读出，在当前脚本中应用，达到跟踪这个用户的目的。

- 注册一个会话变量和读取Session

```php
<?php
    session_start();
	$_SESSION['username'] = "riochen";
	$_SESSION['uid'] = 1;
    ?>
```

- 注销变量与销毁Session

```php
bool session_destroy(void) 				
   	//销毁和当前Session有关的所有资料
    //不会释放和当前Session有关的变量，也不会删除保存在客户端Cookie中的Session ID.
```

如果我们想释放和Session有关的变量，我们可以用  *unset()* 函数去实现：

```php
unset($_SESSION["username"]);	//删除在Session中注册的用户名变量
unset($_SESSION["password"]);	//删除在Session中注册的用户密码变量
```

不要使用  *unset($_SESSION)* 删除整个  *$_SESSION*  数组，这样将不能再通过 *$_SESSION* 超全局数组注册变量了。如果你想把某个用户在Session中注册的所有变量全部删除，可以用这种方式：

```php
$_SESSION = array();			
```

- Session的自动回收机制

当你关闭浏览器时，储存在客户端Cookie中的Session ID 信息就会被销毁 *（这是由php.ini中的设置 session_cookie_lifetime=0 设置的）* ,而此时储存在服务器端的对应的Session信息却依旧存在，因此需要一个自动清理的机制 *如果一个session文件长时间（这个超时期限见 php.ini中的 session.gc_maxlifetime）没有更新就会被系统删除 删除的频率通过php.ini中的一些参数（session.gc_probability/session_gc_divisor) 决定*。

- 关于session_destroy()和unset()
  - 如果使用session_destroy()，虽然不会释放和当前session有关的变量，但是在重新加载页面时，session_start()会重新读取一遍session数据，因此同样可以实现用户免登陆的功能，不会出现下一个用户登陆时上一个用户session数据仍然存在的现象。