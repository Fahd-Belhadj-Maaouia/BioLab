#pragma once

#include <QDialog>
#include <QLabel>

class MaterialInputDialog : public QDialog {
    Q_OBJECT

public:
    explicit MaterialInputDialog(QWidget *parent = nullptr);
    void appendKey(char key);
    QString getEnteredId() const;

private:
    QLabel *inputLabel;
    QString enteredId;
};
