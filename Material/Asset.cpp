#include "Asset.h"
#include "ui_MainWindow.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QPushButton>
#include "AssetKindWidget.h"
#include "Base/FlowLayout.h"
#include "ThumbnailView.h"
#include "AssetKindCombobox.h"
#include <QScrollBar>
#include "ThumbnailModel.h"
#include "ThumbnailDelegate.h"
#include <QThreadPool>
#include "QToolButton"
#include "UserProfile/LocalSetting.h"
#include <gdiplus.h>
#include <combaseapi.h>
#include <QtWin>
#include <wingdi.h>
#include "DownloadAsset.h"
#include "UI/SearchBox.h"
#include "QToolTipper.h"
#include "CryptoRef.h"

#pragma comment (lib,"Gdiplus.lib")
#pragma comment (lib,"Gdi32.lib")


QString Asset::prefix_dir = "";
Asset::Asset(Ui::CMainWindow *ui)
    : m_detailUi(ui)
{
    m_assetItemVec.reserve(55);
}

Asset::~Asset()
{
    for (auto assetItemThirdLevelInfo : m_assetItemThirdLevelInfo)
    {
        delete assetItemThirdLevelInfo;
        assetItemThirdLevelInfo = nullptr;
    }
}

void Asset::setupSecretAssetKindWidget(QWidget* secretAssetKindWidget)
{
    QVBoxLayout* lowLevel_vBoxlayout = new QVBoxLayout();
    lowLevel_vBoxlayout->setContentsMargins(0, 0, 0, 0);
    int index = -1;
    for (auto assetItemThirdLevelInfo : m_assetItemThirdLevelInfo)
    {
        index ++;
        QWidget* kindWidget = new QWidget(secretAssetKindWidget);
        kindWidget->setAttribute(Qt::WA_TranslucentBackground);

        QFont font;
        font.setPixelSize(12);
        QFontMetrics metrics(font);
        int text_width = metrics.width(m_maxStr);

        bool isChecked = false;
        FlowLayout* flowLayout = nullptr;
        if (index == (m_assetItemThirdLevelInfo.size() - 1))
        {
            if (m_assetKindName == "材质类")
            {
                flowLayout = new FlowLayout(nullptr, 0, 9, 9, 9, 10, 10);
            }
            else
            {
                flowLayout = new FlowLayout(nullptr, 20, 9, 9, 9, 10, 10);
            }

        }
        else
        {
            flowLayout = new FlowLayout(nullptr, 0, 9, 9, 0, 10, 10);
        }

        if (index == 0)
        {
            isChecked = true;
        }

        QString totalCountStr = QString::number(assetItemThirdLevelInfo->m_totalCount);
        int digital_area_width = 30;
        if (totalCountStr.length() == 4)
        {
            digital_area_width = 40;
        }
        else if (totalCountStr.length() == 5)
        {
            digital_area_width = 50;
        }
        else if (totalCountStr.length() == 6)
        {
            digital_area_width = 60;
        }

        AssetKindWidget* assetKindWidget = new AssetKindWidget(assetItemThirdLevelInfo->m_name, totalCountStr, "全部",
                                                               QSize(text_width + 38 + digital_area_width, 22), digital_area_width, QStringList(), isChecked, false, nullptr);
        AssetKindCombobox* combobox = new AssetKindCombobox(assetKindWidget, kindWidget);
        connect(combobox, &AssetKindCombobox::clicked, this, &Asset::onClicked);
        combobox->resize(QSize(text_width + 38 + digital_area_width, 22));

        if (isChecked)
        {
            m_curSelectedCombobox = combobox;
            m_curSelectAssetKindName = combobox->getAssetKindWidget()->getAssetName();
        }

        flowLayout->addWidget(combobox);
        for (auto secondAssetItemThirdLevelInfo : assetItemThirdLevelInfo->m_child)
        {
            bool needShowArrow = false;
            if (secondAssetItemThirdLevelInfo->m_child.size() > 0)
            {
                needShowArrow = true;
            }

            QStringList childAssetNames;
            for (auto child : secondAssetItemThirdLevelInfo->m_child)
            {
                childAssetNames << child->m_name;
            }

            qDebug() << "assetItemThirdLevelInfo->m_name:  " << assetItemThirdLevelInfo->m_name <<  "secondAssetItemThirdLevelInfo->m_name:  " <<secondAssetItemThirdLevelInfo->m_name;
            qDebug() << "maxStr:" << m_maxStr;
            AssetKindWidget* concret_assetKindWidget = new AssetKindWidget(assetItemThirdLevelInfo->m_name, QString::number(secondAssetItemThirdLevelInfo->m_totalCount),
                                                                           secondAssetItemThirdLevelInfo->m_name, QSize(text_width + 38 + digital_area_width, 22), digital_area_width, childAssetNames,
                                                                           false, needShowArrow, nullptr);

            AssetKindCombobox* concret_combobox = new AssetKindCombobox(concret_assetKindWidget, kindWidget);

            connect(concret_combobox, &AssetKindCombobox::clicked, this, &Asset::onClicked);

            concret_combobox->resize(QSize(text_width + 38 + digital_area_width, 22));
            flowLayout->addWidget(concret_combobox);
        }

        kindWidget->setLayout(flowLayout);

        QHBoxLayout* hboxLayout = new QHBoxLayout();
        hboxLayout->setContentsMargins(0, 0, 0, 0);
        hboxLayout->setSpacing(15);
        hboxLayout->addWidget(kindWidget);

        lowLevel_vBoxlayout->addLayout(hboxLayout);
    }

    secretAssetKindWidget->setLayout(lowLevel_vBoxlayout);
    secretAssetKindWidget->setStyleSheet(QString("QWidget#%1 {background: #232531; border-radius: 4px;}").arg(secretAssetKindWidget->objectName()));
}

