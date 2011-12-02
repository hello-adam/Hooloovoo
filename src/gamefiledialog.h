#ifndef GAMEFILEDIALOG_H
#define GAMEFILEDIALOG_H

#include <QDialog>
#include "filepreviewwidget.h"

namespace Ui {
    class GameFileDialog;
}

class GameFileDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GameFileDialog(QWidget *parent = 0);
    ~GameFileDialog();

    enum FileType { Picture, Audio, GameObject, Level, Game, PlayState };
    enum AcceptMode { Select, Load, Save, Create };

    QString getFileName();
    QStringList getAllAvailableFiles();

    void setAcceptMode(AcceptMode mode);
    void setFileType(FileType type);

private:
    Ui::GameFileDialog *ui;

    FilePreviewWidget* m_previewWidget;
    QWidget* m_currentPreviewWidget;
    FileType m_type;
    QStringList m_extensions;

    void setupPreviewWidget();

private slots:
    void fileSelectionChanged();
};

#endif // GAMEFILEDIALOG_H
