# MiniApp_BE后台文档

- 无特别说明，API返回值及前端传递的值均为json格式

## User

功能：用户绑定

- getCode

    -  功能：通过接受的code获取openid和sectionkey并将sectionkey传给前端

    -  HTTP METHOD: POST

    -  URL: https://xxxxxxxxx/mini/user/getcode

    -  request:

  ```js
  {
      'code':string,   // 微信登录 code
  }
  ```

    -  return:

  ```json
  {
      'data':[
          'session_key':string //仅当errorcode=1的时候 有该键值
      ],
      'errorCode':int, 
      'errorMsg':string,   
  }
  ```

- auth

    -  功能：接受学号

    -  HTTP METHOD: POST

    -  URL:  https://xxxxxxxxx/mini/user/auth

    -  request:

  ```js
  {
      'studentId':string,   // 学号
      'password':string,   //密码
  }
  ```

  - return:


  ```json
  {
      'data',           //为空
      'errorCode':int,  //
      'errorMsg':string,//
      
  }
  ```
