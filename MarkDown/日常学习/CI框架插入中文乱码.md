# CI框架插入中文乱码

### 原因：	

- mysql默认字符集为gbk

- codeigniter框架字符集为utf8


### 解决：

在my.ini中修改字符集配置：

```mysql
[client]
default-character-set=utf8
```

```mysql
[mysqld]

 character-set-server=utf8
```

如果是已经创立的数据库和数据表，还需要自己手动修改

```mysql
alter database db_name DEFAULT CHARACTER SET character_name;	
```

把表默认的字符集和所有字符列（CHAR,VARCHAR,TEXT）改为新的字符集：

```mysql
alter table tb_name convert to character set character_name;
```

只是修改表的默认字符集：

```mysql

```

修改字段的字符集：

```mysql

```

查看数据库编码：

```mysql
SHOW CREATE DATABASE db_name;
```

查看表编码：

```mysql

```

查看字段编码：

```mysql

```

> MySQL中默认字符集的设置有四级:**服务器级，数据库级，表级** 。最终是**字段级** 的字符集设置。注意前三种均为默认设置，并不代码你的字段最终会使用这个字符集设置。所以我们建议要用show create table table ; 或show full fields from tableName; 来检查当前表中字段的字符集设置。

# MYSQL字符集问题

> MySQL的字符集支持(Character Set Support)有两个方面：
> ​      字符集(Character set)和排序方式(Collation)。
> 对于字符集的支持细化到四个层次:
> ​      服务器(server)，数据库(database)，数据表(table)和连接(connection)。

### MySQL默认字符集

​	MySQL对于字符集的指定可以细化到一个数据库，一张表，一列，应该用什么字符集。
​	但是，传统的程序在创建数据库和数据表时并没有使用那么复杂的配置，它们用的是默认的配置，那么，默认的配置从何而来呢？     

1. 编译MySQL 时，指定了一个默认的字符集，这个字符集是 latin1；
2. 安装MySQL 时，可以在配置文件 (my.ini) 中指定一个默认的的字符集，如果没指定，这个值继承自编译时指定的；
3. 启动mysqld 时，可以在命令行参数中指定一个默认的的字符集，如果没指定，这个值继承自配置文件中的配置,此时 character_set_server 被设定为这个默认的字符集；
4. 当创建一个新的数据库时，除非明确指定，这个数据库的字符集被缺省设定为character_set_server；
5. 当选定了一个数据库时，character_set_database 被设定为这个数据库默认的字符集；
6. 在这个数据库里创建一张表时，表默认的字符集被设定为 character_set_database，也就是这个数据库默认的字符集；
7. 当在表内设置一栏时，除非明确指定，否则此栏缺省的字符集就是表默认的字符集；
8. 简单的总结一下，如果什么地方都不修改，那么所有的数据库的所有表的所有栏位的都用latin1 存储，不过我们如果安装 MySQL，一般都会选择多语言支持，也就是说，安装程序会自动在配置文件中把default_character_set 设置为 UTF-8，这保证了缺省情况下，所有的数据库的所有表的所有栏位的都用 UTF-8 存储。