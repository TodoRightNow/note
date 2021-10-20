#include "PopupListViewDelegate.h"
#include <QPainter>
#include <QEvent>
#include <QMouseEvent>
#include "AssetKindCombobox.h"
#include <QDebug>

PopupListViewDelegate::PopupListViewDelegate(int width, QObject *parent)
    : QStyledItemDelegate(parent)
    , m_width(width)
{

}

void PopupListViewDelegate::setCombobox(AssetKindCombobox *combobox)
{
    m_combobox = combobox;
}

void PopupListViewDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QString text = index.model()->data(index, Qt::DisplayRole).toString();

    if (option.state & QStyle::State_Selected || option.state & QStyle::State_MouseOver )
    {
        painter->setPen(QColor(255, 255, 255));
        painter->fillRect(option.rect, QBrush(QColor(47, 62, 96)));
    }
    else
    {
        painter->setPen(QColor(189, 189, 189));
        painter->fillRect(option.rect, QBrush(QColor(45, 45, 56)));
    }
    painter->drawText(option.rect.adjusted(10, 0 , 0 , 0), Qt::AlignLeft | Qt::AlignVCenter, text);
}

QSize PopupListViewDelegate::sizeHint(const QStyleOptionViewItem &, const QModelIndex &) const
{
    return QSize(m_width, 22);
}

bool PopupListViewDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if (event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        if (mouseEvent->button() == Qt::LeftButton)
        {
            m_combobox->setAssetName(model->data(index, Qt::DisplayRole).toString());
            m_combobox->hidePopupWidget();
        }
        return true;
    }
    return false;
}
