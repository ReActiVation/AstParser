#ifndef LOTSDISPLAYMODEL_H
#define LOTSDISPLAYMODEL_H

#include <QObject>
#include <QAbstractTableModel>

#include "global.h"

/*
 * Class overrides QAbstractTableModel standart methods to display found lots information.
 */
class LotsDisplayModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    LotsDisplayModel(QObject *parent = nullptr);
    // Update internal container (Resets model on update).
    void updateData(std::vector<Lot> dataContainer);

private:
    int rowCount(const QModelIndex &parent) const;

    int columnCount(const QModelIndex &parent) const;

    QVariant data(const QModelIndex &index, int role) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
    // Container for storing lot information.
    std::vector<Lot> lotsContainer;
};

#endif // LOTSDISPLAYMODEL_H
