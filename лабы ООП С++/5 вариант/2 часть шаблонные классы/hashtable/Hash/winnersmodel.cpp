#include "winnersmodel.h"


WinnersModel::WinnersModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

WinnersModel::WinnersModel(winners *w, QObject *parent)
    : QAbstractTableModel(parent)
{
    pWinners = w;
}

// Количество строк
int WinnersModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return pWinners->students.count();
}

// Количество колонок
int WinnersModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 2;
}

// Данные строки
QVariant WinnersModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= pWinners->students.count() || index.row() < 0)
        return QVariant();

    if (role == Qt::DisplayRole) {
        WinHash::iterator it = pWinners->students.begin();	// Первая запись
        for( int i = 0; i < index.row(); i++ )
            ++it;	// Перемещаемся

        if (index.column() == 0)
            return it.key();
        else if (index.column() == 1)
            return it.value();
    }
    return QVariant();
}

// Заголовки колонок
QVariant WinnersModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return tr("ФИО студента");
        case 1:
            return tr("Кол-во побед");
        default:
            return QVariant();
        }
    }
    return QVariant();
}
