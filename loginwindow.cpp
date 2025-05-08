#include "loginwindow.h"
#include "mainwindow.h"
#include "passwordrecoverydialog.h"
#include <QMessageBox>
#include <QPrinter>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QProcess>
#include <QTimer>
#include <QDebug>

LoginWindow::LoginWindow(QWidget *parent) : QWidget(parent), pythonProcess(nullptr)
{
    setWindowTitle("Connexion BIOLAB");
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint);

    // === Sidebar ===
    QWidget *sidebarWidget = new QWidget;
    sidebarWidget->setFixedWidth(220);
    sidebarWidget->setStyleSheet(R"(
        background-color: #1e1e1e;
        border-right: 1px solid #333333;
        QPushButton {
            background: transparent;
            color: white;
            text-align: left;
            padding: 12px 20px;
            font-size: 14px;
            border-radius: 6px;
            margin: 5px 0;
        }
        QPushButton:hover {
            background-color: rgba(255, 255, 255, 0.08);
        }
        QPushButton:checked {
            background-color: #198754;
            font-weight: 500;
        }
    )");

    QVBoxLayout *sidebarLayout = new QVBoxLayout(sidebarWidget);
    sidebarLayout->setContentsMargins(15, 30, 15, 30);
    sidebarLayout->setSpacing(20);

    QLabel *logoLabel = new QLabel(sidebarWidget);
    QPixmap logoPixmap(":/icons/svg/BIOLAB.svg");
    logoLabel->setPixmap(logoPixmap.scaled(160, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    logoLabel->setAlignment(Qt::AlignCenter);
    sidebarLayout->addWidget(logoLabel);
    sidebarLayout->addStretch();

    // === Login Form ===
    QWidget *formWidget = new QWidget;
    formWidget->setStyleSheet(R"(
        background-color: #ffffff;
        border-top-right-radius: 12px;
        border-bottom-right-radius: 12px;
        border-left: 1px solid #e0e0e0;
    )");

    QVBoxLayout *formLayout = new QVBoxLayout(formWidget);
    formLayout->setContentsMargins(60, 60, 60, 50);
    formLayout->setSpacing(25);

    QLabel *titleLabel = new QLabel("Connexion à BIOLAB");
    titleLabel->setStyleSheet(R"(
        QLabel {
            font-size: 24px;
            color: #2d3748;
            font-weight: 600;
            font-family: 'Segoe UI', 'Roboto', sans-serif;
            padding-bottom: 5px;
        }
    )");
    titleLabel->setAlignment(Qt::AlignCenter);

    QWidget *fieldsContainer = new QWidget;
    QVBoxLayout *fieldsLayout = new QVBoxLayout(fieldsContainer);
    fieldsLayout->setContentsMargins(0, 0, 0, 0);
    fieldsLayout->setSpacing(20);

    QString fieldStyle = R"(
        QLineEdit {
            background: #f8f9fa;
            border: 1px solid #e2e8f0;
            border-radius: 8px;
            padding: 12px 15px;
            font-size: 14px;
            color: #2d3748;
            min-height: 45px;
        }
        QLineEdit:focus {
            border: 1px solid #198754;
            background: white;
            box-shadow: 0 0 0 3px rgba(25, 135, 84, 0.1);
        }
    )";

    emailInput = new QLineEdit();
    emailInput->setPlaceholderText("Email généré par le système");
    emailInput->setStyleSheet(fieldStyle);

    passwordInput = new QLineEdit();
    passwordInput->setPlaceholderText("Mot de passe généré par le système");
    passwordInput->setEchoMode(QLineEdit::Password);
    passwordInput->setStyleSheet(fieldStyle);

    fieldsLayout->addWidget(emailInput);
    fieldsLayout->addWidget(passwordInput);

    // Face ID Button
    QPushButton *faceIdButton = new QPushButton("Face ID");
    faceIdButton->setFixedHeight(45);
    faceIdButton->setStyleSheet(R"(
        QPushButton {
            background-color: #3a7bd5;
            color: white;
            border: none;
            border-radius: 8px;
            padding: 12px;
            font-size: 15px;
            font-weight: 500;
            min-width: 100%;
        }
        QPushButton:hover {
            background-color: #2c65b8;
        }
        QPushButton:pressed {
            background-color: #1e4e9b;
            transform: translateY(1px);
        }
    )");
    fieldsLayout->addWidget(faceIdButton);

    // Login Button
    QPushButton *loginButton = new QPushButton("Se connecter");
    loginButton->setFixedHeight(45);
    loginButton->setStyleSheet(R"(
        QPushButton {
            background-color: #198754;
            color: white;
            border: none;
            border-radius: 8px;
            padding: 12px;
            font-size: 15px;
            font-weight: 500;
            min-width: 100%;
        }
        QPushButton:hover {
            background-color: #157347;
        }
        QPushButton:pressed {
            background-color: #0d6e3d;
            transform: translateY(1px);
        }
    )");

    QPushButton *forgotPasswordButton = new QPushButton("Mot de passe oublié ?");
    forgotPasswordButton->setStyleSheet(R"(
        QPushButton {
            background: transparent;
            color: #4a5568;
            text-decoration: none;
            border: none;
            font-size: 13px;
            padding: 5px;
        }
        QPushButton:hover {
            color: #198754;
            text-decoration: underline;
        }
    )");

    errorLabel = new QLabel();
    errorLabel->setStyleSheet(R"(
        color: #e53e3e;
        font-size: 13px;
        padding: 8px;
        background-color: #fff5f5;
        border-radius: 6px;
        border: 1px solid #fed7d7;
    )");
    errorLabel->setAlignment(Qt::AlignCenter);
    errorLabel->setWordWrap(true);

    formLayout->addWidget(titleLabel);
    formLayout->addSpacing(10);
    formLayout->addWidget(fieldsContainer);
    formLayout->addWidget(errorLabel);
    formLayout->addWidget(loginButton);
    formLayout->addWidget(forgotPasswordButton, 0, Qt::AlignCenter);
    formLayout->addStretch();

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(sidebarWidget);
    mainLayout->addWidget(formWidget);

    setLayout(mainLayout);
    setFixedSize(850, 550);

    // Connections
    connect(loginButton, &QPushButton::clicked, this, &LoginWindow::handleLogin);
    connect(forgotPasswordButton, &QPushButton::clicked, this, &LoginWindow::showRecoveryDialog);
    connect(faceIdButton, &QPushButton::clicked, this, &LoginWindow::handleFaceIdLogin);

    QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect(formWidget);
    formWidget->setGraphicsEffect(effect);
    QPropertyAnimation *anim = new QPropertyAnimation(effect, "opacity");
    anim->setDuration(350);
    anim->setStartValue(0);
    anim->setEndValue(1);
    anim->start();
}

