#ifndef PERSONFORM_H
#define PERSONFORM_H

#include <QDialog>
#include "person.h"

namespace Ui {
class PersonForm;
}

class PersonForm : public QDialog
{
    Q_OBJECT

public:
    explicit PersonForm(QWidget *parent = 0);
    ~PersonForm();

    Person person() const;
    void setPerson(const Person &person);

private slots:
    void updateFormData();
    void updatePersonData();
    void uploadResume();
    void downloadResume();

private:
    Ui::PersonForm *ui;
    Person m_person;
};

#endif // PERSONFORM_H
