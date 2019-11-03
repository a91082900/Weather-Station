<?php

    $host = "localhost";
    $username = "USERNAME";
    $password = "PWD";
    $db = "iot";
    $link = mysqli_connect($host, $username, $password, $db);
    mysqli_set_charset($link, "utf8");
    if(!$link)
        echo mysqli_connect_errno() . ":" . mysqli_connect_error();
    //else echo 'OK';
?>

