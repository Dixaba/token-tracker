#include "tracker.h"

Tracker::Tracker(QObject *parent, QSettings *s) : QObject(parent)
{
  if (s != nullptr)
    { setSettings(s); }
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
}

void Tracker::save()
{
  if (settings == nullptr)
    { return; }
}

void Tracker::reset()
{
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
}

void Tracker::addWin()
{
  currentCount += winCount;
}

void Tracker::addLose()
{
  currentCount += loseCount;
}

void Tracker::setCount(int count)
{
  currentCount = count;
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
