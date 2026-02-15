#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QPixmap>
#include <QIcon>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setupUI();
    setupMenus();
    setupStatusBar();
    updateStatusBar();
}

MainWindow::~MainWindow() {
}

void MainWindow::setupUI() {
    setWindowTitle("I-ROS Management System");
    setMinimumSize(800, 600);
    
    // Create central widget and layout
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    
    // Create stacked widget for different views
    stackedWidget = new QStackedWidget();
    
    // Create welcome screen
    QWidget *welcomeScreen = new QWidget();
    QVBoxLayout *welcomeLayout = new QVBoxLayout(welcomeScreen);
    
    QLabel *logoLabel = new QLabel();
    logoLabel->setPixmap(QPixmap(":/images/logo.png").scaled(200, 200, Qt::KeepAspectRatio));
    logoLabel->setAlignment(Qt::AlignCenter);
    
    QLabel *titleLabel = new QLabel("I-ROS Management System");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; margin: 20px;");
    
    QLabel *subtitleLabel = new QLabel("Integrated Retail & Ordering System");
    subtitleLabel->setAlignment(Qt::AlignCenter);
    subtitleLabel->setStyleSheet("font-size: 16px; color: gray;");
    
    QPushButton *adminButton = new QPushButton("Admin Panel");
    adminButton->setIcon(QIcon(":/icons/admin.png"));
    adminButton->setMinimumHeight(50);
    adminButton->setStyleSheet("font-size: 16px;");
    
    QPushButton *customerButton = new QPushButton("Customer Panel");
    customerButton->setIcon(QIcon(":/icons/customer.png"));
    customerButton->setMinimumHeight(50);
    customerButton->setStyleSheet("font-size: 16px;");
    
    welcomeLayout->addStretch();
    welcomeLayout->addWidget(logoLabel);
    welcomeLayout->addWidget(titleLabel);
    welcomeLayout->addWidget(subtitleLabel);
    welcomeLayout->addSpacing(30);
    welcomeLayout->addWidget(adminButton);
    welcomeLayout->addWidget(customerButton);
    welcomeLayout->addStretch();
    
    // Add screens to stacked widget
    stackedWidget->addWidget(welcomeScreen);
    
    mainLayout->addWidget(stackedWidget);
    setCentralWidget(centralWidget);
    
    // Connect signals
    connect(adminButton, &QPushButton::clicked, this, &MainWindow::showAdminLogin);
    connect(customerButton, &QPushButton::clicked, this, &MainWindow::showCustomerLogin);
}

void MainWindow::setupMenus() {
    // Create menu bar
    QMenuBar *menuBar = this->menuBar();
    
    // File menu
    QMenu *fileMenu = menuBar->addMenu("&File");
    
    adminLoginAction = new QAction("Admin Login", this);
    adminLoginAction->setIcon(QIcon(":/icons/admin.png"));
    connect(adminLoginAction, &QAction::triggered, this, &MainWindow::showAdminLogin);
    fileMenu->addAction(adminLoginAction);
    
    customerLoginAction = new QAction("Customer Login", this);
    customerLoginAction->setIcon(QIcon(":/icons/customer.png"));
    connect(customerLoginAction, &QAction::triggered, this, &MainWindow::showCustomerLogin);
    fileMenu->addAction(customerLoginAction);
    
    fileMenu->addSeparator();
    
    logoutAction = new QAction("Logout", this);
    logoutAction->setIcon(QIcon(":/icons/logout.png"));
    logoutAction->setEnabled(false);
    connect(logoutAction, &QAction::triggered, this, &MainWindow::logout);
    fileMenu->addAction(logoutAction);
    
    fileMenu->addSeparator();
    
    QAction *exitAction = new QAction("Exit", this);
    connect(exitAction, &QAction::triggered, this, &QMainWindow::close);
    fileMenu->addAction(exitAction);
    
    // Tools menu
    QMenu *toolsMenu = menuBar->addMenu("&Tools");
    
    productSearchAction = new QAction("Product Search", this);
    productSearchAction->setIcon(QIcon(":/icons/search.png"));
    connect(productSearchAction, &QAction::triggered, this, &MainWindow::showProductSearch);
    toolsMenu->addAction(productSearchAction);
    
    // Help menu
    QMenu *helpMenu = menuBar->addMenu("&Help");
    
    QAction *aboutAction = new QAction("About", this);
    connect(aboutAction, &QAction::triggered, [this]() {
        QMessageBox::about(this, "About I-ROS", 
                          "I-ROS Management System v1.0\n\n"
                          "Integrated Retail & Ordering System\n\n"
                          "© 2023 I-ROS Team. All rights reserved.");
    });
    helpMenu->addAction(aboutAction);
}

void MainWindow::setupStatusBar() {
    statusLabel = new QLabel("Ready");
    statusBar()->addWidget(statusLabel);
    
    // Add permanent widget for branch info
    QLabel *branchLabel = new QLabel("Branch: Not Selected");
    statusBar()->addPermanentWidget(branchLabel);
}

void MainWindow::showAdminLogin() {
    LoginDialog loginDialog(true, this);
    if (loginDialog.exec() == QDialog::Accepted) {
        // In a real implementation, this would open the admin panel
        statusLabel->setText("Logged in as Admin: " + QString::fromStdString(loginDialog.getCurrentAdmin().username));
        logoutAction->setEnabled(true);
        
        QMessageBox::information(this, "Login Successful", 
                                "Welcome to the Admin Panel!");
    }
}

void MainWindow::showCustomerLogin() {
    LoginDialog loginDialog(false, this);
    if (loginDialog.exec() == QDialog::Accepted) {
        // In a real implementation, this would open the customer panel
        statusLabel->setText("Logged in as Customer: " + QString::fromStdString(loginDialog.getCurrentCustomer().username));
        logoutAction->setEnabled(true);
        
        QMessageBox::information(this, "Login Successful", 
                                "Welcome to the Customer Panel!");
    }
}

void MainWindow::showProductSearch() {
    ProductSearchDialog searchDialog(this);
    searchDialog.exec();
}

void MainWindow::logout() {
    statusLabel->setText("Logged out");
    logoutAction->setEnabled(false);
    
    QMessageBox::information(this, "Logout", 
                            "You have been successfully logged out.");
}

void MainWindow::updateStatusBar() {
    // Update status bar with current time
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, [this]() {
        statusLabel->setText("Ready | " + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    });
    timer->start(1000);
}