#ifndef POINTEDIT_H
#define POINTEDIT_H

#include <QWidget>

namespace Ui {
    class PointEdit;
}

class PointEdit : public QWidget
{
    Q_OBJECT

public:
    explicit PointEdit(QPointF point = QPointF(0, 0), QWidget *parent = 0);
    ~PointEdit();

    void setPoint(QPointF point);
    QPointF getPoint();

private:
    Ui::PointEdit *ui;
};

#endif // POINTEDIT_H
