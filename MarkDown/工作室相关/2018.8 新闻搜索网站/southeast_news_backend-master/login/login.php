<?php
//用于实现用户名和密码的认证以及管理员身份判定
session_start();
$json_array = json_decode(file_get_contents("php://input"), true);                   //因为前端传递时使用json格式，所以不能用$POST直接获取
$username = $json_array['username'];
$password = md5($json_array['password']);
$loginInfo = array('success' => 1, 'errorcode' => 200, 'errormessage' => "", 'isadmin' => 0);
if ($username == "" or $password == "") {
    $loginInfo['success'] = 0;
    $loginInfo['errorcode'] = 403;
    $loginInfo['errormessage'] = "用户名与密码不能为空";
    $loginInfo['isadmin'] = 0;
    echo json_encode($loginInfo, JSON_UNESCAPED_UNICODE);
    header("http/1.1 403 Forbidden");
    die();
}
include "../connect.php";    //载入数据库配置
try {
    $conn = new PDO($dsn, $user, $passwd);                                             //初始化一个PDO对象用于连接数据库； 
    $conn->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);                   //设置PDO错误模式，用于抛出异常
    $sql = $conn->prepare("SELECT * FROM user where username = ? and password = ?"); //通过预处理的方式防止网站遭受sql注入攻击。
    $sql->bindParam(1, $username);
    $sql->bindParam(2, $password);
    $sql->execute();
} catch (PDOException $e) {
    $loginInfo['success'] = 0;
    $loginInfo['errorcode'] = $e->getCode();
    $loginInfo['errormessage'] = $e->getMessage();
    echo json_encode($loginInfo, JSON_UNESCAPED_UNICODE);
    header("http/1.1 400 BadRequest");
    die();
}
$result = $sql->fetch(PDO::FETCH_ASSOC);        //从PDO的预处理语句运行后的结果集中获取用户信息 
if (empty($result)) {
    $loginInfo['success'] = 0;
    $loginInfo['errorcode'] = 403;
    $loginInfo['errormessage'] = "用户名或密码错误";
    echo json_encode($loginInfo, JSON_UNESCAPED_UNICODE);
    header("http/1.1 403 Forbidden");
    die();
}
$_SESSION['username'] = $result['username'];
if ($result['isadmin'] == 1) {
    $loginInfo['isadmin'] = 1;
    $_SESSION['isadmin'] = 1;
}
echo json_encode($loginInfo, JSON_UNESCAPED_UNICODE);
?>