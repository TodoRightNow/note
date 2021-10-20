#ifndef TREEITEM_H
#define TREEITEM_H

#include <QList>

// 这个类提供访问parent_item的 和各个 child_item的信息



class TreeItem
{
public:
    explicit TreeItem(const QVector<QVariant> &itemData, TreeItem* parent = nullptr);
    ~TreeItem();

    TreeItem *child(int number);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    bool insertChildren(int position, int count, int columns);
    bool insertColumns(int position, int columns);
    TreeItem *parent();
    bool removeChildren(int position, int count);
    bool removeColumns(int position, int columns);
    int childNumber() const;
    bool setData(int column, const QString &value);


private:
    QList<TreeItem*> m_childItems;
    QVector<QVariant> m_itemData; // 每项里的数据
    TreeItem* m_parentItem;
};

#endif // TREEITEM_H
