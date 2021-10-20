#ifndef QTOOLTIPPER_H
#define QTOOLTIPPER_H

#include <QObject>

class QToolTipper : public QObject
{
    Q_OBJECT
public:
    explicit QToolTipper(QObject *parent = nullptr);

signals:

protected:
    bool eventFilter(QObject* obj, QEvent* event) override;

public slots:
};

#endif // QTOOLTIPPER_H
