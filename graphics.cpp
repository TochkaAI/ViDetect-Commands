#include "graphics.h"
#include "pproto/commands/pool.h"

namespace pproto {
namespace data {
namespace graph {

bool Identity::isNull() const
{
    return (id.isNull() && (index < 0) && name.isEmpty());
}

void Identity::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << id;
    stream << index;
    stream << name;
    stream << findField;
    B_SERIALIZE_RETURN
}

void Identity::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> id;
    stream >> index;
    stream >> name;
    stream >> findField;
    B_DESERIALIZE_END
}

void Color::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << r;
    stream << g;
    stream << b;
    B_SERIALIZE_RETURN
}

void Color::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> r;
    stream >> g;
    stream >> b;
    B_DESERIALIZE_END
}

void Brush::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << thickness;
    stream << color;
    B_SERIALIZE_RETURN
}

void Brush::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> thickness;
    stream >> color;
    B_DESERIALIZE_END
}

bool Point::isNull() const
{
    return (x == 0) && (y == 0);
}

void Point::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << x;
    stream << y;
    B_SERIALIZE_RETURN
}

void Point::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> x;
    stream >> y;
    B_DESERIALIZE_END
}

bool Size::isNull() const
{
    return (width == 0) && (height == 0);
}

void Size::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << width;
    stream << height;
    B_SERIALIZE_RETURN
}

void Size::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> width;
    stream >> height;
    B_DESERIALIZE_END
}

void Offset::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << dx;
    stream << dy;
    B_SERIALIZE_RETURN
}

void Offset::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> dx;
    stream >> dy;
    B_DESERIALIZE_END
}

#ifdef OPENCV_IS_PRESENT

cv::Point Caption::position_(const cv::Point& offset) const
{
    return cv::Point(position.x + offset.x, position.y + offset.y);
}

void Caption::setPosition(const cv::Point& p)
{
    position.x = p.x;
    position.y = p.y;
}

cv::Point Caption::offset_() const
{
    return cv::Point(offset.dx, offset.dy);
}

void Caption::setOffset(const cv::Point& p)
{
    offset.dx = p.x;
    offset.dy = p.y;
}

#endif // OPENCV_IS_PRESENT

void Caption::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << identity;
    stream << visible;
    stream << brush;
    stream << position;
    stream << offset;
    stream << scale;
    stream << text;
    stream << cornerBind;
    B_SERIALIZE_RETURN
}

void Caption::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> identity;
    stream >> visible;
    stream >> brush;
    stream >> position;
    stream >> offset;
    stream >> scale;
    stream >> text;
    stream >> cornerBind;
    B_DESERIALIZE_END
}

#ifdef OPENCV_IS_PRESENT

cv::Point Line::p1(const cv::Point& offset) const
{
    return cv::Point(point1.x + offset.x, point1.y + offset.y);
}

void Line::setP1(const cv::Point& p)
{
    point1.x = p.x;
    point1.y = p.y;
}

cv::Point Line::p2(const cv::Point& offset) const
{
    return cv::Point(point2.x + offset.x, point2.y + offset.y);
}

void Line::setP2(const cv::Point& p)
{
    point2.x = p.x;
    point2.y = p.y;
}

#endif // OPENCV_IS_PRESENT

bool Line::isNull() const
{
    return (point1.isNull() && point2.isNull());
}

void Line::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << identity;
    stream << visible;
    stream << brush;
    stream << point1;
    stream << point2;
    B_SERIALIZE_RETURN
}

void Line::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> identity;
    stream >> visible;
    stream >> brush;
    stream >> point1;
    stream >> point2;
    B_DESERIALIZE_END
}

#ifdef OPENCV_IS_PRESENT

vector<cv::Point> Polyline::points_(const cv::Point& offset) const
{
    vector<cv::Point> pnts;
    pnts.reserve(points.size());
    for (const Point& p : points)
        pnts.emplace_back(p.x + offset.x, p.y + offset.y);
    return pnts;
}

#endif // OPENCV_IS_PRESENT

bool Polyline::isNull() const
{
    for (const Point& p : points)
        if (!p.isNull())
            return false;
    return true;
}

void Polyline::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << identity;
    stream << visible;
    stream << brush;
    stream << points;
    stream << closed;
    B_SERIALIZE_RETURN
}

void Polyline::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> identity;
    stream >> visible;
    stream >> brush;
    stream >> points;
    stream >> closed;
    B_DESERIALIZE_END
}

#ifdef OPENCV_IS_PRESENT

cv::Point Rectangle::p1(const cv::Point& offset) const
{
    return cv::Point(point1.x + offset.x, point1.y + offset.y);
}

void Rectangle::setP1(const cv::Point& p)
{
    point1.x = p.x;
    point1.y = p.y;
}

cv::Point Rectangle::p2(const cv::Point& offset) const
{
    return cv::Point(point2.x + offset.x, point2.y + offset.y);
}

void Rectangle::setP2(const cv::Point& p)
{
    point2.x = p.x;
    point2.y = p.y;
}

cv::Rect Rectangle::rect(const cv::Point& offset) const
{
    cv::Rect rect;
    rect.x = point1.x + offset.x;
    rect.y = point1.y + offset.y;
    rect.width  = point2.x - point1.x;
    rect.height = point2.y - point1.y;
    return rect;
}

void Rectangle::setRect(const cv::Rect& rect)
{
    point1.x = rect.x;
    point1.y = rect.y;
    point2.x = rect.x + rect.width;
    point2.y = rect.y + rect.height;
}

#endif // OPENCV_IS_PRESENT

bool Rectangle::isNull() const
{
    return (point1.isNull() && point2.isNull());
}

Size Rectangle::size() const
{
    return {point2.x - point1.x, point2.y - point1.y};
}

void Rectangle::setSize(const Size& size)
{
    point2.x = point1.x + size.width;
    point2.y = point1.y + size.height;
}

Point Rectangle::center() const
{
    Point p;
    p.x = point1.x + (point2.x - point1.x) / 2;
    p.y = point1.y + (point2.y - point1.y) / 2;
    return p;
}

void Rectangle::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << identity;
    stream << visible;
    stream << brush;
    stream << point1;
    stream << point2;
    B_SERIALIZE_RETURN
}

void Rectangle::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> identity;
    stream >> visible;
    stream >> brush;
    stream >> point1;
    stream >> point2;
    B_DESERIALIZE_END
}

#ifdef OPENCV_IS_PRESENT

cv::Point Circle::center_(const cv::Point& offset) const
{
    return cv::Point(center.x + offset.x, center.y + offset.y);
}

void Circle::setCenter(const cv::Point& p)
{
    center.x = p.x;
    center.y = p.y;
}

#endif // OPENCV_IS_PRESENT

void Circle::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << identity;
    stream << visible;
    stream << brush;
    stream << center;
    stream << radius;
    stream << filled;
    B_SERIALIZE_RETURN
}

void Circle::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> identity;
    stream >> visible;
    stream >> brush;
    stream >> center;
    stream >> radius;
    stream >> filled;
    B_DESERIALIZE_END
}

void Set::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << identity;
    stream << visible;
    stream << printLog;
    stream << border;
    stream << captions;
    stream << lines;
    stream << polylines;
    stream << rectangles;
    stream << circles;
    B_SERIALIZE_RETURN
}

void Set::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> identity;
    stream >> visible;
    stream >> printLog;
    stream >> border;
    stream >> captions;
    stream >> lines;
    stream >> polylines;
    stream >> rectangles;
    stream >> circles;
    B_DESERIALIZE_END
}

} // namespace graph
} // namespace data
} // namespace pproto
