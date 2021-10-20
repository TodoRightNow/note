#include "DownloadAsset.h"
#include <QNetworkAccessManager>
#include <QEventLoop>
#include <QTimer>
#include <QNetworkReply>
#include <QFile>
#include <QDir>
#include "AssetItem.h"
//#include "Crypto/Crypto.hpp"
//#include "Crypto/AESCrypto.h"
#include "CryptoRef.h"

QString DownloadAsset::m_prefixUrlPath = "http://14.215.91.29:58992/qipancloud/";
QVector<QString> DownloadAsset::m_penddingImage;

// TODO 先暂时 这么写 后面优化
DownloadAsset::DownloadAsset(const QString &remoteDir, const QString &fileName, const QString &fileSaveDir, QObject *parent)
    : QObject(parent)
    , m_fileSaveDir(fileSaveDir)
{
    QString partialUrlPath = m_prefixUrlPath + remoteDir;
    if (!partialUrlPath.endsWith('/'))
    {
        partialUrlPath += '/';
    }

    for (QString&fileSaveDir : m_fileSaveDir)
    {
        if (!fileSaveDir.endsWith('/'))
        {
            fileSaveDir += '/';
        }
    }


    if (!m_penddingImage.contains(partialUrlPath + fileName))
    {
        m_penddingImage.append(partialUrlPath + fileName);
        m_downloadFlag = true;
    }

    m_remoteDirFilePathMap.insert(partialUrlPath, QStringList{fileName});

    setAutoDelete(true);
}

DownloadAsset::DownloadAsset(const QString& remoteDir, const QStringList& fileNames, const QString& fileSaveDir, QObject *parent)
    : QObject (parent)
    , m_fileSaveDir(fileSaveDir)
{
    QString partialUrlPath = m_prefixUrlPath + remoteDir;
    if (!partialUrlPath.endsWith('/'))
    {
        partialUrlPath += '/';
    }

    for (QString&fileSaveDir : m_fileSaveDir)
    {
        if (!fileSaveDir.endsWith('/'))
        {
            fileSaveDir += '/';
        }
    }

    QStringList filterFileNames;
    for (auto fileName : fileNames)
    {
        if (!m_penddingImage.contains(partialUrlPath + fileName))
        {
            filterFileNames << fileName;
            m_penddingImage.append(partialUrlPath + fileName);
            m_downloadFlag = true;
        }
    }

    m_remoteDirFilePathMap.insert(partialUrlPath, filterFileNames);

    setAutoDelete(true);
}

DownloadAsset::DownloadAsset(const QString &remoteDir, const QString &fileName, AssetItem* item, QObject *parent)
    : QObject (parent)
    , m_assetItem(item)
{
    QString partialUrlPath = m_prefixUrlPath + remoteDir;
    if (!partialUrlPath.endsWith('/'))
    {
        partialUrlPath += '/';
    }

    if (!m_penddingImage.contains(partialUrlPath + fileName))
    {
        m_penddingImage.append(partialUrlPath + fileName);
        m_downloadFlag = true;
    }

    m_remoteDirFilePathMap.insert(partialUrlPath, QStringList{fileName});

    setAutoDelete(true);
}

DownloadAsset::DownloadAsset(const QString &remoteDir, const QStringList &fileNames, AssetItem *item, QObject *parent)
    : QObject (parent)
    , m_assetItem(item)
{
    QString partialUrlPath = m_prefixUrlPath + remoteDir;
    if (!partialUrlPath.endsWith('/'))
    {
        partialUrlPath += '/';
    }

    QStringList filterFileNames;
    for (auto fileName : fileNames)
    {
        if (!m_penddingImage.contains(partialUrlPath + fileName))
        {
            filterFileNames << fileName;
            m_penddingImage.append(partialUrlPath + fileName);
            m_downloadFlag = true;
        }
    }

    m_remoteDirFilePathMap.insert(partialUrlPath, filterFileNames);

    setAutoDelete(true);
}

