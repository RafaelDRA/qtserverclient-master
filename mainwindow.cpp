#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>


MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{

  ui->setupUi(this);
  socket = new QTcpSocket(this);

  //linka o botão start com o slot putData
  connect(ui->pushButtonPut,
          SIGNAL(clicked(bool)),
          this,
          SLOT(start()));

  //linka o botão connect com o slot getIP
  connect(ui->pushButtonConnect,
          SIGNAL(clicked(bool)),
          this,
          SLOT(getIP()));

  //linka o botão disconnect com o slot disconnectS
  connect(ui->pushButtonDisconnect,
          SIGNAL(clicked(bool)),
          this,
          SLOT(disconnectS()));

  // linka o botão stop com o slot Stop
  connect(ui->pushButtonStop,
          SIGNAL(clicked(bool)),
          this,
          SLOT(Stop()));

  // conecta o hprizontal slider com o slot setVelocidade
  connect(ui->horizontalSliderFreq,
          SIGNAL(valueChanged(int)),
          this,
          SLOT(setVelocidade(int)));

  delay = 0;
  frequencia = 0;
}

// conecta o cliente ao servidor especificado
void MainWindow::tcpConnect(QString str){
  socket->connectToHost(str,1234);
  qDebug() << str;
  if(socket->waitForConnected(3000)){
    qDebug() << "Connected";
  }
  else{
    qDebug() << "Disconnected";
  }
}

//copia o texto da caixa de texto para uma qstring e conecta
// no servidor indicado por ela
void MainWindow::getIP()
{
    QString str= ui->lineEditIP->text();
    tcpConnect(str);
}

// define a velocidade de obtenção de dados
void MainWindow::setVelocidade(int freq)
{
    this->frequencia = freq;
}

//desconecta o cliente do servidor
void MainWindow::disconnectS()
{
    socket->disconnectFromHost();
    qDebug() << "Disconnected";
}

//inicia a produção de dados
void MainWindow::start()
{
    if (!flag && socket->state() == QAbstractSocket::ConnectedState){
    timer = startTimer(10);
    flag = true;
    }
}

//Para a obtenção de dados parando o timer
void MainWindow::Stop()
{
    killTimer(timer);
    flag = false;
}

//envia dados para o servidor com datahora de acordo com a frequencia ajustada
void MainWindow::timerEvent(QTimerEvent *e)
{
    delay += frequencia;
    if (delay>=100 && socket->state() == QAbstractSocket::ConnectedState && ui->lcdNumberMax->value() > 0){
        QDateTime datetime;
        QString str;
        datetime = QDateTime::currentDateTime();
        str = "set "+
                datetime.toString(Qt::ISODate)+
                " "+
                QString::number(qrand()%int(ui->lcdNumberMax->value() - ui->lcdNumberMin->value()) + ui->lcdNumberMin->value());
        dataL.push_back(str);
        ui->plainTextEdit->appendPlainText(str);
        qDebug() << socket->write(str.toStdString().c_str()) << " bytes written";
        delay = 0;
    }
}

//destrutor da janela
MainWindow::~MainWindow()
{
  delete socket;
  delete ui;
}
