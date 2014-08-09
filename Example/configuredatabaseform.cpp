/*
 * Orm4Qt - An Object Relational Mapping Library for the Qt Framework
 * Copyright (c) 2014, Michael Dougras da Silva, All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3.0 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.
 */

#include "configuredatabaseform.h"
#include "ui_configuredatabaseform.h"
#include "repository.h"
#include "postgresqlprovider.h"
#include "sqliteprovider.h"
#include <QFileDialog>

/**
 * Constructor with parent initializer
 * @param parent
 * The QWidget parent of this instance
 */
ConfigureDatabaseForm::ConfigureDatabaseForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigureDatabaseForm)
{
    ui->setupUi(this);

    connect(ui->fileButton, SIGNAL(clicked()), this, SLOT(chooseFile()));
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(configureDatabase()));
}

/**
 * Virtual destructor
 */
ConfigureDatabaseForm::~ConfigureDatabaseForm()
{
    delete ui;
}

/**
 * Show a dialog to select the sqlite database file.
 */
void ConfigureDatabaseForm::chooseFile()
{
    QString filename = QFileDialog::getSaveFileName(this, "Choose the sqlite file", QDir::homePath(),
                                                    "Sqlite (*.sqlite *.db)", nullptr, QFileDialog::DontConfirmOverwrite);
    if(!filename.isNull())
        ui->fileDisplay->setText(filename);
}

/**
 * Register a provider in the Orm4Qt library based on the configurations supplied by the user
 */
void ConfigureDatabaseForm::configureDatabase()
{
    //Configure the provider to use Sqlite connection
    if(ui->comboBox->currentIndex() == 0)
    {
        QString filename = ui->fileDisplay->text();
        Orm4Qt::Repository::registerProvider([=](){return new Orm4Qt::SqliteProvider(filename);});
    }
    //Configure the provider to use PostgreSql connection
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
    //Close the window and emit the accepted signal
    accept();
}
