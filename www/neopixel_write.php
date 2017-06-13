<?php
$cmd=$_GET['write'];
echo `/home/chip/bin/neopixel_write.sh $cmd`;
?>
