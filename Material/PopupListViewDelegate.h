#ifndef POPUPLISTVIEWDELEGATE_H
#define POPUPLISTVIEWDELEGATE_H

#include <QStyledItemDelegate>
class AssetKindCombobox;

class PopupListViewDelegate : public QStyledItemDelegate
{
public:
    explicit PopupListViewDelegate(int width, QObject* parent = nullptr);
    void setCombobox(AssetKindCombobox* combobox);
public:

protected:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &) const override;

    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;
private:
    AssetKindCombobox* m_combobox;
    int m_width;
};

#endif // POPUPLISTVIEWDELEGATE_H
