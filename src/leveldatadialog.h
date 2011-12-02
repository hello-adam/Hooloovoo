#ifndef LEVELDATADIALOG_H
#define LEVELDATADIALOG_H

#include <QDialog>

namespace Ui {
    class LevelDataDialog;
}

class LevelDataDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LevelDataDialog(QWidget *parent = 0);
    ~LevelDataDialog();

    bool editLevelData();

private:
    Ui::LevelDataDialog *ui;
};

#endif // LEVELDATADIALOG_H
