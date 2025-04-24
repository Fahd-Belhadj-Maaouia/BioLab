#include "passwordrecoverydialog.h"
#include <QFormLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QSqlQuery>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QParallelAnimationGroup>

PasswordRecoveryDialog::PasswordRecoveryDialog(QWidget *parent)
    : QDialog(parent)
{
    // Configuration de base
    setWindowTitle("Récupération des identifiants");
    setFixedSize(500, 400);

    // Style minimaliste et professionnel
    setStyleSheet(R"(
        QDialog {
            background-color: #ffffff;
            font-family: 'Segoe UI', Arial, sans-serif;
        }
        QLabel {
            color: #333333;
            font-size: 13px;
            min-width: 120px;
        }
        QLineEdit, QComboBox {
            background: #ffffff;
            border: 1px solid #dddddd;
            border-radius: 3px;
            padding: 8px 12px;
            font-size: 13px;
            min-width: 250px;
        }
        QLineEdit:focus, QComboBox:focus {
            border: 1px solid #2e7d32;
        }
        QPushButton {
            background-color: #2e7d32;
            color: white;
            border: none;
            border-radius: 3px;
            padding: 8px 20px;
            font-size: 13px;
            min-width: 120px;
        }
        QPushButton:hover {
            background-color: #1b5e20;
        }
    )");

    // Layout principal organisé
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(30, 20, 30, 20);
    mainLayout->setSpacing(20);

    // Titre sobre
    QLabel *titleLabel = new QLabel("Récupération des identifiants");
    titleLabel->setStyleSheet("font-size: 16px; font-weight: 600; color: #333333;");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    // Conteneur du formulaire
    QWidget *formContainer = new QWidget();
    QFormLayout *formLayout = new QFormLayout(formContainer);
    formLayout->setContentsMargins(0, 0, 0, 0);
    formLayout->setSpacing(12);
    formLayout->setFieldGrowthPolicy(QFormLayout::FieldsStayAtSizeHint);
    formLayout->setFormAlignment(Qt::AlignHCenter | Qt::AlignTop);
    formLayout->setLabelAlignment(Qt::AlignRight);

    // Champs parfaitement alignés
    emailInput = new QLineEdit();
    cinInput = new QLineEdit();
    questionCombo = new QComboBox();
    answerInput = new QLineEdit();

    // Configuration des champs
    emailInput->setPlaceholderText("exemple@domaine.com");
    cinInput->setPlaceholderText("12345678");
        cinInput->setValidator(new QRegularExpressionValidator(QRegularExpression("\\d{8}"), this));
    answerInput->setPlaceholderText("Votre réponse");

    // Liste des questions (identique à celle de l'inscription)
    QStringList questions = {
        "Quel était le nom de votre premier animal de compagnie?",
        "Quel est le nom de jeune fille de votre mère?",
        "Dans quelle ville êtes-vous né(e)?",
        "Quel était votre surnom d'enfance?",
        "Quel est votre livre préféré?",
        "Quel est le nom de votre meilleur ami d'enfance?",
        "Quel était le nom de votre école primaire?",
        "Quel est votre plat préféré?",
        "Quel est le modèle de votre première voiture?",
        "Quel est votre sport préféré?",
        "Quel est le nom de votre professeur préféré?",
        "Quel est votre groupe de musique préféré?",
        "Quel est votre film préféré?",
        "Quel était votre rêve professionnel étant enfant?",
        "Quel est le nom de votre premier employeur?",
        "Quel est votre destination de vacances préférée?",
        "Quel est votre hobby préféré?",
        "Quel est votre artiste préféré?",
        "Quel est votre jeu vidéo préféré?",
        "Quel est votre citation préférée?"
    };
    questionCombo->addItems(questions);

    // Ajout des champs avec alignement parfait
    formLayout->addRow("Email :", emailInput);
    formLayout->addRow("CIN :", cinInput);
    formLayout->addRow("Question :", questionCombo);
    formLayout->addRow("Réponse :", answerInput);

    mainLayout->addWidget(formContainer, 0, Qt::AlignHCenter);

    // Bouton centré
    QPushButton *checkButton = new QPushButton("Vérifier");
    connect(checkButton, &QPushButton::clicked, this, &PasswordRecoveryDialog::onCheckClicked);
    mainLayout->addWidget(checkButton, 0, Qt::AlignCenter);

    // Zone de résultat
    credentialsLabel = new QLabel();
    credentialsLabel->setStyleSheet(R"(
        background-color: #f8f8f8;
        border: 1px solid #eeeeee;
        padding: 12px;
        margin-top: 15px;
    )");
    credentialsLabel->setWordWrap(true);
    credentialsLabel->setVisible(false);
    mainLayout->addWidget(credentialsLabel);

    // Timer
    closeTimer = new QTimer(this);
    closeTimer->setSingleShot(true);
    connect(closeTimer, &QTimer::timeout, this, &PasswordRecoveryDialog::closeAfterTimeout);
}

