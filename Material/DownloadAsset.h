#ifndef DOWNLOADASSET_H
#define DOWNLOADASSET_H

#include <QObject>
#include <QRunnable>
#include <QMap>

class QNetworkAccessManager;
class QNetworkReply;
class AssetItem;

// 下载资产任务
class DownloadAsset : public QObject, public QRunnable
{
    Q_OBJECT
public:
    // remote 指的放在  14.215.91.29:58992/qipancloud/ 中 qipancloud 文件夹下的目录
    explicit DownloadAsset(const QString& remoteDir, const QString& fileName, AssetItem* item, QObject *parent = nullptr);
    explicit DownloadAsset(const QString& remoteDir, const QStringList& fileNames, AssetItem* item, QObject *parent = nullptr);
    explicit DownloadAsset(const QString& remoteDir, const QString& fileName, const QString &fileSaveDir, bool needCrypt, QObject *parent = nullptr);
    explicit DownloadAsset(const QString& remoteDir, const QString& fileName, const QString& fileSaveDir,  QObject *parent = nullptr);
    explicit DownloadAsset(const QString& remoteDir, const QStringList& fileNames, const QString& fileSaveDir, QObject *parent = nullptr);
    explicit DownloadAsset(const  QMap<QString, QStringList> remoteDirFilePathMap, const QString& fileSaveDir, QObject *parent = nullptr);
    explicit DownloadAsset(const  QMap<QString, QStringList> remoteDirFilePathMap, const QStringList& fileSaveDir, QObject *parent = nullptr);
    explicit DownloadAsset(const  QMap<QString, QStringList> remoteDirFilePathMap, const QStringList& fileSaveDir, const QString& matAliasName, QObject *parent = nullptr);

    void run() override;

signals:
    void sendImageData(const QByteArray& imageData);
    void downloadAllFinished();
    void downloadSingleFinished();
    void downloadSingleProgress(double percentage);
public slots:

    void onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);
private:
    bool onDownloadFinished(QNetworkReply *reply,const QString& partialUrlPath, const QString& filePath, int index);


private:
    QStringList m_fileSaveDir;

    bool m_downloadFlag {false};

    //    bool m_needSave;

    AssetItem* m_assetItem{nullptr};

    QMap<QString, QStringList> m_remoteDirFilePathMap;

    bool m_needCrypt {false};

    QString m_matAliasName{""};

    static QString m_prefixUrlPath;

    static QVector<QString> m_penddingImage;
};

#endif // DOWNLOADASSET_H