DownloadAsset::DownloadAsset(const QString &remoteDir, const QString &fileName,const QString& fileSaveDir, bool needCrypt, QObject *parent)
    : QObject(parent)
    , m_fileSaveDir(fileSaveDir)
    , m_needCrypt(needCrypt)
{
    QString partialUrlPath = m_prefixUrlPath + remoteDir;
    if (!partialUrlPath.endsWith('/'))
    {
        partialUrlPath += '/';
    }

    for (QString&fileSaveDir : m_fileSaveDir)
    {
        if (!fileSaveDir.endsWith('/'))
        {
            fileSaveDir += '/';
        }
    }


    if (!m_penddingImage.contains(partialUrlPath + fileName))
    {
        m_penddingImage.append(partialUrlPath + fileName);
        m_downloadFlag = true;
    }

    m_remoteDirFilePathMap.insert(partialUrlPath, QStringList{fileName});

    setAutoDelete(true);
}

DownloadAsset::DownloadAsset(const QMap<QString, QStringList> remoteDirFilePathMap, const QString &fileSaveDir, QObject *parent)
    : QObject (parent)
    , m_fileSaveDir(fileSaveDir)

{
    QMap<QString, QStringList>::const_iterator iter = remoteDirFilePathMap.constBegin();
    m_remoteDirFilePathMap.clear();

    for(auto penddingImage: m_penddingImage)
    {
        qDebug() << "penddingImage: " <<penddingImage;
    }

    while (iter != remoteDirFilePathMap.constEnd())
    {
        QString partialUrlPath = m_prefixUrlPath + iter.key();
        if (!partialUrlPath.endsWith('/'))
        {
            partialUrlPath += '/';
        }

        QStringList filterFileNames;
        for (auto fileName : iter.value())
        {
            if (!m_penddingImage.contains(partialUrlPath + fileName))
            {
                qDebug() << "partialUrlPath + fileName: " <<partialUrlPath + fileName;
                filterFileNames << fileName;
                m_penddingImage.append(partialUrlPath + fileName);
                m_downloadFlag = true;
            }
        }
        if (filterFileNames.size() > 0)
        {
            m_remoteDirFilePathMap.insert(partialUrlPath, filterFileNames);
        }
        ++iter;
    }

    for (QString&fileSaveDir : m_fileSaveDir)
    {
        if (!fileSaveDir.endsWith('/'))
        {
            fileSaveDir += '/';
        }
    }

    setAutoDelete(true);
}

DownloadAsset::DownloadAsset(const QMap<QString, QStringList> remoteDirFilePathMap, const QStringList &fileSaveDir, QObject *parent)
    : QObject (parent)
    , m_fileSaveDir(fileSaveDir)
{
    QMap<QString, QStringList>::const_iterator iter = remoteDirFilePathMap.constBegin();
    m_remoteDirFilePathMap.clear();

    for(auto penddingImage: m_penddingImage)
    {
        qDebug() << "penddingImage: " <<penddingImage;
    }

    while (iter != remoteDirFilePathMap.constEnd())
    {
        QString partialUrlPath = m_prefixUrlPath + iter.key();
        if (!partialUrlPath.endsWith('/'))
        {
            partialUrlPath += '/';
        }

        QStringList filterFileNames;
        for (auto fileName : iter.value())
        {
            if (!m_penddingImage.contains(partialUrlPath + fileName))
            {
                qDebug() << "partialUrlPath + fileName: " <<partialUrlPath + fileName;
                filterFileNames << fileName;
                m_penddingImage.append(partialUrlPath + fileName);
                m_downloadFlag = true;
            }
        }
        if (filterFileNames.size() > 0)
        {
            m_remoteDirFilePathMap.insert(partialUrlPath, filterFileNames);
        }
        ++iter;
    }

    for (QString&fileSaveDir : m_fileSaveDir)
    {
        if (!fileSaveDir.endsWith('/'))
        {
            fileSaveDir += '/';
        }
    }

    setAutoDelete(true);
}