void PasswordRecoveryDialog::onCheckClicked()
{
    // Animation du bouton
    QPropertyAnimation *pressAnim = new QPropertyAnimation(this);
    pressAnim->setTargetObject(this->findChild<QPushButton*>());
    pressAnim->setPropertyName("geometry");
    pressAnim->setDuration(200);
    pressAnim->setStartValue(this->findChild<QPushButton*>()->geometry());
    pressAnim->setEndValue(this->findChild<QPushButton*>()->geometry().adjusted(0, 2, 0, 2));
    pressAnim->start(QAbstractAnimation::DeleteWhenStopped);

    // Vérification des champs
    QString email = emailInput->text().trimmed();
    QString cin = cinInput->text().trimmed();
    int questionIndex = questionCombo->currentIndex();
    QString answer = answerInput->text().trimmed();

    if(email.isEmpty() || cin.isEmpty() || answer.isEmpty()) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowTitle("Champs manquants");
        msgBox.setText("Veuillez remplir tous les champs");
        msgBox.setStyleSheet("QMessageBox { background: white; }");
        msgBox.exec();
        return;
    }

    // Animation de chargement
    credentialsLabel->setText("<center>Vérification en cours...</center>");
    credentialsLabel->setVisible(true);
    QGraphicsOpacityEffect *effect = qobject_cast<QGraphicsOpacityEffect*>(credentialsLabel->graphicsEffect());

    QPropertyAnimation *fadeIn = new QPropertyAnimation(effect, "opacity");
    fadeIn->setDuration(300);
    fadeIn->setStartValue(0);
    fadeIn->setEndValue(1);
    fadeIn->start(QAbstractAnimation::DeleteWhenStopped);

    // Vérification dans la base de données
    QSqlQuery query;
    query.prepare("SELECT EMAILG, MOTDEPASSEG FROM PERSONNELS WHERE "
                 "EMAIL = :email AND CIN = :cin AND "
                 "QUESTION_INDEX = :question AND "
                 "UPPER(REPONSE) = UPPER(:answer)");
    query.bindValue(":email", email);
    query.bindValue(":cin", cin);
    query.bindValue(":question", questionIndex);
    query.bindValue(":answer", answer);

    if(query.exec() && query.next()) {
        QString loginEmail = query.value(0).toString();
        QString password = query.value(1).toString();

        credentialsLabel->setText(QString("<center><b>Vos identifiants de connexion:</b><br>"
                                       "📧 Email: <span style='color:#2b6cb0'>%1</span><br>"
                                       "🔑 Mot de passe: <span style='color:#2b6cb0'>%2</span><br><br>"
                                       "<small>Cette fenêtre se fermera automatiquement</small></center>")
                                .arg(loginEmail).arg(password));

        // Fermer après 30 secondes avec animation
        closeTimer->start(30000);
    } else {
        credentialsLabel->setText("<center><span style='color:#e53e3e'>Les informations fournies ne correspondent à aucun compte</span></center>");

        QTimer::singleShot(3000, this, [this]() {
            QPropertyAnimation *fadeOut = new QPropertyAnimation(credentialsLabel->graphicsEffect(), "opacity");
            fadeOut->setDuration(500);
            fadeOut->setStartValue(1);
            fadeOut->setEndValue(0);
            connect(fadeOut, &QPropertyAnimation::finished, [this]() {
                credentialsLabel->setVisible(false);
            });
            fadeOut->start(QAbstractAnimation::DeleteWhenStopped);
        });
    }
}

void PasswordRecoveryDialog::closeAfterTimeout()
{
    QPropertyAnimation *fadeOut = new QPropertyAnimation(this, "windowOpacity");
    fadeOut->setDuration(500);
    fadeOut->setStartValue(1);
    fadeOut->setEndValue(0);
    connect(fadeOut, &QPropertyAnimation::finished, this, &PasswordRecoveryDialog::accept);
    fadeOut->start(QAbstractAnimation::DeleteWhenStopped);
}
