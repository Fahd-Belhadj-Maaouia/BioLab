#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>
#include <QPixmap>
#include <QFont>
#include <QPen>
#include <QSqlQuery>
#include <QSqlError>
#include <QCryptographicHash>
#include <QRandomGenerator>
#include <QProcess>

class LoginWindow : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);

signals:
    void loginSuccessful(int userId, QString nom, QString prenom);
    void loginFailed();
    void showRecoveryDialog();

private slots:
    void handleLogin();


private:
    QLineEdit *emailInput;
    QLineEdit *passwordInput;
    QLabel *errorLabel;



};

#endif // LOGINWINDOW_H
