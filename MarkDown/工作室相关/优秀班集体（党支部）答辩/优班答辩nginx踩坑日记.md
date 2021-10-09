# 2018.11.28

今天在配置服务器的时候遇到了很多问题，在此做一下记录。根本原因是因为之前自己用CI框架的时候都是用的apache服务器，没想到如果使用nginx服务器的话，需要重新配置，否则会导致CI的路由功能无法正常使用。

## nginx.conf

- 首先，需要添加try_files配置，否则nginx会按照你输入的url去寻找文件，显然是寻找不到的，这个时候就会产生404 NOT FOUND的错误，因为你在index.php之后输入的东西都是要作为“参数”去传给CodeIgniter框架去分析的。

  ```nginx
   location / {
               try_files $uri $uri/ /index.php;
       }
  ```

- 其次，需要在php.ini里将cgi.fix_pathinfo=1,这个设置的作用是让php可以生成$SERVER['phpinfo']这个值，如果设置为0的话，因为CodeIgniter需要使用这个变量去解析路由。

# 2018.12.7

问题
报错：error: The requested URL returned error: 401 Unauthorized while accessing
git版本：1.7.1

解决方法一：指定用户
git clone https://github.com/org/project.git

换成
git clone https://username@github.com/org/project.git
或者
git clone https://username:password@github.com/org/project.git

在push或者pull出出现的话，则需要更改远程地址
git remote set-url origin https://username@github.com/org/project.git

解决方法二：去除验证
git config –global http.sslverify false

解决方法三：（推荐）
升级git 版本≥1.7.10

解决方法四：
添加ssh秘钥