#ifndef TOOLSMANAGER_H
#define TOOLSMANAGER_H
#include <QPainter>
#include <QPrinter>
#include <QPdfWriter>
#include <QTextDocument>
#include <QPdfWriter>
#include <QTextDocument>
#include <QObject>
#include <QTableWidget>
#include <QSqlQuery>
#include <QMessageBox>
#include <QDate>
#include <QMap>
#include <QVariant>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QPixmap>
#include <QIcon>
#include <QFileInfo>
#include <utility>
#include <QPair>
#include "connection.h"
#include "qrcodegen.hpp"
using qrcodegen::QrCode;
using qrcodegen::QrSegment;

class ToolsManager : public QObject {
    Q_OBJECT

public:
    explicit ToolsManager(QTableWidget *table = nullptr, QObject *parent = nullptr);
    ~ToolsManager();

    QSqlQuery getAllToolsQuery() const;
    // Setters for tool properties
    void setId(int id) { this->id = id; }
    void setNomMateriel(const QString &nom) { nomMateriel = nom; }
    void setCategorie(const QString &categorie) { this->categorie = categorie; }
    void setDescription(const QString &description) { this->description = description; }
    void setStock(int stock) { this->stock = stock; }
    void setQuantiteMaximale(int quantiteMax) { quantiteMaximale = quantiteMax; }
    void setUploadImage(const QString &imagePath) { uploadImage = imagePath; }
    void setFournisseur(const QString &fournisseur) { this->fournisseur = fournisseur; }
    void setPositionFournisseur(const QString &position) { positionFournisseur = position; }

    QStringList getFournisseurNames() const;
    void showFournisseurLocation(const QString &name);

    // Database operations
    int getNextId();
    void loadTools();
    bool addTool();
    bool editTool();
    bool deleteTool();
    QMap<QString, QVariant> getToolById(int id);
    bool idProExists(int idPro);
    QStringList getCategories() const;

    void searchTools(const QString &text, const QString &field);
    void sortTools(const QString &field, Qt::SortOrder order);

    // methods for pagination
    void setItemsPerPage(int count);
    void goToPage(int page);
    int getTotalPages() const;
    int getCurrentPage() const;

    // PDF export method
    bool exportToPDF(const QString &filePath);
    // bool verifyImageMatchesName(const QString& imagePath, const QString& productName);
    //   bool downloadPreTrainedModel(const QString& modelDir);
    void detectScissors(const QString &imagePath, const QString &productName);
signals:
    void editToolRequested(int id);
    void deleteToolRequested(int id);
    void paginationChanged(int currentPage, int totalPages);
    void dataChanged();
    void qrCodeRequested(int toolId);
    void fournisseurClicked(const QString &name);


private:


    mutable DatabaseConnection dbConnection;
    bool validateToolData() const;
    void setupTable();
    QTableWidget *toolsTable;



    void initializeFournisseurs();

    // Tool properties
    int id = 0;
    QString nomMateriel;
    QString categorie;
    QString description;
    int stock = 0;
    int quantiteMaximale = 0;
    QString uploadImage;
    QString fournisseur;
    QString positionFournisseur;
    int idPro = 0;
    // Pagination members
    int itemsPerPage;
    int currentPage;
    int totalTools;

    void updatePagination();
    void countTotalTools();
};

#endif // TOOLSMANAGER_H
