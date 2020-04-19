#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QtCharts>

#include "tracker.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

using namespace QtCharts;

class MainWindow : public QMainWindow {
    Q_OBJECT

  public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

  signals:
    void read();

  private slots:
    void checkRead();
    void doneRead();
    void updateChart(const QVector<QPointF> &newPoints);

  private:
    Ui::MainWindow *ui;
    QSettings s;

    Tracker tracker;

    QChart *chart;
    QLineSeries *tokens, *target;
    QDateTimeAxis *xaxis;
    QValueAxis *yaxis;
};
#endif // MAINWINDOW_H
