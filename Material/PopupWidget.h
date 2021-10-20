#ifndef POPUPWIDGET_H
#define POPUPWIDGET_H

#include <QWidget>

class QListView;
class QAbstractItemModel;
class QAbstractItemDelegate;

// 下拉列表
class PopupWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PopupWidget(QWidget *parent = nullptr);

    void setViewModel(QAbstractItemModel* model);
    void setViewDelegate(QAbstractItemDelegate* delegate);
    QListView* getListView();
signals:

public slots:

private:
    QListView* m_listView;
};

#endif // POPUPWIDGET_H
