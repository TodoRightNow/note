#ifndef THUMBNAILMODEL_H
#define THUMBNAILMODEL_H

#include <QAbstractTableModel>
#include "AssetItem.h"

class ThumbnailModel : public QAbstractTableModel
{
public:
    explicit ThumbnailModel(int colunmCount, bool isSupportDrag, QObject* parent = nullptr);

    void setAssetItemVec(const QVector<AssetItem *> &items);

    void setColumnCount(int columnCount);

    void update();

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;

    QModelIndex getIndex(AssetItem *item);

protected:
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    bool setData(const QModelIndex &index, const QVariant &value, int = Qt::EditRole) override;



//    Qt::ItemFlags flags(const QModelIndex &index) const override;

    QStringList mimeTypes() const override;

    QMimeData* mimeData(const QModelIndexList &indexes) const override;


//     bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
private:
    bool m_isSupportDrag;
    int m_colunmCount;
    QVector<AssetItem*> m_assetItemVec;
};

#endif // THUMBNAILMODEL_H
