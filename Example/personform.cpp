#include "personform.h"
#include "ui_personform.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QDataStream>

PersonForm::PersonForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PersonForm)
{
    ui->setupUi(this);

    connect(ui->resumeDownloadButton, SIGNAL(clicked()), this, SLOT(downloadResume()));
    connect(ui->resumeUploadButton, SIGNAL(clicked()), this, SLOT(uploadResume()));
}

PersonForm::~PersonForm()
{
    delete ui;
}
Person PersonForm::person() const
{
    return m_person;
}

void PersonForm::setPerson(const Person &person)
{
    m_person = person;
    updateFormData();
}

void PersonForm::updateFormData()
{
    ui->nameEdit->setText(m_person.name());
    ui->emailEdit->setText(m_person.email());
    ui->birthdayEdit->setDate(m_person.birthday());
    ui->balanceEdit->setValue(m_person.balance());
    ui->ageEdit->setValue(m_person.age());
    ui->resumeDownloadButton->setEnabled(!m_person.resume().isNull());
}

void PersonForm::updatePersonData()
{
    m_person.setName(ui->nameEdit->text());
    m_person.setEmail(ui->emailEdit->text());
    m_person.setBirthday(ui->birthdayEdit->date());
    m_person.setBalance(ui->balanceEdit->value());
    m_person.setAge(ui->ageEdit->value());
}

void PersonForm::uploadResume()
{
    if(!m_person.resume().isNull())
    {
        int response = QMessageBox::question(this, "Replacing resumé",
                              "This person already has a resumé. Do you want to replace it?", QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
        if(response == QMessageBox::No)
            return;
    }
    QString filename = QFileDialog::getOpenFileName(this, "Choose the resumé file", QDir::homePath(), "PDF(*.pdf)");
    if(!filename.isNull())
    {
        QFile file(filename);
        if(file.open(QIODevice::ReadOnly))
        {
            m_person.setResume(file.readAll());
            ui->resumeDownloadButton->setEnabled(!m_person.resume().isNull());
            file.close();
        }
        else
        {
            QMessageBox::critical(this, "Can't open the file", "An error occurred while reading the file. The resumé wasn't updated.");
        }
    }
}

void PersonForm::downloadResume()
{
    QString filename = QFileDialog::getSaveFileName(this, "Choose where to save the resumé", QDir::homePath(), "PDF (*.pdf)");
    if(!filename.isNull())
    {
        QFile file(filename);
        if(file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        {
            QDataStream stream(&file);
            stream << m_person.resume();
            file.close();
        }
        else
        {
            QMessageBox::critical(this, "Can't save the resumé", "An error occurred while saving the resumé.");
        }
    }
}

