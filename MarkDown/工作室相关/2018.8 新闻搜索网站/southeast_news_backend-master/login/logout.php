<?php
session_start();
session_destroy();
$_SESSION = array();
$logout = array('success' => 1, 'errorcode' => '200', 'errormessage' => '');
echo $logout;
?>