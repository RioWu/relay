<?php
$language = ['indonesia', 'vietnam'];
$url = "http://localhost:9200/_cat/indices";
$curl = curl_init();
curl_setopt($curl, CURLOPT_URL, $url);
curl_setopt($curl, CURLOPT_RETURNTRANSFER, 1);   //参数为1表示传输数据，为0表示直接输出显示。
curl_setopt($curl, CURLOPT_HEADER, 0);           //参数为0表示不带头文件，为1表示带头文件
$es_return = curl_exec($curl);
$array = explode("\n", $es_return);                 //将es返回的数据处理成数组（一行一个数组）
array_pop($array);
foreach ($array as $key => $value) {
    $value = explode(" ", $value);               //再根据空格生成多个数组
    $value = array_unique($value);               //删除数组中的重复元素
    $value = $value[2];
    $array[$key] = $value;                          //取出索引的名称
    $judge = explode("_", $value);
    $judge2 = strstr($value,'kibana');
    if ($judge === false or $judge2 === true) {
        unset($array[$key]);
    } else {
        $judge = $judge[0];
        if (in_array($judge, $language) === false) {
            unset($array[$key]);
        }
    }
}
unset($value);
global $return;
$return = [];
foreach ($array as $value) {
    $value = explode("_", $value);
    array_push($return, $value[0]);
}
$return = array_unique($return);          //删掉数组中的重复元素
foreach ($return as $key => $value) {
    $value = ucfirst($value);             //应前端要求，首字母大写
    $return[$value] = [];
    foreach ($array as $value_2) {
        $value_2 = explode("_", $value_2);
        if (ucfirst($value_2[0]) == $value) {
            array_push($return[$value], $value_2[1]);    //array_push 向数组中导入元素
        }
    }
    unset($return[$key]);
}

echo json_encode($return, JSON_UNESCAPED_UNICODE);
?>
