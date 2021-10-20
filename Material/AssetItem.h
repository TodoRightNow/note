#ifndef ASSETITEM_H
#define ASSETITEM_H

#include <QStringList>
#include <QMetaType>
#include <QPixmap>

struct AssetItem
{
    bool m_isShowDownloadText {true};
    bool m_isDeepDownloadBkgCol{false};
    bool m_isChecked {false};
    bool m_isFillPolygon {false};

    QStringList m_downLoadPaths;
    QString m_matFilePath;
    QString m_matFile_thumbnail;
    QString m_name; // 防止溢出
    QString m_realName; // 一般用不到 如果m_name是elideText 则 m_realName 作为tooltip显示
    QString m_rt;
    QStringList m_textureMap_thumbnails;
    QString m_maxVer;
    QString m_renderers;
    QString m_size;
    QString m_displayText {"下载"};
    double m_percentage{0.0};

    QVector<QByteArray> m_textureThumbImages;
    bool m_hasInsert {false};

    QByteArray m_imageData;

    QString m_thumbnailDir;
    QString m_remoteDir;
    QString m_localSaveDir;

    bool operator==(const AssetItem& item)
    {
        if (item.m_name == this->m_name)
        {
            return true;
        }
        return false;
    }

};
Q_DECLARE_METATYPE(AssetItem*);
Q_DECLARE_METATYPE(AssetItem);

#endif // ASSETITEM_H
