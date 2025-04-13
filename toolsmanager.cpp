#include "toolsmanager.h"
#include "connection.h"
#include <QHeaderView>
#include <QSqlError>
#include <QDebug>
#include <QSqlRecord>

ToolsManager::ToolsManager(QTableWidget *table, QObject *parent)
    : QObject(parent), toolsTable(table), itemsPerPage(10), currentPage(1) {
    setupTable();
    countTotalTools();
    loadTools();
}

ToolsManager::~ToolsManager() {

}
QSqlQuery ToolsManager::getAllToolsQuery() const {
    QSqlQuery query(dbConnection.getDatabase());
    query.prepare("SELECT categorie, stock, quantiteMaximale FROM RessourcesMedicales");
    query.exec();
    return query;
}
void ToolsManager::setItemsPerPage(int count) {
    if (count > 0) {
        itemsPerPage = count;
        currentPage = 1; // Reset to first page when changing items per page
        loadTools();
        emit paginationChanged(currentPage, getTotalPages());
    }
}

void ToolsManager::goToPage(int page) {
    int totalPages = getTotalPages();
    if (page >= 1 && page <= totalPages) {
        currentPage = page;
        loadTools();
        emit paginationChanged(currentPage, totalPages);
    }
}

int ToolsManager::getTotalPages() const {
    return (totalTools + itemsPerPage - 1) / itemsPerPage; // Ceiling division
}

int ToolsManager::getCurrentPage() const {
    return currentPage;
}

void ToolsManager::countTotalTools() {
    totalTools = 0;

    if (!dbConnection.createConnection()) {
        QMessageBox::critical(nullptr, "Erreur de base de données", "Impossible de se connecter à la base de données");
        return;
    }

    QSqlQuery query(dbConnection.getDatabase());
    query.prepare("SELECT COUNT(*) FROM RessourcesMedicales");

    if (query.exec() && query.next()) {
        totalTools = query.value(0).toInt();
    } else {
        qDebug() << "Erreur lors du comptage des outils:" << query.lastError().text();
    }
}

void ToolsManager::setupTable() {
    QStringList headers = {
        "Nom Matériel", "Catégorie", "Description",
        "Stock", "Quantité Maximale", "Image", "Fournisseur", "ID Projet", "Actions"
    };

    toolsTable->setColumnCount(headers.size());
    toolsTable->setHorizontalHeaderLabels(headers);

    toolsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    toolsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    toolsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Set different resize modes for columns
    toolsTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Interactive); // Nom Matériel
    toolsTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents); // Catégorie (will show full text)
    toolsTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Interactive); // Description
    toolsTable->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Fixed); // stck
    toolsTable->setColumnWidth(3, 30);
    toolsTable->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Fixed); // qt max
    toolsTable->setColumnWidth(4, 30);
    toolsTable->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Fixed); // Image
    toolsTable->setColumnWidth(5, 100);    // Fixed width for image column
    toolsTable->horizontalHeader()->setSectionResizeMode(6, QHeaderView::Interactive); // Fournisseur
    toolsTable->horizontalHeader()->setSectionResizeMode(7, QHeaderView::Fixed); // id
    toolsTable->setColumnWidth(7, 30); // ID Projet
    toolsTable->horizontalHeader()->setSectionResizeMode(8, QHeaderView::Fixed); // Actions
    toolsTable->setColumnWidth(8, 150); // Fixed width for actions column

    // Enable word wrap for the category column
    toolsTable->setWordWrap(true);
}
void ToolsManager::searchTools(const QString &text, const QString &field) {
    for (int row = 0; row < toolsTable->rowCount(); ++row) {
        bool match = false;
        int column = -1;

        if (field == "Nom Matériel") column = 0;
        else if (field == "Catégorie") column = 1;
        else if (field == "Fournisseur") column = 6;

        if (column >= 0) {
            QTableWidgetItem *item = toolsTable->item(row, column);
            if (item && item->text().contains(text, Qt::CaseInsensitive)) {
                match = true;
            }
        }
        toolsTable->setRowHidden(row, !match);
    }
}

void ToolsManager::sortTools(const QString &field, Qt::SortOrder order) {
    int column = -1;
    if (field == "Catégorie") column = 1;
    else if (field == "Nom Matériel") column = 0;
    else if (field == "Stock") column = 3;

    if (column >= 0) {
        toolsTable->sortByColumn(column, order);
    }
}

