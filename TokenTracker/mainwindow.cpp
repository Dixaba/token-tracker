#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDateTime>
#include <QTimer>
#include <cmath>

int counter = 0;

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow), s("DixabaCorp", "TokenTracker"), tracker()
{
  QApplication::installTranslator(&translator);
  ui->setupUi(this);
  resetUIValues();
  connect(this, &MainWindow::read,
          this, &MainWindow::doneRead);
  connect(&tracker, &Tracker::pointsUpdated,
          this, &MainWindow::updateChart);
  chart = ui->chart->chart();
  tokens = new QLineSeries();
  chart->addSeries(dynamic_cast<QAbstractSeries *>(tokens));
  target = new QLineSeries();
  chart->addSeries(dynamic_cast<QAbstractSeries *>(target));
  xaxis = new QDateTimeAxis();
  xaxis->setFormat("MM/dd hh:mm");
  chart->addAxis(dynamic_cast<QAbstractAxis *>(xaxis), Qt::AlignBottom);
  dynamic_cast<QAbstractSeries *>(tokens)->attachAxis(
    dynamic_cast<QAbstractAxis *>(xaxis));
  dynamic_cast<QAbstractSeries *>(target)->attachAxis(
    dynamic_cast<QAbstractAxis *>(xaxis));
  yaxis = new QValueAxis();
  yaxis->setLabelFormat("%d");
  chart->addAxis(dynamic_cast<QAbstractAxis *>(yaxis), Qt::AlignLeft);
  dynamic_cast<QAbstractSeries *>(tokens)->attachAxis(
    dynamic_cast<QAbstractAxis *>(yaxis));
  dynamic_cast<QAbstractSeries *>(target)->attachAxis(
    dynamic_cast<QAbstractAxis *>(yaxis));
  QTimer::singleShot(10, this, &MainWindow::checkRead);
  on_comboBoxLang_currentIndexChanged(-1);
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::updateLabels()
{
  if (!tracker.isEventActive())
    { return; }

  int currentCount = tracker.getCount();
  int targetCount = tracker.getTargetCount();
  int winCount = tracker.getWinCount();
  int loseCount = tracker.getLoseCount();
  QDateTime startDate = tracker.getStartDate();
  QDateTime endDate = tracker.getEndDate();
  QDateTime now = QDateTime::currentDateTime();
  ui->spinBoxWin->setValue(winCount);
  ui->spinBoxLose->setValue(loseCount);
  ui->spinBoxTarget->setValue(targetCount);
  ui->dateStart->setDate(startDate.date());
  ui->dateEnd->setDate(endDate.date());
  int remainingTokens = targetCount - currentCount;
  bool done = remainingTokens <= 0;
  QString rTokens = done
                    ? tr("You did it!")
                    : QString::number(remainingTokens);
  ui->labelRemainingTokens->setText(rTokens);
  int remainingGames = std::ceil(remainingTokens / ((winCount + loseCount) /
                                 2.0));
  ui->labelRemainingGames->setText(done ? "" : QString::number(remainingGames));
  int remainingDays = now.secsTo(endDate) / (60 * 60 * 24);
  int remainingHours = now.secsTo(endDate) / (60 * 60) % 24;
  QString days = tr("%n day(s)", nullptr, remainingDays);
  QString hours = tr("%n hour(s)", nullptr, remainingHours);
  QString format = QString("%1, %2").arg(days, hours);
  ui->labelRemainingTime->setText(format);
  double eventProcess = 1.0 * startDate.secsTo(now) / startDate.secsTo(endDate);
  int plannedTokens = targetCount * eventProcess;
  ui->labelPlanned->setText(QString::number(plannedTokens));
  int diff = currentCount - plannedTokens;
  ui->labelDifference->setText(QString::number(diff));
  double gamesPerDay = remainingGames /
                       std::ceil(now.secsTo(endDate) / (60.0 * 60 * 24));
  ui->labelGamesPerDay->setText(QString::number(gamesPerDay, 'f', 1));
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
  tracker.load();
  ui->comboBoxLang->setCurrentIndex(tracker.getLangID());
  ui->centralwidget->setEnabled(true);
}

void MainWindow::updateChart(const QVector<QPointF> &newPoints)
{
  ui->groupBoxTokenChange->setEnabled(tracker.isEventActive());
  dynamic_cast<QWidget *>(ui->chart)->setEnabled(tracker.isEventActive());
  ui->buttonNewTarget->setEnabled(tracker.isEventActive());
  int currentCount = tracker.getCount();
  ui->spinBoxTokens->setValue(currentCount);
  int targetCount = tracker.getTargetCount();
  QDateTime endDate = tracker.getEndDate();
  xaxis->setRange(tracker.getStartDate(), endDate);
  yaxis->setRange(0, std::max(targetCount, currentCount));
  dynamic_cast<QXYSeries *>(target)->clear();
  dynamic_cast<QXYSeries *>(target)->append(
    QPointF(xaxis->min().toMSecsSinceEpoch(), 0));
  dynamic_cast<QXYSeries *>(target)->append(
    QPointF(xaxis->max().toMSecsSinceEpoch(), targetCount));
  dynamic_cast<QXYSeries *>(tokens)->replace(newPoints);
  updateLabels();
}

void MainWindow::on_buttonWin_clicked()
{
  tracker.addWin();
  ui->spinBoxTokens->setValue(tracker.getCount());
}

void MainWindow::on_buttonTokens_clicked()
{
  tracker.setCount(ui->spinBoxTokens->value());
}

void MainWindow::on_buttonLose_clicked()
{
  tracker.addLose();
  ui->spinBoxTokens->setValue(tracker.getCount());
}

void MainWindow::on_buttonNewTarget_clicked()
{
  tracker.setTargetCount(ui->spinBoxTarget->value());
}

void MainWindow::resetUIValues()
{
  ui->spinBoxWin->setValue(10);
  ui->spinBoxLose->setValue(5);
  ui->spinBoxTarget->setValue(2000);
  ui->dateStart->setDate(QDate::currentDate());
  ui->dateEnd->setDate(QDate::currentDate().addDays(14));
}

void MainWindow::on_buttonStartEvent_clicked()
{
  int targetCount = ui->spinBoxTarget->value();
  int winCount = ui->spinBoxWin->value();
  int loseCount = ui->spinBoxLose->value();
  QDate start = ui->dateStart->date();
  QDate end = ui->dateEnd->date();

  if (!(start.daysTo(end) >= 1))
    {
      QMessageBox::critical(this, tr("Wrong dates!"),
                            tr("End date must be at least one day greater than start date!"),
                            QMessageBox::Ok);
      return;
    }

  QDate today = QDate::currentDate();

  if (today < start || today > end)
    {
      QMessageBox::critical(this, tr("Wrong dates!"),
                            tr("Event must be active now!"), QMessageBox::Ok);
      return;
    }

  tracker.reset();
  tracker.startNewTracker(targetCount, winCount, loseCount, start, end);
}

void MainWindow::on_buttonUpdateStatus_clicked()
{
  updateLabels();
}

void MainWindow::on_comboBoxLang_currentIndexChanged(int index)
{
  QString translation;

  switch (index)
    {
      case 0:
      {
        // English
        translation = ":/tr/en";
        break;
      }

      case 1:
      {
        // Russian
        translation = ":/tr/ru";
        break;
      }

      default:
      {
        // English
        translation = ":/tr/en";
      }
    }

  translator.load(translation);
  ui->retranslateUi(this);
  updateLabels();
  dynamic_cast<QAbstractSeries *>(tokens)->setName(tr("Tokens"));
  dynamic_cast<QAbstractSeries *>(target)->setName(tr("Target value"));
  tracker.setLangID(index);
}
