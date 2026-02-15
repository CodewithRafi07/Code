#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QCheckBox>
#include <QToolButton>
#include <QSettings>
#include "backend/customer.h"
#include "backend/admin.h"

class LoginDialog : public QDialog {
    Q_OBJECT

public:
    explicit LoginDialog(bool isAdmin = false, QWidget *parent = nullptr);
    ~LoginDialog();
    
    Customer getCurrentCustomer() const;
    Admin getCurrentAdmin() const;

private slots:
    void attemptLogin();
    void togglePasswordVisibility();
    void openRegistrationDialog();

private:
    QLineEdit *usernameEdit;
    QLineEdit *passwordEdit;
    QCheckBox *rememberMeCheckBox;
    QToolButton *togglePasswordButton;
    QPushButton *loginButton;
    QPushButton *registerButton;
    QLabel *statusLabel;
    QLabel *forgotPasswordLabel;
    bool isAdminMode;
    Customer currentCustomer;
    Admin currentAdmin;
    
    void setupUI();
    void loadSavedCredentials();
    void saveCredentials();
    void showError(const QString &message);
};

#endif // LOGINDIALOG_H