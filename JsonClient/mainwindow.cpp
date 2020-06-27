#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    label = new QLabel(this);
    label->setText("Result: ");
    label->setWordWrap(true);

    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(readyRead()), this, SLOT(socketReady()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(socketDisconnected()));


    socket->connectToHost("127.0.0.1", 12345);
    socket->write("{\"data\":"
                  "["
                  "{\"rectDescription1\":\"10,10,450,230\"},"
                  "{\"rectDescription2\":\"50,12,40,204\"}"
                  "]"
                  "}");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::socketReady()
{
    QJsonParseError err;
    data = socket->readAll();

    QJsonDocument doc = QJsonDocument::fromJson(data,&err);

    if (err.errorString().toInt() != QJsonParseError::NoError)
    {
        qDebug() << "Error reading json";
        return;
    }

    QJsonObject jObj = doc.object();
    QString resultMessage = jObj.value("result").toString();

    label->setText("Result: " + resultMessage);
    label->setAlignment(Qt::AlignCenter);
}

void MainWindow::socketDisconnected()
{
    qDebug() << "Disconnected";
    socket->deleteLater();
}

