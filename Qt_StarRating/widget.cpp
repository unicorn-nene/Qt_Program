#include "widget.h"
#include "./ui_widget.h"
#include "personmodel.h"
#include "persondelegate.h"
#include "stardelegate.h"
#include <QInputDialog>
#include <QMessageBox>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    qDebug() << "Widget init";

    PersonDelegate *personDelegate = new PersonDelegate(this);
    StarDelegate *starDelegate = new StarDelegate(this);

    m_model = new PersonModel(this);

    ui->listView->setModel(m_model);
    ui->listView->setItemDelegate(personDelegate);

    ui->tableView->setModel(m_model);
    ui->tableView->setItemDelegate(personDelegate);
    ui->tableView->setItemDelegateForColumn(3, starDelegate);
    ui->tableView->setColumnWidth(3, 22 * 6);

    ui->treeView->setModel(m_model);
    ui->treeView->setItemDelegate(personDelegate);
    ui->treeView->setItemDelegateForColumn(3, starDelegate);

    ui->tableView->setSelectionModel(ui->listView->selectionModel());
    ui->treeView->setSelectionModel(ui->listView->selectionModel());
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_addPersonButton_clicked()
{
    bool ok{};
    QString name = QInputDialog::getText(nullptr, "Names", tr("Person name:"), QLineEdit::Normal, "Type in name", &ok);

    if(ok && !name.isEmpty())
    {
        int age = QInputDialog::getInt(nullptr, "Age", tr("Person age"), 20, 0, 120);
        Person *person = new Person(name, "blue", age, 3, this);
        m_model->addPerson(person);
    }
    else
    {
        QMessageBox::information(nullptr, "Failure","Must specify name and age");
    }
}


void Widget::on_removePersonButton_clicked()
{
    QModelIndex index = ui->tableView->currentIndex();
    m_model->removePerson(index);
}

