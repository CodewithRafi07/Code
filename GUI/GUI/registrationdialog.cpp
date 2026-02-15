#include "registrationdialog.h"
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QRegExpValidator>
#include <QRegExp>

RegistrationDialog::RegistrationDialog(QWidget *parent) : QDialog(parent) {
    setupUI();
}

RegistrationDialog::~RegistrationDialog() {
}

void RegistrationDialog::setupUI() {
    setWindowTitle("Create Account");
    setModal(true);
    setFixedSize(400, 350);
    
    // Create form elements
    usernameEdit = new QLineEdit();
    passwordEdit = new QLineEdit();
    passwordEdit->setEchoMode(QLineEdit::Password);
    confirmPasswordEdit = new QLineEdit();
    confirmPasswordEdit->setEchoMode(QLineEdit::Password);
    emailEdit = new QLineEdit();
    termsCheckBox = new QCheckBox("I agree to the Terms and Conditions");
    registerButton = new QPushButton("Register");
    statusLabel = new QLabel();
    statusLabel->setAlignment(Qt::AlignCenter);
    
    // Set up validators
    QRegExp usernameRegex("^[a-zA-Z0-9_]{4,20}$");
    usernameEdit->setValidator(new QRegExpValidator(usernameRegex, this));
    
    QRegExp emailRegex("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$");
    emailEdit->setValidator(new QRegExpValidator(emailRegex, this));
    
    // Create layout
    QFormLayout *formLayout = new QFormLayout();
    formLayout->addRow("Username:", usernameEdit);
    formLayout->addRow("Email:", emailEdit);
    formLayout->addRow("Password:", passwordEdit);
    formLayout->addRow("Confirm Password:", confirmPasswordEdit);
    formLayout->addRow(termsCheckBox);
    
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(registerButton);
    
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(formLayout);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(statusLabel);
    
    // Connect signals
    connect(registerButton, &QPushButton::clicked, this, &RegistrationDialog::attemptRegistration);
    connect(passwordEdit, &QLineEdit::textChanged, this, &RegistrationDialog::validatePassword);
    connect(confirmPasswordEdit, &QLineEdit::textChanged, this, &RegistrationDialog::validatePassword);
    connect(usernameEdit, &QLineEdit::textChanged, this, &RegistrationDialog::checkUsernameAvailability);
}

void RegistrationDialog::attemptRegistration() {
    if (!validateInputs()) {
        return;
    }
    
    // Check if username already exists
    for (const auto &customer : customers) {
        if (customer.username == usernameEdit->text().toStdString()) {
            showError("Username already exists!");
            return;
        }
    }
    
    // Create new customer
    Customer newCustomer;
    newCustomer.username = usernameEdit->text().toStdString();
    newCustomer.password = passwordEdit->text().toStdString();
    newCustomer.isMember = false;
    newCustomer.loyaltyPoints = 0;
    
    // Save customer
    saveCustomer(newCustomer);
    
    // Show success message
    showSuccess("Registration successful! You can now log in.");
    
    // Close dialog after a short delay
    QTimer::singleShot(1500, this, &QDialog::accept);
}

bool RegistrationDialog::validateInputs() {
    if (usernameEdit->text().isEmpty()) {
        showError("Username is required!");
        return false;
    }
    
    if (emailEdit->text().isEmpty()) {
        showError("Email is required!");
        return false;
    }
    
    if (passwordEdit->text().length() < 6) {
        showError("Password must be at least 6 characters!");
        return false;
    }
    
    if (passwordEdit->text() != confirmPasswordEdit->text()) {
        showError("Passwords do not match!");
        return false;
    }
    
    if (!termsCheckBox->isChecked()) {
        showError("You must agree to the terms and conditions!");
        return false;
    }
    
    return true;
}

void RegistrationDialog::validatePassword() {
    QString password = passwordEdit->text();
    QString confirmPassword = confirmPasswordEdit->text();
    
    if (!confirmPassword.isEmpty() && password != confirmPassword) {
        confirmPasswordEdit->setStyleSheet("QLineEdit { background-color: #ffcccc; }");
    } else {
        confirmPasswordEdit->setStyleSheet("");
    }
}

void RegistrationDialog::checkUsernameAvailability() {
    QString username = usernameEdit->text();
    
    if (username.length() < 4) {
        return;
    }
    
    bool available = true;
    for (const auto &customer : customers) {
        if (customer.username == username.toStdString()) {
            available = false;
            break;
        }
    }
    
    if (available) {
        usernameEdit->setStyleSheet("QLineEdit { background-color: #ccffcc; }");
    } else {
        usernameEdit->setStyleSheet("QLineEdit { background-color: #ffcccc; }");
    }
}

void RegistrationDialog::showError(const QString &message) {
    statusLabel->setText("<font color='red'>" + message + "</font>");
}

void RegistrationDialog::showSuccess(const QString &message) {
    statusLabel->setText("<font color='green'>" + message + "</font>");
}