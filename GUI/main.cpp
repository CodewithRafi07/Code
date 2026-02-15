#include <QApplication>
#include "mainwindow.h"
#include "backend/product.h"
#include "backend/customer.h"
#include "backend/admin.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    // Set application information
    app.setApplicationName("I-ROS Management System");
    app.setApplicationVersion("1.0");
    app.setOrganizationName("I-ROS Team");
    
    // Load application style
    QFile styleFile(":/stylesheets/main.qss");
    if (styleFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        app.setStyleSheet(styleFile.readAll());
        styleFile.close();
    }
    
    // Initialize backend data
    ensureDataFolder();
    loadBranches();
    loadProducts();
    loadCustomers();
    loadAdmins();
    
    // Create and show main window
    MainWindow mainWindow;
    mainWindow.show();
    
    return app.exec();
}