<?php
session_start();
if (!isset($_SESSION['logged_in'])) {
    header("Location: auth/login.php");
    exit;
}
?>

<!DOCTYPE html>
<html>
<head>
<title>Dashboard</title>
</head>
<body>

<h1>Welcome 🎉</h1>
<p>You are logged in as <?= $_SESSION['user_email'] ?></p>

<a href="auth/logout.php">Logout</a>

</body>
</html>
