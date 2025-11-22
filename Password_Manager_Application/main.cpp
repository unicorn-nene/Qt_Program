#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QPushButton>
#include <QMap>
#include <QPair>
#include <QInputDialog>
#include <QMessageBox>
#include <QClipboard>
#include <QCryptographicHash> // added for Pashword Hashing
#include <QFile>
#include <QDataStream>
#include <QApplication>

QMap<QString , QPair<QString, QString>> passwords{}; // <service, <username, password>>

QMainWindow * mainWindow{};
QTableWidget *passwordTable{};

QPushButton *addButton{};
QPushButton *copyButton{};
QPushButton *removeButton{};

bool vertifyMasterPassword();

void addPassword();
void copyPassword();
void removePassword();

void savePasswords();
void loadPasswords();

void setupUI();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    setupUI();
    if(vertifyMasterPassword())
    {
        loadPasswords();        // 当程序第一次打开时, 加载密码文件
        mainWindow->show();
        return a.exec();
    }
    else // 密码输入错误, 提示密码输入错误, 并关闭程序
    {
        QMessageBox::warning(nullptr, "ERROR", "PASSWORD ERROR!");
        return 0; //  退出程序循环
    }
}

void setupUI()
{
    mainWindow = new QMainWindow();
    mainWindow->setWindowTitle("Password Manager");
    mainWindow->setMinimumSize(600, 800);

    QWidget *centralWidget = new QWidget();
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    passwordTable = new QTableWidget{0, 3};
    passwordTable->setHorizontalHeaderLabels({"Service", "Username", "Password"});
    passwordTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    passwordTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    passwordTable->setSelectionBehavior(QAbstractItemView::SelectRows);

    mainLayout->addWidget(passwordTable);

    QHBoxLayout *buttonLayout = new QHBoxLayout();

    addButton = new QPushButton("Add Password");
    copyButton = new QPushButton("Copy");
    removeButton = new QPushButton("Remove Password");

    QObject::connect(addButton, &QPushButton::clicked, addPassword);
    QObject::connect(copyButton, &QPushButton::clicked, copyPassword);
    QObject::connect(removeButton, &QPushButton::clicked, removePassword);

    // 程序关闭时候, 保存密码本
    QObject::connect(QApplication::instance(), &QCoreApplication::aboutToQuit, savePasswords);

    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(copyButton);
    buttonLayout->addWidget(removeButton);

    mainLayout->addLayout(buttonLayout);


    mainWindow->setCentralWidget(centralWidget);
}


/**
 * @brief addPassword
 */
void addPassword()
{
    QString service = QInputDialog::getText(nullptr, "Add Password", "Service");
    if(service.isEmpty())
        return;

    QString username = QInputDialog::getText(nullptr, "Add Password", "Username");
    QString password = QInputDialog::getText(nullptr, "Add Password", "Password");

    passwords[service] = qMakePair(username, password);

    int row = passwordTable->rowCount();
    passwordTable->insertRow(row);
    passwordTable->setItem(row, 0 , new QTableWidgetItem(service));     // service
    passwordTable->setItem(row, 1 , new QTableWidgetItem(username));    // username
    passwordTable->setItem(row, 2 , new QTableWidgetItem("********"));  // password
}

/**
 * @brief copyPassword
 */
void copyPassword()
{
    int row = passwordTable->currentRow();

    if(row >= 0)
    {
        QString service = passwordTable->item(row, 0)->text();
        QApplication::clipboard()->setText(passwords[service].second);
        QMessageBox::information(nullptr, "Copied", "Password copied to clipboard");
    }
}

/**
 * @brief removePassword
 */
void removePassword()
{
    int row = passwordTable->currentRow();
    QString service = passwordTable->item(row, 0)->text();
    passwords.remove(service);
    passwordTable->removeRow(row);
}

/**
 * @brief vertifyMasterPassword 主密码验证函数
 * @return
 */
bool vertifyMasterPassword()
{

    QString input = QInputDialog::getText(nullptr, "Master Password", "Enter Master Password", QLineEdit::Password);

    if(input.isEmpty())
        return false;

    // 对预设的密码进行 Sha256 加密
    QString storeHash = QCryptographicHash::hash(QString("123456").toUtf8(), QCryptographicHash::Sha256).toHex();

    // 对输入的密码进行 Sha256 加密
    QString inputHash = QCryptographicHash::hash(input.toUtf8(), QCryptographicHash::Sha256).toHex();

    // 比较两个密码是否相同
    return storeHash == inputHash;
}


/**
 * @brief savePasswords
 */
void savePasswords()
{
    // data = Gmail | user1@gmail.com | 123456 + "\n"
    QString data;

    for(auto it = passwords.begin(); it != passwords.end(); ++it)
    {
        data += it.key() + "|" + it.value().first + "|" + it.value().second + "\n";
    }

    QFile file("password.dat");

    if(file.open(QIODevice::WriteOnly))
    {
        QDataStream out(&file);
        out << data;
        file.close();
    }
}

/**
 * @brief loadPasswords
 */
void loadPasswords()
{
    QFile file("password.dat");

    if(file.open(QIODevice::ReadOnly))
    {
        QDataStream in(&file);
        QString data;
        in >> data;

        QStringList entries = data.split('\n');

        for(const QString &entry : entries)
        {
            QStringList parts = entry.split('|');

            if(parts.size() == 3)
            {
                // service | username | password
                passwords[parts[0]] = qMakePair(parts[1], parts[2]);

                int row = passwordTable->rowCount();
                passwordTable->insertRow(row);
                passwordTable->setItem(row, 0, new QTableWidgetItem(parts[0]));     // serivce
                passwordTable->setItem(row, 1, new QTableWidgetItem(parts[1]));     // username
                passwordTable->setItem(row, 2, new QTableWidgetItem("********"));   // password
            }
        }

        file.close();
    }
}





















