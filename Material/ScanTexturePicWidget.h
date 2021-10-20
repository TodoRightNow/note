#ifndef SCANTEXTUREPICWIDGET_H
#define SCANTEXTUREPICWIDGET_H

#include <QWidget>

class ScanTexturePicWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ScanTexturePicWidget(QWidget *parent = nullptr);
//    void setTexturePixVec(const QStringList &texturePixVec);
    void setTexturePixVec(const QVector<QByteArray>& imageDataVec);

signals:

public slots:

protected:
    void paintEvent(QPaintEvent*) override;
    void mousePressEvent(QMouseEvent* event) override;

private:
    QImage m_rightArrowPixmap{":/images/right-arrow.png"};
    QImage m_leftArrowPixmap{":/images/left-arrow.png"};

    QVector<QByteArray> m_texturePixVec;
    int m_currentIndex{0};

    QRectF m_leftPixmapArea;
    QRectF m_rightPixmapArea;
    QRectF m_midPixmapArea;
    QRectF& currentBigArea  = m_midPixmapArea;

};

#endif // SCANTEXTUREPICWIDGET_H
