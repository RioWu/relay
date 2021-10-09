<?php
include "../connect.php";
$json_array = json_decode(file_get_contents("php://input"), true);           //因为前端传递时使用json格式，所以不能用$POST直接获取
$userid = $json_array['userid'];
$new_pwd = md5($json_array['password']);
$changeInfo = array('success' => 1, 'errorcode' => 200, 'errormessage' => '');
try {
    $conn = new PDO($dsn, $user, $passwd);
    $conn->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
    $sql = $conn->prepare("UPDATE user SET password = ?");   	//使用预处理防止sql注入
    $sql->bindParam(1, $new_pwd);
    $sql->execute();
} catch (PDOException $e) {
    $changeInfo['success'] = 0;
    $changeInfo['errorcode'] = $e->getCode();
    $changeInfo['errormessage'] = $e->getMessage();
    header("http/1.1 400 BadRequest");
    echo json_encode($changeInfo, JSON_UNESCAPED_UNICODE);
}
echo json_encode($changeInfo, JSON_UNESCAPED_UNICODE);
?>