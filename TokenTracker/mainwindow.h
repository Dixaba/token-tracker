#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QtCharts>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

using namespace QtCharts;

class MainWindow : public QMainWindow {
    Q_OBJECT

  public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

  signals:
    void read();

  private slots:
    void checkRead();
    void doneRead();

  private:
    Ui::MainWindow *ui;
    QSettings s;

    QChart *chart;
    QLineSeries *tokens;
    QDateTimeAxis *xaxis;
    QValueAxis *yaxis;
};
#endif // MAINWINDOW_H
