<?php
require_once("./db.php");
$sql = "SELECT * FROM `weather` ORDER BY `time` DESC LIMIT 1;";
$result = mysqli_query($link, $sql);
while($row = mysqli_fetch_array($result)) {
    echo "temperture: ".$row['temp']." deg C<br>humidity: ".$row['humid']." %<br>uv: ".$row['uv']." mW/cm^2<br>time: ".$row['time'];
}
?>
