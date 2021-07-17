#ifndef INFORMERWINDOW_H
#define INFORMERWINDOW_H

#include <QObject>
#include <QtNetwork>
#include <QKeyEvent>
#include <QClipboard>
#include <QMessageBox>
#include <QMainWindow>
#include <QApplication>

#include "global.h"

class LotsDisplayModel;

QT_BEGIN_NAMESPACE
namespace Ui { class InformerWindow; }
QT_END_NAMESPACE

/*
 * Class implements actions for UI controls
 *
 * This window expects lot number as input data.
 * On find button click lot number is inserted to XML filter template,
 * and then used as payload for request to search lot on sberbank-ast.
 * After casting response data to XML doc and parsing results.
 */
class InformerWindow : public QMainWindow
{
    Q_OBJECT

public:
    InformerWindow(QWidget *parent = nullptr);
    ~InformerWindow();

    // Initilizes lot table model and sets it to table view.
    void setModels();
    // Sets rules fot column and row sizes.
    void resizeTables();
    // Sends search request to sberbank ast.
    void sendRequest();
    // Initializes network access manager.
    void prepareManager();
    // Validates lot number.
    bool validateSearchString(const QString &string);

public slots:
    // On request response handler.
    void onResponse(QNetworkReply *reply);

private slots:
    // Search button click handler.
    void on_searchBtn_clicked();
    // Clear table view button handler.
    void on_clearTableBtn_clicked();
protected:
    // Key press override for hotkeys handling.
    void keyPressEvent(QKeyEvent *e) override;

private:
    Ui::InformerWindow *ui;
    // Byte Array thats stores response data after request is completed.
    QByteArray responseData;
    // Network access manager pointer (initialized on demand).
    QNetworkAccessManager *manager {nullptr};
    // Table model (initialized in constructor).
    LotsDisplayModel *tableModel {nullptr};
    // Vector for storing found lots.
    std::vector<Lot> lotsContainer;
};
#endif // INFORMERWINDOW_H
