#include "toolsmanager.h"
#include "connection.h"
#include <QHeaderView>
#include <QSqlError>
#include <QDebug>
#include <QSqlRecord>
#include <QCoreApplication>
#include <QTemporaryFile>
#include <QProcess>
#include <QDir>
#include <QProgressDialog>
#include <QApplication>

#include <QProcess>
#include <QMessageBox>
#include <QString>
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
        "Stock", "Quantité Maximale", "Image", "Fournisseur", "Position Fournisseur", "ID Projet", "Actions"
    };

    toolsTable->setColumnCount(headers.size());
    toolsTable->setHorizontalHeaderLabels(headers);

    toolsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    toolsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    toolsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Set different resize modes for columns
    toolsTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Interactive); // Nom Matériel
    toolsTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents); // Catégorie
    toolsTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Interactive); // Description
    toolsTable->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Fixed); // Stock
    toolsTable->setColumnWidth(3, 30);
    toolsTable->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Fixed); // Quantité Maximale
    toolsTable->setColumnWidth(4, 30);
    toolsTable->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Fixed); // Image
    toolsTable->setColumnWidth(5, 100);
    toolsTable->horizontalHeader()->setSectionResizeMode(6, QHeaderView::Interactive); // Fournisseur
    toolsTable->horizontalHeader()->setSectionResizeMode(7, QHeaderView::Interactive); // Position Fournisseur
    toolsTable->horizontalHeader()->setSectionResizeMode(8, QHeaderView::Fixed); // ID Projet
    toolsTable->setColumnWidth(8, 30);
    toolsTable->horizontalHeader()->setSectionResizeMode(9, QHeaderView::Fixed); // Actions
    toolsTable->setColumnWidth(9, 150);

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
        "quantiteMaximale, uploadImage, fournisseur, positionFournisseur, idPro FROM ("
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

            QString fournisseurName = query.value("fournisseur").toString();
            QTableWidgetItem *fournisseurItem = new QTableWidgetItem(fournisseurName);
            fournisseurItem->setFlags(fournisseurItem->flags() | Qt::ItemIsEditable);
            fournisseurItem->setForeground(QBrush(QColor(0, 0, 255)));
            fournisseurItem->setToolTip("Cliquez pour voir l'emplacement");
            toolsTable->setItem(row, 6, fournisseurItem);

            QString positionFournisseur = query.value("positionFournisseur").toString();
            QTableWidgetItem *positionItem = new QTableWidgetItem(positionFournisseur);
            toolsTable->setItem(row, 7, positionItem);

            toolsTable->setItem(row, 8, new QTableWidgetItem(query.value("idPro").toString()));

            QWidget *actionWidget = new QWidget();
            QHBoxLayout *layout = new QHBoxLayout(actionWidget);
            layout->setContentsMargins(3, 3, 3, 3);
            layout->setSpacing(3);

            QPushButton *editBtn = new QPushButton("Modifier");
            QPushButton *deleteBtn = new QPushButton("Supprimer");
            QPushButton *qrCodeBtn = new QPushButton("QR Code");

            // Style the buttons
            editBtn->setStyleSheet("padding: 2px 5px; font-size: 11px;");
            deleteBtn->setStyleSheet("padding: 2px 5px; font-size: 11px;");
            qrCodeBtn->setStyleSheet("padding: 2px 5px; font-size: 11px;");

            layout->addWidget(editBtn);
            layout->addWidget(deleteBtn);
            layout->addWidget(qrCodeBtn);
            actionWidget->setLayout(layout);

            int toolId = query.value("idR").toInt();
            connect(editBtn, &QPushButton::clicked, [this, toolId]() {
                emit editToolRequested(toolId);
            });
            connect(deleteBtn, &QPushButton::clicked, [this, toolId]() {
                emit deleteToolRequested(toolId);
            });
            connect(qrCodeBtn, &QPushButton::clicked, [this, toolId]() {
                emit qrCodeRequested(toolId);
            });

            toolsTable->setCellWidget(row, 9, actionWidget);
            connect(toolsTable, &QTableWidget::cellClicked, this, [this](int clickedRow, int clickedCol) {
                if (clickedCol == 6) { // Only for fournisseur column
                    QString name = toolsTable->item(clickedRow, clickedCol)->text();
                    emit fournisseurClicked(name);
                }
            });

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

    // Set resolution for better control of element sizing
    pdfWriter.setResolution(100);

    // Set up painter with antialiasing
    QPainter painter(&pdfWriter);
    painter.setRenderHint(QPainter::Antialiasing, true);

    // Font configuration - reduced sizes
    QFont titleFont("Arial", 14, QFont::Bold);
    QFont headerFont("Arial", 10, QFont::Bold);
    QFont contentFont("Arial", 9);
    QFont footerFont("Arial", 8);

    // Page dimensions
    const int pageWidth = pdfWriter.width();
    const int pageHeight = pdfWriter.height();
    const int margin = 100; // pixels at 100 DPI
    const int headerHeight = 40; // reduced from 500
    const int rowHeight = 50;    // reduced from 500
    const int footerHeight = 30; // reduced from 500

    // Draw title and header - moved down from top margin
    painter.setFont(titleFont);
    painter.drawText(QRect(margin, margin, pageWidth - 2*margin, 60),
                     Qt::AlignCenter, "Liste des Ressources Médicales");

    // Draw generation date at the bottom
    painter.setFont(footerFont);
    painter.drawText(QRect(margin, pageHeight - footerHeight - margin/2, pageWidth - 2*margin, footerHeight),
                     Qt::AlignLeft, "Généré le: " + QDateTime::currentDateTime().toString("dd/MM/yyyy à hh:mm:ss"));

    // Table setup - start table lower to leave space for title
    const int tableTop = margin + 80; // reduced from 400
    int yPos = tableTop;
    const int totalWidth = pageWidth - 2 * margin;
    const int numColumns = 7;

    // Variable column widths for better space usage
    QVector<int> colWidths = {
        static_cast<int>(totalWidth * 0.07),  // ID - smaller
        static_cast<int>(totalWidth * 0.15),  // Nom Matériel
        static_cast<int>(totalWidth * 0.12),  // Catégorie
        static_cast<int>(totalWidth * 0.30),  // Description - larger
        static_cast<int>(totalWidth * 0.07),  // Stock - smaller
        static_cast<int>(totalWidth * 0.12),  // Quantité Max
        static_cast<int>(totalWidth * 0.17)   // Fournisseur
    };

    // Draw table headers
    painter.setFont(headerFont);
    QStringList headers = {"ID", "Nom Matériel", "Catégorie", "Description",
                           "Stock", "Quantité Max.", "Fournisseur"};

    // Draw header background
    painter.setBrush(QBrush(QColor(240, 240, 240)));
    painter.setPen(Qt::NoPen);
    painter.drawRect(margin, yPos, totalWidth, headerHeight);
    painter.setPen(Qt::black);

    // Draw header text with variable column widths
    int xOffset = margin;
    for (int i = 0; i < headers.size(); ++i)
    {
        painter.drawText(QRect(xOffset, yPos, colWidths[i], headerHeight),
                         Qt::AlignCenter, headers[i]);
        xOffset += colWidths[i];
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
        // Check for page break - accounting for footer space
        if (yPos + rowHeight > pageHeight - margin - footerHeight)
        {
            painter.end();
            pdfWriter.newPage();
            painter.begin(&pdfWriter);
            painter.setRenderHint(QPainter::Antialiasing, true);
            yPos = margin;

            // Redraw headers on new page
            painter.setFont(headerFont);
            painter.setBrush(QBrush(QColor(240, 240, 240)));
            painter.drawRect(margin, yPos, totalWidth, headerHeight);
            painter.setPen(Qt::black);

            xOffset = margin;
            for (int i = 0; i < headers.size(); ++i)
            {
                painter.drawText(QRect(xOffset, yPos, colWidths[i], headerHeight),
                                 Qt::AlignCenter, headers[i]);
                xOffset += colWidths[i];
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

        // Draw row data with variable column widths
        xOffset = margin;
        for (int col = 0; col < numColumns; col++)
        {
            QString text = query.value(col).toString();
            QRect textRect(xOffset, yPos + 5, colWidths[col], rowHeight - 10);

            // Handle text rendering based on column - special treatment for description
            if (col == 3) // Description column
            {
                // Add padding and enable word wrapping for description
                painter.drawText(textRect.adjusted(5, 0, -5, 0),
                                 Qt::AlignLeft | Qt::AlignVCenter | Qt::TextWordWrap, text);
            }
            else
            {
                // Center align other columns
                painter.drawText(textRect, Qt::AlignCenter | Qt::AlignVCenter, text);
            }

            xOffset += colWidths[col];
        }

        // Move to next row
        yPos += rowHeight;
        row++;

        // Draw row separator
        painter.drawLine(margin, yPos, margin + totalWidth, yPos);
    }

    // Draw vertical grid lines for better readability
    xOffset = margin;
    for (int i = 0; i <= numColumns; i++)
    {
        painter.drawLine(xOffset, tableTop, xOffset, yPos);
        if (i < numColumns) xOffset += colWidths[i];
    }

    painter.end();
    return true;
}
/*void ToolsManager::detectScissors(const QString &imagePath, const QString &productName) {
    // Define the absolute paths to the model files
    QString modelDir = "C:/Users/DHOUIB Najet/Documents/interfacemateriels1/models/";
    QString prototxtPath = modelDir + "deploy.prototxt";
    QString modelPath = modelDir + "MobileNetSSD_deploy.caffemodel";

    // Check if the model files exist
    if (!QFileInfo::exists(prototxtPath) || !QFileInfo::exists(modelPath)) {
        QMessageBox::critical(nullptr, "Error", "Model files not found. Please ensure the paths are correct.");
        return;
    }

    // Load the pre-trained model
    cv::dnn::Net net = cv::dnn::readNetFromCaffe(prototxtPath.toStdString(), modelPath.toStdString());

    // Load the image
    cv::Mat frame = cv::imread(imagePath.toStdString());

    if (frame.empty()) {
        QMessageBox::warning(nullptr, "Error", "Unable to load image: " + imagePath);
        return;
    }

    // Create a blob from the image
    cv::Mat blob;
    cv::dnn::blobFromImage(frame, blob, 1.0, cv::Size(300, 300), cv::Scalar(104, 177, 123), false, false);

    // Set the input blob for the network
    net.setInput(blob);

    // Run the forward pass to get the output from the network
    cv::Mat detections;
    net.forward(detections, net.getUnconnectedOutLayersNames());

    // Process the detections
    for (int i = 0; i < detections.rows; i++) {
        float confidence = detections.at<float>(i, 2);

        if (confidence > 0.5) { // Confidence threshold
            int classId = static_cast<int>(detections.at<float>(i, 1));

            // Check if the detected class is "scissors"
            if (classId == 71) { // Class ID for "scissors" in COCO dataset
                QMessageBox::information(nullptr, "Detection Result", "Scissors detected!");
                return;
            }
        }
    }

    QMessageBox::information(nullptr, "Detection Result", "No scissors detected.");
}
*/
/*bool ToolsManager::verifyImageMatchesName(const QString& imagePath, const QString& productName) {
    // Check if image path or product name is empty
    if (imagePath.isEmpty() || productName.isEmpty()) {
        return false;
    }

    // Get absolute paths to required directories
    QString currentDir = QCoreApplication::applicationDirPath();
    QString yolov5Path = "C:/Users/DHOUIB Najet/Documents/interfacemateriels1/external/yolov5";
    QString modelDir = "C:/Users/DHOUIB Najet/Documents/interfacemateriels1/external/Surgical-Instrument-Detector-main/object_detector/Rona_mid/Rona_mid_model/weights";
    QString modelPath = modelDir + "/best.pt";
    QString imgPathCopy = imagePath;

    // Ensure paths exist
    if (!QFileInfo::exists(yolov5Path)) {
        QMessageBox::warning(nullptr, "Erreur",
                             "Chemin YOLOv5 non trouvé: " + yolov5Path);
        return true; // Allow submission anyway
    }

    // Check if model exists, download if not
    if (!QFileInfo::exists(modelPath)) {
        bool downloaded = downloadPreTrainedModel(modelDir);
        if (!downloaded) {
            QMessageBox::warning(nullptr, "Erreur",
                                 "Impossible de télécharger le modèle. La vérification d'image ne sera pas disponible.");
            return true; // Allow submission anyway
        }
    }

    // Python script for inference
    QString pythonScript = "import sys\n"
                           "import os\n"
                           "# Add YOLOv5 directory to path\n"
                           "sys.path.append('" + yolov5Path.replace("\\", "\\\\") + "')\n\n"
                                                                "import torch\n"
                                                                "import cv2\n\n"
                                                                "try:\n"
                                                                "    # Load the model\n"
                                                                "    model_path = '" + modelPath.replace("\\", "\\\\") + "'\n"
                                                               "    model = torch.hub.load('ultralytics/yolov5', 'custom', path=model_path, force_reload=True, trust_repo=True)\n"
                                                               "    if model is None:\n"
                                                               "        print('ERROR: Model could not be loaded')\n"
                                                               "        sys.exit(1)\n\n"
                                                               "    model.conf = 0.4  # Confidence threshold\n\n"
                                                               "    # Load image\n"

                                                               "    img_path = '" + imgPathCopy.replace("\\", "\\\\") + "'\n"
                                                               "    img = cv2.imread(img_path)\n"
                                                               "    if img is None:\n"
                                                               "        print('ERROR: Unable to load image: ' + img_path)\n"
                                                               "        sys.exit(1)\n\n"
                                                               "    # Run inference\n"
                                                               "    results = model(img)\n\n"
                                                               "    # For debugging, save the detection result\n"
                                                               "    output_dir = os.path.dirname(img_path)\n"
                                                               "    output_path = os.path.join(output_dir, 'detection_result.jpg')\n"
                                                               "    results.save(output_path)\n\n"
                                                               "    # Get detected class names\n"
                                                               "    class_names = []\n"
                                                               "    detections = results.pandas().xyxy[0]\n"
                                                               "    \n"
                                                               "    if not detections.empty:\n"
                                                               "        for _, detection in detections.iterrows():\n"
                                                               "            class_name = detection['name']\n"
                                                               "            confidence = detection['confidence']\n"
                                                               "            class_names.append(f'{class_name} ({confidence:.2f})')\n"
                                                               "            print(f'Detected: {class_name} with confidence {confidence:.2f}')\n\n"
                                                               "    # Print result for Qt to capture\n"
                                                               "    if len(class_names) > 0:\n"
                                                               "        print('DETECTED_ITEMS:' + ','.join(class_names))\n"
                                                               "    else:\n"
                                                               "        print('NO_DETECTIONS')\n"
                                                               "except Exception as e:\n"
                                                               "    print('ERROR: ' + str(e))\n";

    // Create a temporary Python script file
    QTemporaryFile tempFile;
    if (tempFile.open()) {
        tempFile.write(pythonScript.toUtf8());
        QString scriptPath = tempFile.fileName();
        tempFile.close();

        // Run the Python script and capture the output
        QProcess process;
        process.start("python", QStringList() << scriptPath);
        process.waitForFinished(-1);

        // Read the output
        QString output = process.readAllStandardOutput();
        QString error = process.readAllStandardError();

        qDebug() << "Detection output:" << output;
        qDebug() << "Detection errors:" << error;

        if (output.contains("ERROR:") || !error.isEmpty()) {
            QMessageBox::warning(nullptr, "Erreur Python",
                                 "Erreur lors de l'exécution du script: " +
                                     (output.contains("ERROR:") ? output : error));
            return true; // Allow submission anyway
        }

        // Check if any surgical tools were detected
        if (output.contains("DETECTED_ITEMS:")) {
            // Extract the detected items
            QString detectedItems = output.split("DETECTED_ITEMS:").at(1).trimmed();
            QStringList detectedList = detectedItems.split(",");

            // Convert product name to lowercase for better matching
            QString productNameLower = productName.toLower();

            qDebug() << "Detected items:" << detectedList;
            qDebug() << "Product name:" << productNameLower;

            // Show the detection result
            QString detectionResult = "Outils détectés dans l'image:\n\n";
            foreach (const QString& item, detectedList) {
                detectionResult += "- " + item + "\n";
            }

            QMessageBox::information(nullptr, "Résultat de la détection", detectionResult);

            // Check if any detected item matches the product name
            bool matchFound = false;
            foreach (const QString& item, detectedList) {
                // Extract just the class name without confidence
                QString className = item.split(" (").first();

                // Check for matches
                if (productNameLower.contains(className.toLower()) ||
                    className.toLower().contains(productNameLower)) {
                    matchFound = true;
                    break;
                }
            }

            return matchFound;
        } else if (output.contains("NO_DETECTIONS")) {
            // No tools detected
            QMessageBox::information(nullptr, "Résultat de la détection",
                                     "Aucun outil détecté dans l'image.");
            return false;
        } else {
            // Unexpected output
            QMessageBox::warning(nullptr, "Erreur",
                                 "Sortie inattendue du modèle: " + output);
            return true; // Allow submission anyway
        }
    } else {
        QMessageBox::warning(nullptr, "Erreur",
                             "Impossible de créer un fichier temporaire pour le script Python.");
        return true; // Allow submission anyway
    }
}
bool ToolsManager::downloadPreTrainedModel(const QString& modelDir) {
    // Create directory structure if it doesn't exist
    QDir().mkpath(modelDir);
    QString escapedModelDir = modelDir;


    QString modelPath = modelDir + "/best.pt";

    // Python script to download the model
    QString pythonScript = "import os\n"
                           "import torch\n"
                           "import urllib.request\n\n"
                           "# Create directory if it doesn't exist\n"
                           "os.makedirs('" + escapedModelDir.replace("\\", "\\\\") + "', exist_ok=True)\n\n"
                                                              "# URL to the Hugging Face model\n"
                                                              "model_url = 'https://huggingface.co/datasets/DocCheck/Rona-dataset/resolve/main/models/Rona_detector_model_best.pt'\n"
                                                              "save_path = '" + modelPath.replace("\\", "\\\\") + "'\n\n"
                                                               "print('Downloading pre-trained model from Hugging Face...')\n"
                                                               "try:\n"
                                                               "    urllib.request.urlretrieve(model_url, save_path)\n"
                                                               "    print('Model successfully downloaded to: ' + save_path)\n"
                                                               "except Exception as e:\n"
                                                               "    print('Error downloading model: ' + str(e))\n"
                                                               "    # Fallback: Try to download a standard YOLOv5 model\n"
                                                               "    try:\n"
                                                               "        print('Trying to download standard YOLOv5s model as fallback...')\n"
                                                               "        model = torch.hub.load('ultralytics/yolov5', 'yolov5s', pretrained=True, force_reload=True, trust_repo=True)\n"
                                                               "        torch.save(model.state_dict(), save_path)\n"
                                                               "        print('Standard YOLOv5s model downloaded and saved')\n"
                                                               "    except Exception as e2:\n"
                                                               "        print('Error with fallback download: ' + str(e2))\n";

    QTemporaryFile tempFile;
    if (tempFile.open()) {
        tempFile.write(pythonScript.toUtf8());
        QString scriptPath = tempFile.fileName();
        tempFile.close();

        // Show a progress dialog
        QProgressDialog progress("Téléchargement du modèle de détection...", "Annuler", 0, 0, nullptr);
        progress.setWindowModality(Qt::WindowModal);
        progress.show();

        QProcess process;
        process.start("python", QStringList() << scriptPath);

        while (process.state() == QProcess::Running) {
            QApplication::processEvents();
            if (progress.wasCanceled()) {
                process.kill();
                return false;
            }
        }

        progress.close();

        QString output = process.readAllStandardOutput();
        QString error = process.readAllStandardError();

        qDebug() << "Download output:" << output;

        if (output.contains("Model successfully downloaded")) {
            QMessageBox::information(nullptr, "Information",
                                     "Modèle téléchargé avec succès!");
            return true;
        } else if (output.contains("Standard YOLOv5s model downloaded")) {
            QMessageBox::information(nullptr, "Information",
                                     "Modèle YOLO standard téléchargé avec succès comme alternative!");
            return true;
        } else {
            QMessageBox::warning(nullptr, "Erreur",
                                 "Erreur lors du téléchargement du modèle:\n" +
                                     output + "\n" + error);
            return false;
        }
    }

    return false;
}
*/
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
            "(idR, nomMateriel, categorie, description, stock, quantiteMaximale, uploadImage, fournisseur, positionFournisseur) "
            "VALUES (:idR, :nomMateriel, :categorie, :description, :stock, :quantiteMaximale, :uploadImage, :fournisseur, :positionFournisseur)"
            );

        query.bindValue(":idR", nextId);
        query.bindValue(":nomMateriel", nomMateriel);
        query.bindValue(":categorie", categorie);
        query.bindValue(":description", description);
        query.bindValue(":stock", stock);
        query.bindValue(":quantiteMaximale", quantiteMaximale);
        query.bindValue(":uploadImage", uploadImage);
        query.bindValue(":fournisseur", fournisseur);
        query.bindValue(":positionFournisseur", positionFournisseur);

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
            "fournisseur = :fournisseur, positionFournisseur = :positionFournisseur "
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
        query.bindValue(":positionFournisseur", positionFournisseur);

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
        toolData["positionFournisseur"] = query.value("positionFournisseur");
        toolData["idPro"] = query.value("idPro");
    } else {
        qDebug() << "Erreur lors de la récupération de l'outil:" << query.lastError().text();
    }

    return toolData;
}


