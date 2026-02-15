<?php
include "../config/init.php";
$msg = "";

if (isset($_POST['reset'])) {
    $email = trim($_POST['email']);

    if (!isset($_SESSION['users'][$email])) {
        $msg = "No account found with this email.";
    } else {
        $msg = "Password reset link sent (demo response).";
    }
}
?>

<!DOCTYPE html>
<html>
<head>
<title>Forgot Password</title>
<link rel="stylesheet" href="../assets/css/style.css">
</head>
<body>

<div class="auth-box">
<h2>Forgot Password</h2>

<form method="post">
<input type="email" name="email" placeholder="Enter your email" required>
<button name="reset">Send Reset Link</button>
</form>

<div class="links">
<a href="login.php">Back to Login</a>
</div>

<p class="success"><?= $msg ?></p>
</div>

</body>
</html>
