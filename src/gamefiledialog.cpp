#include "gamefiledialog.h"
#include "ui_gamefiledialog.h"
#include <QDir>
#include "filemanager.h"
#include "audiopreviewwidget.h"

GameFileDialog::GameFileDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GameFileDialog)
{
    ui->setupUi(this);
    m_previewWidget = 0;

    this->setAcceptMode(Select);
    this->setFileType(Picture);

    connect(ui->lw_existingFiles, SIGNAL(currentTextChanged(QString)), ui->le_selectedFile, SLOT(setText(QString)));
    connect(ui->Accept, SIGNAL(clicked()), this, SLOT(accept()));
    connect(ui->Cancel, SIGNAL(clicked()), this, SLOT(reject()));

    connect(ui->le_selectedFile, SIGNAL(textChanged(QString)), this, SLOT(fileSelectionChanged()));
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

    if (m_type != Game)
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

    for (int i; i<ui->lw_existingFiles->count(); i++)
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

void GameFileDialog::setFileType(FileType type)
{
    m_type = type;

    QStringList files;
    if (type == Picture)
    {
        files = FileManager::getInstance().getAvailablePictures();
        m_extensions = FileManager::getPictureExtensions();
    }
    else if (type == Audio)
    {
        files = FileManager::getInstance().getAvailableAudio();
        m_extensions = FileManager::getAudioExtensions();
    }
    else if (type == GameObject)
    {
        files = FileManager::getInstance().getAvailableGameObjects();
        m_extensions = FileManager::getObjectExtensions();
    }
    else if (type == Level)
    {
        files = FileManager::getInstance().getAvailableLevels();
        m_extensions = FileManager::getLevelExtensions();
    }
    else if (type == Game)
    {
        files = FileManager::getInstance().getAvailableGames();
        m_extensions = FileManager::getGameExtensions();
    }
    else if (type == PlayState)
    {
        files = FileManager::getInstance().getAvailablePlayStates();
        m_extensions = FileManager::getPlayStateExtensions();
    }

    QStringList noExtensionFiles;

    if (type != Game)
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

    if (m_type == Audio)
    {
        m_previewWidget = new AudioPreviewWidget();
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
