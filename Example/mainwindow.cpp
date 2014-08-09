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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "personform.h"
#include "repository.h"
#include "configuredatabaseform.h"
#include <QMessageBox>

/**
 * Default constructor with optional parent argument.
 * @param parent
 * The parent widget of this main window.
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Create and set the model
    m_objectModel = new ObjectModel<Person>(QList<Person>(), this);
    m_proxyModel = new QSortFilterProxyModel(this);
    m_proxyModel->setSourceModel(m_objectModel);
    ui->tableView->setModel(m_proxyModel);
    //Hide the resumé column
    ui->tableView->setColumnHidden(5, true);

    //Set the shortcuts
    ui->actionAdd_Person->setShortcut(QKeySequence::New);
    ui->actionDelete_Person->setShortcut(QKeySequence::Delete);
    ui->actionEdit_Person->setShortcut(QKeySequence("Ctrl+e"));
    ui->actionClose->setShortcut(QKeySequence::Close);
    ui->actionRefreshPeople->setShortcut(QKeySequence::Refresh);

    //Connect Signals
    connect(ui->actionAbout_Qt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect(ui->actionAdd_Person, SIGNAL(triggered()), this, SLOT(addPerson()));
    connect(ui->actionConfigure_Database, SIGNAL(triggered()), this, SLOT(configureDatabaseConnection()));
    connect(ui->actionCreate_Table, SIGNAL(triggered()), this, SLOT(createTable()));
    connect(ui->actionDelete_Person, SIGNAL(triggered()), this, SLOT(deletePerson()));
    connect(ui->actionEdit_Person, SIGNAL(triggered()), this, SLOT(editPerson()));
    connect(ui->actionRefreshPeople, SIGNAL(triggered()), this, SLOT(selectPeople()));
    connect(ui->tableView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(editPerson()));

    //Force the user to configure the database connection on start
    configureDatabaseConnection();
}

/**
 * Destructor
 */
MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * Show a dialog to configure the database connection
 */
void MainWindow::configureDatabaseConnection()
{
    //Create and show the database configuration window
    ConfigureDatabaseForm *form = new ConfigureDatabaseForm(this);
    form->exec();
    delete form;
}

/**
 * Use the Orm4Qt to create the table of people
 */
void MainWindow::createTable()
{
    auto repository = Orm4Qt::Repository::createRepository();
    if(repository->createTable<Person>())
    {
        QMessageBox::information(this, "Table created", "The table was created!");
    }
    else
    {
        QMessageBox::critical(this, "Error while creating the table", QString("An error occurred while creating the table. \n"
                              "Description: %1 \nSql Error: %2").arg(repository->lastError()->description())
                              .arg(repository->lastError()->sqlError().text()));
    }
}

/**
 * Show a form that creates a new person
 */
void MainWindow::addPerson()
{
    PersonForm *form = new PersonForm(this);
    if(form->exec() == QDialog::Accepted)
    {
        savePerson(form->person());
    }
    selectPeople();
    delete form;
}

/**
 * Show a form that edit a person
 */
void MainWindow::editPerson()
{
    //Capture the current row of the table view
    QModelIndex index = m_proxyModel->mapToSource(ui->tableView->currentIndex());
    Person person = m_objectModel->dataSource()[index.row()];

    //Create a new edit form and set the person instance
    PersonForm *form = new PersonForm(this);
    form->setPerson(person);
    if(form->exec() == QDialog::Accepted)
    {
        savePerson(form->person());
    }
    selectPeople();
    delete form;
}

/**
 * Delete the current register of the table view
 */
void MainWindow::deletePerson()
{
    QModelIndex index = m_proxyModel->mapToSource(ui->tableView->currentIndex());
    Person person = m_objectModel->dataSource()[index.row()];
    int response = QMessageBox::question(this, "Confirm", QString("Do you confirm the exclusion of the person  \"%1\"").arg(person.name()),
                                         QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if(response == QMessageBox::Yes)
    {
        deletePerson(person);
        selectPeople();
    }
}

/**
 * Retrieve the current registers from the database and populate the table model
 */
void MainWindow::selectPeople()
{
    auto repository = Orm4Qt::Repository::createRepository();
    QList<Person> list;
    if(repository->select<Person>(list))
    {
        m_objectModel->setDataSource(list);
        ui->tableView->resizeColumnsToContents();
    }
    else
    {
        QMessageBox::critical(this, "Error while  person", QString("An error occurred while saving a person. \n"
                              "Description: %1 \nSql Error: %2").arg(repository->lastError()->description())
                              .arg(repository->lastError()->sqlError().text()));
    }
}

/**
 * Perform an insert or an update in the database
 * @param person
 * The register to be saved
 */
void MainWindow::savePerson(Person person)
{
    auto repository = Orm4Qt::Repository::createRepository();
    if(repository->saveObject<Person>(person))
    {
        QMessageBox::information(this, "Register saved", "The register was saved!");
    }
    else
    {
        QMessageBox::critical(this, "Error while saving person", QString("An error occurred while saving a person. \n"
                              "Description: %1 \nSql Error: %2").arg(repository->lastError()->description())
                              .arg(repository->lastError()->sqlError().text()));
    }
}

/**
 * Perform a delete in the database
 * @param person
 * The register to be deleted
 */
void MainWindow::deletePerson(Person person)
{
    auto repository = Orm4Qt::Repository::createRepository();
    if(repository->deleteObject<Person>(person))
    {
        QMessageBox::information(this, "Register removed", "The register was removed!");
    }
    else
    {
        QMessageBox::critical(this, "Error while removing person", QString("An error occurred while removing a person. \n"
                              "Description: %1 \nSql Error: %2").arg(repository->lastError()->description())
                              .arg(repository->lastError()->sqlError().text()));
    }
}
