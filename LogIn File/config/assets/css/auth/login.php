<?php
include "../config/init.php";
$error = "";

if (isset($_POST['login'])) {
    $email = trim($_POST['email']);
    $password = $_POST['password'];

    if (!isset($_SESSION['users'][$email])) {
        $error = "No account found. Please register first.";
    } elseif (!password_verify($password, $_SESSION['users'][$email])) {
        $error = "Incorrect password.";
    } else {
        $_SESSION['logged_in'] = true;
        $_SESSION['user_email'] = $email;
        header("Location: ../dashboard.php");
        exit;
    }
}
?>

<!DOCTYPE html>
<html>
<head>
<title>Login</title>
<link rel="stylesheet" href="../assets/css/style.css">
</head>
<body>

<div class="auth-box">
<h2>Login</h2>

<form method="post">
<input type="email" name="email" placeholder="Email" required>
<input type="password" name="password" placeholder="Password" required>
<button name="login">Login</button>
</form>

<div class="links">
<a href="register.php">Register</a> |
<a href="forgot-password.php">Forgot Password?</a>
</div>

<p class="error"><?= $error ?></p>
</div>

</body>
</html>
