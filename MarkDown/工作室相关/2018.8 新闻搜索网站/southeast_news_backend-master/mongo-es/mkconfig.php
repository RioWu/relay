<?php
fwrite(STDOUT,"Enter the language:  ");
$language = trim(fgets(STDIN));
fwrite(STDOUT,"Enter the website name:   ");
$website = trim(fgets(STDIN));
$config_json = file_get_contents("./config.json");
$config_array = json_decode($config_json,true);
$config_array["elasticsearch"]["indices"][0]["index"] = "$language" . "_" . "$website";
$config_array["tasks"][0]["extract"]["db"] = $language;
$config_array["tasks"][0]["extract"]["collection"] = $website;
$config_array["tasks"][0]["load"]["index"] = "$language" . "_" . "$website";
$config = json_encode($config_array,JSON_UNESCAPED_SLASHES);
file_put_contents("./config/$language/$website.json",$config);
?>
