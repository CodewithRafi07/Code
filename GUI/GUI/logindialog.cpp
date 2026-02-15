#include "logindialog.h"
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSettings>
#include <QIcon>
#include "registrationdialog.h"

LoginDialog::LoginDialog(bool isAdmin, QWidget *parent) : 
    QDialog(parent), isAdminMode(isAdmin) {
    setupUI();
    loadSavedCredentials();
}

LoginDialog::~LoginDialog() {
}

void LoginDialog::setupUI() {
    setWindowTitle(isAdminMode ? "Admin Login" : "Customer Login");
    setModal(true);
    setFixedSize(400, 300);
    
    // Create form elements
    usernameEdit = new QLineEdit();
    passwordEdit = new QLineEdit();
    passwordEdit->setEchoMode(QLineEdit::Password);
    rememberMeCheckBox = new QCheckBox("Remember Me");
    togglePasswordButton = new QToolButton();
    togglePasswordButton->setIcon(QIcon(":/icons/eye-off.png"));
    togglePasswordButton->setCursor(Qt::PointingHandCursor);
    loginButton = new QPushButton("Login");
    registerButton = new QPushButton("Register New Account");
    statusLabel = new QLabel();
    statusLabel->setAlignment(Qt::AlignCenter);
    forgotPasswordLabel = new QLabel("<a href='#'>Forgot Password?</a>");
    forgotPasswordLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
    
    // Create layout
    QFormLayout *formLayout = new QFormLayout();
    formLayout->addRow("Username:", usernameEdit);
    formLayout->addRow("Password:", passwordEdit);
    
    QHBoxLayout *passwordLayout = new QHBoxLayout();
    passwordLayout->addWidget(passwordEdit);
    passwordLayout->addWidget(togglePasswordButton);
    formLayout->insertRow(2, "Password:", passwordLayout);
    
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(loginButton);
    if (!isAdminMode) {
        buttonLayout->addWidget(registerButton);
    }
    
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(rememberMeCheckBox);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(forgotPasswordLabel);
    mainLayout->addWidget(statusLabel);
    
    // Connect signals
    connect(loginButton, &QPushButton::clicked, this, &LoginDialog::attemptLogin);
    connect(togglePasswordButton, &QToolButton::clicked, this, &LoginDialog::togglePasswordVisibility);
    connect(registerButton, &QPushButton::clicked, this, &LoginDialog::openRegistrationDialog);
    connect(forgotPasswordLabel, &QLabel::linkActivated, this, [this]() {
        statusLabel->setText("<font color='blue'>Password reset link sent to your email</font>");
    });
}

void LoginDialog::attemptLogin() {
    QString username = usernameEdit->text();
    QString password = passwordEdit->text();
    
    if (username.isEmpty() || password.isEmpty()) {
        showError("Please enter both username and password!");
        return;
    }
    
    bool loginSuccess = false;
    
    if (isAdminMode) {
        for (const auto &admin : admins) {
            if (admin.username == username.toStdString() && admin.password == password.toStdString()) {
                currentAdmin = admin;
                loginSuccess = true;
                break;
            }
        }
    } else {
        for (const auto &customer : customers) {
            if (customer.username == username.toStdString() && customer.password == password.toStdString()) {
                currentCustomer = customer;
                loginSuccess = true;
                break;
            }
        }
    }
    
    if (loginSuccess) {
        if (rememberMeCheckBox->isChecked()) {
            saveCredentials();
        }
        accept();
    } else {
        showError("Invalid username or password!");
    }
}

void LoginDialog::togglePasswordVisibility() {
    if (passwordEdit->echoMode() == QLineEdit::Password) {
        passwordEdit->setEchoMode(QLineEdit::Normal);
        togglePasswordButton->setIcon(QIcon(":/icons/eye-on.png"));
    } else {
        passwordEdit->setEchoMode(QLineEdit::Password);
        togglePasswordButton->setIcon(QIcon(":/icons/eye-off.png"));
    }
}

void LoginDialog::openRegistrationDialog() {
    RegistrationDialog regDialog(this);
    if (regDialog.exec() == QDialog::Accepted) {
        usernameEdit->setText(regDialog.getUsername());
        passwordEdit->setFocus();
    }
}

void LoginDialog::loadSavedCredentials() {
    QSettings settings;
    if (settings.value("rememberMe", false).toBool()) {
        usernameEdit->setText(settings.value("username", "").toString());
        passwordEdit->setText(settings.value("password", "").toString());
        rememberMeCheckBox->setChecked(true);
    }
}

void LoginDialog::saveCredentials() {
    QSettings settings;
    settings.setValue("rememberMe", true);
    settings.setValue("username", usernameEdit->text());
    settings.setValue("password", passwordEdit->text());
}

void LoginDialog::showError(const QString &message) {
    statusLabel->setText("<font color='red'>" + message + "</font>");
}

Customer LoginDialog::getCurrentCustomer() const {
    return currentCustomer;
}

Admin LoginDialog::getCurrentAdmin() const {
    return currentAdmin;
}