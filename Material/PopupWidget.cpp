#include "PopupWidget.h"
#include <QHBoxLayout>
#include <QListView>

PopupWidget::PopupWidget(QWidget *parent) : QWidget(parent)
{
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setWindowFlag(Qt::FramelessWindowHint);
    setObjectName("PopupWidget");
//    setStyleSheet("QWidget#PopupWidget {border:transparent; background-color: transparent;}");
    m_listView = new QListView;
    m_listView->setObjectName("popupListView");
    QHBoxLayout* layout = new QHBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    m_listView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // todo: QAbstractItemView::scrollPerItem »áÁôÒ»¸ö¿Õ°×item
    m_listView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    layout->addWidget(m_listView);
    setStyleSheet("PopupWidget#PopupWidget{border-radius: 4px; background: #2D2D38;}");

    m_listView->setStyleSheet(R"(
                              QListView{
                                 border-radius: 4px;
                                 margin-top:4px;
                                 margin-bottom:4px;
                              }

                              QScrollBar {
                                  border: 1px solid #2D2F43;
                                  margin-left: 0px;
                                  margin-right: 0px;
                                  margin-top: 0px;
                                  padding-right: 0px;
                                  margin-bottom: 0px;
                                  background-color: rgb(45, 45, 56);
                              }
                              QScrollBar::handle:vertical:hover{
                                  border: transparent;
                                  border-radius: 4px;
                                  background-color: #48485c;
                              }
                              QScrollBar:vertical {width: 8px; }
                              )");

    setLayout(layout);
}

void PopupWidget::setViewModel(QAbstractItemModel *model)
{
    m_listView->setModel(model);
}

void PopupWidget::setViewDelegate(QAbstractItemDelegate *delegate)
{
    m_listView->setItemDelegate(delegate);
}

QListView *PopupWidget::getListView()
{
    return m_listView;
}
