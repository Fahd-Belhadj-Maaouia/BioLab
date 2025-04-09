#include "buttondelegate.h"
#include <QPainter>
#include <QMouseEvent>
#include <QApplication>
#include <QDebug>

ButtonDelegate::ButtonDelegate(QObject *parent) : QStyledItemDelegate(parent) {}

QRect ButtonDelegate::getButtonRect(const QRect &cellRect, bool isUpdateButton) const
{
    const int buttonWidth = 60;
    const int buttonHeight = 25;
    const int spacing = 10;

    int startX = cellRect.left() + (isUpdateButton ? 10 : (10 + buttonWidth + spacing));
    int startY = cellRect.top() + (cellRect.height() - buttonHeight) / 2;

    return QRect(startX, startY, buttonWidth, buttonHeight);
}

void ButtonDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyledItemDelegate::paint(painter, option, index);

    QStyleOptionButton updateButton, deleteButton;
    updateButton.rect = getButtonRect(option.rect, true);
    deleteButton.rect = getButtonRect(option.rect, false);

    updateButton.text = "Update";
    deleteButton.text = "Delete";
    updateButton.state = QStyle::State_Enabled;
    deleteButton.state = QStyle::State_Enabled;

    // Style buttons
    updateButton.palette.setColor(QPalette::Button, QColor(76, 175, 80));
    deleteButton.palette.setColor(QPalette::Button, QColor(244, 67, 54));
    updateButton.palette.setColor(QPalette::ButtonText, Qt::white);
    deleteButton.palette.setColor(QPalette::ButtonText, Qt::white);

    QApplication::style()->drawControl(QStyle::CE_PushButton, &updateButton, painter);
    QApplication::style()->drawControl(QStyle::CE_PushButton, &deleteButton, painter);
}

bool ButtonDelegate::editorEvent(QEvent *event, QAbstractItemModel *model,
                                 const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if (event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);

        QRect updateRect = getButtonRect(option.rect, true);
        QRect deleteRect = getButtonRect(option.rect, false);
        QPoint mousePos = static_cast<QMouseEvent*>(event)->pos();

        qDebug() << "Mouse clicked at cell-relative:" << mousePos;
        qDebug() << "Update rect:" << updateRect;
        qDebug() << "Delete rect:" << deleteRect;

        if (updateRect.contains(mousePos)) {
            qDebug() << "[Update] button clicked at row:" << index.row();
            emit updateClicked(index);
            return true;
        }
        else if (deleteRect.contains(mousePos)) {
            qDebug() << "[Delete] button clicked at row:" << index.row(); // <--- THIS
            emit deleteClicked(index);
            return true;
        } else {
            qDebug() << "Clicked outside buttons.";
        }

    }
    return false;
}




QSize ButtonDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QSize(200, 40); // Make sure cells are tall enough
}
