#include "detect_rect.h"

namespace pproto {
namespace data {

#ifdef OPENCV_IS_PRESENT

cv::Point DetectRect::p1(const cv::Point& offset) const
{
    return cv::Point(x_min + offset.x, y_min + offset.y);
}

void DetectRect::setP1(const cv::Point& p)
{
    x_min = p.x;
    y_min = p.y;
}

cv::Point DetectRect::p2(const cv::Point& offset) const
{
    return cv::Point(x_max + offset.x, y_max + offset.y);
}

void DetectRect::setP2(const cv::Point& p)
{
    x_max = p.x;
    y_max = p.y;
}

cv::Rect DetectRect::rect(const cv::Point& offset) const
{
    cv::Rect rect;
    rect.x = x_min + offset.x;
    rect.y = y_min + offset.y;
    rect.width  = x_max - x_min;
    rect.height = y_max - y_min;
    return rect;
}

void DetectRect::setRect(const cv::Rect& rect)
{
    x_min = rect.x;
    y_min = rect.y;
    x_max = rect.x + rect.width;
    y_max = rect.y + rect.height;
}

#endif // OPENCV_IS_PRESENT

graph::Point DetectRect::center() const
{
    graph::Point p;
    p.x = x_min + (x_max - x_min) / 2;
    p.y = y_min + (y_max - y_min) / 2;
    return p;
}

void DetectRect::Segment::toRaw(bserial::DataStream& stream) const
{
    SByteArray buff;
    buff.resize(contour.count() * sizeof(quint16) * 2);
    quint16* b = (quint16*)buff.constData();
    for (int i = 0; i < contour.count(); ++i)
    {
        *b++ = quint16(qMax(contour[i].x, 0));
        *b++ = quint16(qMax(contour[i].y, 0));
    }

    B_SERIALIZE_V1
    stream << buff;
    B_SERIALIZE_RETURN
}

void DetectRect::Segment::fromRaw(const bserial::RawVector& vect)
{
    contour.clear();
    SByteArray buff;

    B_DESERIALIZE_V1(vect)
    stream >> buff;
    B_DESERIALIZE_END

    if (buff.length())
    {
        int count = buff.length() / (sizeof(quint16) * 2);
        quint16* b = (quint16*)buff.constData();
        for (int i = 0; i < count; ++i)
        {
            qint32 x = *b++;
            qint32 y = *b++;
            contour.append({x, y});
        }
    }
}

void DetectRect::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << x_min;
    stream << y_min;
    stream << x_max;
    stream << y_max;
    stream << score;
    stream << time;
    stream << number;
    stream << segment;
    stream << classIndex;
    stream << classes;
    B_SERIALIZE_RETURN
}

void DetectRect::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> x_min;
    stream >> y_min;
    stream >> x_max;
    stream >> y_max;
    stream >> score;
    stream >> time;
    stream >> number;
    stream >> segment;
    stream >> classIndex;
    stream >> classes;
    B_DESERIALIZE_END
}

} // namespace data
} // namespace pproto
