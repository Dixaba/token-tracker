#include "tracker.h"

#include <QDataStream>

Tracker::Tracker(QObject *parent, QSettings *s) : QObject(parent)
{
  if (s != nullptr)
    { setSettings(s); }

  reset();
}

Tracker::~Tracker()
{
  if (settings != nullptr)
    {
      settings->sync();
    }
}

void Tracker::setSettings(QSettings *s)
{
  if (s != nullptr && s->status() == QSettings::NoError)
    { settings = s; }
  else
    { throw std::invalid_argument("Argument must point to initialized QSettings object!"); }
}

void Tracker::load()
{
  if (settings == nullptr)
    { return; }

  startDate = settings->value("startDate ").toDateTime();
  endDate = settings->value("endDate ").toDateTime();
  targetCount = settings->value("targetCount ").toInt();
  currentCount = settings->value("currentCount").toInt();
  winCount = settings->value("winCount").toInt();
  loseCount = settings->value("loseCount ").toInt();
  QByteArray bytes = settings->value("points").toByteArray();
  QDataStream stream(bytes);
  stream >> points;
}

void Tracker::save()
{
  if (settings == nullptr)
    { return; }

  settings->setValue("targetCount", targetCount);
  settings->setValue("currentCount", currentCount);
  settings->setValue("winCount", winCount);
  settings->setValue("loseCount", loseCount);
  settings->setValue("startDate", startDate);
  settings->setValue("endDate", endDate);
  QByteArray bytes;
  QDataStream stream(&bytes, QIODevice::WriteOnly);
  stream << points;
  settings->setValue("points", bytes);
  settings->sync();
}

void Tracker::reset()
{
  targetCount = 0;
  currentCount = 0;
  winCount = 0;
  loseCount = 0;
  startDate = QDateTime::currentDateTime();
  endDate = QDateTime::currentDateTime();
  points.clear();
}

void Tracker::startNewTracker(int _targetCount, int _winCount, int _loseCount,
                              QDate _startDate, QDate _endDate)
{
  targetCount = _targetCount;
  currentCount = 0;
  winCount = _winCount;
  loseCount = _loseCount;
  startDate.setDate(_startDate);
  endDate.setDate(_endDate);
  points.clear();
}

void Tracker::addWin()
{
  currentCount += winCount;
  points.append(QPointF(QDateTime::currentDateTime().toMSecsSinceEpoch(),
                        currentCount));
  emit pointsUpdated(points);
}

void Tracker::addLose()
{
  currentCount += loseCount;
  points.append(QPointF(QDateTime::currentDateTime().toMSecsSinceEpoch(),
                        currentCount));
  emit pointsUpdated(points);
}

void Tracker::setCount(int count)
{
  currentCount = count;
  points.append(QPointF(QDateTime::currentDateTime().toMSecsSinceEpoch(),
                        currentCount));
  emit pointsUpdated(points);
}

int Tracker::getWinCount()
{
  return winCount;
}

int Tracker::getLoseCount()
{
  return loseCount;
}

int Tracker::getCount()
{
  return currentCount;
}

int Tracker::getTargetCount()
{
  return targetCount;
}

const QVector<QPointF> &Tracker::getPoints()
{
  return points;
}
