<?php
include "../connect.php";
$json_array = json_decode(file_get_contents("php://input"), true);           //因为前端传递时使用json格式，所以不能用$POST直接获取
$userid = $json_array['userid'];
$delInfo = array('success' => 1, 'errorcode' => 200, 'errormessage' => '');
try {
    $conn = new PDO($dsn, $user, $passwd);
    $conn->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
    $conn->exec("DELETE FROM user where id = $userid");
} catch (PDOException $e) {
    $delInfo['success'] = 0;
    $delInfo['errorcode'] = $e->getCode();
    $delInfo['errormessage'] = $e->getMessage();
    header("http/1.1 400 BadRequest");
    echo json_encode($delInfo, JSON_UNESCAPED_UNICODE);
    die;
}
echo json_encode($delInfo, JSON_UNESCAPED_UNICODE);
?>