void LoginWindow::showRecoveryDialog()
{
    PasswordRecoveryDialog dialog(this);
    dialog.exec();
}

void LoginWindow::handleLogin()
{
    QString email = emailInput->text().trimmed();
    QString password = passwordInput->text().trimmed();

    if (email.isEmpty() || password.isEmpty()) {
        errorLabel->setText("Veuillez remplir tous les champs");
        return;
    }

    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        errorLabel->setText("Erreur de connexion à la base de données");
        return;
    }

    QSqlQuery query(db);
    query.prepare("SELECT IDPER, NOM, PRENOM FROM WAEL.PERSONNELS WHERE EMAILG = :email AND MOTDEPASSEG = :password");
    query.bindValue(":email", email);
    query.bindValue(":password", password);

    if (!query.exec()) {
        errorLabel->setText("Erreur technique: " + query.lastError().text());
        emit loginFailed();
        return;
    }

    if (query.next()) {
        int userId = query.value(0).toInt();
        QString nom = query.value(1).toString();
        QString prenom = query.value(2).toString();
        emit loginSuccessful(userId, nom, prenom);
    } else {
        errorLabel->setText("Email ou mot de passe incorrect");
        emit loginFailed();
    }
}

// Simplified face recognition login that trusts face matches without database verification
void LoginWindow::handleFaceIdLogin()
{
    if (pythonProcess && pythonProcess->state() == QProcess::Running) {
        pythonProcess->terminate();
        pythonProcess->waitForFinished(1000);
        delete pythonProcess;
        pythonProcess = nullptr;
        return;
    }

    pythonProcess = new QProcess(this);
    connect(pythonProcess, &QProcess::readyReadStandardOutput, this, &LoginWindow::readPythonOutput);
    connect(pythonProcess, &QProcess::readyReadStandardError, this, &LoginWindow::readPythonError);
    connect(pythonProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            [this](int exitCode, QProcess::ExitStatus exitStatus) {
                qDebug() << "Python process exited with code:" << exitCode;
            });

    QString pythonExe = "C:/Users/INES/AppData/Local/Programs/Python/Python310/python.exe";
    QString scriptPath = "C:/Users/INES/Desktop/interfacemateriels1/face 3 scripts/New folder/face_recognition.py";

    // Clear conflicting environment variables
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.remove("PYTHONHOME");
    env.remove("PYTHONPATH");
    pythonProcess->setProcessEnvironment(env);

    pythonProcess->setWorkingDirectory(QFileInfo(scriptPath).path());
    pythonProcess->start(pythonExe, QStringList() << scriptPath);

    if (!pythonProcess->waitForStarted(3000)) {
        QMessageBox::warning(this, "Error", "Failed to start face recognition");
        delete pythonProcess;
        pythonProcess = nullptr;
    } else {
        errorLabel->setText("Looking for your face...");
    }
}

