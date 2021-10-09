<?php
include "../connect.php";
$json_array = json_decode(file_get_contents("php://input"), true);           //因为前端传递时使用json格式，放在request payload中，所以不能用$POST直接获取
$username = $json_array['username'];
$password = md5($json_array['password']);                    //将前端传递过来的密码进行md5哈希处理，将处理结果放入数据库中
$isadmin = $json_array['isadmin'];
$addInfo = array('success' => 1, 'errorcode' => 200, 'errormessage' => '');
try {
    $conn = new PDO($dsn, $user, $passwd);
    $conn->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
    $sql_select = $conn->prepare("SELECT * FROM user where username = ?"); //通过预处理的方式防止网站遭受sql注入攻击。
    $sql_select->bindParam(1, $username);
    $sql_select->execute();
    $select_result = $sql_select->fetch(PDO::FETCH_ASSOC);
    if (!empty($select_result)) {
        $addInfo['success'] = 0;
        $addInfo['errorcode'] = 400;
        $addInfo['errormessage'] = "用户名已被注册";
        header("http/1.1 400 BadRequest");
        echo json_encode($addInfo, JSON_UNESCAPED_UNICODE);
        die;
    } else {
        $sql_insert = $conn->prepare("INSERT INTO user(username,password,isadmin) VALUES (?,?,?) ");   	//使用预处理防止sql注入
        $sql_insert->bindParam(1, $username);
        $sql_insert->bindParam(2, $password);
        $sql_insert->bindParam(3, $isadmin);
        $sql_insert->execute();
    }
} catch (PDOException $e) {
    $addInfo['success'] = 0;
    $addInfo['errorcode'] = $e->getCode();
    $addInfo['errormessage'] = $e->getMessage();
    header("http/1.1 400 BadRequest");
    echo json_encode($addInfo, JSON_UNESCAPED_UNICODE);
    die;
}
echo json_encode($addInfo, JSON_UNESCAPED_UNICODE);
?>