void Asset::setupFilterWidget(QWidget* secretFilterWidget)
{
    QLabel* label = new QLabel(secretFilterWidget);
    label->setFixedSize(QSize(44, 25));
    label->setText("筛选:");
    label->setStyleSheet("color: #BDBDBD; font-size: 12px;");
    QStringList buttonNames;
    buttonNames <<"全部" << "免费" << "最新";
    secretFilterWidget->layout()->addWidget(label);
    for (auto buttonName : buttonNames)
    {
        QPushButton* button = new QPushButton(secretFilterWidget);
        button->setFixedSize(QSize(44,25));
        button->setCheckable(true);
        if (buttonName == "全部")
        {
            button->setChecked(true);
            m_curFilterCheckedBtn = button;
        }

        button->setText(buttonName);
        button->setFlat(true);

        connect(button, &QPushButton::clicked, [this, button]{
            m_curFilterCheckedBtn->setChecked(false);
            button->setChecked(true);
            m_curFilterCheckedBtn = button;
        });

        button->setStyleSheet(R"(
                              QPushButton{border: none; border-radius: 4px; color: #BDBDBD;background:transparent;}
                              QPushButton:hover {color: #ffffff; background-color:#4786F4;}
                              QPushButton:checked{background-color:#226AE9; color: #ffffff;}
                              QPushButton:unchecked{background: transparent; color:#BDBDBD; }
                              QPushButton:pressed {background-color:#226AE9;  color: #ffffff;}
                              QPushButton:default:disabled {background-color:#3c7bea;border-color:#3c7bea}
                              )");
        secretFilterWidget->layout()->addWidget(button);
    }

    //    QWidget* widget = new QWidget(secretFilterWidget);
    //    widget->setFixedSize(1, 14);
    //    widget->setStyleSheet("background: #525266;");
    //    secretFilterWidget->layout()->addWidget(widget);

    //    QToolButton* toolButton = new QToolButton(secretFilterWidget);
    //    toolButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    //    toolButton->setCheckable(true);
    //    toolButton->setIcon(QIcon(":/images/myCollect.png"));
    //    toolButton->setText("我的收藏");
    //    toolButton->setFixedSize(QSize(74,25));
    //    toolButton->setStyleSheet(R"(
    //                              QToolButton{border: none; border-radius: 4px; font-size: 12px; color: #BDBDBD;background:transparent; text-align:center}
    //                              QToolButton:hover {color: #ffffff; background-color:#4786F4;}
    //                              QToolButton:checked{background-color:#226AE9; color: #ffffff;}
    //                              QToolButton:unchecked{background: transparent; color:#BDBDBD; }
    //                              QToolButton:pressed {background-color:#226AE9;  color: #ffffff;}
    //                              QToolButton:default:disabled {background-color:#3c7bea;border-color:#3c7bea}
    //                              )");



    //    secretFilterWidget->layout()->addWidget(toolButton);

    //    QWidget* widget2 = new QWidget(secretFilterWidget);
    //    widget2->setFixedSize(1, 14);
    //    widget2->setStyleSheet("background: #525266;");
    //    secretFilterWidget->layout()->addWidget(widget2);

    //    QToolButton* toolButton2 = new QToolButton(secretFilterWidget);
    //    toolButton2->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    //    toolButton2->setIcon(QIcon(":/images/myDownload.png"));
    //    toolButton2->setText("我的下载");
    //    toolButton2->setFixedSize(QSize(74,25));
    //    toolButton2->setCheckable(true);
    //    toolButton2->setStyleSheet(R"(
    //                               QToolButton{border: none; border-radius: 4px; font-size: 12px; color: #BDBDBD;background:transparent; text-align:center}
    //                               QToolButton:hover {color: #ffffff; background-color:#4786F4;}
    //                               QToolButton:checked{background-color:#226AE9; color: #ffffff;}
    //                               QToolButton:unchecked{background: transparent; color:#BDBDBD; }
    //                               QToolButton:pressed {background-color:#226AE9;  color: #ffffff;}
    //                               QToolButton:default:disabled {background-color:#3c7bea;border-color:#3c7bea}
    //                               )");

    //    secretFilterWidget->layout()->addWidget(toolButton2);

    static_cast<QHBoxLayout*>(secretFilterWidget->layout())->addStretch();
}

AssetItemThirdLevelInfo* Asset::findAssetItemThirdLevelInfo(const QString &topAssetName, const QStringList &assetNames, bool isToplevel)
{
    for (auto assetItemThirdLevelInfo : m_assetItemThirdLevelInfo)
    {
        if (assetItemThirdLevelInfo->m_name == topAssetName)
        {
            if (assetNames.at(0) == "全部")
            {
                return assetItemThirdLevelInfo;
            }

            if (isToplevel)
            {
                for (auto secondAssetItemThirdLevelInfo : assetItemThirdLevelInfo->m_child)
                {
                    if (secondAssetItemThirdLevelInfo->m_name == assetNames.at(0))
                    {
                        return secondAssetItemThirdLevelInfo;
                    }
                }
            }
            else
            {
                for (auto secondAssetItemThirdLevelInfo : assetItemThirdLevelInfo->m_child)
                {
                    if (secondAssetItemThirdLevelInfo->m_name == assetNames.at(0))
                    {
                        for (auto thirdAssetItemThirdLevelInfo : secondAssetItemThirdLevelInfo->m_child)
                        {
                            if (thirdAssetItemThirdLevelInfo->m_name == assetNames.at(1))
                            {
                                return thirdAssetItemThirdLevelInfo;
                            }
                        }
                    }
                }
            }
        }
    }
    return nullptr;
}

bool Asset::isHasLoadJsonFile() const
{
    return m_isHasLoadJsonFile;
}

void Asset::setIsHasLoadJsonFile(bool flag)
{
    m_isHasLoadJsonFile = flag;
}

void Asset::appendToCache(AssetItem *item)
{
    if (m_assetItemVec.size() == 55)
    {
        AssetItem *item = m_assetItemVec.first();
        item->m_imageData = QByteArray();
        item->m_textureThumbImages.clear();
        item->m_hasInsert = false;
        m_assetItemVec.removeFirst();
        qDebug() << "remove item:" << item->m_name << " from cache";
    }
    m_assetItemVec.append(item);
}

QString Asset::getAssetKindName()
{
    return m_assetKindName;
}

void Asset::setupThumbnailViewWidget(CustomTableView* secretTableView, bool isSupportDrag)
{
    Q_ASSERT(m_assetItemThirdLevelInfo.size() > 0);

    int col = qMax(secretTableView->sizeHint().width() / 159, 1);
    Q_ASSERT(col > 0);

    ThumbnailModel* model = new ThumbnailModel(col, isSupportDrag, secretTableView);
    model->setAssetItemVec(m_assetItemThirdLevelInfo.at(0)->m_assetItems);

    ThumbnailDelegate* delegate = nullptr;
    if (m_assetKindName == "贴图类")
    {
        delegate = new ThumbnailDelegate(secretTableView, secretTableView);
    }
    else
    {
        delegate = new ThumbnailDelegate(secretTableView, this, secretTableView);
    }

    connect(delegate, &ThumbnailDelegate::startDownload, this, &Asset::onstartDownload);

    //     动态转换
    MaterialAsset* materialAsset = dynamic_cast<MaterialAsset*>(this);
    if (materialAsset)
    {
        connect(delegate, &ThumbnailDelegate::modelDataSelected, materialAsset, &MaterialAsset::onModelDataSelected);

    }
    else
    {
        LightAsset* lightAsset = dynamic_cast<LightAsset*>(this);
        if (lightAsset)
        {
            connect(delegate, &ThumbnailDelegate::modelDataSelected, lightAsset, &LightAsset::onModelDataSelected);
        }
    }

    secretTableView->setShowGrid(false);
    secretTableView->setModel(model);
    secretTableView->setItemDelegate(delegate);

    secretTableView->horizontalHeader()->hide();
    secretTableView->verticalHeader()->hide();
    secretTableView->verticalHeader()->setDefaultSectionSize(194);
    secretTableView->horizontalHeader()->setDefaultSectionSize(160);

}

void Asset::onClicked(const QString& topAssetName, const QStringList& assetNames, bool isTopLevel)
{
    AssetKindCombobox* box = qobject_cast<AssetKindCombobox*>(sender());

    if (m_detailUi->material_detail_widget->isVisible())
    {
        m_detailUi->material_detail_widget->hide();
    }
    if (topAssetName != "" && (isTopLevel || m_curSelectAssetKindName != assetNames.last()))
    {
        CustomTableView* tableView = nullptr;
        if (m_assetKindName == "材质类")
        {
            tableView = m_detailUi->tableView;
        }
        else if (m_assetKindName == "贴图类")
        {
            tableView = m_detailUi->texture_tableView;
        }
        else if (m_assetKindName == "灯光类")
        {
            tableView = m_detailUi->light_tableView;
        }

        ThumbnailModel* model = static_cast<ThumbnailModel*>(tableView->model());

        AssetItemThirdLevelInfo* assetItemThirdLevelInfo =  findAssetItemThirdLevelInfo(topAssetName, assetNames, isTopLevel);
        if (assetItemThirdLevelInfo == nullptr)
        {
            qDebug() << "assetItemThirdLevelInfo is nullptr";
            return;
        }
        qDebug() << "assetItemThirdLevelInfo->m_assetItems size: " << assetItemThirdLevelInfo->m_assetItems.size();
        model->setAssetItemVec(assetItemThirdLevelInfo->m_assetItems);

        m_curSelectedCombobox->getAssetKindWidget()->setIsChecked(false);
        box->getAssetKindWidget()->setIsChecked(true);
        m_curSelectedCombobox = box;
    }
    else
    {
        if (m_curSelectedCombobox != box)
        {
            m_curSelectedCombobox->hideStatusNotBySelf();
        }
        box->onClicked();
    }
}


void Asset::onstartDownload(AssetItem* item)
{
    QStringList matMapFiles;
    QString matFile;
    QString matMapFiles_remoteDir;
    QString matFile_remoteDir{""};
    QDir dir;
    int needDownloadFileCount = 0;
    CustomTableView* tableView;
    qDebug() << "m_assetKindName:  " <<m_assetKindName;
    if (m_assetKindName == "材质类")
    {
        for (auto downLoadPath : item->m_downLoadPaths)
        {
            qDebug() << "item->m_downLoadPaths: "  <<downLoadPath << "    downLoadPath.startsWith(other)" <<downLoadPath.startsWith("other");
            if (downLoadPath.startsWith("other"))
            {
                if (matMapFiles_remoteDir.isEmpty())
                {
                    matMapFiles_remoteDir = downLoadPath.left(downLoadPath.lastIndexOf('/'));
                    dir.mkpath(prefix_dir + matMapFiles_remoteDir);
                }
                QFile file(prefix_dir + downLoadPath);
                if (!file.exists())
                {
                    matMapFiles << downLoadPath.right(downLoadPath.length() - downLoadPath.lastIndexOf('/') - 1);
                    needDownloadFileCount++;
                }
            }
            else
            {

                matFile = downLoadPath.right(downLoadPath.length() - downLoadPath.lastIndexOf('/') - 1);
                matFile_remoteDir = downLoadPath.left(downLoadPath.lastIndexOf('/'));
                needDownloadFileCount ++;
                qDebug() << "mat file: " <<matFile << "     matFile_remoteDir:" <<matFile_remoteDir;
            }
        }

        tableView = m_detailUi->tableView;
    }
    else
    {
        // 灯光
        dir.mkpath(item->m_localSaveDir);
        matMapFiles_remoteDir = item->m_downLoadPaths.at(0).left(item->m_downLoadPaths.at(0).lastIndexOf('/'));
        for (auto downLoadPath : item->m_downLoadPaths)
        {
            QFile file(prefix_dir + downLoadPath);
            if (!file.exists())
            {
                matMapFiles << downLoadPath.right(downLoadPath.length() - downLoadPath.lastIndexOf('/') - 1);
                needDownloadFileCount++;
            }
        }
        tableView = m_detailUi->light_tableView;
    }

    // TODO 多个文件夹 下 多个文件的同时下载
    DownloadAsset* downloadAsset = nullptr;

    if (matMapFiles.size())
    {
        QMap<QString, QStringList> remoteDirFilePathMap;
        remoteDirFilePathMap.insert(matMapFiles_remoteDir, matMapFiles);

        if (!matFile_remoteDir.isEmpty())
        {
            qDebug() << "item->m_matFilePath:  " << item->m_matFilePath;
            dir.mkpath(prefix_dir + matFile_remoteDir.left(matFile_remoteDir.indexOf('/')));
            remoteDirFilePathMap.insert(matFile_remoteDir, QStringList{matFile});
            downloadAsset = new DownloadAsset(remoteDirFilePathMap,QStringList() << (prefix_dir + matMapFiles_remoteDir) << (prefix_dir + matFile_remoteDir.left(matFile_remoteDir.indexOf('/'))),
                                              item->m_matFilePath.right(item->m_matFilePath.length() - item->m_matFilePath.lastIndexOf('/') - 1));
        }
        else
        {
            downloadAsset = new DownloadAsset(remoteDirFilePathMap,QStringList() << prefix_dir + matMapFiles_remoteDir);
        }

    }
    else
    {
        if (!matFile_remoteDir.isEmpty())
        {
            downloadAsset = new DownloadAsset(matFile_remoteDir, matFile, prefix_dir + matFile_remoteDir.left(matFile_remoteDir.indexOf('/')));
        }
    }


    m_downloadAssetItem = item;
    m_fileIndex = 0;
    if (downloadAsset)
    {
        connect(downloadAsset, &DownloadAsset::downloadSingleProgress, [this, needDownloadFileCount, tableView](double percentage){
            ThumbnailModel* model = static_cast<ThumbnailModel*>(tableView->model());
            QModelIndex index = model->getIndex(m_downloadAssetItem);

            m_downloadAssetItem->m_percentage = percentage/needDownloadFileCount + (double)m_fileIndex/needDownloadFileCount;
            qDebug() << "real percentage:   " <<  m_downloadAssetItem->m_percentage ;
            m_downloadAssetItem->m_displayText = QString("%1%").arg(static_cast<int>( m_downloadAssetItem->m_percentage *100));
            if (percentage == 1.0)
            {
                m_fileIndex++;
            }

            tableView->model()->setData(index, QVariant::fromValue(m_downloadAssetItem));
            emit model->dataChanged(index, index);
        });

        connect(downloadAsset, &DownloadAsset::downloadAllFinished, [this, tableView, item]{
            ThumbnailModel* model = static_cast<ThumbnailModel*>(tableView->model());
            QModelIndex index = model->getIndex(m_downloadAssetItem);
            m_downloadAssetItem->m_isShowDownloadText = false;
            tableView->model()->setData(index, QVariant::fromValue(m_downloadAssetItem));

            emit model->dataChanged(index, index);

            QMetaObject::invokeMethod(this, "onDownloadBtnDisappeared", Qt::QueuedConnection, Q_ARG(AssetItem*, item));
        });
    }
    else
    {
        ThumbnailModel* model = static_cast<ThumbnailModel*>(tableView->model());
        QModelIndex index = model->getIndex(m_downloadAssetItem);
        m_downloadAssetItem->m_isShowDownloadText = false;
        tableView->model()->setData(index, QVariant::fromValue(m_downloadAssetItem));
        emit model->dataChanged(index, index);
        QMetaObject::invokeMethod(this, "onDownloadBtnDisappeared", Qt::AutoConnection, Q_ARG(AssetItem*, item));
    }

    QThreadPool::globalInstance()->start(downloadAsset);
}

void MaterialAsset::onModelDataSelected(AssetItem* item)
{
    // todo :优化
    ThumbnailModel* model = static_cast<ThumbnailModel*>(m_detailUi->tableView->model());
    QModelIndex index = model->getIndex(m_curSelectAssetItem);
    if (index.isValid())
    {
        m_curSelectAssetItem->m_isChecked = false;
        m_detailUi->tableView->model()->setData(index, QVariant::fromValue(m_curSelectAssetItem));
        emit model->dataChanged(index, index);
    }
    m_curSelectAssetItem = item;

    m_detailUi->asset_name_label->setText(item->m_name);
    if (item->m_isFillPolygon)
    {
        m_detailUi->collect_icon_label->setPixmap(QPixmap(":/images/collected.png"));
    }
    else
    {
        m_detailUi->collect_icon_label->setPixmap(QPixmap(":/images/uncollect.png"));
    }

    QPixmap pixmap;
    pixmap.loadFromData(item->m_imageData);
    m_detailUi->asset_pic_label->setPixmap(pixmap.scaled(220, 220));
    m_detailUi->render_name_label->setText(item->m_renderers);
    m_detailUi->support_ver_label->setText(item->m_maxVer);
    m_detailUi->file_size_label->setText(item->m_size);

    QFile file;
    QStringList downloadFiles;
    bool has_download_flag = false;
    if (!item->m_isShowDownloadText)
    {
        for (auto downloadFile : item->m_downLoadPaths)
        {
            if (downloadFile.endsWith(".mat"))
            {
                file.setFileName(item->m_matFilePath);
                if (file.exists())
                {
                    qDebug() << " file.exists ";
                    has_download_flag = true;
                }
                break;
            }
        }
    }

    if (has_download_flag)
    {
        if (!item->m_hasInsert)
        {
            QStringList textureMaps;
            for (auto textureMap : item->m_textureMap_thumbnails)
            {
                textureMaps << textureMap.right(textureMap.length() - textureMap.lastIndexOf('/') - 1);
            }
            DownloadAsset* downloadAsset = new DownloadAsset(item->m_thumbnailDir, textureMaps, item);
            connect(downloadAsset, &DownloadAsset::sendImageData, [item](const QByteArray& imageData){
                qDebug() << "OK  receive sendImageData";
                item->m_textureThumbImages.push_back(imageData);
            });
            connect(downloadAsset, &DownloadAsset::downloadAllFinished, [this, item] {
                item->m_textureThumbImages.insert(0, item->m_imageData);
                item->m_hasInsert = true;
                QMetaObject::invokeMethod(this, "onDownloadAllFinished", Qt::QueuedConnection, Q_ARG(AssetItem*, item));
            });
            item->m_textureThumbImages.clear();
            QThreadPool::globalInstance()->start(downloadAsset);
        }
        else
        {
            onDownloadAllFinished(item);
        }
    }
    else
    {
        m_detailUi->displayTextureWidget->hide();
    }
    m_detailUi->material_detail_widget->setMaximumWidth(300);
    m_detailUi->main_material_detail_widget->show();
    m_detailUi->material_detail_widget->show();
}

void MaterialAsset::onDownloadAllFinished(AssetItem* item)
{
    qDebug() << "MaterialAsset::onDownloadAllFinished";
    m_detailUi->scan_pic_widget->setTexturePixVec(item->m_textureThumbImages);
    m_detailUi->displayTextureWidget->show();
}

MaterialAsset::MaterialAsset(Ui::CMainWindow *ui)
    : Asset(ui)
{
    m_assetKindName = "材质类";
    qRegisterMetaType<QVector<int>>("QVector<int>");
}

// TODO  区分 错误类型  有1.文件不存在 2.json 格式不对 这两种错误  (暂时先 基本是文件不存在的问题)
bool MaterialAsset::parseFileJosn(const QString &jsonPath)
{
    QFile file(jsonPath);
    bool succeed = file.open(QIODevice::ReadOnly | QIODevice::Text);
    if (!succeed)
    {
        qWarning() << "open " << jsonPath << " failed!";
        return false;
    }

    QByteArray data = file.readAll();
    file.close();

    gCrypto.setKey("QJCDCOMRAYVISION");
    gCrypto.setIV("A1E2F3C456789DB9");
    QJsonParseError error;
    QJsonDocument document = QJsonDocument::fromJson(gCrypto.decode(data), &error);
    if (document.isNull() || !document.isArray())
    {
        qWarning() << "parse " << jsonPath << "failed. error string:" << error.errorString();
        return false;
    }
    m_assetItemThirdLevelInfo.clear();
    QJsonArray array = document.array();
    m_assetItemThirdLevelInfo.reserve(array.size());
    // 贴图文件夹名
    QString textureThumbnailDir{""};

    QFontMetrics fontMerics(QFont("Microsoft YaHei", 12));

    QDir dir;
    for (auto value : array)
    {
        int total_count = 0;
        Q_ASSERT(value.isObject() && "value is not json object");

        QJsonObject object = value.toObject();
        AssetItemThirdLevelInfo* firstAssetItemThirdLevelInfo = new AssetItemThirdLevelInfo;
        firstAssetItemThirdLevelInfo->m_name = object.value("name").toString();
        firstAssetItemThirdLevelInfo->m_dir =  object.value("dir").toString();
        qDebug() << "firstAssetItemThirdLevelInfo->m_dir:   " << firstAssetItemThirdLevelInfo->m_dir;
        QJsonArray secondArray = object.value(firstAssetItemThirdLevelInfo->m_dir).toArray();

        QString prefix_dir_name = firstAssetItemThirdLevelInfo->m_dir + "/";

        for (auto secondValue : secondArray)
        {
            QString second_prefix_dir_name = prefix_dir_name;

            QJsonObject subObj = secondValue.toObject();
            AssetItemThirdLevelInfo* secondAssetItemThirdLevelInfo = new AssetItemThirdLevelInfo;
            secondAssetItemThirdLevelInfo->m_name = subObj.value("name").toString();
            secondAssetItemThirdLevelInfo->m_dir = subObj.value("dir").toString();                   // xm00200
            firstAssetItemThirdLevelInfo->m_child.append(secondAssetItemThirdLevelInfo);
            QJsonArray thirdArray = subObj.value(secondAssetItemThirdLevelInfo->m_dir).toArray();

            if (secondAssetItemThirdLevelInfo->m_name.length() > m_maxStr.length())
            {
                m_maxStr = secondAssetItemThirdLevelInfo->m_name;
            }

            second_prefix_dir_name += secondAssetItemThirdLevelInfo->m_dir + '/';

            int sub_total_count = 0;
            for (auto thirdValue : thirdArray)
            {
                QString third_prefix_dir_name = second_prefix_dir_name;

                subObj = thirdValue.toObject();
                AssetItemThirdLevelInfo* third_assetItemThirdLevelInfo = new AssetItemThirdLevelInfo;
                third_assetItemThirdLevelInfo->m_name = subObj.value("name").toString();
                third_assetItemThirdLevelInfo->m_dir = subObj.value("dir").toString();
                secondAssetItemThirdLevelInfo->m_child.append(third_assetItemThirdLevelInfo);

                third_prefix_dir_name += third_assetItemThirdLevelInfo->m_dir + '/';

                QJsonArray forthArray = subObj.value(third_assetItemThirdLevelInfo->m_dir).toArray();
                total_count += forthArray.size();
                sub_total_count += forthArray.size();

                for (auto forthValue : forthArray)
                {
                    QString forth_prefix_dir_name = third_prefix_dir_name;

                    subObj = forthValue.toObject();
                    AssetItem* assetItem = new AssetItem;
                    QString forthPrefixDirCopy = prefix_dir_name;

                    forth_prefix_dir_name += subObj.keys().at(0) + '/';

                    QJsonObject  endObjValue = subObj.value(subObj.keys().at(0)).toObject();
                    QJsonArray endArray = endObjValue.value("Download").toArray();
                    for (auto str : endArray)
                    {
                        assetItem->m_downLoadPaths << str.toString();
                    }
                    endArray = endObjValue.value("TextureMap_Thumbnail").toArray();
                    for (auto str : endArray)
                    {
                        assetItem->m_textureMap_thumbnails << str.toString();
                    }
                    assetItem->m_remoteDir = forth_prefix_dir_name;
                    assetItem->m_thumbnailDir = endArray.at(0).toString().left(endArray.at(0).toString().lastIndexOf('/'));
                    //                    dir.mkpath(prefix_dir + assetItem->m_thumbnailDir);

                    assetItem->m_localSaveDir = prefix_dir + prefix_dir_name;

                    assetItem->m_matFilePath = prefix_dir +  prefix_dir_name + endObjValue.value("MD5").toString();
                    assetItem->m_matFile_thumbnail = prefix_dir + prefix_dir_name + endObjValue.value("MatFile_Thumbnail").toString();

                    //                    if (textureThumbnailDir.isEmpty())
                    //                    {
                    //                        QString oneTextureMap = assetItem->m_textureMap_thumbnails.at(0);
                    //                        textureThumbnailDir = oneTextureMap.left(oneTextureMap.lastIndexOf('/'));
                    //                        dir.mkpath(prefix_dir + textureThumbnailDir);
                    //                    }
                    assetItem->m_name = fontMerics.elidedText(endObjValue.value("Name").toString(),Qt::ElideRight, 136);


                    if (assetItem->m_name.endsWith("…"))
                    {
                        assetItem->m_realName = endObjValue.value("Name").toString();
                    }

                    assetItem->m_rt = endObjValue.value("RT").toString();
                    assetItem->m_maxVer = endObjValue.value("Maxver").toString();
                    assetItem->m_renderers = endObjValue.value("Renderers").toString();
                    assetItem->m_size =  QString("%1MB").arg(QString::number(endObjValue.value("Size").toDouble()/(1024 * 1024), 'f', 2));


                    firstAssetItemThirdLevelInfo->m_assetItems.append(assetItem);
                    secondAssetItemThirdLevelInfo->m_assetItems.append(assetItem);
                    third_assetItemThirdLevelInfo->m_assetItems.append(assetItem);
                }
            }
            secondAssetItemThirdLevelInfo->m_totalCount = sub_total_count;

        }
        firstAssetItemThirdLevelInfo->m_totalCount = total_count;
        m_assetItemThirdLevelInfo.append(firstAssetItemThirdLevelInfo);
    }
    return true;
}

void MaterialAsset::insertWidgetToAssetKindWidget()
{
    QVBoxLayout* lowLevel_vBoxlayout = static_cast<QVBoxLayout*>(m_detailUi->material_kind_widget->layout());
    int i = 0;
    for (auto assetItemThirdLevelInfo : m_assetItemThirdLevelInfo)
    {
        QWidget* widget = new QWidget(m_detailUi->material_kind_widget);
        widget->setAttribute(Qt::WA_TranslucentBackground);
        QVBoxLayout* vBoxLayout = new QVBoxLayout();
        vBoxLayout->setContentsMargins(9, 9, 0, 9);

        // corona材质vray材质
        QLabel* label = new QLabel(widget);
        label->setFixedSize(QSize(88, 22));
        label->setText(assetItemThirdLevelInfo->m_name + ":");
        label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        label->setStyleSheet("color: #BDBDBD; font-size: 12px;");
        label->setAttribute(Qt::WA_TranslucentBackground);
        vBoxLayout->addWidget(label);
        vBoxLayout->addStretch();
        widget->setLayout(vBoxLayout);
        QHBoxLayout* hboxLayout = static_cast<QHBoxLayout*>(lowLevel_vBoxlayout->itemAt(i)->layout());
        Q_ASSERT(hboxLayout != nullptr);
        hboxLayout->insertWidget(0, widget);
        hboxLayout->setStretch(0, 1);
        hboxLayout->setStretch(1, 100);
        i++;
    }
}

void MaterialAsset::onDownloadBtnDisappeared(AssetItem* item)
{
    qDebug() << "MaterialAsset::onDownloadBtnDisappeared";
    if (m_detailUi->material_detail_widget->isVisible())
    {
        if (!item->m_hasInsert)
        {
            QStringList textureMaps;
            for (auto textureMap : item->m_textureMap_thumbnails)
            {
                textureMaps << textureMap.right(textureMap.length() - textureMap.lastIndexOf('/') - 1);
            }
            DownloadAsset* downloadAsset = new DownloadAsset(item->m_thumbnailDir, textureMaps, item);
            connect(downloadAsset, &DownloadAsset::sendImageData, [item](const QByteArray& imageData){
                qDebug() << "OK  receive sendImageData";
                item->m_textureThumbImages.push_back(imageData);
            });
            connect(downloadAsset, &DownloadAsset::downloadAllFinished, [this, item] {
                item->m_textureThumbImages.insert(0, item->m_imageData);
                item->m_hasInsert = true;
                QMetaObject::invokeMethod(this, "onDownloadAllFinished", Qt::QueuedConnection, Q_ARG(AssetItem*, item));
            });
            item->m_textureThumbImages.clear();
            QThreadPool::globalInstance()->start(downloadAsset);
        }
        else
        {
            onDownloadAllFinished(item);
        }
    }
}

bool LightAsset::parseFileJosn(const QString &jsonPath)
{
    QFile file(jsonPath);
    bool succeed = file.open(QIODevice::ReadOnly | QIODevice::Text);
    if (!succeed)
    {
        qWarning() << "open " << jsonPath << " failed!";
        return false;
    }

    QByteArray data = file.readAll();
    file.close();

    Crypto<AESCrypto> m_crypto;
    m_crypto.setKey("QJCDCOMRAYVISION");
    m_crypto.setIV("A1E2F3C456789DB9");

    QJsonParseError error;
    QJsonDocument document = QJsonDocument::fromJson(m_crypto.decode(data), &error);

    if (document.isNull() || !document.isObject())
    {
        qWarning() << "parse " << jsonPath << "failed. error string:" << error.errorString();
        return false;
    }
    QString dirName = "";

    QJsonObject jsonObject = document.object();
    // IES
    QString rootKey = jsonObject.keys().at(0);
    dirName += rootKey + '/';

    QJsonArray jsonArray = jsonObject.value(rootKey).toArray();
    m_assetItemThirdLevelInfo.clear();

    AssetItemThirdLevelInfo* firstAssetItemThirdLevelInfo = new AssetItemThirdLevelInfo;
    firstAssetItemThirdLevelInfo->m_name = rootKey;

    QDir dir;

    for (int i = 0; i < jsonArray.size(); ++i)
    {
        QJsonObject lightKindObj = jsonArray.at(i).toObject();
        QString lightKindName = lightKindObj.value("name").toString();

        AssetItemThirdLevelInfo* secondAssetItemThirdLevelInfo = new AssetItemThirdLevelInfo;
        secondAssetItemThirdLevelInfo->m_name = lightKindObj.value("value").toString();
        secondAssetItemThirdLevelInfo->m_dir = lightKindName;

        if (secondAssetItemThirdLevelInfo->m_name.length() > m_maxStr.length())
        {
            m_maxStr = secondAssetItemThirdLevelInfo->m_name;
        }

        QJsonArray secretLightArray = lightKindObj.value(lightKindName).toArray();

        secondAssetItemThirdLevelInfo->m_totalCount = secretLightArray.size();
        QString prefix_dir_name = prefix_dir + dirName + lightKindName + '/';

        for (int j = 0; j < secretLightArray.size(); j++)
        {
            QJsonObject secretObj = secretLightArray.at(j).toObject();
            AssetItem* assetItem = new AssetItem;

            QString matFile = secretObj.value("File").toString();
            QString fileThumb = secretObj.value("File_Thumbnail").toString();
            assetItem->m_matFilePath = prefix_dir_name + matFile;
            assetItem->m_matFile_thumbnail = prefix_dir_name + fileThumb;

            assetItem->m_rt = secretObj.value("RT").toString();
            assetItem->m_name = secretObj.value("Name").toString();
            assetItem->m_size = QString("%1MB").arg(QString::number(secretObj.value("Size").toDouble() / 1024*1024, 'f', 2));
            assetItem->m_maxVer = secretObj.value("Maxver").toString();
            assetItem->m_renderers = secretObj.value("Renderers").toString();

            QJsonArray endArray = secretObj.value("Download").toArray();
            for (auto download : endArray)
            {
                assetItem->m_downLoadPaths << dirName + lightKindName + '/' + download.toString() ;
            }
            assetItem->m_remoteDir = dirName + lightKindName + '/';
            //            assetItem.m_thumbnailDir = dirName + lightKindName + '/';
            assetItem->m_localSaveDir = prefix_dir_name;

            secondAssetItemThirdLevelInfo->m_assetItems.append(assetItem);
            firstAssetItemThirdLevelInfo->m_assetItems.append(assetItem);
        }
        firstAssetItemThirdLevelInfo->m_totalCount += secondAssetItemThirdLevelInfo->m_totalCount;
        firstAssetItemThirdLevelInfo->m_child.append(secondAssetItemThirdLevelInfo);
    }

    m_assetItemThirdLevelInfo.append(firstAssetItemThirdLevelInfo);
    return true;
}

void LightAsset::onDownloadBtnDisappeared(AssetItem *)
{
    if (m_detailUi->light_detail_widget->isVisible())
    {
        m_detailUi->main_light_detail_widget->setEnabled(true);
    }
}

LightAsset::~LightAsset()
{

    if (!m_DeleteIESBitmap)
    {
        m_DeleteIESBitmap = (DeleteIESBitmap)m_library.resolve("deleteIESBitmap");
    }
    if (m_renderIes)
    {
        m_DeleteIESBitmap((HANDLE)m_renderIes);
    }
    if (m_renderIesDiagram)
    {
        m_DeleteIESBitmap((HANDLE)m_renderIesDiagram);
    }


}

LightAsset::LightAsset(Ui::CMainWindow *ui) : Asset (ui)
{
    m_assetKindName = "灯光类";
}

void LightAsset::onCheckBoxChanged()
{
    QCheckBox* checkbox = qobject_cast<QCheckBox*>(sender());

    if (m_curSelectCheckBox->text() != checkbox->text())
    {
        if (checkbox->text() == "显示渲染图")
        {
            beginRenderIes();
            m_detailUi->widget_4->show();
        }
        else
        {
            displayShape();
            m_detailUi->widget_4->hide();
        }
    }
    m_curSelectCheckBox->setChecked(false);
    checkbox->setChecked(true);
    m_curSelectCheckBox = checkbox;
}

void LightAsset::onModelDataSelected(AssetItem* item)
{
    ThumbnailModel* model = static_cast<ThumbnailModel*>(m_detailUi->light_tableView->model());
    QModelIndex index = model->getIndex(m_curSelectAssetItem);
    if (index.isValid())
    {
        m_curSelectAssetItem->m_isChecked = false;
        m_detailUi->light_tableView->model()->setData(index, QVariant::fromValue(m_curSelectAssetItem));
        emit model->dataChanged(index, index);
    }
    m_curSelectAssetItem = item;

    if (item->m_isShowDownloadText)
    {
        m_detailUi->main_light_detail_widget->setEnabled(false);
    }
    else
    {
        m_detailUi->main_light_detail_widget->setEnabled(true);
    }
    QPixmap pixmap;
    pixmap.loadFromData(item->m_imageData);
    m_detailUi->light_pic_label->setPixmap(pixmap.scaled(220, 220));
    m_detailUi->light__name_label->setText(item->m_name);
    if (item->m_isFillPolygon)
    {
        m_detailUi->light_collect_icon_label->setPixmap(QPixmap(":/images/collected.png"));
    }
    else
    {
        m_detailUi->light_collect_icon_label->setPixmap(QPixmap(":/images/uncollect.png"));
    }
    m_detailUi->color_temperature_num_label->setText(QString::number(m_detailUi->colorTemperatureSlider->value()));
    m_detailUi->light_detail_widget->show();

    if (m_library.fileName().isEmpty())
    {
        m_library.setFileName("Rayvision");
        m_renderIes = (RenderIes) m_library.resolve("RenderIes");
        m_renderIesDiagram = (RenderIesDiagram) m_library.resolve("RenderIesDiagram");
        Q_ASSERT(m_renderIes && m_renderIesDiagram);

        connect(m_detailUi->colorTemperatureSlider, &QSlider::valueChanged, this, &LightAsset::beginRenderIes);
        connect(m_detailUi->brightness_slider, &QSlider::valueChanged, this, &LightAsset::beginRenderIes);
        connect(m_detailUi->distance_slider, &QSlider::valueChanged, this, &LightAsset::beginRenderIes);

        // 默认
        m_curSelectCheckBox = m_detailUi->display_render_checkBox;

        m_detailUi->brightness_slider->setToolTip(QString("<p style= 'color:#BDBDBD;'>%1</p>").arg(m_detailUi->brightness_slider->value()));
        m_detailUi->distance_slider->setToolTip(QString("<p style= 'color:#BDBDBD;'>%1</p>").arg((double)m_detailUi->distance_slider->value() / m_detailUi->distance_slider->maximum()));

        connect(m_detailUi->display_render_checkBox, &QCheckBox::clicked, this, &LightAsset::onCheckBoxChanged);
        connect(m_detailUi->display_shape_checkbox, &QCheckBox::clicked, this, &LightAsset::onCheckBoxChanged);
    }
}


void LightAsset::beginRenderIes()
{
    using namespace Gdiplus;

    qDebug() << "LightAsset::beginRenderIes";



    m_detailUi->brightness_slider->setToolTip(QString("<p style= 'color:#BDBDBD;'>%1</p>").arg(m_detailUi->brightness_slider->value()));
    m_detailUi->distance_slider->setToolTip(QString("<p style= 'color:#BDBDBD;'>%1</p>").arg((double)m_detailUi->distance_slider->value() / m_detailUi->distance_slider->maximum()));

    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

    HANDLE result = nullptr;
    int cy = static_cast<int>(220*((double)m_detailUi->distance_slider->value() / (double)m_detailUi->distance_slider->maximum()));
    //    int cy = cx;
    result = m_renderIes(m_curSelectAssetItem->m_matFilePath.toStdString().c_str(), 220,
                         110,
                         cy, 0.0, 0.0, 0.0, m_detailUi->brightness_slider->value(), 0.1, 1.0,true, m_detailUi->colorTemperatureSlider->value());

    HBITMAP res = (HBITMAP)(result);

    QPixmap pix = QtWin::fromHBITMAP(res);

    m_detailUi->light_pic_label->setPixmap(pix);

    GdiplusShutdown(gdiplusToken);
}

void LightAsset::displayShape()
{
    using namespace Gdiplus;

    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

    HANDLE result = m_renderIesDiagram(m_curSelectAssetItem->m_matFilePath.toStdString().c_str(), 220, true);

    QPixmap pix = QtWin::fromHBITMAP((HBITMAP)result);
    m_detailUi->light_pic_label->setPixmap(pix);
    GdiplusShutdown(gdiplusToken);
}

AssetPage::AssetPage(Ui::CMainWindow *ui, const QString &jsonFile)
    : m_detailUi(ui)
{
    m_detailUi->material_detail_widget->hide();
    m_detailUi->light_detail_widget->hide();

    connect(m_detailUi->colorTemperatureSlider, &QSlider::valueChanged, this, &AssetPage::onSliderValueChnaged);
    init(jsonFile);
    m_detailUi->scrollAreaWidgetContents->setMinimumHeight(520);
}

void AssetPage::init(const QString &jsonFile)
{
    QFile file(jsonFile);
    bool succeed = file.open(QIODevice::ReadOnly | QIODevice::Text);
    if (!succeed)
    {
        qDebug("open %s failed!", jsonFile.toStdString().c_str());
        return;
    }
    Asset::prefix_dir = LocalSetting::getInstance()->getAssetSavePath();

    QByteArray data = file.readAll();
    file.close();

    Crypto<AESCrypto> m_crypto;
    m_crypto.setKey("QJCDCOMRAYVISION");
    m_crypto.setIV("A1E2F3C456789DB9");

    m_assetKindPath = getRootMap(m_crypto.decode(data));
    m_assetVec.reserve(m_assetKindPath.size());

    for (int i = m_assetKindPath.size() - 1; i >= 0; i--)
    {
        auto pair = m_assetKindPath.at(i);
        QPushButton* button = new QPushButton(m_detailUi->asset_kind_widget);
        button->setText(pair.first);
        button->setStyleSheet("QPushButton{Text-align:left; font-size: 14px;}");

        button->setFixedSize(QSize(80,40));
        button->setFlat(true);
        button->setCheckable(true);

        connect(button, &QPushButton::clicked,this, &AssetPage::onClicked);
        static_cast<QHBoxLayout*>(m_detailUi->asset_kind_widget->layout())->insertWidget(0, button);
        if (i < 2)
        {
            button->clicked();
        }
    }


    static_cast<QHBoxLayout*>(m_detailUi->asset_kind_widget->layout())->addStretch();

    //    SearchBox* searchBox = new SearchBox(m_detailUi->asset_kind_widget);
    //    connect(searchBox, &SearchBox::searchTriggered, this, &AssetPage::onSearchContentsChanged);
    //    searchBox->setFixedHeight(30);
    //    searchBox->setSearchHolderTxt("搜索");
    //    searchBox->setStyleSheet(R"(
    //                             background: #232531;
    //                             border-radius: 4px;
    //                             color: #BDBDBD;
    //                             font-size: 12px;)");
    //    m_detailUi->asset_kind_widget->layout()->addWidget(searchBox);
}


QVector<QPair<QString, QString>> AssetPage::getRootMap(const QByteArray &data)
{
    QJsonParseError error;
    QJsonDocument document = QJsonDocument::fromJson(data, &error);
    if (document.isNull())
    {
        qDebug() << "parse Json failed, reason:" << error.errorString() <<"  raw data:" << data;
    }
    QJsonObject object = document.object();

    QJsonArray jsonArray = object.value(object.keys().at(0)).toArray();
    QVector<QPair<QString, QString>> map;
    for (auto value: jsonArray)
    {
        QJsonObject obj = value.toObject();
        qDebug() << "obj.keys().at(0):  " <<obj.keys().at(0);
        map.append(QPair<QString, QString>(obj.keys().at(0), obj.value(obj.keys().at(0)).toString()));
    }
    return map;
}

void AssetPage::onClicked()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());

    qDebug() << "AssetPage::onClicked():   " << button->text();
    if ((m_isDownloadFlag && m_isDownloadFileRetryCount < 5) || m_curCheckedBtn != button)
    {

        if (m_curCheckedBtn)
        {
            m_curCheckedBtn->setChecked(false);
        }
        m_curCheckedBtn = button;
        button->setChecked(true);

        QString buttonTextName = button->text();
        bool find = false;
        for (auto assetItem : m_assetVec)
        {
            if (assetItem->getAssetKindName() == buttonTextName)
            {
                find = true;
                m_curAsset = assetItem;
                break;
            }
        }

        if (!find)
        {
            m_curAsset = AssetFactory::Inst()->getSecretAssetKind(m_detailUi, buttonTextName);
            m_assetVec.append(m_curAsset);
        }

        if (!m_curAsset->isHasLoadJsonFile())
        {
            qDebug()<<"------------------------------------------------";
            QString fileName = "";
            for (auto iter : m_assetKindPath)
            {
                if (iter.first == buttonTextName)
                {
                    fileName = iter.second;
                    break;
                }
            }

            bool isSuccess = m_curAsset->parseFileJosn(LocalSetting::getInstance()->getAssetSavePath() + fileName);
            if (!isSuccess)
            {
                DownloadAsset* downloadAsset = new DownloadAsset("", fileName, LocalSetting::getInstance()->getAssetSavePath(), true);
                connect(downloadAsset, &DownloadAsset::downloadAllFinished, this, &AssetPage::onSecretJsonDownloadCompleted);
                QThreadPool::globalInstance()->start(downloadAsset);
                return;
            }
            else
            {
                m_isDownloadFileRetryCount = 0;
                m_isDownloadFlag = false;
                m_curAsset->setIsHasLoadJsonFile(true);
            }

            if (m_curAsset->getAssetKindName() == "材质类")
            {
                m_curAsset->setupSecretAssetKindWidget(m_detailUi->material_kind_widget);
                MaterialAsset* materialAsset = static_cast<MaterialAsset*>(m_curAsset);
                materialAsset->insertWidgetToAssetKindWidget();
                m_curAsset->setupFilterWidget(m_detailUi->filter_widget);
                m_detailUi->tableView->viewport()->installEventFilter(new QToolTipper(m_detailUi->tableView));
                m_detailUi->tableView->setMouseTracking(true);
                m_detailUi->tableView->setDragEnabled(true);
                m_detailUi->tableView->setAcceptDrops(true);
                m_detailUi->tableView->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
                m_curAsset->setupThumbnailViewWidget(m_detailUi->tableView, true);

            }
            else if (m_curAsset->getAssetKindName() == "灯光类")
            {

                m_detailUi->light_tableView->setDragEnabled(false);
                m_detailUi->light_tableView->setAcceptDrops(false);
                m_curAsset->setupSecretAssetKindWidget(m_detailUi->light_kind_widget);
                m_curAsset->setupFilterWidget(m_detailUi->light_filter_widget);
                m_curAsset->setupThumbnailViewWidget(m_detailUi->light_tableView, false);
            }
            else
            {
                m_detailUi->texture_tableView->setDragEnabled(false);
                m_detailUi->texture_tableView->setAcceptDrops(false);
                m_curAsset->setupSecretAssetKindWidget(m_detailUi->texture_kind_widget);
                m_curAsset->setupFilterWidget(m_detailUi->texture_filter_widget);
                m_curAsset->setupThumbnailViewWidget(m_detailUi->texture_tableView, false);
            }
        }
        if (m_curAsset->getAssetKindName() == "材质类")
        {
            m_detailUi->subMaterial_stackedWidget->setCurrentIndex(0);
        }
        else if (m_curAsset->getAssetKindName() == "贴图类")
        {
            m_detailUi->subMaterial_stackedWidget->setCurrentIndex(1);
        }
        else
        {
            m_detailUi->subMaterial_stackedWidget->setCurrentIndex(2);
        }
    }

    button->setChecked(true);
}

void AssetPage::onSliderValueChnaged(int value)
{
    m_detailUi->color_temperature_num_label->setText(QString::number(value));
}

void AssetPage::onSecretJsonDownloadCompleted()
{
    m_isDownloadFileRetryCount++;
    m_isDownloadFlag= true;
    m_curCheckedBtn->click();
}

void AssetPage::onSearchContentsChanged(const QString &)
{

}

Asset *AssetFactory::getSecretAssetKind(Ui::CMainWindow* ui, const QString &assetKindName)
{
    Asset* asset = nullptr;
    if (assetKindName == "材质类")
    {
        asset = new MaterialAsset(ui);
    }
    else if (assetKindName == "灯光类")
    {
        asset = new LightAsset(ui);
    }
    else if (assetKindName == "贴图类")
    {
        asset = new TextureAsset(ui);
    }
    return asset;
}

TextureAsset::TextureAsset(Ui::CMainWindow *ui) : Asset (ui)
{
    m_assetKindName = "贴图类";
}

bool TextureAsset::parseFileJosn(const QString &jsonPath)
{
    QFile file(jsonPath);
    bool succeed = file.open(QIODevice::ReadOnly | QIODevice::Text);
    if (!succeed)
    {
        qWarning() << "open " << jsonPath << " failed!";
        return false;
    }

    QByteArray data = file.readAll();
    file.close();

    Crypto<AESCrypto> m_crypto;
    m_crypto.setKey("QJCDCOMRAYVISION");
    m_crypto.setIV("A1E2F3C456789DB9");

    QJsonParseError error;
    QJsonDocument document = QJsonDocument::fromJson(m_crypto.decode(data), &error);

    if (document.isNull() || !document.isObject())
    {
        qWarning() << "parse " << jsonPath << "failed. error string:" << error.errorString();
        return false;
    }
    QString dirName = "";

    QJsonObject jsonObject = document.object();
    // bitmap
    QString rootKey = jsonObject.keys().at(0);
    dirName += rootKey + '/';

    QJsonArray jsonArray = jsonObject.value(rootKey).toArray();
    m_assetItemThirdLevelInfo.clear();

    AssetItemThirdLevelInfo* firstAssetItemThirdLevelInfo = new AssetItemThirdLevelInfo;
    firstAssetItemThirdLevelInfo->m_name = rootKey;

    QDir dir;



    for (int i = 0; i < jsonArray.size(); ++i)
    {
        QJsonObject textureKindObj = jsonArray.at(i).toObject();
        QString textureKindName = textureKindObj.value("name").toString();

        AssetItemThirdLevelInfo* secondAssetItemThirdLevelInfo = new AssetItemThirdLevelInfo;
        secondAssetItemThirdLevelInfo->m_name = textureKindObj.value("value").toString();
        secondAssetItemThirdLevelInfo->m_dir = textureKindName;

        if (secondAssetItemThirdLevelInfo->m_name.length() > m_maxStr.length())
        {
            m_maxStr = secondAssetItemThirdLevelInfo->m_name;
        }

        QJsonArray secretLightArray = textureKindObj.value(textureKindName).toArray();

        secondAssetItemThirdLevelInfo->m_totalCount = secretLightArray.size();
        bool m_hasMkPath = false;
        for (int j = 0; j < secretLightArray.size(); j++)
        {
            QJsonObject secretObj = secretLightArray.at(j).toObject();
            AssetItem* assetItem = new AssetItem;
            QString prefix_dir_name = prefix_dir + dirName + textureKindName + '/';
            assetItem->m_matFilePath = prefix_dir_name + secretObj.value("File").toString();

            assetItem->m_rt = secretObj.value("RT").toString();
            assetItem->m_name = secretObj.value("Name").toString();
            assetItem->m_size = QString("%1MB").arg(secretObj.value("Size").toDouble() / 1024*1024);
            assetItem->m_maxVer = secretObj.value("Maxver").toString();
            assetItem->m_isShowDownloadText = false;
            assetItem->m_renderers = secretObj.value("Renderers").toString();

            assetItem->m_matFile_thumbnail = prefix_dir_name + secretObj.value("File_Thumbnail").toString();

            QString matFile = secretObj.value("File").toString();
            QString fileThumb = secretObj.value("File_Thumbnail").toString();
            assetItem->m_matFilePath = prefix_dir_name + matFile;
            assetItem->m_matFile_thumbnail = prefix_dir_name + fileThumb;

            assetItem->m_remoteDir = dirName + textureKindName + '/';

            //            assetItem.m_thumbnailDir = dirName + textureKindName + '/';
            assetItem->m_localSaveDir = prefix_dir_name;
            if (!m_hasMkPath)
            {
                dir.mkpath(assetItem->m_localSaveDir);
            }

            assetItem->m_textureMap_thumbnails << prefix_dir_name + secretObj.value("TextureMap_Thumbnail").toString();
            secondAssetItemThirdLevelInfo->m_assetItems.append(assetItem);
            firstAssetItemThirdLevelInfo->m_assetItems.append(assetItem);
        }
        firstAssetItemThirdLevelInfo->m_totalCount += secondAssetItemThirdLevelInfo->m_totalCount;
        firstAssetItemThirdLevelInfo->m_child.append(secondAssetItemThirdLevelInfo);
    }

    m_assetItemThirdLevelInfo.append(firstAssetItemThirdLevelInfo);
    return true;
}


