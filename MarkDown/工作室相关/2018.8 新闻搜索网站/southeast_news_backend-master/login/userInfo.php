<?php
//用于判断用户是否已经登陆，如果已经登陆，通过SESSION识别用户权限
session_start();
$userInfo = array('username' => '', 'islogin' => 0, 'isadmin' => 0);
if (isset($_SESSION['username'])) {
    $userInfo['islogin'] = 1;
    $userInfo['isadmin'] = $_SESSION['isadmin'];
    $userInfo['username'] = $_SESSION['username'];
}
echo json_encode($userInfo, JSON_UNESCAPED_UNICODE);
?>