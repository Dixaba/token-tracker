#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDateTime>
#include <QTimer>

int counter = 0;

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow), s("DixabaCorp", "TokenTracker"), tracker()
{
  ui->setupUi(this);
  connect(this, &MainWindow::read,
          this, &MainWindow::doneRead);
  chart = ui->chart->chart();
  tokens = new QLineSeries();
  tokens->setName("Tokens");
  chart->addSeries(dynamic_cast<QAbstractSeries *>(tokens));
  xaxis = new QDateTimeAxis();
  xaxis->setTickCount(5);
  xaxis->setFormat("MM/dd hh:mm");
  chart->addAxis(dynamic_cast<QAbstractAxis *>(xaxis), Qt::AlignBottom);
  dynamic_cast<QAbstractSeries *>(tokens)->attachAxis(
    dynamic_cast<QAbstractAxis *>(xaxis));
  yaxis = new QValueAxis();
  yaxis->setLabelFormat("%d");
  chart->addAxis(dynamic_cast<QAbstractAxis *>(yaxis), Qt::AlignLeft);
  dynamic_cast<QAbstractSeries *>(tokens)->attachAxis(
    dynamic_cast<QAbstractAxis *>(yaxis));
  QTimer::singleShot(10, this, &MainWindow::checkRead);
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::checkRead()
{
  if (s.status() != QSettings::NoError)
    {
      QTimer::singleShot(10, this, &MainWindow::checkRead);
      counter++;
    }
  else
    {
      emit read();
    }
}

void MainWindow::doneRead()
{
  tracker.setSettings(&s);
  ui->centralwidget->setEnabled(true);
}
