#include "configuredatabaseform.h"
#include "ui_configuredatabaseform.h"
#include "repository.h"
#include "postgresqlprovider.h"
#include "sqliteprovider.h"
#include <QFileDialog>

ConfigureDatabaseForm::ConfigureDatabaseForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigureDatabaseForm)
{
    ui->setupUi(this);

    connect(ui->fileButton, SIGNAL(clicked()), this, SLOT(chooseFile()));
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(configureDatabase()));
}

ConfigureDatabaseForm::~ConfigureDatabaseForm()
{
    delete ui;
}

void ConfigureDatabaseForm::chooseFile()
{
    QString filename = QFileDialog::getSaveFileName(this, "Choose the sqlite file");
    if(!filename.isNull())
        ui->fileDisplay->setText(filename);
}

void ConfigureDatabaseForm::configureDatabase()
{
    if(ui->comboBox->currentIndex() == 0)
    {
        QString filename = ui->fileDisplay->text();
        Orm4Qt::Repository::registerProvider([=](){return new Orm4Qt::SqliteProvider(filename);});
    }
    else
    {
        QString user = ui->userEdit->text();
        QString password = ui->passwordEdit->text();
        QString database = ui->databaseEdit->text();
        QString host = ui->hostEdit->text();
        int port = ui->portEdit->value();
        Orm4Qt::Repository::registerProvider([=](){return new Orm4Qt::PostgreSqlProvider(
                        user,
                        password,
                        database,
                        host,
                        port
                        );});
    }
    accept();
}
