#ifndef CONFIGUREDATABASEFORM_H
#define CONFIGUREDATABASEFORM_H

#include <QDialog>

namespace Ui {
class ConfigureDatabaseForm;
}

class ConfigureDatabaseForm : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigureDatabaseForm(QWidget *parent = 0);
    ~ConfigureDatabaseForm();

private slots:
    void chooseFile();
    void configureDatabase();

private:
    Ui::ConfigureDatabaseForm *ui;
};

#endif // CONFIGUREDATABASEFORM_H
