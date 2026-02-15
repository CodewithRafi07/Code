<?php
include "../config/init.php";
$error = $success = "";

if (isset($_POST['register'])) {
    $email = trim($_POST['email']);
    $password = $_POST['password'];

    if (!filter_var($email, FILTER_VALIDATE_EMAIL)) {
        $error = "Invalid email address.";
    } elseif (!preg_match("/^(?=.*[A-Z])(?=.*\d)(?=.*[@$!%*?&]).{8,}$/", $password)) {
        $error = "Password must be 8+ chars, with capital, number & symbol.";
    } elseif (isset($_SESSION['users'][$email])) {
        $error = "Account already exists. Please login.";
    } else {
        $_SESSION['users'][$email] = password_hash($password, PASSWORD_DEFAULT);
        $success = "Registration successful. You can login now.";
    }
}
?>

<!DOCTYPE html>
<html>
<head>
<title>Register</title>
<link rel="stylesheet" href="../assets/css/style.css">
</head>
<body>

<div class="auth-box">
<h2>Register</h2>

<form method="post">
<input type="email" name="email" placeholder="Email" required>
<input type="password" name="password" placeholder="Password" required>
<button name="register">Register</button>
</form>

<div class="links">
<a href="login.php">Already have an account? Login</a>
</div>

<p class="error"><?= $error ?></p>
<p class="success"><?= $success ?></p>
</div>

</body>
</html>
