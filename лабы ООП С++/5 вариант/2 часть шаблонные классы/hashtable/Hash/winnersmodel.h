#ifndef WINNERSMODEL_H
#define WINNERSMODEL_H

#include <QObject>
#include <QAbstractTableModel>

#include "winners.h"

// Модель данных для таблицы студентов
class WinnersModel : public QAbstractTableModel
{
public:
    WinnersModel(QObject *parent = 0);
    WinnersModel(winners *w, QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    void start_update()
    {
        beginResetModel();
    }
    void end_update()
    {
        endResetModel();
    }


private:
    winners *pWinners;
};

#endif // WINNERSMODEL_H
