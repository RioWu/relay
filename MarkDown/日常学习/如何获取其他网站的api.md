# 如何获取其他网站的api

```php
<?php

echo file_get_contents('https://api.weixin.qq.com/sns/jscode2session?appid=' . 'WxPayConfig::APPID' . '&secret=' . 'WxPayConfig::APPSECRET' . '&js_code=' . '123' . '&grant_type=authorization_code');

$url = 'https://api.weixin.qq.com/sns/jscode2session?appid=' . 'WxPayConfig::APPID' . '&secret=' . 'WxPayConfig::APPSECRET' . '&js_code=' . '123' . '&grant_type=authorization_code';

$curl = curl_init();

curl_setopt($curl, CURLOPT_URL, $url);

curl_setopt($curl, CURLOPT_HEADER, 1);

curl_setopt($curl, CURLOPT_SSL_VERIFYPEER, false); //这个是重点,规避ssl的证书检查。

curl_setopt($curl, CURLOPT_SSL_VERIFYHOST, FALSE);

curl_setopt($curl, CURLOPT_RETURNTRANSFER, 1);

$data = curl_exec($curl);

//echo 'Curl error: ' . curl_error($curl);

curl_close($curl);

var_dump($data);

?>
```

