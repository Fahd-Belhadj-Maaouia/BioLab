#ifndef PASSWORDRECOVERYDIALOG_H
#define PASSWORDRECOVERYDIALOG_H

#include <QDialog>
#include <QTimer>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QFormLayout>
#include <QSqlQuery>

class PasswordRecoveryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PasswordRecoveryDialog(QWidget *parent = nullptr);
    void checkRecoveryInfo();

private slots:
    void onCheckClicked();
    void closeAfterTimeout();

private:
    QLineEdit *emailInput;
    QLineEdit *cinInput;
    QComboBox *questionCombo;
    QLineEdit *answerInput;
    QLabel *credentialsLabel;
    QTimer *closeTimer;
};

#endif // PASSWORDRECOVERYDIALOG_H
