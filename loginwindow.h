#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

#include <QPainter>
#include <QPixmap>
#include <QFont>
#include <QPen>


class LoginWindow : public QWidget {
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);

signals:
    void loginSuccessful();

private slots:
    void handleLogin();

private:
    QLineEdit *emailInput;
    QLineEdit *passwordInput;
    QLineEdit *captchaInput;
    QLabel *captchaLabel;
    QLabel *errorLabel;

    QString captchaText;

    void generateCaptcha();
};

#endif // LOGINWINDOW_H
