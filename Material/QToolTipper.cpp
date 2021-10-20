#include "QToolTipper.h"

#include <QAbstractItemView>
#include <QHelpEvent>
#include <QToolTip>
#include "AssetItem.h"
#include <QDebug>

QToolTipper::QToolTipper(QObject *parent) : QObject(parent)
{

}
bool QToolTipper::eventFilter(QObject* obj, QEvent* event)
{
    if (event->type() == QEvent::ToolTip)
    {
        QAbstractItemView* view = qobject_cast<QAbstractItemView*>(obj->parent());
        if (!view)
        {
            qDebug() << "is not a QAbstractItemView";
            return false;
        }
        QHelpEvent* helpEvent = static_cast<QHelpEvent*>(event);
        QPoint pos = helpEvent->pos();
        QModelIndex index = view->indexAt(pos);
        if (!index.isValid())
        {
            return false;
        }
        QRect rect = view->visualRect(index).adjusted(12, 155, -12, -19);
        QString toolTipStr = view->model()->data(index, Qt::ToolTipRole).toString();

        if (!toolTipStr.isEmpty() && rect.contains(helpEvent->pos()))
        {
            QToolTip::showText(helpEvent->globalPos(), toolTipStr, view, rect);
        }
        else
        {
            QToolTip::hideText();
        }
        return true;
    }
    return false;
}
