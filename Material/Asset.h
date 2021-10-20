#ifndef ASSET_H
#define ASSET_H

#include <QStringList>
#include <QObject>
#include <QVector>
#include "AssetItem.h"
#include <QModelIndex>
#include "Common/MainFrame.h"

namespace Ui
{
class CMainWindow;
}

struct AssetItemThirdLevelInfo
{
    ~AssetItemThirdLevelInfo()
    {
        qDeleteAll(m_child.begin(), m_child.end());
        qDeleteAll(m_assetItems.begin(), m_assetItems.end());
    }

    int m_totalCount {0};

    QString m_name;
    QString m_dir;

    QVector<AssetItem*> m_assetItems;

    // 遍历 两层
    QList<AssetItemThirdLevelInfo*> m_child;
};


class FlowLayout;
class QPushButton;
class AssetKindWidget;
class Asset;
class QVBoxLayout;
class CustomTableView;
class AssetKindCombobox;
class QCheckBox;

class AssetPage : public QObject
{
    Q_OBJECT

public:
    AssetPage(Ui::CMainWindow *ui, const QString &jsonFile);
    QVector<QPair<QString, QString> > getRootMap(const QByteArray& data);

public slots:
    void onClicked();
    void onSliderValueChnaged(int value);

private slots:
    void onSecretJsonDownloadCompleted();
    void onSearchContentsChanged(const QString&);
private:
    void init(const QString& jsonFile);

private:
    Ui::CMainWindow* m_detailUi;
    QPushButton* m_curCheckedBtn {nullptr};  // 最上面的  材质类 贴图类 灯光类 按钮
    QVector<Asset*> m_assetVec;

    Asset* m_curAsset; // 当前资产

    int m_isDownloadFileRetryCount {0};
    bool m_isDownloadFlag{false};
    QVector<QPair<QString, QString> > m_assetKindPath;
};

class AssetFactory : public TSingleton<AssetFactory>
{
public:
    Asset* getSecretAssetKind(Ui::CMainWindow* ui, const QString& assetKindName);
    AssetFactory() = default;
};

class Asset : public QObject
{
    Q_OBJECT

public:
    //    explicit Asset(Ui::CMainWindow* ui, const QByteArray& data);
    explicit Asset(Ui::CMainWindow* ui);
    virtual ~Asset();

    void setupSecretAssetKindWidget(QWidget *secretAssetKindWidget);

    void setupFilterWidget(QWidget *secretFilterWidget);

    AssetItemThirdLevelInfo* findAssetItemThirdLevelInfo(const QString& topAssetName, const QStringList &assetNames, bool  isToplevel);

    bool isHasLoadJsonFile() const;
    void setIsHasLoadJsonFile(bool flag);

    void appendToCache(AssetItem* item);

    virtual QString getAssetKindName();

    void setupThumbnailViewWidget(CustomTableView *secretTableView, bool isSupportDrag);

    virtual bool parseFileJosn(const QString& jsonPath) = 0;

public slots:
    void onClicked(const QString &topAssetName, const QStringList &assetNames, bool isTopLevel);
    void onstartDownload(AssetItem *item);
    virtual void onDownloadBtnDisappeared(AssetItem *) = 0;
public:
    static QString prefix_dir;

protected:
    Ui::CMainWindow* m_detailUi;

    bool m_isHasLoadJsonFile {false};

    AssetItem* m_curSelectAssetItem;

    QList<AssetItem*> m_assetItemVec;   // cache imageData count; 为了简单  设置为最大窗口的max

    QString m_assetKindName;

    AssetItem* m_downloadAssetItem;

    QString m_maxStr;

    QPushButton* m_curFilterCheckedBtn;  // 全部 免费 VIP 最新 按钮

    AssetKindCombobox* m_curSelectedCombobox;

    QString m_curSelectAssetKindName;

    int m_fileIndex{0};

    QVector<AssetItemThirdLevelInfo*> m_assetItemThirdLevelInfo;
};

class MaterialAsset : public Asset
{
    Q_OBJECT

public:
    MaterialAsset(Ui::CMainWindow* ui);

    bool parseFileJosn(const QString& jsonPath) override;

    void insertWidgetToAssetKindWidget();


public slots:
    void onModelDataSelected(AssetItem *item);
    void onDownloadAllFinished(AssetItem *item);
    void onDownloadBtnDisappeared(AssetItem *item) override;

private:

};

#include <QLibrary>

class TextureAsset : public Asset
{
    Q_OBJECT

public:
    TextureAsset(Ui::CMainWindow* ui);

    bool parseFileJosn(const QString& jsonPath) override;

public slots:
    void onDownloadBtnDisappeared(AssetItem *) override{}
};

class LightAsset : public Asset
{
    Q_OBJECT

    typedef HANDLE(*RenderIes)(const char *, int, int, int, double, double, double, double, double, double, bool, int);
    typedef HANDLE(*RenderIesDiagram)(const char *, int, bool);
    typedef HANDLE(*DeleteIESBitmap)(HANDLE);
    ~LightAsset() override;
public:
    LightAsset(Ui::CMainWindow* ui);
    bool parseFileJosn(const QString& jsonPath) override;

public slots:
    void onModelDataSelected(AssetItem *item);

    void onCheckBoxChanged();

    void beginRenderIes();

    void onDownloadBtnDisappeared(AssetItem *) override;
private:
    void displayShape();

private:
    QLibrary m_library;

    QCheckBox* m_curSelectCheckBox;

    RenderIes m_renderIes;
    RenderIesDiagram m_renderIesDiagram;
    DeleteIESBitmap m_DeleteIESBitmap;
};


#endif // ASSET_H
