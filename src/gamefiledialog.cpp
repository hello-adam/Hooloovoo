#include "gamefiledialog.h"
#include "ui_gamefiledialog.h"
#include <QDir>

GameFileDialog::GameFileDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GameFileDialog)
{
    ui->setupUi(this);

    this->setWindowTitle("File");

    m_extensions = QStringList("*.game");
    m_subdirectory = "games";

    connect(ui->lw_existingFiles, SIGNAL(currentTextChanged(QString)), ui->le_selectedFile, SLOT(setText(QString)));
    connect(ui->Accept, SIGNAL(clicked()), this, SLOT(accept()));
    connect(ui->Cancel, SIGNAL(clicked()), this, SLOT(reject()));

    this->updateList();
}

GameFileDialog::~GameFileDialog()
{
    delete ui;
}

QString GameFileDialog::getFileName()
{
    QString fileName = ui->le_selectedFile->text();

    QString fileExtension = m_extensions.at(0).mid(1);

    if (!fileName.endsWith(fileExtension))
    {
        fileName += fileExtension;
    }

    return fileName;
}


void GameFileDialog::setAccept(QString accept)
{
    ui->Accept->setText(accept);
    this->setWindowTitle(accept + " File");
}

void GameFileDialog::setCancel(QString cancel)
{
    ui->Cancel->setText(cancel);
}

void GameFileDialog::updateList()
{
    ui->lw_existingFiles->clear();
    QStringList files = QDir(QApplication::applicationDirPath() + "/" + m_subdirectory + "/").entryList(m_extensions, QDir::Files);
    ui->lw_existingFiles->addItems(files);
}
