#include "ThumbnailView.h"
#include <QPainter>
#include <QPixmap>
#include <QMouseEvent>
#include <QDebug>
#include "Common/Util.h"

//QPixmap ThumbnailView::unCollectedPixmap = QPixmap(":/images/uncollect.png");
//QPixmap ThumbnailView::collectedPixmap = QPixmap(":/images/collected.png");

ThumbnailView::ThumbnailView(const QPixmap &ThumbnailPixmap, const QPixmap &starPixmap, const QString& name, QWidget *parent, bool isShowDownloadText,
                             bool isChecked, bool isFillPolygon)
    : QWidget(parent)
    , m_isShowDownloadText(isShowDownloadText)
    , m_isChecked(isChecked)
    , m_isFillPolygon(isFillPolygon)
    , m_pixmap(ThumbnailPixmap)
    , m_name(name)
    , m_starPixmap(starPixmap)
{
    setFixedSize(ThumbnailViewItemWidth, ThumbnailViewItemHeight);
//    setAttribute(Qt::WA_Hover);
    setMouseTracking(true);
}

void ThumbnailView::setIsShowDownloadText(bool flag)
{
    if (m_isShowDownloadText != flag)
    {
        m_isShowDownloadText = flag;
        update(QRect(12, 112, 124, 26));
    }
}

void ThumbnailView::setIsChecked(bool flag)
{
    if (m_isChecked != flag)
    {
        m_isChecked = flag;
        update(QRect(0, 148, 148, 34));
    }

}

void ThumbnailView::setIsFillPolygon(bool flag)
{
    if (m_isFillPolygon != flag)
    {
        m_isFillPolygon = flag;
        if (m_isFillPolygon)
        {
            m_starPixmap = QPixmap(":/images/collected.png");
        }
        else
        {
            m_starPixmap = QPixmap(":/images/uncollect.png");
        }
        repaint(QRect(122, 157, 16, 16));
    }

}

void ThumbnailView::setPixmap(const QPixmap& pixmap)
{
    m_pixmap = pixmap;
    update(QRect(0, 0, 148, 148));
}

void ThumbnailView::setDeepDownloadBkgColor(bool flag)
{
    if (m_isDeepDownloadBkgCol != flag)
    {
        m_isDeepDownloadBkgCol = flag;
        repaint(QRect(12, 112, 124, 26));
    }
}

void ThumbnailView::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(QRectF(0, 0, 148, 148), m_pixmap, m_pixmap.rect());

    if (m_isShowDownloadText)
    {
        QRegion download_region = Util::roundRect(m_downloadRect, 4, true, true, true, true);
        QPainterPath path;
        path.addRegion(download_region);
        if (!m_isDeepDownloadBkgCol)
        {
            painter.fillPath(path, QBrush(QColor(60, 123, 234, 178)));
        }
        else
        {
            painter.fillPath(path, QBrush(QColor(60, 123, 234, 229)));
        }

        painter.setPen(Qt::white);
        QFont font = painter.font();
        font.setPixelSize(12);
        painter.setFont(font);
        painter.drawText(m_downloadRect, Qt::AlignCenter, QStringLiteral("下载"));
    }

    if (m_isChecked)
    {
        painter.fillRect(QRect(0, 148, 148, 34), QBrush(QColor(60, 123, 234)));
    }
    else
    {
        painter.fillRect(QRect(0, 148, 148, 34), QBrush(QColor(20, 26, 39, 0)));
    }
    QRect boundRect = QRect(12, 148, 148, 34).adjusted(0, 0, -36, 0);
    painter.drawText(QRect(12, 148, 148, 34), Qt::AlignLeft | Qt::AlignVCenter, m_name, &boundRect);

    // 五角星
    painter.drawPixmap(QRect(122, 157, 16, 16), m_starPixmap, m_starPixmap.rect());

}

void ThumbnailView::mousePressEvent(QMouseEvent *event)
{
    // 下载 区域
    QRect rect(12, 112, 124, 26);
    if (rect.contains(event->pos()))
    {
        qDebug() << "download";
        setIsShowDownloadText(false);
    }
    else if (QRect(122, 157, 16, 16).contains(event->pos()))
    {
        setIsFillPolygon(!m_isFillPolygon);
    }
    else
    {
        setIsChecked(!m_isChecked);
    }
}

void ThumbnailView::mouseMoveEvent(QMouseEvent *event)
{
    if (m_isShowDownloadText)
    {
        if (m_downloadRect.contains(event->pos()))
        {
            setDeepDownloadBkgColor(true);
        }
        else
        {
            setDeepDownloadBkgColor(false);
        }
    }
}
