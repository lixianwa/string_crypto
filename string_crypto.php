<?php

if(!extension_loaded('string_crypto')) {
	dl('string_crypto.' . PHP_SHLIB_SUFFIX);
}


for($i = 0; $i < 100000; $i++)
{
	$en =  string_cookie_encrypt("username:lixian&password:123456");
	$de = string_cookie_decrypt($en);
	echo "$i $de\n";
}

?>
