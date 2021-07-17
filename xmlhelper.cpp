#include "xmlhelper.h"

QByteArray XmlHelper::getXmlData()
{
    // Resulting array of xml file data
    QByteArray result;
    // Template file stored in misc directory with name "filter.xml"
    QFile file(QDir::currentPath() + QDir::separator() + "misc" + QDir::separator() + "filter.xml");

    if(!file.open(QIODevice::ReadOnly)) return result;

    QTextStream stream(&file);
    QString data = stream.readAll();
    file.close();

    data.replace("\n", "");
    data.replace(" ", "");

    result = data.trimmed().toUtf8();

    return result;
}

Lot XmlHelper::parseResponse(const QByteArray &response)
{
    // Create lot structure
    Lot lot;

    // Parse JSON response data
    QJsonDocument json_doc = QJsonDocument::fromJson(response);

    // If request failed returns empty lot
    if(json_doc["result"].toString().trimmed() != "success") return lot;

    // Find data section of JSON response
    QByteArray sub_json_data = json_doc["data"].toString().toUtf8();
    QJsonDocument sub_json_doc = QJsonDocument::fromJson(sub_json_data);

    // Get lot XML info from JSON response
    QString xml_data = sub_json_doc["tableXml"].toString();

    // Create XML document
    QDomDocument xml_doc;

    // Set content from response data
    xml_doc.setContent(xml_data);

    // Find document node
    QDomNode root = xml_doc.firstChild();

    // Find all child nodes
    QDomNodeList rootNodes = root.childNodes();

    // Create target nodes object containing info of each hit
    QDomNodeList targetNodes;

    // Parse hit nodes
    for(int i = 0; i < rootNodes.count(); i++){
        if(rootNodes.at(i).nodeName() == "hits")
            targetNodes = rootNodes.at(i).childNodes();
    }

    // Create lot nodes object containing info of found lot
    QDomNodeList lotNodes;

    // Parse lot nodes
    for(int i = 0; i < targetNodes.count(); i++){
        if(targetNodes.at(i).nodeName() == "_source")
            lotNodes = targetNodes.at(i).childNodes();
    }

    // Loop through lot nodes and read data about lot
    for(int i = 0; i < lotNodes.count(); i++){
        QDomNode node = lotNodes.at(i);

        if(node.nodeName() == "AuctionBeginDate")
            lot.startDate = node.toElement().text();

        if(node.nodeName() == "AuctionEndDate")
            lot.endDate = node.toElement().text();

        if(node.nodeName() == "BidName")
            lot.bidName = node.toElement().text();

        if(node.nodeName() == "CreateRequestHrefTerm")
            lot.sberLink = node.toElement().text();

        if(node.nodeName() == "OOSHref")
            lot.zakGovLink = node.toElement().text();

        if(node.nodeName() == "purchCode")
            lot.bidNumber = node.toElement().text();

        if(node.nodeName() == "requestCount")
            lot.participants = node.toElement().text();

        if(node.nodeName() == "purchAmount")
            lot.startPrice = node.toElement().text();
    }

    return lot;
}

bool XmlHelper::writeFilter(const QString &searchString)
{
    // Find template xml file
    QFile file(QDir::currentPath() + QDir::separator() + "misc" + QDir::separator() + "filter.xml");

    QStringList result;

    //Create XML document object
    QDomDocument xml_doc;

    // Opens file and sets XML content if successfull
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        if(!xml_doc.setContent(&file)){
            file.close();
            return false;
        }

        file.close();
    }else{
        return false;
    }

    // Find document node.
    QDomNode root = xml_doc.firstChild();

    if(root.isNull()) return false;

    // Find filter info nodes
    QDomNode filters = root.firstChild();;

    if(filters.isNull()) return false;

    // Find each filter parameter nodes
    QDomNodeList params = filters.childNodes();

    if(params.count() == 0) return false;

    // Loop through filter parameters nodes and set lot number for search
    for(int i = 0; i < params.count(); i++){
        QDomNode node = params.at(i);

        if(node.toElement().tagName() == "mainSearchBar"){
            QDomElement child = node.firstChild().toElement();

            QDomElement newNodeTag = xml_doc.createElement(QString("value"));
            QDomText newNodeText = xml_doc.createTextNode(QString(searchString).trimmed().replace(" ", "+"));
            newNodeTag.appendChild(newNodeText);

            node.replaceChild(newNodeTag, child);
        }
    }


    if(file.open(QIODevice::WriteOnly | QIODevice::Truncate)){
        file.write(xml_doc.toByteArray());

        file.close();
    }else{
        return false;
    }

    return true;
}

XmlHelper::XmlHelper()
{

}