bool ToolsManager::validateToolData() const {
    if (nomMateriel.trimmed().isEmpty()) {
        QMessageBox::warning(nullptr, "Erreur", "Le nom du matériel est obligatoire");
        return false;
    }

    if (categorie.trimmed().isEmpty()) {
        QMessageBox::warning(nullptr, "Erreur", "La catégorie est obligatoire");
        return false;
    }

    if (stock < 0) {
        QMessageBox::warning(nullptr, "Erreur", "Le stock ne peut pas être négatif");
        return false;
    }

    if (quantiteMaximale < stock) {
        QMessageBox::warning(nullptr, "Erreur", "La quantité maximale doit être supérieure ou égale au stock");
        return false;
    }

    return true;
}

int ToolsManager::getNextId() {
    if (!dbConnection.createConnection()) {
        QMessageBox::critical(nullptr, "Erreur de base de données", "Impossible de se connecter à la base de données");
        return 1; // Default ID
    }

    QSqlQuery query(dbConnection.getDatabase());
    query.prepare("SELECT COALESCE(MAX(idR), 0) + 1 AS NEXT_ID FROM RessourcesMedicales");

    if (query.exec() && query.next()) {
        return query.value("NEXT_ID").toInt();
    } else {
        qDebug() << "Erreur lors de la recherche du prochain ID:" << query.lastError().text();
        return 1;
    }
}

