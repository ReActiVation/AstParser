#ifndef XMLHELPER_H
#define XMLHELPER_H

#include <QDir>
#include <QFile>
#include <QtXml>
#include <QObject>
#include <QByteArray>
#include <QTextStream>

#include "global.h"

/*
 * Class helps to interact with XML search template.
 */
class XmlHelper
{
public:
    // Read data from XML file.
    static QByteArray getXmlData();
    // Parse post request response data.
    static Lot parseResponse(const QByteArray &response);
    // Writes lot number to XML filter template.
    static bool writeFilter(const QString &searchString);

private:
    // Private constructor for static class.
    XmlHelper();
};

#endif // XMLHELPER_H
