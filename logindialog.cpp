#include "logindialog.h"
#include "personnelmanager.h"
#include <QMessageBox>
#include <QCamera>
#include <QBuffer>

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    m_camera(nullptr)

{

    // Style et initialisation
    // Dans le constructeur de LoginDialog
    setStyleSheet(R"(
    QDialog {
        background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
            stop:0 #1a1a1a, stop:1 #0d0d0d);
        border-radius: 15px;
    }
    QLabel { color: white; font-size: 14px; }
    QLineEdit {
        background: rgba(255,255,255,0.1);
        border: 1px solid rgba(255,255,255,0.2);
        border-radius: 8px;
        padding: 10px;
        color: white;
    }
    QPushButton {
        background: #198754;
        color: white;
        padding: 12px 25px;
        border-radius: 8px;
        font-size: 14px;
    }
    QPushButton:hover { background: #157347; }
)");
    initCamera();

}

void LoginDialog::initCamera()
{
       this, &LoginDialog::processCapturedImage;

}

void LoginDialog::processCapturedImage(int id, const QImage &img)
{
    Q_UNUSED(id)

    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    img.save(&buffer, "PNG");
    QString capturedBase64 = QString::fromLatin1(byteArray.toBase64().data());

    PersonnelManager manager(nullptr);
    QSqlQuery q(manager.getConnection().getDatabase());
    q.prepare("SELECT emailg FROM personnels WHERE photo_base64=:photo");
    q.bindValue(":photo", capturedBase64);

    if(q.exec() && q.next()) {
        accept();
    } else {
        QMessageBox::warning(this, "Échec", "Visage non reconnu");
    }
    m_camera->stop();
}

// Autres méthodes...
