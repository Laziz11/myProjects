#include "mainDialog.h"

MainDialog::MainDialog(QWidget *parent)
    : QDialog(parent)
{
    // Поля изменения данных
    labelFIO = new QLabel(tr("Студент:"));
    editFIO = new QLineEdit();
    labelWins = new QLabel(tr("Wins:"));
    editWins = new QLineEdit();
    editWins->setValidator(new QIntValidator(0, 1000));

    QVBoxLayout *editLayout = new QVBoxLayout();
    editLayout->addWidget(labelFIO);
    editLayout->addWidget(editFIO);
    editLayout->addWidget(labelWins);
    editLayout->addWidget(editWins);

    // Кнопки изменения данных
    buttonSave = new QPushButton(tr("Записать"));
    buttonInsert = new QPushButton(tr("Добавить"));
    buttonDelete = new QPushButton(tr("Удалить"));

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(buttonSave);
    buttonLayout->addWidget(buttonInsert);
    buttonLayout->addWidget(buttonDelete);

    connect(buttonSave, SIGNAL(clicked()), this, SLOT(saveSlot()));
    connect(buttonInsert, SIGNAL(clicked()), this, SLOT(insertSlot()));
    connect(buttonDelete, SIGNAL(clicked()), this, SLOT(deleteSlot()));

    // Настройка таблицы
    tableStudents = new QTableView;
    myModel = new WinnersModel(&w);
    tableStudents->setModel(myModel);
    tableStudents->setSortingEnabled(false);
    tableStudents->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableStudents->horizontalHeader()->setStretchLastSection(true);
    tableStudents->verticalHeader()->hide();
    tableStudents->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableStudents->setSelectionMode(QAbstractItemView::SingleSelection);

    // Сигнал смены выделенной строки
    connect(tableStudents->selectionModel(),
        SIGNAL(selectionChanged (const QItemSelection &, const QItemSelection &)),
        this, SLOT(selectionChangedSlot(const QItemSelection &, const QItemSelection &)));

    listWinners = new QListWidget;

    // Сборка интерфейса
    QVBoxLayout *rightLayout = new QVBoxLayout();
    rightLayout->addLayout(editLayout);
    rightLayout->addLayout(buttonLayout);
    rightLayout->addWidget(listWinners);

    QHBoxLayout *mainLayout = new QHBoxLayout();
    mainLayout->addWidget(tableStudents);
    mainLayout->addLayout(rightLayout);

    setLayout(mainLayout);
}

MainDialog::~MainDialog()
{
}

void MainDialog::resetWinners() {
    winners::top t = this->w.podium();	// Победители
    listWinners->clear();
    listWinners->addItem(t.first);
    listWinners->addItem(t.second);
    listWinners->addItem(t.third);
}

void MainDialog::update() {
    this->tableStudents->selectRow(0);
    resetWinners();
}

// Смена выделенной строки
void MainDialog::selectionChangedSlot(
    const QItemSelection & /*newSelection*/,
    const QItemSelection & /*oldSelection*/)
{
    QModelIndex index = tableStudents->selectionModel()->currentIndex();

    this->key = index.sibling(index.row(), 0).data(Qt::DisplayRole).toString();

    editFIO->setText(this->key);
    editWins->setText(index.sibling(index.row(), 1).data(Qt::DisplayRole).toString());
}

// Вставка новой записи
void MainDialog::insertSlot() {
    myModel->start_update();
    this->w.students.insert(editFIO->text(), editWins->text().toInt());
    myModel->end_update();
    update();
}

// Удаляем
void MainDialog::deleteSlot() {
    myModel->start_update();
    this->w.students.remove(key);
    myModel->end_update();
    update();
}

// Корректировка
void MainDialog::saveSlot() {
    QString fio = editFIO->text();

    myModel->start_update();
    if ( this->key != fio ) {
        // Сменилась фамилия
        this->w.students.remove(key);	// Удаляем старый ключ
        this->w.students.insert(fio, editWins->text().toInt());	// Вставляем новый
    }
    else {
        // Только корректируем количество побед
        this->w.students[fio] =  editWins->text().toInt();
    }
    this->w.students.insert(editFIO->text(), editWins->text().toInt());
    myModel->end_update();

    update();
}



