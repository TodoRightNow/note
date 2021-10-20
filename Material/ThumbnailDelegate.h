#ifndef THUMBNAILDELEGATE_H
#define THUMBNAILDELEGATE_H

#include <QStyledItemDelegate>
#include "AssetItem.h"
#include "CustomTableView.h"
class Asset;
class QToolTip;

class ThumbnailDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit ThumbnailDelegate(CustomTableView* tableview, QObject* parent = nullptr);
    explicit ThumbnailDelegate(CustomTableView* tableview, Asset* asset, QObject* parent = nullptr);
    virtual ~ThumbnailDelegate() override;

    void setAsset(Asset* asset);

signals:
    void modelDataSelected(AssetItem* item);
    void startDownload(AssetItem* item);

protected:

     void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

     QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

     bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;
private:
    QPixmap* m_fillingPolygon;
    QPixmap* m_unFillingPolygon;

    CustomTableView* m_tableview;

    Asset* m_asset;

    bool m_needSaveThumbnail;
    QToolTip* m_tooltip{nullptr};

    mutable bool m_hasPressed;
};

#endif // THUMBNAILDELEGATE_H
