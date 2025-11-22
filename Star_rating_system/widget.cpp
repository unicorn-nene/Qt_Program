#include "widget.h"
#include "./ui_widget.h"
#include <QInputDialog>
#include <QMessageBox>
#include "persondelegate.h"
#include "stardelegate.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    m_model = new PersonModel(this);
    PersonDelegate *personDelegate = new PersonDelegate(this);
    StarDelegate *starDelegate = new StarDelegate(this);

    ui->listView->setModel(m_model);

    ui->tableView->setModel(m_model);
    ui->tableView->setItemDelegateForColumn(2, personDelegate);
    ui->tableView->setItemDelegateForColumn(3, starDelegate);
    ui->tableView->setColumnWidth(3, 110);


    ui->treeView->setModel(m_model);
    ui->treeView->setItemDelegate(personDelegate);
    ui->treeView->setItemDelegateForColumn(3, starDelegate);



    // view syn
    ui->tableView->setSelectionModel(ui->listView->selectionModel());
    ui->treeView->setSelectionModel(ui->listView->selectionModel());

    // 展示全部列
    this->showMaximized();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_addPersonButton_clicked()
{
    bool ok;
    QString name = QInputDialog::getText(nullptr, "Names", tr("Person name:"), QLineEdit::Normal, "Type in name", &ok);

    if(ok && !name.isEmpty())
    {
        int age = QInputDialog::getInt(nullptr, "Person Age", "Age", 20, 15, 120);
        Person * person = new Person(name , "blue", age, 3,this);
        m_model->addPerson(person);
    }
    else
    {
        QMessageBox::information(nullptr, "Failure", "Must specify name and age");
    }
}


void Widget::on_removePersonButton_clicked()
{
    QModelIndex index = ui->listView->currentIndex();
    m_model->removePerson(index);   //remove the person at the current index
}

