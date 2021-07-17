#include "lotsdisplaymodel.h"

LotsDisplayModel::LotsDisplayModel(QObject *parent) : QAbstractTableModel(parent) {}

void LotsDisplayModel::updateData(std::vector<Lot> dataContainer)
{
    beginResetModel();
    this->lotsContainer.clear();
    this->lotsContainer = dataContainer;
    endResetModel();
}

int LotsDisplayModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return lotsContainer.size();
}

int LotsDisplayModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 8;
}

QVariant LotsDisplayModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if(role == Qt::DisplayRole)
    {
        if(index.column() == 0)
            return lotsContainer[index.row()].bidNumber;

        if(index.column() == 1)
            return lotsContainer[index.row()].bidName;

        if(index.column() == 2)
            return lotsContainer[index.row()].participants;

        if(index.column() == 3)
            return lotsContainer[index.row()].startPrice;

        if(index.column() == 4)
            return lotsContainer[index.row()].startDate;

        if(index.column() == 5)
            return lotsContainer[index.row()].endDate;

        if(index.column() == 6)
            return lotsContainer[index.row()].zakGovLink;

        if(index.column() == 7)
            return lotsContainer[index.row()].sberLink;
    }

    return QVariant();
}

QVariant LotsDisplayModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
          switch (section)
          {
              case 0:
                return QString("Номер");
              case 1:
                return QString("Название");
              case 2:
                return QString("Кол-во участников");
              case 3:
                return QString("Сумма контракта");
              case 4:
                return QString("Дата начала");
              case 5:
                return QString("Дата завершения");
              case 6:
                return QString("Ссылка на закупки");
              case 7:
                return QString("Ссылка на сбер");
          }
        }
    }

    return QVariant();
}
