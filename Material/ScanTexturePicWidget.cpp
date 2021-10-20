#include "ScanTexturePicWidget.h"
#include <QPainter>
#include <QMatrix>
#include <QMouseEvent>
#include <QDebug>

// TODO 到底是在这里限制图像的宽高还是 在外面限制 （为了方便 在类里面限制）
// m_texturePixVec 至少个数为1的时候显示
ScanTexturePicWidget::ScanTexturePicWidget(QWidget *parent)
    : QWidget(parent)
    , m_leftPixmapArea(21.8, 13, 53, 53)
    , m_rightPixmapArea(144.6, 13, 53, 53)
    , m_midPixmapArea(77.6, 0, 66, 66)
{
    setAttribute(Qt::WA_TranslucentBackground);
    if (m_texturePixVec.size() < 1)
    {
        hide();
    }
//    currentBigArea = m_midPixmapArea;
}

//void ScanTexturePicWidget::setTexturePixVec(const QStringList &texturePixVec)
//{
//    m_texturePixVec = texturePixVec;
//    if (m_texturePixVec.size() > 0 )
//    {
//        show();

//    }
//    if (m_texturePixVec.size() >=2)
//    {
//        m_currentIndex = 1;
//    }


//    update();
//}

void ScanTexturePicWidget::setTexturePixVec(const QVector<QByteArray> &imageDataVec)
{
    m_texturePixVec = imageDataVec;

    if (m_texturePixVec.size() >=2)
    {
        m_currentIndex = 1;
    }

    if (m_texturePixVec.size() > 0 )
    {
        show();
    }

    qDebug() << "m_texturePixVec:  " << m_texturePixVec.size();

    update();
}

void ScanTexturePicWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    painter.fillRect(QRectF(0, 13, 19.8, 53), QBrush(QColor("#363641")));
    painter.drawImage(QRectF(6.4, 33.6, 7, 12), m_leftArrowPixmap);

    painter.fillRect(QRectF(200.2, 13, 19.8, 53), QBrush(QColor("#363641")));
    painter.drawImage(QRectF(206.6, 33.6, 7, 12), m_rightArrowPixmap);

    QPen pen = painter.pen();
    pen.setWidth(1);
    pen.setColor(QColor("#3B3B4A"));
    painter.setPen(pen);

    if (m_texturePixVec.size() < 2)
    {
        painter.drawRect(m_leftPixmapArea);
        painter.drawRect(m_rightPixmapArea);
    }

    else if (m_texturePixVec.size() == 2)
    {
         painter.drawImage(m_leftPixmapArea, QImage::fromData(m_texturePixVec.at(m_currentIndex - 1)).scaled(m_leftPixmapArea.width(), m_leftPixmapArea.width()));
    }
    else if (m_texturePixVec.size() >= 3)
    {
        // 按钮控制 m_currentIndex 最小为1 m_currentIndex最大为 m_texturePixVec.size() - 2
        painter.drawImage(m_leftPixmapArea, QImage::fromData(m_texturePixVec.at(m_currentIndex - 1)).scaled(m_leftPixmapArea.width(), m_leftPixmapArea.width()));
        painter.drawImage(m_rightPixmapArea, QImage::fromData(m_texturePixVec.at(m_currentIndex + 1)).scaled(m_rightPixmapArea.width(), m_rightPixmapArea.width()));
    }


    painter.drawImage(m_midPixmapArea, QImage::fromData(m_texturePixVec.at(m_currentIndex)).scaled(m_midPixmapArea.width(), m_midPixmapArea.width()));

}

void ScanTexturePicWidget::mousePressEvent(QMouseEvent *event)
{
    if (m_leftPixmapArea.contains(event->pos()))
    {
        m_leftPixmapArea.setRect(21.8, 0,66, 66);
        m_midPixmapArea.setRect(90.6, 13, 53, 53);
        m_rightPixmapArea.setRect(146.4, 13, 53, 53);
        update();
    }
    else if (m_midPixmapArea.contains(event->pos()))
    {
        m_midPixmapArea.setRect(77.6, 0, 66, 66);
        m_leftPixmapArea.setRect(21.8, 13, 53, 53);
        m_rightPixmapArea.setRect(144.6, 13, 53, 53);
        update();
    }
    else if (m_rightPixmapArea.contains(event->pos()))
    {
        m_rightPixmapArea.setRect(133.4, 0, 66, 66);
        m_midPixmapArea.setRect(77.6, 13, 53, 53);
        m_leftPixmapArea.setRect(21.8, 13, 53, 53);
        update();
    }
    else if (m_currentIndex > 1  && QRectF(0, 13, 19.8, 53).contains(event->pos()))
    {
        m_currentIndex--;
        update();
    }
    else if (m_currentIndex < m_texturePixVec.size() - 2 && QRectF(200.2, 13, 19.8, 53).contains(event->pos()))
    {
        m_currentIndex ++;
        update();
    }
}
