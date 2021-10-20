#ifndef TOOLBUTTON_H
#define TOOLBUTTON_H

#include <QWidget>

class ToolButton : public QWidget
{
    Q_OBJECT
public:
    explicit ToolButton(const QString text, const QPixmap& pixmap, QWidget *parent = nullptr);


    void setPixmap(const QPixmap& pixmap);
    void setText(const QString& text);
signals:

protected:
    void paintEvent(QPaintEvent* ) override;

    void mousePressEvent(QMouseEvent* event) override;


public slots:

private:
    QPixmap m_pixmap;
    QString m_content;
};

#endif // TOOLBUTTON_H
