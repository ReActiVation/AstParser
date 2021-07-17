#ifndef GLOBAL_H
#define GLOBAL_H

#include <QString>

/*
 * Lot structure describes basic information
 * that lot should contain.
 */
struct Lot
{
    QString bidNumber;
    QString bidName;
    QString startDate;
    QString endDate;
    QString startPrice;
    QString participants;
    QString zakGovLink;
    QString sberLink;
};

#endif // GLOBAL_H
