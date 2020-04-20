#ifndef TRACKER_H
#define TRACKER_H

#include <QDateTime>
#include <QObject>
#include <QPointF>
#include <QSettings>

class Tracker : public QObject {
    Q_OBJECT
  public:
    explicit Tracker(QObject *parent = nullptr, QSettings *s = nullptr);
    ~Tracker();

    void setSettings(QSettings *s);

    void load();
    void save();
    void reset();

    void startNewTracker(int _targetCount, int _winCount, int _loseCount,
                         QDate _startDate, QDate _endDate);
    void setTargetCount(int count);

    void addWin();
    void addLose();
    void setCount(int count);

    int getWinCount();
    int getLoseCount();
    int getCount();
    int getTargetCount();
    QDateTime &getStartDate();
    QDateTime &getEndDate();
    QVector<QPointF> &getPoints();

    bool isEventActive();

    void setLangID(int lang);
    int getLangID();

  signals:
    void pointsUpdated(const QVector<QPointF> &points);

  private:
    QSettings *settings;

    QDateTime startDate, endDate;
    int targetCount, currentCount;
    int winCount, loseCount;

    QVector<QPointF> points;

    int langID;
};

#endif // TRACKER_H
