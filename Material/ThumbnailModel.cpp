#include "ThumbnailModel.h"
#include <QDebug>
#include <QMimeData>
#include <QDir>
#include "UserProfile/LocalSetting.h"

ThumbnailModel::ThumbnailModel(int colunmCount, bool isSupportDrag, QObject *parent)
    : QAbstractTableModel (parent)
    , m_isSupportDrag(isSupportDrag)
    , m_colunmCount(colunmCount)
{

}

void ThumbnailModel::setAssetItemVec(const QVector<AssetItem*> &items)
{
    beginResetModel();
    m_assetItemVec = items;
    endResetModel();
}

void ThumbnailModel::setColumnCount(int columnCount)
{
    m_colunmCount = columnCount;
}

void ThumbnailModel::update()
{
    emit layoutChanged();
}

int ThumbnailModel::columnCount(const QModelIndex &) const
{
    return m_colunmCount;
}

QVariant ThumbnailModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid() && role == Qt::DisplayRole)
    {
        return QVariant::fromValue(m_assetItemVec.at(m_colunmCount* index.row() + index.column()));
    }
    else if (index.isValid() && role == Qt::ToolTipRole)
    {
        return QVariant::fromValue(m_assetItemVec.at(m_colunmCount* index.row() + index.column())->m_realName);
    }
    return QVariant();
}

int ThumbnailModel::rowCount(const QModelIndex &) const
{
    return (m_assetItemVec.size() + m_colunmCount - 1) / m_colunmCount;
}

bool ThumbnailModel::setData(const QModelIndex &index, const QVariant &value, int )
{
    if (!index.isValid())
    {
        return false;
    }
    m_assetItemVec[index.row() * m_colunmCount + index.column()] = value.value<AssetItem*>();
    return true;
}

QModelIndex ThumbnailModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent) ||  row * m_colunmCount + column >= m_assetItemVec.size())
    {
        return QModelIndex();
    }
    return createIndex(row, column);
}

QModelIndex ThumbnailModel::getIndex(AssetItem* item)
{
    int findIndex = m_assetItemVec.indexOf(item);
    if (findIndex == -1)
    {
        return QModelIndex();
    }
    return index(findIndex/m_colunmCount, findIndex % m_colunmCount);
}

QStringList ThumbnailModel::mimeTypes() const
{
    if (!m_isSupportDrag)
    {
        return QAbstractTableModel::mimeTypes();
    }
    QStringList types;
    types << "text/uri-list";
    return types;
}

QMimeData *ThumbnailModel::mimeData(const QModelIndexList &indexes) const
{
    if (!m_isSupportDrag)
    {
        return nullptr;
    }
    QMimeData* mimeData = new QMimeData;
    QByteArray encodeData;

    if (indexes.size() > 1)
    {
        return nullptr;
    }


    if (indexes.at(0).isValid())
    {
        AssetItem* item = m_assetItemVec.at(indexes.at(0).row() * m_colunmCount + indexes.at(0).column());
        QString mat_path = item->m_matFilePath;
        qDebug() << "mat_path:  " << mat_path;
        QString fileName = QDir::currentPath() + "/qipancloud.ms";
        qDebug() << "fileName:  " << fileName;
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly))
        {
             QTextStream out(&file);
             out << "G_QPCloud.QiPanCloud_Mat #(";
             for (int i = 0; i < item->m_downLoadPaths.size() -1; i++)
             {
                  out << QString("@\"%1\",").arg(LocalSetting::getInstance()->getAssetSavePath() + item->m_downLoadPaths.at(i));
             }
             out << QString("@\"%1\")").arg(LocalSetting::getInstance()->getAssetSavePath() + item->m_downLoadPaths.last());
        }


        file.close();
        fileName = "file:///" + fileName;

        mimeData->setData("text/uri-list", fileName.toLocal8Bit());
        return mimeData;
    }
    return nullptr;
}
