<?php
session_start();

/*
 Demo storage using session
 Replace with database later
*/
if (!isset($_SESSION['users'])) {
    $_SESSION['users'] = [];
}
