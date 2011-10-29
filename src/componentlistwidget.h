#ifndef COMPONENTLISTWIDGET_H
#define COMPONENTLISTWIDGET_H

#include <QListWidget>

class ComponentListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit ComponentListWidget(QWidget *parent = 0);

    int getLastSelectedRow() {return m_lastSelectedRow;}

protected:
    void dropEvent(QDropEvent *event);
    int m_lastSelectedRow;

signals:
    void itemAdded(QString);

public slots:

};

#endif // COMPONENTLISTWIDGET_H
