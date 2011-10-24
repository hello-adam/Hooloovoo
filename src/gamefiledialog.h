#ifndef GAMEFILEDIALOG_H
#define GAMEFILEDIALOG_H

#include <QDialog>

namespace Ui {
    class GameFileDialog;
}

class GameFileDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GameFileDialog(QWidget *parent = 0);
    ~GameFileDialog();

    QString getFileName();
    void setAccept(QString accept);
    void setCancel(QString cancel);
    void setSubdirectory(QString sub) { m_subdirectory = sub; updateList(); }
    void setFileExtensions(QStringList extensions) { m_extensions = extensions; updateList(); }
    void updateList();

private:
    Ui::GameFileDialog *ui;

    QString m_subdirectory;
    QStringList m_extensions;
};

#endif // GAMEFILEDIALOG_H
