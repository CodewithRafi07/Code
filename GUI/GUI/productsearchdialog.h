#ifndef PRODUCTSEARCHDIALOG_H
#define PRODUCTSEARCHDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QLabel>
#include <QProgressBar>
#include <QTimer>
#include <QHeaderView>
#include "backend/product.h"

class ProductSearchDialog : public QDialog {
    Q_OBJECT

public:
    explicit ProductSearchDialog(QWidget *parent = nullptr);
    ~ProductSearchDialog();

private slots:
    void startSearch();
    void performSearch();
    void displayResults(const QVector<Product> &results);
    void addToCart();
    void viewDetails();

private:
    QLineEdit *searchEdit;
    QPushButton *searchButton;
    QTableWidget *resultsTable;
    QLabel *statusLabel;
    QProgressBar *loadingIndicator;
    QTimer *searchTimer;
    
    void setupUI();
    void setupTable();
    void clearResults();
    void showLoading(bool show);
};

#endif // PRODUCTSEARCHDIALOG_H