void ToolsManager::loadTools() {
    toolsTable->setRowCount(0);
    toolsTable->setIconSize(QSize(64, 64));

    if (!dbConnection.createConnection()) {
        QMessageBox::critical(nullptr, "Erreur de base de données", "Impossible de se connecter à la base de données");
        return;
    }

    // Calcul des bornes pour la pagination Oracle
    int start_row = (currentPage - 1) * itemsPerPage + 1;
    int end_row = currentPage * itemsPerPage;

    // Requête compatible Oracle
    QString oracleQuery =
        "SELECT idR, nomMateriel, categorie, description, stock, "
        "quantiteMaximale, uploadImage, fournisseur, idPro FROM ("
        "   SELECT a.*, ROWNUM rnum FROM ("
        "       SELECT * FROM RessourcesMedicales ORDER BY idR"
        "   ) a "
        "   WHERE ROWNUM <= :end_row"
        ") "
        "WHERE rnum >= :start_row";

    QSqlQuery query(dbConnection.getDatabase());
    query.prepare(oracleQuery);
    query.bindValue(":start_row", start_row);
    query.bindValue(":end_row", end_row);

    if (query.exec()) {
        int row = 0;
        while (query.next()) {
            toolsTable->insertRow(row);

            toolsTable->setItem(row, 0, new QTableWidgetItem(query.value("nomMateriel").toString()));
            toolsTable->setItem(row, 1, new QTableWidgetItem(query.value("categorie").toString()));
            toolsTable->setItem(row, 2, new QTableWidgetItem(query.value("description").toString()));
            toolsTable->setItem(row, 3, new QTableWidgetItem(query.value("stock").toString()));
            toolsTable->setItem(row, 4, new QTableWidgetItem(query.value("quantiteMaximale").toString()));
            QString imagePath = query.value("uploadImage").toString();
            QTableWidgetItem *imageItem = new QTableWidgetItem();

            if (!imagePath.isEmpty() && QFileInfo::exists(imagePath)) {
                QPixmap pixmap(imagePath);
                if (!pixmap.isNull()) {
                    // Scale while maintaining aspect ratio
                    pixmap = pixmap.scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation);
                    imageItem->setData(Qt::DecorationRole, pixmap);
                    imageItem->setToolTip("Click to view full image");
                } else {
                    imageItem->setText("[Invalid Image]");
                }
            } else {
                imageItem->setText("[No Image]");
            }
            toolsTable->setItem(row, 5, imageItem);

            toolsTable->setItem(row, 6, new QTableWidgetItem(query.value("fournisseur").toString()));
            toolsTable->setItem(row, 7, new QTableWidgetItem(query.value("idPro").toString()));
            QWidget *actionWidget = new QWidget();
            QHBoxLayout *layout = new QHBoxLayout(actionWidget);
            layout->setContentsMargins(3, 3, 3, 3);
            layout->setSpacing(3);

            QPushButton *editBtn = new QPushButton("Modifier");
            QPushButton *deleteBtn = new QPushButton("Supprimer");

            // Style the buttons
            editBtn->setStyleSheet("padding: 2px 5px; font-size: 11px;");
            deleteBtn->setStyleSheet("padding: 2px 5px; font-size: 11px;");

            layout->addWidget(editBtn);
            layout->addWidget(deleteBtn);
            actionWidget->setLayout(layout);

            int toolId = query.value("idR").toInt();
            connect(editBtn, &QPushButton::clicked, [this, toolId]() {
                emit editToolRequested(toolId);
            });
            connect(deleteBtn, &QPushButton::clicked, [this, toolId]() {
                emit deleteToolRequested(toolId);
            });

            toolsTable->setCellWidget(row, 8, actionWidget);

            row++;
        }
    } else {
        qDebug() << "Erreur lors du chargement des outils:" << query.lastError().text();
        QMessageBox::warning(nullptr, "Erreur", "Impossible de charger les outils");
    }
    emit paginationChanged(currentPage, getTotalPages());
}
void ToolsManager::updatePagination() {
    countTotalTools();
    if (currentPage > getTotalPages() && getTotalPages() > 0) {
        currentPage = getTotalPages();
    }
    loadTools();
}
bool ToolsManager::exportToPDF(const QString &filePath)
{
    if (!dbConnection.createConnection())
    {
        QMessageBox::critical(nullptr, "Erreur de base de données",
                              "Impossible de se connecter à la base de données");
        return false;
    }

    // Create PDF writer with settings
    QPdfWriter pdfWriter(filePath);
    pdfWriter.setPageSize(QPageSize(QPageSize::A4));
    pdfWriter.setPageMargins(QMarginsF(30, 30, 30, 30)); // 30mm margins
    pdfWriter.setTitle("Liste des Ressources Médicales");

    // Set up painter with antialiasing
    QPainter painter(&pdfWriter);
    painter.setRenderHint(QPainter::Antialiasing, true);

    // Font configuration
    QFont titleFont("Arial", 18, QFont::Bold);
    QFont headerFont("Arial", 12, QFont::Bold);
    QFont contentFont("Arial", 10);
    QFont footerFont("Arial", 8);

    // Page dimensions
    const int pageWidth = pdfWriter.width();
    const int pageHeight = pdfWriter.height();
    const int margin = 30; // pixels
    const int headerHeight = 500;
    const int rowHeight = 500;
    const int footerHeight = 500;

    // Draw title and header
    painter.setFont(titleFont);
    painter.drawText(QRect(0, margin, pageWidth, 50),
                     Qt::AlignCenter, "Liste des Ressources Médicales");

    // Draw generation date
    painter.setFont(footerFont);
    painter.drawText(QRect(margin, pageHeight - footerHeight, pageWidth - 2*margin, footerHeight),
                     Qt::AlignLeft, "Généré le: " + QDateTime::currentDateTime().toString("dd/MM/yyyy à hh:mm:ss"));

    // Table setup
    const int tableTop = 400;
    int yPos = tableTop;
    const int totalWidth = pageWidth - 2 * margin;
    const int numColumns = 7;
    const int colWidth = totalWidth / numColumns;

    // Draw table headers
    painter.setFont(headerFont);
    QStringList headers = {"ID", "Nom Matériel", "Catégorie", "Description",
                           "Stock", "Quantité Max.", "Fournisseur"};

    // Draw header background
    painter.setBrush(QBrush(QColor(240, 240, 240)));
    painter.setPen(Qt::NoPen);
    painter.drawRect(margin, yPos, totalWidth, headerHeight);
    painter.setPen(Qt::black);

    // Draw header text
    for (int i = 0; i < headers.size(); ++i)
    {
        painter.drawText(QRect(margin + i * colWidth, yPos, colWidth, headerHeight),
                         Qt::AlignCenter, headers[i]);
    }

    // Draw header separator
    yPos += headerHeight;
    painter.drawLine(margin, yPos, margin + totalWidth, yPos);

    // Query database for tools data
    QSqlQuery query(dbConnection.getDatabase());
    query.prepare("SELECT idR, nomMateriel, categorie, description, stock, "
                  "quantiteMaximale, fournisseur FROM RessourcesMedicales ORDER BY idR");

    if (!query.exec())
    {
        painter.end();
        qDebug() << "Erreur lors de la génération du PDF:" << query.lastError().text();
        return false;
    }

    // Draw table content
    painter.setFont(contentFont);
    int row = 0;

    while (query.next())
    {
        // Check for page break
        if (yPos + rowHeight > pageHeight - margin - footerHeight)
        {
            painter.end();
            pdfWriter.newPage();
            painter.begin(&pdfWriter);
            painter.setFont(contentFont);
            yPos = margin;

            // Redraw headers on new page
            painter.setFont(headerFont);
            painter.setBrush(QBrush(QColor(240, 240, 240)));
            painter.drawRect(margin, yPos, totalWidth, headerHeight);
            painter.setPen(Qt::black);

            for (int i = 0; i < headers.size(); ++i)
            {
                painter.drawText(QRect(margin + i * colWidth, yPos, colWidth, headerHeight),
                                 Qt::AlignCenter, headers[i]);
            }

            yPos += headerHeight;
            painter.drawLine(margin, yPos, margin + totalWidth, yPos);
            painter.setFont(contentFont);
        }

        // Alternate row colors for better readability
        if (row % 2 == 0)
        {
            painter.setBrush(QBrush(QColor(250, 250, 250)));
        }
        else
        {
            painter.setBrush(QBrush(QColor(230, 230, 230)));
        }

        painter.setPen(Qt::NoPen);
        painter.drawRect(margin, yPos, totalWidth, rowHeight);
        painter.setPen(Qt::black);

        // Draw row data
        for (int col = 0; col < numColumns; col++)
        {
            QString text = query.value(col).toString();
            QRect textRect(margin + col * colWidth, yPos, colWidth, rowHeight);

            // Use text eliding for long descriptions
            if (col == 3) // Description column
            {
                text = painter.fontMetrics().elidedText(text, Qt::ElideRight, colWidth - 10);
            }

            painter.drawText(textRect, Qt::AlignCenter | Qt::TextWordWrap, text);
        }

        // Move to next row
        yPos += rowHeight;
        row++;

        // Draw row separator
        painter.drawLine(margin, yPos, margin + totalWidth, yPos);
    }

    painter.end();
    return true;
}

