#ifndef TREEDELEGATE_H
#define TREEDELEGATE_H

#include <QStyledItemDelegate>

class TreeDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit TreeDelegate(QObject* parent = nullptr);


protected:
    void  paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;

};

#endif // TREEDELEGATE_H
