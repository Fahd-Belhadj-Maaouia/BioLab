#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H
#include<QObject>
#include <QDialog>
#include <QCamera>
#include "connection.h"
#include "personnelmanager.h"

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

private slots:
    void onLoginClicked();
    void onForgotPasswordClicked();
    void onFaceIdClicked();
    void processCapturedImage(int id, const QImage &img);
    void onSecurityAnswerChanged();

private:
    Ui::LoginDialog *ui;
    QCamera *m_camera;
    void initCamera();
    void checkFaceRecognition(const QImage &captured);
};

#endif // LOGINDIALOG_H
