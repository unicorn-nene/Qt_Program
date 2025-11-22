#include <QApplication>
#include <QVBoxLayout>
#include <QMainWindow>
#include <QTextEdit>
#include <QListWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QDataStream>
#include <QFile>

QMainWindow *mainWindow{};
QListWidget *notesList{};
QTextEdit *notesEditor{};
QPushButton *saveButton{};
QPushButton *deleteButton{};
QMap<QString, QString> notes{}; // <noteTitle, noteText>

void setupUI();
void saveCurrentNote();
void deleteSelectNote();
void loadNotes();
void saveNotes();
void updateNotesList();
void onNoteSelected(QListWidgetItem *item);

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    setupUI();

    mainWindow->show();

    return app.exec();
}

void setupUI()
{
    mainWindow = new QMainWindow();
    mainWindow->setWindowTitle("Notes Maker");
    mainWindow->resize(800, 600);

    QWidget *centralWiget = new QWidget();
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWiget);


    //left panel: Save notes lists, delete note, new note
    QWidget *leftPanel = new QWidget();
    QVBoxLayout *leftLayout = new QVBoxLayout(leftPanel);
    mainLayout->addWidget(leftPanel, 1);

    notesList = new QListWidget();
    leftLayout->addWidget(notesList);

    deleteButton = new QPushButton("Delete Notes");
    QObject::connect(deleteButton, &QPushButton::clicked, deleteSelectNote);
    leftLayout->addWidget(deleteButton);

    QPushButton *addNewNoteButton = new QPushButton("New Note");
    QObject::connect(addNewNoteButton, &QPushButton::clicked, []()
                     {
                         notesEditor->clear();
                         notesList->clearSelection();
                         notesEditor->setFocus();
                     });
    leftLayout->addWidget(addNewNoteButton);


    // Right panel: Note Editor, save note
    QWidget *rightPanel = new QWidget();
    QVBoxLayout *rightLayout = new QVBoxLayout(rightPanel);
    mainLayout->addWidget(rightPanel, 2);

    notesEditor = new QTextEdit();
    notesEditor->setPlaceholderText("Write down your notes here...");
    rightLayout->addWidget(notesEditor);

    saveButton = new QPushButton("Save Note");
    QObject::connect(saveButton, &QPushButton::clicked, saveCurrentNote);
    rightLayout->addWidget(saveButton);


    // mainWindow
    QObject::connect(notesList, &QListWidget::itemClicked, onNoteSelected);
    QObject::connect(mainWindow, &QMainWindow::destroyed, saveCurrentNote);

    mainWindow->setCentralWidget(centralWiget);
}

/**
 * @brief saveCurrentNote
 */
void saveCurrentNote()
{
    // 将字符串转换为纯文本形式,提取前30个字符,提出除去前后空白符,作为这行文本的标题
    QString noteTitle = notesEditor->toPlainText().left(30).trimmed();
    if(noteTitle.isEmpty())
    {
        QMessageBox::warning(mainWindow, "Error", "Note cannot be empty");
        return;
    }

    notes[noteTitle] = notesEditor->toPlainText();
    saveNotes();
    updateNotesList();
}

/**
 * @brief deleteSelectNote
 */
void deleteSelectNote()
{
    QListWidgetItem *selectedItem = notesList->currentItem();
    if(!selectedItem)
    {
        QMessageBox::warning(mainWindow, "Error", "No Note Selected");
        return;
    }

    QString title = selectedItem->text();
    notes.remove(title);
    int row = notesList->row(selectedItem);
    QListWidgetItem *deleteItem = notesList->takeItem(row);
    delete deleteItem;
    // delete notesList->takeItem(notesList->row(selectedItem);

    saveNotes();
    updateNotesList();
    notesEditor->clear();
}

/**
 * @brief onNoteSelected
 * @param item
 */
void onNoteSelected(QListWidgetItem *item)
{
    QString title = item->text();
    notesEditor->setText(notes[title]);
}

void loadNotes()
{
    QFile file("notes.dat");
    if(file.open(QIODevice::ReadOnly))
    {
        QDataStream in(&file);
        in >> notes;
        file.close();
    }
}

void saveNotes()
{
    QFile file("notes.dat");
    if(file.open(QIODevice::WriteOnly))
    {
        QDataStream out(&file);
        out << notes;
        file.close();
    }

    updateNotesList();
}

/**
 * @brief updateNotesList
 */
void updateNotesList()
{
    notesList->clear();

    for(const auto &title : notes.keys())
    {
        notesList->addItem(title);
    }
}










