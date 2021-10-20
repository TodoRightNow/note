#ifndef TREEITEM_H
#define TREEITEM_H

#include <QList>

// ������ṩ����parent_item�� �͸��� child_item����Ϣ



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
    QVector<QVariant> m_itemData; // ÿ���������
    TreeItem* m_parentItem;
};

#endif // TREEITEM_H
