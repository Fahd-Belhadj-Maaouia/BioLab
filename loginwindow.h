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
#include "mainwindow.h"
class LoginWindow : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    virtual ~LoginWindow();

signals:
    void loginSuccessful(int userId, QString nom, QString prenom);
    void loginFailed();
    // Remove showRecoveryDialog from signals

private slots:
    void showRecoveryDialog();  // Moved here from signals
    void handleLogin();
    void handleFaceIdLogin();
    void readPythonOutput();
    void readPythonError();

private:
    QLineEdit *emailInput;
    QLineEdit *passwordInput;
    QLabel *errorLabel;
    QProcess *pythonProcess;
};

#endif // LOGINWINDOW_H
