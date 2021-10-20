#ifndef ASSETKINDWIDGET_H
#define ASSETKINDWIDGET_H

#include <QWidget>
class PopupWidget;

//  数字 + 具体资产名
class AssetKindWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AssetKindWidget(const QString topAssetName, const QString& num,const QString& assetName, QSize size, int digitalWidth,
                             const QStringList& childAssetNameList, bool isChecked, bool needArrowIcon, QWidget *parent = nullptr);
    void setIsChecked(bool flag);
    void setIsNeedArrowIcon(bool flag);
    void setAssetName(const QString& name);
    void setChildAssetNameList(const QStringList& names);
    QStringList getChildAssetNameList() const;

    QRect getArrowIconRect() const;
    QRect getAssetNameRect() const;

    bool operator ==(const AssetKindWidget&other);
    bool operator !=(const AssetKindWidget&other);

    QString getAssetName() const;

    bool getArrowIconShowStatus();
signals:
    void clicked(const QString& topAssetName, const QStringList& assetName, bool isTopLevel);
public slots:

protected:
    void paintEvent(QPaintEvent*) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void leaveEvent(QEvent *) override;
    void mousePressEvent(QMouseEvent*event) override;

private:
    enum class HoverRegion
    {
        UNKNOWN_REGION,
        ASSET_NAME_REGION,
        ARROW_ICON_REGION
    };

private:
    bool m_isChecked; // false
    bool m_needArrowIcon;
    HoverRegion m_hoverRegion;

    QString m_topAssetName; // Vray材质 获知corona材质
    int m_digitalWidth;

    QString m_curSelectAssetName;

    QString m_num;
    QString m_assetName;
    QStringList m_childAssetNameList;
    QPixmap m_pixmap;
};

#endif // ASSETKINDWIDGET_H
