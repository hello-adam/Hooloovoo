#include "gamefiledialog.h"
#include "ui_gamefiledialog.h"
#include <QDir>
#include "filemanager.h"
#include "audiopreviewwidget.h"
#include "picturepreviewwidget.h"
#include <QFileDialog>

GameFileDialog::GameFileDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GameFileDialog)
{
    ui->setupUi(this);
    m_previewWidget = 0;

    connect(ui->lw_existingFiles, SIGNAL(currentTextChanged(QString)), ui->le_selectedFile, SLOT(setText(QString)));
    connect(ui->Accept, SIGNAL(clicked()), this, SLOT(accept()));
    connect(ui->Cancel, SIGNAL(clicked()), this, SLOT(reject()));

    connect(ui->pb_Import, SIGNAL(clicked()), this, SLOT(importFiles()));

    connect(ui->le_selectedFile, SIGNAL(textChanged(QString)), this, SLOT(fileSelectionChanged()));

    m_type = FileManager::Picture;
    m_directory = "";
}

GameFileDialog::~GameFileDialog()
{
    if (m_previewWidget)
        delete m_previewWidget;

    delete ui;
}

QString GameFileDialog::getFileName()
{
    QString fileName = ui->le_selectedFile->text();

    if (m_type != FileManager::Game)
    {
        QString fileExtension = m_extensions.at(0).mid(1);

        if (!fileName.endsWith(fileExtension))
        {
            fileName += fileExtension;
        }
    }

    return fileName;
}

QStringList GameFileDialog::getAllAvailableFiles()
{
    QStringList items;

    for (int i=0; i<ui->lw_existingFiles->count(); i++)
    {
        items << ui->lw_existingFiles->itemAt(i, 0)->text();
    }

    return items;
}

void GameFileDialog::setAcceptMode(AcceptMode mode)
{
    if (mode == Select)
    {
        ui->Accept->setText("Select");
        this->setWindowTitle("Select File");
        ui->le_selectedFile->setReadOnly(true);
    }
    else if (mode == Save)
    {
        ui->Accept->setText("Save");
        this->setWindowTitle("Save File");
        ui->le_selectedFile->setReadOnly(false);
    }
    else if (mode == Load)
    {
        ui->Accept->setText("Load");
        this->setWindowTitle("Load File");
        ui->le_selectedFile->setReadOnly(true);
    }
    else if (mode == Create)
    {
        ui->Accept->setText("Create");
        this->setWindowTitle("Create File");
        ui->le_selectedFile->setReadOnly(false);
    }
}

void GameFileDialog::setFileType(FileManager::FileType type)
{
    m_type = type;

    ui->pb_Import->setEnabled(true);

    QStringList files;

    files = FileManager::getInstance().getAvailableFilesOfType(type);
    m_extensions = FileManager::getFileExtensionsForType(type);
    m_directory = FileManager::getInstance().getActivePathForType(type);

    if (type == FileManager::Game)
    {
        ui->pb_Import->setEnabled(false);
    }

    QStringList noExtensionFiles;

    if (type != FileManager::Game)
    {
        foreach (QString file, files)
        {
            foreach (QString extension, m_extensions)
            {
                extension = extension.mid(1);
                if (file.endsWith(extension))
                {
                    file.chop(extension.size());
                    noExtensionFiles << file;
                    break;
                }
            }
        }
    }
    else
    {
        noExtensionFiles = files;
    }

    ui->lw_existingFiles->clear();
    ui->lw_existingFiles->addItems(noExtensionFiles);

    setupPreviewWidget();
    fileSelectionChanged();
}

void GameFileDialog::setupPreviewWidget()
{
    if (m_previewWidget)
    {
        ui->previewLayout->removeWidget(m_previewWidget);
        delete m_previewWidget;
        m_previewWidget = 0;
    }

    if (m_type == FileManager::Audio)
    {
        m_previewWidget = new AudioPreviewWidget();
    }
    if (m_type == FileManager::Picture)
    {
        m_previewWidget = new PicturePreviewWidget();
    }

    if (m_previewWidget)
        ui->previewLayout->addWidget(m_previewWidget);
}

void GameFileDialog::fileSelectionChanged()
{
    QString fileName = getFileName();

    if (m_previewWidget)
        m_previewWidget->setFile(fileName);
}

void GameFileDialog::importFiles()
{
    QFileDialog dlg;

    dlg.setAcceptMode(QFileDialog::AcceptOpen);
    dlg.setFilters(m_extensions);
    dlg.setDirectory("");
    dlg.setFileMode(QFileDialog::ExistingFiles);

    if (dlg.exec())
    {
        QStringList files = dlg.selectedFiles();

        foreach(QString file, files)
        {
            QString fileName = file.mid(file.lastIndexOf(QRegExp("\\\\|/")));
            QFile::copy(file, m_directory + fileName);
        }

        //reset the file list
        this->setFileType(m_type);
    }
}
