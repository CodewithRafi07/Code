#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QAction>
#include <QMenuBar>
#include <QStatusBar>
#include <QLabel>
#include "logindialog.h"
#include "productsearchdialog.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void showAdminLogin();
    void showCustomerLogin();
    void showProductSearch();
    void logout();
    void updateStatusBar();

private:
    QStackedWidget *stackedWidget;
    QLabel *statusLabel;
    QAction *adminLoginAction;
    QAction *customerLoginAction;
    QAction *productSearchAction;
    QAction *logoutAction;
    
    void setupUI();
    void setupMenus();
    void setupStatusBar();
};

#endif // MAINWINDOW_H