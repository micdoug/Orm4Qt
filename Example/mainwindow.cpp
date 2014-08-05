#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "personform.h"
#include "repository.h"
#include "configuredatabaseform.h"
#include <QMessageBox>

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
    ui->tableView->setColumnHidden(5, true);

    //Set the shortcuts
    ui->actionAdd_Person->setShortcut(QKeySequence::New);
    ui->actionDelete_Person->setShortcut(QKeySequence::Delete);
    ui->actionEdit_Person->setShortcut(QKeySequence("Ctrl+e"));
    ui->actionClose->setShortcut(QKeySequence::Close);

    //Connect Signals
    connect(ui->actionAbout_Qt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect(ui->actionAdd_Person, SIGNAL(triggered()), this, SLOT(addPerson()));
    connect(ui->actionConfigure_Database, SIGNAL(triggered()), this, SLOT(configureDatabaseConnection()));
    connect(ui->actionCreate_Table, SIGNAL(triggered()), this, SLOT(createTable()));
    connect(ui->actionDelete_Person, SIGNAL(triggered()), this, SLOT(deletePerson()));
    connect(ui->actionEdit_Person, SIGNAL(triggered()), this, SLOT(editPerson()));
    connect(ui->actionRefreshPeople, SIGNAL(triggered()), this, SLOT(selectPeople()));

    configureDatabaseConnection();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::configureDatabaseConnection()
{
    ConfigureDatabaseForm *form = new ConfigureDatabaseForm(this);
    form->exec();
    delete form;
}

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

void MainWindow::editPerson()
{
    QModelIndex index = m_proxyModel->mapToSource(ui->tableView->currentIndex());
    Person person = m_objectModel->dataSource()[index.row()];
    PersonForm *form = new PersonForm(this);
    form->setPerson(person);

    if(form->exec() == QDialog::Accepted)
    {
        savePerson(form->person());
    }
    selectPeople();
    delete form;
}

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

void MainWindow::selectPeople()
{
    auto repository = Orm4Qt::Repository::createRepository();
    QList<Person> list;
    if(repository->select<Person>(list))
    {
        m_objectModel->setDataSource(list);
    }
    else
    {
        QMessageBox::critical(this, "Error while  person", QString("An error occurred while saving a person. \n"
                              "Description: %1 \nSql Error: %2").arg(repository->lastError()->description())
                              .arg(repository->lastError()->sqlError().text()));
    }
}

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