bool ToolsManager::addTool() {
    if (!validateToolData()) {
        return false;
    }

    if (!dbConnection.createConnection()) {
        QMessageBox::critical(nullptr, "Erreur de base de données", "Impossible de se connecter à la base de données");
        return false;
    }

    QSqlDatabase& db = dbConnection.getDatabase();

    if (!db.transaction()) {
        QMessageBox::critical(nullptr, "Erreur", "Impossible de démarrer la transaction");
        return false;
    }

    try {
        int nextId = getNextId();

        QSqlQuery query(db);
        query.prepare(
            "INSERT INTO RessourcesMedicales "
            "(idR, nomMateriel, categorie, description, stock, quantiteMaximale, uploadImage, fournisseur) "
            "VALUES (:idR, :nomMateriel, :categorie, :description, :stock, :quantiteMaximale, :uploadImage, :fournisseur)"
            );

        query.bindValue(":idR", nextId);
        query.bindValue(":nomMateriel", nomMateriel);
        query.bindValue(":categorie", categorie);
        query.bindValue(":description", description);
        query.bindValue(":stock", stock);
        query.bindValue(":quantiteMaximale", quantiteMaximale);
        query.bindValue(":uploadImage", uploadImage);
        query.bindValue(":fournisseur", fournisseur);

        if (!query.exec()) {
            throw std::runtime_error(query.lastError().text().toStdString());
        }

        if (!db.commit()) {
            throw std::runtime_error("Impossible de valider la transaction");
        }

        loadTools();
        updatePagination();
        emit dataChanged();
        QMessageBox::information(nullptr, "Succès", "Outil ajouté avec succès");
        return true;

    } catch (const std::exception& e) {
        db.rollback();
        QMessageBox::warning(nullptr, "Erreur", QString("Erreur lors de l'ajout de l'outil: %1").arg(e.what()));
        return false;
    }

}

