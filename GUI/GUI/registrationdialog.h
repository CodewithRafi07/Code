#ifndef REGISTRATIONDIALOG_H
#define REGISTRATIONDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QCheckBox>
#include <QMessageBox>
#include "backend/customer.h"

class RegistrationDialog : public QDialog {
    Q_OBJECT

public:
    explicit RegistrationDialog(QWidget *parent = nullptr);
    ~RegistrationDialog();

private slots:
    void attemptRegistration();
    void validatePassword();
    void checkUsernameAvailability();

private:
    QLineEdit *usernameEdit;
    QLineEdit *passwordEdit;
    QLineEdit *confirmPasswordEdit;
    QLineEdit *emailEdit;
    QCheckBox *termsCheckBox;
    QPushButton *registerButton;
    QLabel *statusLabel;
    
    void setupUI();
    bool validateInputs();
    void showError(const QString &message);
    void showSuccess(const QString &message);
};

#endif // REGISTRATIONDIALOG_H