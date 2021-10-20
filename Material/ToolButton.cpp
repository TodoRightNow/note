#include "ToolButton.h"
#include <QPainter>

ToolButton::ToolButton(const QString text, const QPixmap &pixmap, QWidget *parent)
    : QWidget(parent)
    , m_pixmap(pixmap)
    , m_content(text)
{
    setAttribute(Qt::WA_TranslucentBackground);
}

void ToolButton::setPixmap(const QPixmap &pixmap)
{
    m_pixmap = pixmap;
    QSize size = m_pixmap.size();
    repaint(QRect(QPoint(0, (height() - size.height()) / 2), size));
}

void ToolButton::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QSize size = m_pixmap.size();

    painter.drawPixmap(QRect(QPoint(0, (height() - size.height()) / 2), size), m_pixmap);

    painter.setPen(QColor("#BDBDBD"));
    QFont font = painter.font();
    font.setPixelSize(12);
    painter.setFont(font);
    painter.drawText(rect().adjusted(m_pixmap.width() + 6, 0, 0, 0), Qt::AlignVCenter, m_content);

}

void ToolButton::mousePressEvent(QMouseEvent *event)
{

}
