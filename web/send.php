<?php
require_once("./db.php");
$humid = $_POST["humid"];
$temp = $_POST["temp"];
$uv = $_POST["uv"];
echo "$humid, $temp, $uv\n";

if(!empty($humid) && !empty($temp) && !empty($uv)) {
    $sql = "INSERT INTO `weather` (`temp`, `humid`, `uv`, `longitude`, `latitude`, `time`)
VALUES ('$temp', '$humid', '$uv', NULL, NULL, now());";
    if(mysqli_query($link, $sql)) echo "Upload OK";
}
?>
