#ifndef ASSETKINDCOMBOBOX_H
#define ASSETKINDCOMBOBOX_H

#include <QFrame>

class AssetKindWidget;
class PopupWidget;

class AssetKindCombobox : public QWidget
{
    Q_OBJECT

public:
    explicit AssetKindCombobox(AssetKindWidget* assetKindWidget, QWidget *parent = nullptr);

    ~AssetKindCombobox();

    void hideStatusNotBySelf();

    bool operator==(const AssetKindCombobox& combox);

    AssetKindWidget* getAssetKindWidget();

    PopupWidget* getPopupWidget();

    void setAssetName(const QString& name);

    bool getIsShow() const;

    void hidePopupWidget();

    void onClicked();

signals:
    void clicked(const QString& topAssetName, const QStringList& assetName, bool isTopLevel);

public slots:
    void onHideExpanded();

protected:

private:
    bool m_isShow {false}; // popupWidget ÿ�ε����ĵط�����֮ǰ�ǲ�����ʾ�ˣ����ᱻ�����ˣ����������������¼��
    bool m_hideByOthers {false};
    bool m_ischecked {false};
    AssetKindWidget* m_assetKindWidget;
    PopupWidget* m_popupWidget;
};

#endif // ASSETKINDCOMBOBOX_H
