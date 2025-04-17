#include "loginwindow.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QRandomGenerator>

LoginWindow::LoginWindow(QWidget *parent) : QWidget(parent) {
    setWindowTitle("Connexion BIOLAB");

    emailInput = new QLineEdit();
    passwordInput = new QLineEdit();
    passwordInput->setEchoMode(QLineEdit::Password);
    captchaInput = new QLineEdit();
    captchaLabel = new QLabel();
    errorLabel = new QLabel();
    errorLabel->setStyleSheet("color: red;");

    generateCaptcha();

    QPushButton *loginButton = new QPushButton("Se connecter");
    connect(loginButton, &QPushButton::clicked, this, &LoginWindow::handleLogin);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(new QLabel("Email (institutionnel) :"));
    layout->addWidget(emailInput);
    layout->addWidget(new QLabel("Mot de passe :"));
    layout->addWidget(passwordInput);
    layout->addWidget(new QLabel("Captcha :"));
    layout->addWidget(captchaLabel);
    layout->addWidget(captchaInput);
    layout->addWidget(errorLabel);
    layout->addWidget(loginButton);

    setLayout(layout);
    setFixedSize(300, 300);

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

    QSqlDatabase db = QSqlDatabase::database();  // 🔥 récupère la connexion ouverte
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
    close();  // Ferme la fenêtre login
}

