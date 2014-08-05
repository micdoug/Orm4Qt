#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ObjectModel.h"
#include "person.h"
#include <QSortFilterProxyModel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void configureDatabaseConnection();
    void createTable();
    void addPerson();
    void editPerson();
    void deletePerson();
    void selectPeople();

private:
    Ui::MainWindow *ui;
    QSortFilterProxyModel *m_proxyModel;
    ObjectModel<Person> *m_objectModel;

    void savePerson(Person person);
    void deletePerson(Person person);
};

#endif // MAINWINDOW_H
