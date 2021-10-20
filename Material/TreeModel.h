#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QAbstractItemModel>

class TreeItem;

class TreeModel : public QAbstractItemModel
{
public:
    explicit TreeModel(const QString&data, const QStringList &headers = QStringList(), QObject* parent = nullptr);
     explicit TreeModel(const QJsonObject& obj, const QStringList &headers = QStringList(), QObject* parent = nullptr);
    ~TreeModel() override;

    // ʹm_rootItem index invalid
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;


    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;


private:
    TreeItem *getItem(const QModelIndex &index) const;
    void setupModelData(const QStringList &lines, TreeItem *parent);
    void setupModelData(const QJsonObject &obj);

private:
    bool m_isChecked {false};
    bool m_showHeader;
    TreeItem* m_rootItem;
};

#endif // TREEMODEL_H
