# 东南亚新闻搜索系统 API 接口文档

- 无特别说明，API返回值及前端传递的值均为json格式

- mongo-es 文件夹下的内容是php的elasticsearch接口，在使用前需要使用composer安装elasticsearch-php组件

## connect

功能：用于连接数据库

- 数据库结构

  ```mysql
  DROP TABLE IF EXISTS `user`;
  CREATE TABLE IF NOT EXISTS `user` (
    `id` int(11) NOT NULL AUTO_INCREMENT,
    `username` varchar(256) NOT NULL,
    `password` varchar(256) NOT NULL,
    `isadmin` tinyint(1) NOT NULL,
    PRIMARY KEY (`id`)
  ) ENGINE=MyISAM DEFAULT CHARSET=utf8;
  COMMIT;
  ```


## login

功能：用于实现与登陆相关的用户认证、会话控制、权限认证等功能。

- userInfo

  - 功能：用于判断用户是否已经登陆，如果已经登陆，通过SESSION识别用户权限

  - HTTP METHOD: GET

  - request:

    - null

  - return:

    ```json
    {
        "islogin":int,	//1表示用户已登陆，直接跳转到登陆成功页面，0表示未登录，跳转到登陆页面
        "isadmin":int	//0表示不是管理员，1表示是管理员，跳转到管理员页面
    }
    ```

- login

  - 功能：用于实现用户名和密码的认证以及管理员身份判定。

  - HTTP METHOD:POST

  - request: 

    ```json
    {
     	"username":string,
        "password":string
    }
    ```

  - return:

    ```json
    {
        "success":int,				//1表示认证成功，0表示认证失败
        "errorcode":int,			
        "errormessage":string,		
       	"isadmin":int				//1表示是管理员，0表示不是管理员
    }
    ```

    |         errorcode          |   errormessage   |              meaning               |
    | ------------------------ | -------------- | -------------------------------- |
    |            200             |       null       |              登陆成功              |
    |            403             | 用户名或密码错误 |              登陆失败              |
    | PDOException $e->getCode() | $e->getMessage() | 数据库连接或者操作时可能出现的错误 |

- logout

  - 功能：用于注销用户（注销之后将不会再有免登陆的功能)

  - HTTP METHOD：GET

  - request:null

  - return:

    ```json
    {
        "success":1,
        "errorcode":200,
        "errormessage":""
    }
    ```

## admin

功能：用于管理员实现增加用户、

删除用户、更改密码等功能。

- userList

  - 功能：返回所有的用户信息

  - HTTP METHOD:GET

  - request:null

  - return:

    ```json
    {
        "success":int 						//1表示操作成功，0表示操作失败
    	[
        	{
            	id:int,						//userid,是用户在数据库里的唯一标识
            	username:string,			//用户名
            	isadmin						//是否是管理员
        	},
        	{
            	...
        	},
        	...
    	]
    ```

    |         errorcode          |    errormessage    |              meaning               |
    | :------------------------: | :----------------: | :--------------------------------: |
    |            200             |        null        |              操作成功              |
    | PDOException $e->getCode() |  $e->getMessage()  | 数据库连接或者操作时可能出现的错误 |
    |            403             | 没有权限登陆此界面 |       非管理员用户请求此接口       |

- addUser

  - 功能：用于添加新用户

  - HTTP METHOD：POST

  - request:

    ```json
    {
        "username":string,
        "password":string，
        "isadmin":int  				//1表示是管理员，0表示不是管理员
    }
    ```

  - return:

    ```json
    {
        "success":int,				//1表示添加成功，0表示添加失败
        "errorcode":int,			
        "errormessage":string,		
    }
    ```

    |         errorcode          |   errormessage   |              meaning               |
    | :------------------------: | :--------------: | :--------------------------------: |
    |            200             |       null       |              操作成功              |
    | PDOException $e->getCode() | $e->getMessage() | 数据库连接或者操作时可能出现的错误 |

