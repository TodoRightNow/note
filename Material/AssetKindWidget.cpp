#include "AssetKindWidget.h"
#include <QPainter>
#include "Common/Util.h"
#include <QPaintEngine>
#include <QDebug>
#include <QMouseEvent>

AssetKindWidget::AssetKindWidget(const QString topAssetName, const QString &num, const QString &assetName, QSize size, int digitalWidth, const QStringList &childAssetNameList, bool isChecked , bool needArrowIcon, QWidget *parent)
    : QWidget(parent)
    , m_isChecked(isChecked)
    , m_needArrowIcon(needArrowIcon)
    , m_topAssetName(topAssetName)
    , m_curSelectAssetName{""}
    , m_num(num)
    , m_assetName(assetName)
    , m_childAssetNameList(childAssetNameList)
{
    m_digitalWidth = digitalWidth;
    setFixedSize(size);
    setAttribute(Qt::WA_TranslucentBackground);
    setMouseTracking(true);
    m_pixmap = QPixmap(":/images/down-arrow.png");

    //    m_curSelectAssetName = m_assetName;
}

void AssetKindWidget::setIsChecked(bool flag)
{
    if (m_isChecked != flag)
    {
        m_isChecked = flag;
        repaint();
    }

}

void AssetKindWidget::setIsNeedArrowIcon(bool flag)
{
    if (m_needArrowIcon != flag)
    {
        m_needArrowIcon = flag;
        repaint();
    }

}
// 这个还要
void AssetKindWidget::setAssetName(const QString &name)
{
    if (m_curSelectAssetName != name)
    {
        m_curSelectAssetName = name;
        emit clicked(m_topAssetName, QStringList() << m_assetName << name, false);
    }
}

void AssetKindWidget::setChildAssetNameList(const QStringList &names)
{
    m_childAssetNameList = names;
}

QStringList AssetKindWidget::getChildAssetNameList() const
{
    return m_childAssetNameList;
}

QRect AssetKindWidget::getArrowIconRect() const
{
    return QRect(this->width() - 28, 0, 28, 22);
}

QRect AssetKindWidget::getAssetNameRect() const
{
    return QRect(m_digitalWidth, 0, this->width() - 28 - m_digitalWidth, 22);
}

bool AssetKindWidget::operator ==(const AssetKindWidget &other)
{
    if (m_topAssetName == other.m_topAssetName && m_assetName == other.m_assetName)
    {
        return true;
    }
    return false;
}

bool AssetKindWidget::operator !=(const AssetKindWidget &other)
{
    return !(operator==(other));
}

QString AssetKindWidget::getAssetName() const
{
    return m_assetName;
}

bool AssetKindWidget::getArrowIconShowStatus()
{
    return m_needArrowIcon;
}

void AssetKindWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing);
    // 绘制数字
    QRect digitalRect(0, 0, m_digitalWidth, 22);

    QRegion digitalRegion = Util::roundRect(digitalRect, 2, true, false, true, false);
    QPainterPath path;
    path.addRegion(digitalRegion);

    if (m_isChecked)
    {
        painter.setPen(Qt::white);
        painter.fillPath(path,QBrush(QColor(60, 123, 234)));
    }
    else
    {
        painter.setPen(QColor("#BDBDBD"));
        painter.fillPath(path,QBrush(QColor(71, 71, 81)));
    }

    QFont font = painter.font();
    font.setPixelSize(12);
    painter.setFont(font);

    painter.drawText(digitalRect, Qt::AlignCenter, m_num);


    // 绘制资产名称
    QRect assetNameRect(m_digitalWidth + 10, 0, this->width() - m_digitalWidth - 38, 22);
    QRegion assetNameRegion = Util::roundRect(QRect(m_digitalWidth, 0, this->width() - m_digitalWidth, 22), 2, false, true, false, true);
    path = QPainterPath();
    path.addRegion(assetNameRegion);

    if (m_isChecked)
    {
        painter.fillPath(path,QBrush(QColor(47, 62, 96)));
        if (m_needArrowIcon && m_hoverRegion == HoverRegion::ASSET_NAME_REGION)
        {
            painter.fillRect(QRect(m_digitalWidth, 0, this->width() - 28 - m_digitalWidth, 22), QBrush(QColor(60, 123, 234)));
        }
        QPen pen = painter.pen();
        pen.setColor(Qt::white);
        painter.setPen(pen);
    }
    else
    {
        painter.fillPath(path,QBrush(QColor(54, 54, 65)));
        if (m_needArrowIcon && m_hoverRegion == HoverRegion::ASSET_NAME_REGION)
        {
            painter.fillRect(QRect(m_digitalWidth, 0, this->width() - 28 - m_digitalWidth, 22), QBrush(QColor(71, 71, 81)));

            QPen pen = painter.pen();
            pen.setColor(Qt::white);
            painter.setPen(pen);
        }
    }

    if (m_needArrowIcon && m_hoverRegion == HoverRegion::ARROW_ICON_REGION)
    {
        if (m_isChecked)
        {
            painter.fillRect(QRect(this->width() - 28, 0, 28, 22), QBrush(QColor(60, 123, 234)));
        }
        else
        {
            painter.fillRect(QRect(this->width() - 28, 0, 28, 22), QBrush(QColor(71, 71, 81)));
        }
    }

    QRect boundRect = assetNameRect.adjusted(0, 0, -30, 0);
    painter.drawText(assetNameRect, Qt::AlignLeft | Qt::AlignVCenter, m_assetName, &boundRect);

    if (m_needArrowIcon)
    {
        painter.drawPixmap(QRect(this->width() - 20, 8, 10, 6), m_pixmap);
    }
}

void AssetKindWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (!m_needArrowIcon)
    {
        return;
    }
    if (QRect(40, 0, this->width() - 58, 22).contains(event->pos()))
    {
        if (m_hoverRegion != HoverRegion::ASSET_NAME_REGION)
        {
            m_hoverRegion = HoverRegion::ASSET_NAME_REGION;
            repaint(QRect(40, 0, this->width() - 40, 22));
        }
    }
    else if (QRect(this->width() - 18, 0, 18, 22).contains(event->pos()))
    {
        if (m_hoverRegion != HoverRegion::ARROW_ICON_REGION)
        {
            m_hoverRegion = HoverRegion::ARROW_ICON_REGION;
            repaint(QRect(40, 0, this->width() - 40, 22));
        }
    }
    else
    {
        if (m_hoverRegion != HoverRegion::UNKNOWN_REGION)
        {
            HoverRegion temp = m_hoverRegion;
            m_hoverRegion = HoverRegion::UNKNOWN_REGION;
            if (temp == HoverRegion::ASSET_NAME_REGION)
            {
                repaint(QRect(QRect(40, 0, this->width() - 40, 22)));
            }
            else
            {
                repaint(QRect(QRect(40, 0, this->width() - 40, 22)));
            }

        }
    }
}

void AssetKindWidget::leaveEvent(QEvent *)
{
    if (m_hoverRegion != HoverRegion::UNKNOWN_REGION)
    {
        HoverRegion temp = m_hoverRegion;
        m_hoverRegion = HoverRegion::UNKNOWN_REGION;
        if (temp == HoverRegion::ASSET_NAME_REGION)
        {
            repaint(QRect(40, 0, this->width() - 68, 22));
        }
        else
        {
            repaint(QRect(this->width() - 28, 0, 28, 22));
        }
    }
}

void AssetKindWidget::mousePressEvent(QMouseEvent *event)
{   
    if (QRect(m_digitalWidth, 0, this->width() - m_digitalWidth - (m_needArrowIcon ? 28 : 0), 22).contains(event->pos()))
    {
        m_curSelectAssetName = "";
        setIsChecked(true);
        emit clicked(m_topAssetName, QStringList() << m_assetName, true);
    }
    else if (m_needArrowIcon &&  QRect(this->width() - 28, 0, 28, 22).contains(event->pos()))
    {
        emit clicked("", QStringList() << "", true);
    }

    return QWidget::mousePressEvent(event);
}