bool ToolsManager::editTool() {
    if (!validateToolData()) {
        return false;
    }

    if (!dbConnection.createConnection()) {
        QMessageBox::critical(nullptr, "Erreur de base de données", "Impossible de se connecter à la base de données");
        return false;
    }

    QSqlDatabase& db = dbConnection.getDatabase();

    if (!db.transaction()) {
        QMessageBox::critical(nullptr, "Erreur", "Impossible de démarrer la transaction");
        return false;
    }

    try {
        QSqlQuery query(db);
        query.prepare(
            "UPDATE RessourcesMedicales SET "
            "nomMateriel = :nomMateriel, categorie = :categorie, description = :description, "
            "stock = :stock, quantiteMaximale = :quantiteMaximale, uploadImage = :uploadImage, "
            "fournisseur = :fournisseur "
            "WHERE idR = :idR"
            );

        query.bindValue(":idR", id);
        query.bindValue(":nomMateriel", nomMateriel);
        query.bindValue(":categorie", categorie);
        query.bindValue(":description", description);
        query.bindValue(":stock", stock);
        query.bindValue(":quantiteMaximale", quantiteMaximale);
        query.bindValue(":uploadImage", uploadImage);
        query.bindValue(":fournisseur", fournisseur);

        if (!query.exec()) {
            throw std::runtime_error(query.lastError().text().toStdString());
        }

        if (!db.commit()) {
            throw std::runtime_error("Impossible de valider la transaction");
        }

        loadTools();
        updatePagination();
        emit dataChanged();
        QMessageBox::information(nullptr, "Succès", "Outil modifié avec succès");
        return true;

    } catch (const std::exception& e) {
        db.rollback();
        QMessageBox::warning(nullptr, "Erreur", QString("Erreur lors de la modification de l'outil: %1").arg(e.what()));
        return false;
    }

}

bool ToolsManager::deleteTool() {
    if (!dbConnection.createConnection()) {
        QMessageBox::critical(nullptr, "Erreur de base de données", "Impossible de se connecter à la base de données");
        return false;
    }

    QSqlDatabase& db = dbConnection.getDatabase();

    if (!db.transaction()) {
        QMessageBox::critical(nullptr, "Erreur", "Impossible de démarrer la transaction");
        return false;
    }

    try {
        QSqlQuery query(db);
        query.prepare("DELETE FROM RessourcesMedicales WHERE idR = :idR");
        query.bindValue(":idR", id);

        if (!query.exec()) {
            throw std::runtime_error(query.lastError().text().toStdString());

        }

        if (!db.commit()) {
            throw std::runtime_error("Impossible de valider la transaction");
        }

        loadTools();
        updatePagination();
        emit dataChanged();

         return true;

    } catch (const std::exception& e) {
        db.rollback();
        QMessageBox::warning(nullptr, "Erreur", QString("Erreur lors de la suppression de l'outil: %1").arg(e.what()));
        return false;
    }

}
QStringList ToolsManager::getCategories() const {
    return {
        "Microscopes",
        "Centrifugeuses",
        "Spectromètres",
        "Pipettes",
        "Incubateurs",
        "Autoclaves",
        "Réactifs chimiques",
        "Matériel de protection"
    };
}
QMap<QString, QVariant> ToolsManager::getToolById(int id) {
    QMap<QString, QVariant> toolData;

    if (!dbConnection.createConnection()) {
        QMessageBox::critical(nullptr, "Erreur de base de données", "Impossible de se connecter à la base de données");
        return toolData;
    }

    QSqlQuery query(dbConnection.getDatabase());
    query.prepare("SELECT * FROM RessourcesMedicales WHERE idR = :id");
    query.bindValue(":id", id);

    if (query.exec() && query.next()) {
        toolData["id"] = query.value("idR");
        toolData["nomMateriel"] = query.value("nomMateriel");
        toolData["categorie"] = query.value("categorie");
        toolData["description"] = query.value("description");
        toolData["stock"] = query.value("stock");
        toolData["quantiteMaximale"] = query.value("quantiteMaximale");
        toolData["uploadImage"] = query.value("uploadImage");
        toolData["fournisseur"] = query.value("fournisseur");
        toolData["idPro"] = query.value("idPro");
    } else {
        qDebug() << "Erreur lors de la récupération de l'outil:" << query.lastError().text();
    }

    return toolData;
}
