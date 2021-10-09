<?php
session_start();
include "../connect.php";
$userList = array('success' => 1, 'errorcode' => 200, 'errormessage' => "", 'userlist' => "");
/*if($_SESSION['isadmin'] != 1)
{
    $userList['success'] = 0;
    $userList['errorcode'] = '403';
    $userList['errormessage'] = "没有权限登陆此界面";
    echo json_encode($userList,JSON_UNESCAPED_UNICODE);
    header("http/1.1 403 Forbidden");
    die();
}*/
try {
    $conn = new PDO($dsn, $user, $passwd);                                 //初始化一个PDO对象用于连接数据库； 
    $conn->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);       //设置PDO错误模式，用于抛出异常
    $sql = "SELECT id,username,isadmin FROM user ORDER BY isadmin DESC";
    $result = $conn->query($sql);
    $result_array = $result->fetchAll(PDO::FETCH_ASSOC);    //从查询结果中获取所有用户信息并保存到数组中，FETCH_ASSOC参数表示返回一个索引为结果集列名的数组
} catch (PDOException $e)                            //如果try代码块中的代码执行时出现问题，则会终止  
{
    $userList['success'] = 0;
    $userList['errorcode'] = $e->getCode();
    $userList['errormessage'] = $e->getMessage();
}
global $i;
$i = 1;
foreach ($result_array as &$value) {
    $value['key'] = $i;
    $value['isadmin'] = (int)$value['isadmin'];
    $value['id'] = (int)$value['id'];
    $i++;
}
unset($value);
$userList['userlist'] = $result_array;
echo json_encode($userList, JSON_UNESCAPED_UNICODE);   //将结果编码成json格式返回给前端处理，注意要用JSON_UNESCAPED_UNICODE模式，否则会产生乱码
?>