#include "AssetKindCombobox.h"
#include <QStringListModel>
#include <QVBoxLayout>
#include "AssetKindWidget.h"
#include "PopupWidget.h"
#include "PopupListViewDelegate.h"
#include <QDebug>
#include <QListView>
#include <QMouseEvent>


AssetKindCombobox::AssetKindCombobox(AssetKindWidget* assetKindWidget, QWidget *parent)
    : QWidget(parent)
    , m_assetKindWidget(assetKindWidget)
    , m_popupWidget(nullptr)
{
    m_assetKindWidget = assetKindWidget;
    m_assetKindWidget->setParent(this);
    QVBoxLayout* layout = new QVBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_assetKindWidget);

    connect(m_assetKindWidget, &AssetKindWidget::clicked, this, &AssetKindCombobox::clicked);

    setMouseTracking(true);
    setLayout(layout);
}

AssetKindCombobox::~AssetKindCombobox()
{
    if (m_popupWidget != nullptr)
    {
        delete m_popupWidget;
        m_popupWidget = nullptr;
    }

}

void AssetKindCombobox::hideStatusNotBySelf()
{
    m_hideByOthers = true;
}

bool AssetKindCombobox::operator==(const AssetKindCombobox &combox)
{
    if (combox.m_assetKindWidget == m_assetKindWidget)
    {
        return true;
    }
    return false;
}

AssetKindWidget *AssetKindCombobox::getAssetKindWidget()
{
    return m_assetKindWidget;
}

PopupWidget *AssetKindCombobox::getPopupWidget()
{
    return m_popupWidget;
}

void AssetKindCombobox::setAssetName(const QString &name)
{
    m_assetKindWidget->setAssetName(name);
}

bool AssetKindCombobox::getIsShow() const
{
    return m_isShow;
}

void AssetKindCombobox::hidePopupWidget()
{
    m_popupWidget->hide();
    m_isShow = false;
}

void AssetKindCombobox::onClicked()
{
    if (!m_assetKindWidget->getArrowIconShowStatus())
    {
        return;
    }
    if (!m_popupWidget)
    {
        m_popupWidget = new PopupWidget(this);
        m_popupWidget->setFixedWidth(m_assetKindWidget->width());
        m_popupWidget->setWindowFlag(Qt::Popup);
        m_popupWidget->hide();
        m_popupWidget->getListView()->setUniformItemSizes(true);

        QStringListModel* model = new QStringListModel(m_popupWidget);
        model->setStringList(m_assetKindWidget->getChildAssetNameList());

        m_popupWidget->setViewModel(model);
        // 22 是delegate类写死的 8 是listview 的样式 margin-top + margin-bottom
        int dispalyItemCount = m_assetKindWidget->getChildAssetNameList().size() > 6 ? 6 : m_assetKindWidget->getChildAssetNameList().size();
        m_popupWidget->resize(m_popupWidget->width(), dispalyItemCount * 22 + 8);

        PopupListViewDelegate* delegate = new PopupListViewDelegate(m_assetKindWidget->width(), m_popupWidget);
        delegate->setCombobox(this);
        m_popupWidget->setViewDelegate(delegate);
    }

    if (m_hideByOthers)
    {
        m_hideByOthers = false;
        m_isShow = false;
    }

    if (!m_isShow)
    {
        QPoint globalPos = this->mapToGlobal(QPoint());
        m_popupWidget->move(QPoint(globalPos.x(), globalPos.y() + this->height() + 1));
        m_popupWidget->show();
        m_isShow = true;
    }
    else
    {
        m_popupWidget->hide();
        m_isShow = false;
    }
}

void AssetKindCombobox::onHideExpanded()
{
    m_popupWidget->hide();
    m_isShow = false;
}

