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

  protected:

  private slots:
    void checkRead();
    void doneRead();
    void updateChart(const QVector<QPointF> &newPoints);
    void on_buttonWin_clicked();
    void on_buttonTokens_clicked();
    void on_buttonLose_clicked();
    void on_buttonNewTarget_clicked();
    void on_buttonStartEvent_clicked();
    void on_buttonUpdateStatus_clicked();
    void on_comboBoxLang_currentIndexChanged(int index);

  private:
    Ui::MainWindow *ui;
    QSettings s;

    Tracker tracker;

    QChart *chart;
    QLineSeries *tokens, *target;
    QDateTimeAxis *xaxis;
    QValueAxis *yaxis;

    void resetUIValues();
    void updateLabels();

    QTranslator translator;
};
#endif // MAINWINDOW_H
