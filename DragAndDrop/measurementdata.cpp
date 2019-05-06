#include "measurementdata.h"

MeasurementData::MeasurementData()
{

}

MeasurementData::MeasurementData(QDateTime time, float value, QString text) :
    m_time(time),
    m_value(value),
    m_text(text)
{
    //
}
