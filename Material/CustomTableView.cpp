#include "CustomTableView.h"
#include <QDebug>
#include <QScrollBar>
#include "ThumbnailModel.h"
CustomTableView::CustomTableView(QWidget *parent)
    : QTableView (parent)
{

}

QRect CustomTableView::visualRect(const QModelIndex &index) const
{
    return QTableView::visualRect(index);
}

void CustomTableView::resizeEvent(QResizeEvent *)
{
    //    static bool first_init = true;
    //    if (first_init)
    //    {
    //        first_init = false;
    //    }
    //    else
    //    {
    ThumbnailModel* model = static_cast<ThumbnailModel*> (this->model());
    if (model)
    {
        model->setColumnCount(width()/ 159);
        emit model->layoutChanged();
    }

}

QSize CustomTableView::viewportSizeHint() const
{
    QSize size = QTableView::viewportSizeHint();
    return QSize(1133, size.height());
}
