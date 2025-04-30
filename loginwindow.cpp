#include "loginwindow.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QRandomGenerator>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPixmap>
#include <QPainter>

LoginWindow::LoginWindow(QWidget *parent) : QWidget(parent) {
    setWindowTitle("Connexion BIOLAB");

    // === Sidebar gauche ===
    QWidget *sidebarWidget = new QWidget;
    sidebarWidget->setFixedWidth(200);
    sidebarWidget->setStyleSheet(R"(
        background-color: #1e1e1e;
        QPushButton {
            background: transparent;
            color: white;
            text-align: left;
            padding: 10px 15px;
            font-size: 15px;
            border-radius: 8px;
        }
        QPushButton:hover {
            background-color: rgba(255, 255, 255, 0.1);
        }
        QPushButton:checked {
            background-color: #198754;
            font-weight: bold;
        }
    )");

    QVBoxLayout *sidebarLayout = new QVBoxLayout(sidebarWidget);
    sidebarLayout->setContentsMargins(15, 15, 15, 15);
    sidebarLayout->setSpacing(15);

    QLabel *logoLabel = new QLabel(sidebarWidget);
    QPixmap logoPixmap(":/icons/svg/BIOLAB.svg");
    logoLabel->setPixmap(logoPixmap.scaled(140, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    logoLabel->setAlignment(Qt::AlignCenter);
    sidebarLayout->addWidget(logoLabel);
    sidebarLayout->addStretch();

    // === Formulaire de login ===
    QWidget *formWidget = new QWidget;
    formWidget->setStyleSheet("background-color: white; border-top-right-radius: 12px; border-bottom-right-radius: 12px;");
    QVBoxLayout *formLayout = new QVBoxLayout(formWidget);

    QLabel *titleLabel = new QLabel("Connexion à BIOLAB");
    titleLabel->setStyleSheet(
        "font-size: 22px;"
        "color: #111;"
        "font-weight: 600;"
        "letter-spacing: 1px;"
        "font-family: 'Segoe UI', 'Roboto', sans-serif;"
        );
    titleLabel->setAlignment(Qt::AlignHCenter | Qt::AlignTop);

    QLabel *emailLabel = new QLabel("Adresse email");
    emailInput = new QLineEdit();
    emailInput->setPlaceholderText("exemple@biolab.com");

    QLabel *passwordLabel = new QLabel("Mot de passe");
    passwordInput = new QLineEdit();
    passwordInput->setPlaceholderText("********");
    passwordInput->setEchoMode(QLineEdit::Password);

    QLabel *captchaTextLabel = new QLabel("Captcha");
    captchaLabel = new QLabel();  // captcha image
    captchaInput = new QLineEdit();
    captchaInput->setPlaceholderText("Recopiez le texte");

    errorLabel = new QLabel();
    errorLabel->setStyleSheet("color: red;");

    QPushButton *loginButton = new QPushButton("Se connecter");
    loginButton->setFixedHeight(40);
    loginButton->setStyleSheet(R"(
        QPushButton {
            background-color: #66bb6a;
            color: white;
            border: none;
            padding: 10px;
            border-radius: 6px;
            font-size: 15px;
        }
        QPushButton:hover {
            background-color: #4caf50;
        }
        QPushButton:pressed {
            background-color: #388e3c;
        }
    )");

    connect(loginButton, &QPushButton::clicked, this, &LoginWindow::handleLogin);

    formLayout->addSpacing(30);
    formLayout->addWidget(titleLabel);
    formLayout->addSpacing(10);
    formLayout->addWidget(emailLabel);
    formLayout->addWidget(emailInput);
    formLayout->addWidget(passwordLabel);
    formLayout->addWidget(passwordInput);
    formLayout->addWidget(captchaTextLabel);
    formLayout->addWidget(captchaLabel);
    formLayout->addWidget(captchaInput);
    formLayout->addWidget(errorLabel);
    formLayout->addSpacing(10);
    formLayout->addWidget(loginButton, 0, Qt::AlignCenter);
    formLayout->addStretch();

    // === Layout principal
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(sidebarWidget);
    mainLayout->addWidget(formWidget);
    mainLayout->setStretch(1, 1);

    setLayout(mainLayout);
    resize(800, 450);

    // === Style global moderne
    setStyleSheet(R"(
        QLineEdit {
            border: 1px solid #81c784;
            border-radius: 6px;
            padding: 6px;
            font-size: 14px;
            color: black;
        }
        QLineEdit:placeholder {
            color: gray;
        }
        QLabel {
            color: #111;
            font-size: 15px;
            font-weight: 500;
            letter-spacing: 0.5px;
            font-family: 'Segoe UI', 'Roboto', sans-serif;
        }
    )");

    generateCaptcha(); // CAPTCHA = original
}

void LoginWindow::generateCaptcha() {
    const QString chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    captchaText.clear();
    for (int i = 0; i < 6; ++i) {
        captchaText += chars[QRandomGenerator::global()->bounded(chars.size())];
    }

    QPixmap pixmap(180, 60);
    pixmap.fill(Qt::white);

    QPainter painter(&pixmap);
    QFont font("Arial", 24, QFont::Bold);
    painter.setFont(font);
    painter.setPen(Qt::black);
    painter.drawText(pixmap.rect(), Qt::AlignCenter, captchaText);

    QPen pen(Qt::gray);
    for (int i = 0; i < 10; ++i) {
        pen.setColor(QColor::fromRgb(QRandomGenerator::global()->generate()));
        painter.setPen(pen);
        int x1 = QRandomGenerator::global()->bounded(pixmap.width());
        int y1 = QRandomGenerator::global()->bounded(pixmap.height());
        int x2 = QRandomGenerator::global()->bounded(pixmap.width());
        int y2 = QRandomGenerator::global()->bounded(pixmap.height());
        painter.drawLine(x1, y1, x2, y2);
    }

    captchaLabel->setPixmap(pixmap);
}

void LoginWindow::handleLogin() {
    QString email = emailInput->text().trimmed();
    QString password = passwordInput->text().trimmed();
    QString captchaEntered = captchaInput->text().trimmed();

    if (captchaEntered != captchaText) {
        errorLabel->setText("Captcha incorrect.");
        generateCaptcha();
        return;
    }

    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query(db);
    query.prepare("SELECT COUNT(*) FROM researcher WHERE emailg = :email AND motdepasseg = :password");
    query.bindValue(":email", email);
    query.bindValue(":password", password);

    if (!query.exec() || !query.next() || query.value(0).toInt() == 0) {
        errorLabel->setText("Email ou mot de passe incorrect.");
        generateCaptcha();
        return;
    }

    emit loginSuccessful();
    close();
}
