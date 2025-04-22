#include "materialinputdialog.h"
#include <QVBoxLayout>

MaterialInputDialog::MaterialInputDialog(QWidget *parent)
    : QDialog(parent), inputLabel(new QLabel(this)) {
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(inputLabel);
    inputLabel->setText("Enter ID: ");
    setLayout(layout);
}

void MaterialInputDialog::appendKey(char key) {
    enteredId += key;
    inputLabel->setText("Enter ID: " + enteredId);
}

QString MaterialInputDialog::getEnteredId() const {
    return enteredId;
}
