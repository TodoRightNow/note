#include "TreeModel.h"
#include "TreeItem.h"
#include <QJsonObject>

TreeModel::TreeModel(const QString&data, const QStringList &headers, QObject* parent)
    : QAbstractItemModel(parent)
{
    m_showHeader = (headers.size() != 0);
    QVector<QVariant> rootData;
    foreach (QString header, headers)
    {
        rootData << header;
    }

    m_rootItem = new TreeItem(rootData);
    setupModelData(data.split('\n'), m_rootItem);

}

TreeModel::TreeModel(const QJsonObject &obj, const QStringList &headers, QObject *parent)
    : QAbstractItemModel(parent)
{
    m_showHeader = (headers.size() != 0);
    QVector<QVariant> rootData;
    foreach (QString header, headers)
    {
        rootData << header;
    }

    m_rootItem = new TreeItem(rootData);
    setupModelData(obj);
}

TreeModel::~TreeModel()
{
    delete m_rootItem;
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
    // 可扩展的列数(parent) 一般都是为第一列的
    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();

    TreeItem *parentItem = getItem(parent);

    // 给child 创建index
    TreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();

    TreeItem *item = getItem(index);

    return item->data(index.column());
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (!m_showHeader)
    {
        return QVariant();
    }
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {

        return m_rootItem->data(section);
    }

    return QVariant();
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
    TreeItem *parentItem = getItem(parent);

    return parentItem->childCount();
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
    return m_rootItem->columnCount();
}

TreeItem *TreeModel::getItem(const QModelIndex &index) const
{
    // 通过 index() 最开始的一个item 也就是rootItem 是 invalid的
    if (index.isValid()) {
        TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
        if (item)
            return item;
    }
    return m_rootItem;
}


// 这个是 解析这样的数据
/**
  * a
  *   a1 a2
  * b
  *   b1
  * c
  *   c1
  *     c11
  *
 */
void TreeModel::setupModelData(const QStringList &lines, TreeItem *parent)
{
    QList<TreeItem*> parents;
    QList<int> indentations;
    parents << parent;
    indentations << 0;

    int number = 0;

    while (number < lines.count()) {
        int position = 0;
        while (position < lines[number].length()) {
            if (lines[number].at(position) != ' ')
                break;
            ++position;
        }

        QString lineData = lines[number].mid(position).trimmed();

        if (!lineData.isEmpty()) {
            // Read the column data from the rest of the line.
            QStringList columnStrings = lineData.split("\t", QString::SkipEmptyParts);
            QVector<QString> columnData;
            for (int column = 0; column < columnStrings.count(); ++column)
                columnData << columnStrings[column];

            if (position > indentations.last()) {
                // The last child of the current parent is now the new parent
                // unless the current parent has no children.

                if (parents.last()->childCount() > 0) {
                    parents << parents.last()->child(parents.last()->childCount()-1);
                    indentations << position;
                }
            } else {
                while (position < indentations.last() && parents.count() > 0) {
                    parents.pop_back();
                    indentations.pop_back();
                }
            }

            // Append a new item to the current parent's list of children.
            TreeItem *parent = parents.last();
            parent->insertChildren(parent->childCount(), 1, m_rootItem->columnCount());
            for (int column = 0; column < columnData.size(); ++column)
                parent->child(parent->childCount() - 1)->setData(column, columnData[column]);
        }

        ++number;
    }

}

/**
 * 实现如下 的格式
 *
 * {
    "材质类": {
        "Vray材质": "F:\\Material\\Vray_Material\\Vray_material.json",
        "Corona材质": ""
    },
    "贴图类": {},
    "灯光类": {}
}
 */
void TreeModel::setupModelData(const QJsonObject &obj)
{

    int column_count = m_rootItem->columnCount();

    TreeItem* parentItem = m_rootItem;

    QStringList keys = obj.keys();

    for(int i = 0; i < keys.size(); ++i)
    {
        auto key = keys.at(i);
        parentItem->insertChildren(i, 1, column_count);
        parentItem->child(i - 1)->setData(0, key);

        if (obj.value(key).isObject())
        {
            QJsonObject copyObj = obj.value(key).toObject();
            parentItem = parentItem->child(i - 1);
            QStringList subKeys = copyObj.keys();
            for (int j = 0; j < subKeys.size(); j++)
            {
                auto key = subKeys.at(j);
                parentItem->insertChildren(j, 1, column_count);
                parentItem->child(j - 1)->setData(0, key);
            }
        }
    }
}