DownloadAsset::DownloadAsset(const QMap<QString, QStringList> remoteDirFilePathMap, const QStringList &fileSaveDir, const QString &matAliasName, QObject *parent)
    : QObject (parent)
    , m_fileSaveDir(fileSaveDir)
    , m_matAliasName(matAliasName)
{
    QMap<QString, QStringList>::const_iterator iter = remoteDirFilePathMap.constBegin();
    m_remoteDirFilePathMap.clear();

    for(auto penddingImage: m_penddingImage)
    {
        qDebug() << "penddingImage: " <<penddingImage;
    }

    while (iter != remoteDirFilePathMap.constEnd())
    {
        QString partialUrlPath = m_prefixUrlPath + iter.key();
        if (!partialUrlPath.endsWith('/'))
        {
            partialUrlPath += '/';
        }

        QStringList filterFileNames;
        for (auto fileName : iter.value())
        {
            if (!m_penddingImage.contains(partialUrlPath + fileName))
            {
                qDebug() << "partialUrlPath + fileName: " <<partialUrlPath + fileName;
                filterFileNames << fileName;
                m_penddingImage.append(partialUrlPath + fileName);
                m_downloadFlag = true;
            }
        }
        if (filterFileNames.size() > 0)
        {
            m_remoteDirFilePathMap.insert(partialUrlPath, filterFileNames);
        }
        ++iter;
    }

    for (QString&fileSaveDir : m_fileSaveDir)
    {
        if (!fileSaveDir.endsWith('/'))
        {
            fileSaveDir += '/';
        }
    }

    setAutoDelete(true);
}


void DownloadAsset::run()
{
    if (!m_downloadFlag)
    {
        return;
    }
    QNetworkAccessManager networkAccessManager;

    QMap<QString, QStringList>::const_iterator iter = m_remoteDirFilePathMap.constBegin();

    int i = 0;
    //    bool sig_flag = false;
    while (iter != m_remoteDirFilePathMap.constEnd())
    {
        QStringList fileNames = iter.value();
        QString partialUrlPath = iter.key();
        for (auto fileName : fileNames)
        {
            QNetworkReply *reply = networkAccessManager.get(QNetworkRequest(QUrl(partialUrlPath + fileName)));

            qDebug() << "partialUrlPath + fileName: " << partialUrlPath + fileName;
            QEventLoop loop;
            QTimer timer;
            connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
            connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit); // 15秒超时，避免卡主线程
            connect(reply, &QNetworkReply::finished, &timer, &QTimer::stop);
            connect(reply, &QNetworkReply::downloadProgress, this, &DownloadAsset::onDownloadProgress);
            timer.start(1000 * 15);
            loop.exec();

            bool flag = false;
            if (!m_matAliasName.isEmpty() && fileName.endsWith("mat"))
            {
                flag = onDownloadFinished(reply, partialUrlPath, m_matAliasName, i);
            }
            else
            {
                flag = onDownloadFinished(reply, partialUrlPath, fileName, i);
            }
            if (flag)
            {
                emit downloadSingleFinished();
                m_penddingImage.removeOne(partialUrlPath + fileName);
            }
        }
        ++iter;
        ++i;
    }
    emit downloadAllFinished();

}

void DownloadAsset::onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    double percentage = ((double)bytesReceived / bytesTotal );
    emit downloadSingleProgress(percentage);
}
bool DownloadAsset::onDownloadFinished(QNetworkReply *reply, const QString &partialUrlPath, const QString &fileName, int index)
{
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    bool flag = false;

    qDebug() << "statusCode: " <<statusCode;

    if (statusCode == 200)
    {
        if (m_assetItem)
        {
            emit sendImageData(reply->readAll());
            flag = true;
        }
        else
        {
            qDebug() << "m_fileSaveDir.at(index) + fileName: " << m_fileSaveDir.at(index) + fileName;
            QFile file(m_fileSaveDir.at(index) + fileName);
            if (file.open(QIODevice::WriteOnly))
            {
                if (m_needCrypt)
                {
                    gCrypto.setKey("QJCDCOMRAYVISION");
                    gCrypto.setIV("A1E2F3C456789DB9");
                    file.write(gCrypto.encode(reply->readAll()));
                }
                else
                {
                    file.write(reply->readAll());
                }
                file.close();
                flag = true;
            }
        }
    }
    else
    {
        QString strError = reply->errorString();
        qDebug() << "download file error from " << partialUrlPath + fileName << "   error string:" << reply->errorString();
    }

    reply->abort();
    reply->deleteLater();
    return flag;
}
