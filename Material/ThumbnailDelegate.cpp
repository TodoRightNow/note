#include "ThumbnailDelegate.h"
#include <QPainter>
#include "Common/Util.h"
#include <QDebug>
#include <QMouseEvent>
#include <QDrag>
#include <QMimeData>
#include "DownloadAsset.h"
#include <QThreadPool>
#include <QToolTip>
#include <QDir>
#include "Asset.h"

ThumbnailDelegate::ThumbnailDelegate(CustomTableView *tableview, Asset* asset, QObject *parent)
    : QStyledItemDelegate (parent)
    , m_tableview(tableview)
    , m_asset(asset)
    , m_needSaveThumbnail(false)
{
    m_fillingPolygon = new QPixmap(":/images/collected.png");
    m_unFillingPolygon = new QPixmap(":/images/uncollect.png");
}

ThumbnailDelegate::ThumbnailDelegate(CustomTableView *tableview, QObject *parent)
    : QStyledItemDelegate (parent)
    , m_tableview(tableview)
    , m_needSaveThumbnail(true)
{
    m_fillingPolygon = new QPixmap(":/images/collected.png");
    m_unFillingPolygon = new QPixmap(":/images/uncollect.png");
}

ThumbnailDelegate::~ThumbnailDelegate()
{
    delete m_fillingPolygon;
    delete m_unFillingPolygon;
}

void ThumbnailDelegate::setAsset(Asset *asset)
{
    m_asset = asset;
}

// TODO 优化 QRect QPixmap 存起来
void ThumbnailDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.isValid() && index.data().canConvert<AssetItem*>())
    {
        AssetItem* item = index.data().value<AssetItem*>();
        QPixmap pixmap;
        if (m_needSaveThumbnail)
        {
            bool flag = pixmap.load(item->m_matFile_thumbnail);
            if (!flag)
            {
                DownloadAsset* downloadAsset = new DownloadAsset(item->m_remoteDir,
                                                                 item->m_matFile_thumbnail.right(item->m_matFile_thumbnail.length() - item->m_matFile_thumbnail.lastIndexOf('/') -1), item->m_localSaveDir, false);
                connect(downloadAsset, &DownloadAsset::downloadAllFinished, [this]{
                    m_tableview->viewport()->update();
                });
                QThreadPool::globalInstance()->start(downloadAsset);
            }
        }
        else
        {
            if (!item->m_imageData.isEmpty())
            {
                pixmap.loadFromData(item->m_imageData);
            }

            if (pixmap.isNull())
            {
                QString fileName = item->m_matFile_thumbnail.right(item->m_matFile_thumbnail.length() - item->m_matFile_thumbnail.lastIndexOf('/') - 1);
                DownloadAsset* downloadAsset = new DownloadAsset(item->m_remoteDir, fileName, item);
                connect(downloadAsset, &DownloadAsset::sendImageData, [item, this](const QByteArray& imageData){
                    item->m_imageData = imageData;
                    m_asset->appendToCache(item);
                });
                connect(downloadAsset, &DownloadAsset::downloadAllFinished, [this]{
                    m_tableview->viewport()->update();
                });
                QThreadPool::globalInstance()->start(downloadAsset);
            }
        }

        painter->drawPixmap(option.rect.adjusted(0, 0, -12, -46), pixmap, pixmap.rect());

        if (item->m_isShowDownloadText)
        {
            QRect downloadRect = option.rect.adjusted(12, 112, -24, -56);

            painter->setBrush(QBrush(QColor(60, 123, 234, 178)));
            painter->setPen(Qt::NoPen);
            painter->drawRoundedRect(downloadRect, 4, 4);

            if (item->m_percentage > 0)
            {
                QPainterPath path;
                int has_download_width = static_cast<int>(downloadRect.width() * item->m_percentage);
                path.addRegion(Util::roundRect(option.rect.adjusted(12, 112, -24 - downloadRect.width() + has_download_width, -56), 4, true, false, true, false));
                painter->fillPath(path, QBrush(QColor(60, 123, 234)));
            }

            painter->setPen(Qt::white);
            QFont font = painter->font();
            font.setPixelSize(12);
            painter->setFont(font);
            // QRect(12, 112, 124, 26)
            painter->drawText(option.rect.adjusted(12, 112, -24, -56), Qt::AlignCenter, item->m_displayText);
        }

        if (item->m_isChecked)
        {
            painter->fillRect(option.rect.adjusted(0, 148, -12, -12), QBrush(QColor(60, 123, 234)));
        }
        else
        {
            painter->fillRect(option.rect.adjusted(0, 148, -12, -12), QBrush(QColor(35, 37, 49)));
        }
        // QRect(12, 148, 112, 34)
        // QRect(12, 148, 148, 34)
        //        QRect textBoundRect =  option.rect.adjusted(12, 148, -36, -12);
        painter->setPen(Qt::white);
        painter->drawText(option.rect.adjusted(12, 148, -12, -12), Qt::AlignLeft | Qt::AlignVCenter, item->m_name, nullptr);

        // 五角星
        // QRect(122, 157, 16, 16)
        if (item->m_isFillPolygon)
        {
            painter->drawPixmap(option.rect.adjusted(122, 157, -22, -21), *m_fillingPolygon, m_fillingPolygon->rect());
        }
        else
        {
            painter->drawPixmap(option.rect.adjusted(122, 157, -22, -21), *m_unFillingPolygon, m_unFillingPolygon->rect());
        }
    }
}

QSize ThumbnailDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QSize(160, 194);
}

bool ThumbnailDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if (!index.isValid())
    {
        return false;
    }
    AssetItem* item = index.data().value<AssetItem*>();
    if (event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        if (mouseEvent->button() == Qt::LeftButton)
        {
            if (option.rect.adjusted(122, 157, -22, -27).contains(mouseEvent->pos()))
            {
                item->m_isFillPolygon = !item->m_isFillPolygon;
                model->setData(index, QVariant::fromValue(item), Qt::DisplayRole);
                emit model->dataChanged(index, index);
            }
            else if (option.rect.adjusted(12, 112, -24, -62).contains(mouseEvent->pos()))
            {
                if(item->m_isShowDownloadText)
                {
                    //                    item.m_isShowDownloadText = false;
                    //                    model->setData(index, QVariant::fromValue(item), Qt::DisplayRole);
                    emit model->dataChanged(index, index);
                    emit startDownload(item);
                }
            }
            else if (!item->m_isChecked && option.rect.adjusted(0, 0, -12, -12).contains(mouseEvent->pos()))
            {
                item->m_isChecked = !item->m_isChecked;
                model->setData(index, QVariant::fromValue(item), Qt::DisplayRole);
                emit model->dataChanged(index, index);
                emit modelDataSelected(item);
            }

        }
        m_hasPressed = true;
        return true;
    }
    else if (event->type() == QEvent::MouseButtonRelease)
    {
        m_hasPressed = false;
    }
    else if (m_hasPressed && !item->m_isShowDownloadText && event->type() == QEvent::MouseMove)
    {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        QDrag* drag = new QDrag(this);
        drag->setMimeData(model->mimeData({index}));
        AssetItem item = index.data().value<AssetItem>();
        drag->setPixmap(QPixmap(item.m_matFile_thumbnail).scaled(148, 148));
        drag->setHotSpot(QPoint(mouseEvent->x() % 160, mouseEvent->y() % 200));

        drag->exec(Qt::MoveAction | Qt::CopyAction, Qt::CopyAction);
    }
    return false;
}