void LoginWindow::readPythonOutput()
{
    while (pythonProcess && pythonProcess->canReadLine()) {
        QString line = QString::fromUtf8(pythonProcess->readLine()).trimmed();
        qDebug() << "Python output:" << line;

        if (line.startsWith("STATUS|")) {
            errorLabel->setText(line.mid(7));
        }
        else if (line.startsWith("RESULT|GRANTED|")) {
            QStringList parts = line.split("|");
            if (parts.size() >= 4) {
                QString recognizedName = parts[2].trimmed();
                QString confidence = parts[3];

                // Clean up process immediately
                if (pythonProcess) {
                    disconnect(pythonProcess, nullptr, this, nullptr);
                    pythonProcess->terminate();
                    QTimer::singleShot(500, [this]() {
                        if (pythonProcess) {
                            if (pythonProcess->state() == QProcess::Running) {
                                pythonProcess->kill();
                            }
                            delete pythonProcess;
                            pythonProcess = nullptr;
                        }
                    });
                }

                // Bypass database check - login directly with dummy values
                QMessageBox::information(
                    this,
                    "Access Granted",
                    QString("Welcome %1\nConfidence: %2%").arg(recognizedName).arg(confidence)
                    );

                // Emit login with dummy values (replace with actual user ID if needed)
                emit loginSuccessful(0, recognizedName, "");
            }
        }
        else if (line.startsWith("ERROR|")) {
            QMessageBox::critical(this, "Error", line.mid(6));
            errorLabel->setText("Recognition failed");
        }
    }
}

void LoginWindow::readPythonError()
{
    if (pythonProcess) {
        QString error = pythonProcess->readAllStandardError();
        if (!error.isEmpty()) {
            qDebug() << "Python Error:" << error;

            if (error.contains("No module named")) {
                QMessageBox::warning(
                    this,
                    "Missing Packages",
                    "Required Python packages not found.\n\n"
                    "Please run this command:\n\n"
                    "pip install opencv-python opencv-contrib-python numpy"
                    );
            }
            else if (error.contains("Camera not accessible")) {
                QMessageBox::warning(this, "Camera Error", "Please check your webcam connection");
            }
            else {
                QMessageBox::warning(this, "Error", "Face recognition failed");
            }
        }
    }
}
LoginWindow::~LoginWindow()
{
    if (pythonProcess) {
        pythonProcess->terminate();
        pythonProcess->waitForFinished();
        delete pythonProcess;
    }
}
