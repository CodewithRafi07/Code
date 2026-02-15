#include "productsearchdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QMessageBox>

ProductSearchDialog::ProductSearchDialog(QWidget *parent) : QDialog(parent) {
    setupUI();
    setupTable();
    
    searchTimer = new QTimer(this);
    searchTimer->setSingleShot(true);
    searchTimer->setInterval(500); // 500ms delay to avoid excessive searching
    
    connect(searchTimer, &QTimer::timeout, this, &ProductSearchDialog::performSearch);
    connect(searchEdit, &QLineEdit::textChanged, [this]() {
        searchTimer->start();
    });
    connect(searchButton, &QPushButton::clicked, this, &ProductSearchDialog::startSearch);
    connect(resultsTable, &QTableWidget::doubleClicked, this, &ProductSearchDialog::viewDetails);
}

ProductSearchDialog::~ProductSearchDialog() {
}

void ProductSearchDialog::setupUI() {
    setWindowTitle("Product Search");
    setModal(true);
    resize(800, 600);
    
    // Create UI elements
    searchEdit = new QLineEdit();
    searchEdit->setPlaceholderText("Enter product name or ID...");
    searchButton = new QPushButton("Search");
    resultsTable = new QTableWidget();
    statusLabel = new QLabel("Enter a search term to find products");
    statusLabel->setAlignment(Qt::AlignCenter);
    loadingIndicator = new QProgressBar();
    loadingIndicator->setRange(0, 0); // Indeterminate progress
    loadingIndicator->setVisible(false);
    
    // Create layout
    QHBoxLayout *searchLayout = new QHBoxLayout();
    searchLayout->addWidget(searchEdit);
    searchLayout->addWidget(searchButton);
    
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(searchLayout);
    mainLayout->addWidget(loadingIndicator);
    mainLayout->addWidget(resultsTable);
    mainLayout->addWidget(statusLabel);
}

void ProductSearchDialog::setupTable() {
    resultsTable->setColumnCount(7);
    resultsTable->setHorizontalHeaderLabels({"ID", "Name", "Category", "Supplier", "Price", "Stock", "Discount"});
    resultsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    resultsTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    resultsTable->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    resultsTable->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
    resultsTable->horizontalHeader()->setSectionResizeMode(6, QHeaderView::ResizeToContents);
    resultsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    resultsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    resultsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    resultsTable->setAlternatingRowColors(true);
    
    // Add context menu for right-click actions
    resultsTable->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(resultsTable, &QTableWidget::customContextMenuRequested, [this](const QPoint &pos) {
        if (resultsTable->itemAt(pos)) {
            QMenu contextMenu(this);
            QAction *addToCartAction = contextMenu.addAction("Add to Cart");
            QAction *viewDetailsAction = contextMenu.addAction("View Details");
            
            connect(addToCartAction, &QAction::triggered, this, &ProductSearchDialog::addToCart);
            connect(viewDetailsAction, &QAction::triggered, this, &ProductSearchDialog::viewDetails);
            
            contextMenu.exec(resultsTable->mapToGlobal(pos));
        }
    });
}

void ProductSearchDialog::startSearch() {
    searchTimer->start();
}

void ProductSearchDialog::performSearch() {
    QString searchTerm = searchEdit->text().trimmed();
    
    if (searchTerm.isEmpty()) {
        clearResults();
        statusLabel->setText("Enter a search term to find products");
        return;
    }
    
    showLoading(true);
    statusLabel->setText("Searching for products...");
    
    // Simulate a delay for searching (in a real app, this might be a network request)
    QTimer::singleShot(800, [this, searchTerm]() {
        QVector<Product> results;
        QString searchTermLower = searchTerm.toLower();
        
        for (const auto &product : products) {
            QString nameLower = QString::fromStdString(product.name).toLower();
            QString idLower = QString::fromStdString(product.id).toLower();
            
            if (nameLower.contains(searchTermLower) || idLower.contains(searchTermLower)) {
                results.append(product);
            }
        }
        
        displayResults(results);
        showLoading(false);
    });
}

void ProductSearchDialog::displayResults(const QVector<Product> &results) {
    clearResults();
    
    if (results.isEmpty()) {
        statusLabel->setText("No products found matching your search criteria");
        return;
    }
    
    resultsTable->setRowCount(results.size());
    
    for (int i = 0; i < results.size(); ++i) {
        const Product &p = results[i];
        
        resultsTable->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(p.id)));
        resultsTable->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(p.name)));
        resultsTable->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(p.category)));
        resultsTable->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(p.supplierName)));
        resultsTable->setItem(i, 4, new QTableWidgetItem(QString::number(p.price, 'f', 2)));
        resultsTable->setItem(i, 5, new QTableWidgetItem(QString::number(p.stock)));
        
        // Create discount item with special formatting
        QTableWidgetItem *discountItem = new QTableWidgetItem(QString::number(p.discount) + "%");
        if (p.discount > 0) {
            discountItem->setBackground(QColor(255, 240, 200)); // Light orange background
            discountItem->setForeground(QColor(180, 0, 0)); // Dark red text
        }
        resultsTable->setItem(i, 6, discountItem);
        
        // Highlight low stock items
        if (p.stock < 15) {
            for (int col = 0; col < resultsTable->columnCount(); ++col) {
                QTableWidgetItem *item = resultsTable->item(i, col);
                if (item) {
                    item->setBackground(QColor(255, 200, 200)); // Light red background
                }
            }
        }
    }
    
    statusLabel->setText(QString("Found %1 product(s)").arg(results.size()));
}

void ProductSearchDialog::addToCart() {
    int currentRow = resultsTable->currentRow();
    if (currentRow >= 0) {
        QString productId = resultsTable->item(currentRow, 0)->text();
        QString productName = resultsTable->item(currentRow, 1)->text();
        
        // In a real implementation, this would call the backend addToCart function
        QMessageBox::information(this, "Added to Cart", 
                                QString("%1 has been added to your cart.").arg(productName));
    }
}

void ProductSearchDialog::viewDetails() {
    int currentRow = resultsTable->currentRow();
    if (currentRow >= 0) {
        QString productId = resultsTable->item(currentRow, 0)->text();
        
        // In a real implementation, this would open a product details dialog
        QMessageBox::information(this, "Product Details", 
                                QString("Details for product ID: %1").arg(productId));
    }
}

void ProductSearchDialog::clearResults() {
    resultsTable->setRowCount(0);
}

void ProductSearchDialog::showLoading(bool show) {
    loadingIndicator->setVisible(show);
    searchEdit->setEnabled(!show);
    searchButton->setEnabled(!show);
    resultsTable->setEnabled(!show);
}