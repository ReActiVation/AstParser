#include "informerwindow.h"
#include "ui_informerwindow.h"

#include "xmlhelper.h"
#include "models/lotsdisplaymodel.h"

InformerWindow::InformerWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::InformerWindow)
{
    ui->setupUi(this);

    setModels();
    resizeTables();
}

InformerWindow::~InformerWindow()
{
    delete ui;
}

void InformerWindow::setModels()
{
    tableModel = new LotsDisplayModel(this);

    ui->mainView->setModel(tableModel);
}

void InformerWindow::resizeTables()
{
    ui->mainView->setColumnWidth(0, 200);
    ui->mainView->setColumnWidth(1, 250);
    ui->mainView->setColumnWidth(2, 150);
    ui->mainView->setColumnWidth(3, 120);
    ui->mainView->setColumnWidth(4, 150);
    ui->mainView->setColumnWidth(5, 150);
    ui->mainView->setColumnWidth(6, 350);
    ui->mainView->setColumnWidth(7, 350);
}

void InformerWindow::prepareManager()
{
    manager = new QNetworkAccessManager(this);

    connect(manager, &QNetworkAccessManager::finished,
            this, &InformerWindow::onResponse);

    connect(manager, &QNetworkAccessManager::finished,
            manager, &QNetworkAccessManager::deleteLater);
}

bool InformerWindow::validateSearchString(const QString &string)
{
    QRegExp regExp("\\d*");

    if(regExp.exactMatch(string)) return true;

    return false;
}

void InformerWindow::onResponse(QNetworkReply *reply)
{
    // Clear previous response data
    responseData.clear();
    // Read response data
    QByteArray response = reply->readAll();

    Lot lot;
    // Convert reponse data from percent encoding
    responseData = responseData.fromPercentEncoding(response);

    try{
        // Set lot from read XML data
        lot = XmlHelper::parseResponse(responseData);
    } catch (...){
        QMessageBox::critical(this, "Ошибка!", "Не удалось распарсить ответ!", QMessageBox::Ok);
    }

    // If lot has some data add lot to container and update display model
    if(!lot.bidNumber.isEmpty()){
        lotsContainer.push_back(lot);
        tableModel->updateData(lotsContainer);
    }
    else{
        QMessageBox::critical(this, "Ошибка!", "Не удалось распарсить ответ!", QMessageBox::Ok);
    }
}

void InformerWindow::sendRequest()
{
    // Show error message if lot number is invalid
    if(!validateSearchString(ui->searchEdit->text().trimmed())){
        QMessageBox::critical(this, "Ошибка!", "Номер закупки должен состоять только из цифр!", QMessageBox::Ok);
        return;
    }

    // If filter successfully written to template
    if(XmlHelper::writeFilter(ui->searchEdit->text().trimmed())){
        // Create network manager
        prepareManager();

        try{
            // Create request url
            QUrl url("https://www.sberbank-ast.ru/SearchQuery.aspx?name=Main", QUrl::ParsingMode::TolerantMode);

            // Prepare XML payload data
            QByteArray request_data;
            request_data.append(XmlHelper::getXmlData());
            request_data = QUrl::toPercentEncoding(request_data);
            request_data.prepend("xmlData=");
            request_data.append("&orgId=0&targetPageCode=ESPurchaseList");
            request_data.replace("%2B", "+");

            //Create network request for URL
            QNetworkRequest request(url);

            // Add request headers
            request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
            request.setHeader(QNetworkRequest::ContentDispositionHeader, "form-data; name=" + request_data);
            request.setHeader(QNetworkRequest::ContentLengthHeader, QString::number(request_data.length()).toUtf8());

            // Send post request with payload data
            manager->post(request, request_data);
        }catch(...){
            QMessageBox::critical(this, "Ошибка!", "Невозможно установить соединение!", QMessageBox::Ok);
            return;
        }
    }else{
        QMessageBox::critical(this, "Ошибка!", "Не удалось записать фильтры!", QMessageBox::Ok);
        return;
    }
}

void InformerWindow::on_searchBtn_clicked()
{
    sendRequest();
}

void InformerWindow::on_clearTableBtn_clicked()
{
    lotsContainer.clear();
    tableModel->updateData(lotsContainer);
}

void InformerWindow::keyPressEvent(QKeyEvent *e)
{
    // Handles Ctrl + C key sequence events on table cells
    if(e->type() == QKeyEvent::KeyPress && e->matches(QKeySequence::Copy))
    {
        QItemSelectionModel * selection = ui->mainView->selectionModel();
        QModelIndexList indexes = selection->selectedIndexes();
        QModelIndex index = indexes[0];

        QVariant data = ui->mainView->model()->data(index);

        QApplication::clipboard()->setText(data.toString().trimmed());
    }
}
