<html>  
<head>  
<script src="https://ajax.googleapis.com/ajax/libs/jquery/1.3.0/jquery.min.js"></script>  
<script> 
$(document).ready(function(){
	$("#content").load("load.php");
});
setInterval(  
	function()  
	{  
		$('#content').load('load.php');  
	}, 300000);  
</script>  
<style>  
#content {   
font-size:15px;  
font-weight:bold;  
padding-top:10px;    
}  
#content,h1{  
     text-align: center;  
     }  
</style>  
<title>Weather Data</title>  
</head>  
<body>  
<div id="content"></div>  
</body>  
</html> 
