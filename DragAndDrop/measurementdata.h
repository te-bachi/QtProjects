#ifndef MEASUREMENTDATA_H
#define MEASUREMENTDATA_H

#include <QDateTime>
#include <QFloat16>
#include <QString>

class MeasurementData
{
    public:
        MeasurementData();
        MeasurementData(QDateTime time, qfloat16 value, QString text);
        MeasurementData(QDateTime time, float value, QString text);
        QDateTime   m_time;
        qfloat16    m_value;
        QString     m_text;
};

#endif // MEASUREMENTDATA_H
