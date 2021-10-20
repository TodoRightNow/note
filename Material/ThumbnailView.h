#ifndef THUMBNAILVIEW_H
#define THUMBNAILVIEW_H

#include <QWidget>

constexpr int ThumbnailViewItemWidth = 148;
constexpr int ThumbnailViewItemHeight = 182;

class ThumbnailView : public QWidget
{
    Q_OBJECT

public:
    explicit ThumbnailView(const QPixmap& ThumbnailPixmap, const QPixmap& starPixmap, const QString& name, QWidget *parent = nullptr, bool isShowDownloadText = true,
                           bool isChecked = false, bool isFillPolygon = false);


    void setIsShowDownloadText(bool flag);
    void setIsChecked(bool flag);
    void setIsFillPolygon(bool flag);
    void setPixmap(const QPixmap& pixmap);
    void setDeepDownloadBkgColor(bool flag);

public slots:

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

private:
    bool m_isShowDownloadText {true};
    bool m_isDeepDownloadBkgCol{false};
    bool m_isChecked {false};
    bool m_isFillPolygon {false};
    QPixmap m_pixmap; // ËØ²ÄËõÂÔÍ¼

    QString m_name;

    QRect m_downloadRect {12, 112, 124, 26};

    QPixmap m_starPixmap;

};

#endif // THUMBNAILVIEW_H
