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
    explicit PersonForm(QWidget *parent = 0, const Person &person = Person());
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