- deleteUser

  - 功能：用于删除用户

  - HTTP METHOD:POST

  - request:

    ```json
    {
        "userid":int
    }
    ```

  - return:

    ```json
    {
        "success":int,
        "errorcode":int,
        "errormessage":string
    }
    ```

    |         errorcode          |   errormessage   |              meaning               |
    | :------------------------: | :--------------: | :--------------------------------: |
    |            200             |       null       |              操作成功              |
    | PDOException $e->getCode() | $e->getMessage() | 数据库连接或者操作时可能出现的错误 |


- changepassword

  - 功能：用于修改密码

  - HTTP METHOD:POST

  - request:

    ```json
    {
        "userid":int,
        "password":string
    }
    ```

  - return:

    ```json
    {
        "success":int,
        "errorcode":int,
        "errormessage":string
    }
    ```

    |         errorcode          |   errormessage   |              meaning               |
    | :------------------------: | :--------------: | :--------------------------------: |
    |            200             |       null       |              操作成功              |
    | PDOException $e->getCode() | $e->getMessage() | 数据库连接或者操作时可能出现的错误 |

## mongo-es/php_api

- searchApi

  - 功能：elasticsearch的php接口，用于从elasticsearch中进行搜索并且获得数据
  - HTTP METHOD ：POST
  - request:

  ```json
  {
      "dateRange":array,		//搜索要求的起始时间,dateRange[0]是起始时间，dateRange[1]是截止时间
      "from":int,				//用于分页
      "language":string, 		//语言
      “index”:array,		    //网站
      "query":string,			//搜索内容
      "sortMode":string		//排序模式，sortMode[0]表示time || score，如果按时间排序，则sortMode[1] 表示升序或者降序，'desc'表示降序,'asc'表示升序
  }
  ```

  - return:

  ```json
  [{
      "author",
      "url",
      "media",
      "abstract",
      "site",
      "news_title",
      "public_date",
      "news_content",
      "highlight"	
  },........]
  ```


- getIndex


    - 功能：获取目前可搜索的所有语言及对应网站

    - HTTP METHOD:GET

    - request:null

    - return:

      ```json
         {"vietnam":["test"], "indonesia":["republika","liputan6"]}
      ```

- chartApi

  - 功能：用于展示图表

  - HTTP METHOD ：POST

  - request:

    ```json
    {
        "dateRange":array,		//搜索要求的时间范围，包括开始时间、结束时间和用户自主选择的时间范围
        "language":string, 		//语言
        “index”:array,		    //网站
        "query":string,			//搜索内容
        "type":string			//想要得到的聚合结果，可选的是csount或者time
    }
    ```

  - return:

    - type如果为time，则return

      ```json
      {
           "count_result": [
           {
               "item": "indonesia_republika",
               "count": 44661
           },
           {
               "item": "indonesia_liputan6",
               "count": 37924
           }],
      	"time_result": {
              "Indonesia_liputan6": [
                  {
                      "item": "2014~2015",
                      "count": 4954
                  },
                  {
                      "item": "2015~2016",
                      "count": 8692
                  },
                  {
                      "item": "2016~2017",
                      "count": 11716
                  },
                  {
                      "item": "2017~2018",
                      "count": 11860
                  },
                  {
                      "item": "2018~2019",
                      "count": 702
                  }
              ],
              "Indonesia_republika": [
                  {
                      "item": "2014~2015",
                      "count": 11457
                  },
                  {
                      "item": "2015~2016",
                      "count": 12348
                  },
                  {
                      "item": "2016~2017",
                      "count": 10229
                  },
                  {
                      "item": "2017~2018",
                      "count": 10056
                  },
                  {
                      "item": "2018~2019",
                      "count": 571
                  }
              ]
         	}
      ```

      - type如果为count，则return

        ```json
        {  
            "count_result": [
             {
                 "item": "indonesia_republika",
                 "count": 44661
             },
             {
                 "item": "indonesia_liputan6",
                 "count": 37924
             }]
        }
        ```

        

    
