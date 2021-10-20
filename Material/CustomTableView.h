#ifndef CUSTOMTABLEVIEW_H
#define CUSTOMTABLEVIEW_H

#include <QTableView>

class CustomTableView : public QTableView
{
public:
    explicit CustomTableView(QWidget *parent = Q_NULLPTR);
    QRect visualRect(const QModelIndex &index) const override;

protected:

    virtual void resizeEvent(QResizeEvent *) override;
    QSize viewportSizeHint() const override;


};

#endif // CUSTOMTABLEVIEW_H
