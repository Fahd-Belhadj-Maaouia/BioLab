#include "loginwindow.h"
#include "passwordrecoverydialog.h"
#include <QMessageBox>
#include<QPrinter>
#include<QGraphicsOpacityEffect>
#include<QPropertyAnimation>

LoginWindow::LoginWindow(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("Connexion BIOLAB");
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint);

    // === Sidebar gauche (conservée mais améliorée) ===
    QWidget *sidebarWidget = new QWidget;
    sidebarWidget->setFixedWidth(220); // Légèrement élargie
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

    // === Formulaire de login modernisé ===
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

    // Titre avec ombre portée
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

    // Conteneur des champs
    QWidget *fieldsContainer = new QWidget;
    QVBoxLayout *fieldsLayout = new QVBoxLayout(fieldsContainer);
    fieldsLayout->setContentsMargins(0, 0, 0, 0);
    fieldsLayout->setSpacing(20);

    // Style moderne pour les champs
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

    // Bouton de connexion modernisé
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

    // Lien "Mot de passe oublié" modernisé
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

    // Message d'erreur modernisé
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

    // Assemblage du formulaire
    formLayout->addWidget(titleLabel);
    formLayout->addSpacing(10);
    formLayout->addWidget(fieldsContainer);
    formLayout->addWidget(errorLabel);
    formLayout->addWidget(loginButton);
    formLayout->addWidget(forgotPasswordButton, 0, Qt::AlignCenter);
    formLayout->addStretch();

    // === Layout principal ===
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(sidebarWidget);
    mainLayout->addWidget(formWidget);

    setLayout(mainLayout);
    setFixedSize(850, 550); // Taille légèrement augmentée

    // Connexions des signaux
    connect(loginButton, &QPushButton::clicked, this, &LoginWindow::handleLogin);
    connect(forgotPasswordButton, &QPushButton::clicked, this, &LoginWindow::showRecoveryDialog);

    // Animation d'entrée
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

    // Validation basique
    if (email.isEmpty() || password.isEmpty()) {
        errorLabel->setText("Veuillez remplir tous les champs");
        return;
    }

    // Vérification dans la base de données
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        errorLabel->setText("Erreur de connexion à la base de données");
        return;
    }

    QSqlQuery query(db);
    query.prepare("SELECT IDPER, NOM, PRENOM FROM WAEL.PERSONNELS WHERE EMAILG = :email AND MOTDEPASSEG = :password");
    query.bindValue(":email", email);
    query.bindValue(":password", password);  // On utilise le mot de passe en clair

    if (!query.exec()) {
        errorLabel->setText("Erreur technique: " + query.lastError().text());
        emit loginFailed();
        return;
    }

    if (query.next()) {
        // Authentification réussie
        int userId = query.value(0).toInt();
        QString nom = query.value(1).toString();
        QString prenom = query.value(2).toString();

        emit loginSuccessful(userId, nom, prenom);
    } else {
        errorLabel->setText("Email ou mot de passe incorrect");
        emit loginFailed();
    }
}

