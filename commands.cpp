#include "commands.h"
#include "pproto/commands/pool.h"

namespace pproto {
namespace command {

#define REGISTRY_COMMAND_SINGLPROC(COMMAND, UUID) \
    const QUuidEx COMMAND = command::Pool::Registry{UUID, #COMMAND, false};

#define REGISTRY_COMMAND_MULTIPROC(COMMAND, UUID) \
    const QUuidEx COMMAND = command::Pool::Registry{UUID, #COMMAND, true};

REGISTRY_COMMAND_SINGLPROC(ConfigChanged,            "aaa0324a-aca7-4ec5-b94e-ada1716df083")
REGISTRY_COMMAND_SINGLPROC(ModelInfo,                "d8f7f3f5-2965-4b1a-9508-3e18f85d0fcc")
REGISTRY_COMMAND_SINGLPROC(TrackersInfo,             "d6272e50-b3e7-4d7d-b45e-e22440c78315")
REGISTRY_COMMAND_SINGLPROC(TrackerEvent,             "50dc6d0c-a0aa-48e6-81be-a10e2998dee3")
REGISTRY_COMMAND_SINGLPROC(TrackersEventsInfo,       "04bd5a2f-c298-4aac-90a9-00b158eae8f6")
REGISTRY_COMMAND_SINGLPROC(TrackersEventsSubscript,  "33bac850-3f26-4816-b1a1-055e02921227")
REGISTRY_COMMAND_SINGLPROC(TrackersSnapshot,         "0d629902-bc81-4ace-a5c0-7cab38ec240d")
REGISTRY_COMMAND_SINGLPROC(TrackRectsInfo,           "0bc0246f-ef01-42a5-adb2-c724a8ea9327")
REGISTRY_COMMAND_SINGLPROC(TrackRectsChanged,        "59188800-949b-4646-b369-ad358eb217f4")
REGISTRY_COMMAND_SINGLPROC(TrackRectsDynamicInfo,    "3303e9c6-2f9e-452f-92ef-eda8cf179620")
REGISTRY_COMMAND_SINGLPROC(TrackRectsDynamicUpdate,  "a8791397-3e64-453c-9b4f-4c220384e496")
REGISTRY_COMMAND_SINGLPROC(TrackRectsDynamicRemove,  "c151c1d0-3ce9-4617-8861-2a8c352d1e19")
REGISTRY_COMMAND_SINGLPROC(TrackRectsDynamicChanged, "b0b6662d-a260-4c7a-8ab4-c0b05bcf49b2")
REGISTRY_COMMAND_SINGLPROC(TrackGlobalIdUpdate,      "e0e06094-7b7a-4b63-94dd-d4c7f784b622")
REGISTRY_COMMAND_SINGLPROC(GraphsetInfo,             "53aedbb1-ee08-4c01-8f45-499e5c099a68")
REGISTRY_COMMAND_SINGLPROC(GraphsetChanged,          "d184366a-a48e-43a6-8c7e-8561c9a43184")
REGISTRY_COMMAND_SINGLPROC(GraphsetDynamicInfo,      "b4fdbb1c-dfdf-4b25-8479-49b0902fe9e0")
REGISTRY_COMMAND_SINGLPROC(GraphsetDynamicUpdate,    "056b6217-5919-4578-8b30-7c80bb52e8df")
REGISTRY_COMMAND_SINGLPROC(GraphsetDynamicRemove,    "164cde90-b098-4e7b-9927-f1945c91c62c")
REGISTRY_COMMAND_SINGLPROC(GraphsetDynamicChanged,   "85072983-a893-48b4-a916-d7a362aea537")
REGISTRY_COMMAND_SINGLPROC(GraphsetVisible,          "dac925e7-8abe-49cb-98d3-15535131850a")
REGISTRY_COMMAND_SINGLPROC(RoisInfo,                 "49ee172b-2954-4f90-9965-de3804fe3f14")
REGISTRY_COMMAND_SINGLPROC(FpFnInfo,                 "0db958aa-d392-4157-9ed8-901f258e69d9")
REGISTRY_COMMAND_SINGLPROC(SaverInfo,                "774271e3-e3c5-40ff-b9e4-b9ec822eef18")
REGISTRY_COMMAND_SINGLPROC(SaverBufferInfo,          "8e7012ea-c237-426c-a6b0-8c8b1acfd7c1")
REGISTRY_COMMAND_SINGLPROC(SaverStart,               "b0340619-0513-4ca1-9149-99b3704c6d51")
REGISTRY_COMMAND_SINGLPROC(SaverStop,                "bd5b7bdf-4fa2-4a9b-8e2c-f3df59490303")
REGISTRY_COMMAND_SINGLPROC(SetSaverBuffer,           "5a01508d-16ae-468a-a872-59729a50f3cb")

#undef REGISTRY_COMMAND_SINGLPROC
#undef REGISTRY_COMMAND_MULTIPROC

} // namespace command

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

void Roi::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << identity;
    stream << visible;
    stream << caption;
    stream << border;
    stream << inverse;
    B_SERIALIZE_RETURN
}

void Roi::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> identity;
    stream >> visible;
    stream >> caption;
    stream >> border;
    stream >> inverse;
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

void ConfigChanged::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << file;
    stream << phase;
    B_SERIALIZE_RETURN
}

void ConfigChanged::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> file;
    stream >> phase;
    B_DESERIALIZE_END
}

void FrameCrop::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << active;
    stream << border;
    B_SERIALIZE_RETURN
}

void FrameCrop::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> active;
    stream >> border;
    B_DESERIALIZE_END
}

void ModelClass::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << index;
    stream << name;
    stream << skip;
    stream << cuttingThresh;
    stream << scoreThresh;
    stream << nmsThresh;
    B_SERIALIZE_RETURN
}

void ModelClass::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> index;
    stream >> name;
    stream >> skip;
    stream >> cuttingThresh;
    stream >> scoreThresh;
    stream >> nmsThresh;
    B_DESERIALIZE_END
}

void ModelInfo::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << file;
    stream << type;
    stream << typeStr;
    stream << imageChannels;
    stream << inputSize;
    stream << input;
    stream << output;
    stream << recordCount;
    stream << frameCrop;
    stream << classes;
    stream << path;
    B_SERIALIZE_RETURN
}

void ModelInfo::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> file;
    stream >> type;
    stream >> typeStr;
    stream >> imageChannels;
    stream >> inputSize;
    stream >> input;
    stream >> output;
    stream >> recordCount;
    stream >> frameCrop;
    stream >> classes;
    stream >> path;
    B_DESERIALIZE_END
}

void ModelInfoA::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << value;
    B_SERIALIZE_RETURN
}

void ModelInfoA::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> value;
    B_DESERIALIZE_END
}

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

void DetectRect::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << x_min;
    stream << y_min;
    stream << x_max;
    stream << y_max;
    stream << score;
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
    B_DESERIALIZE_END
}

void TrackMatching::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << type;
    stream << iou;
    stream << atd;
    B_SERIALIZE_RETURN
}

void TrackMatching::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> type;
    stream >> iou;
    stream >> atd;
    B_DESERIALIZE_END
}

void TrackMatching::Iou::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << threshold;
    B_SERIALIZE_RETURN
}

void TrackMatching::Iou::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> threshold;
    B_DESERIALIZE_END
}

void TrackMatching::Atd::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << axis;
    stream << x1;
    stream << x2;
    stream << x3;
    stream << y1;
    stream << y2;
    stream << y3;
    stream << thresholdX;
    stream << thresholdY;
    B_SERIALIZE_RETURN
}

void TrackMatching::Atd::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> axis;
    stream >> x1;
    stream >> x2;
    stream >> x3;
    stream >> y1;
    stream >> y2;
    stream >> y3;
    stream >> thresholdX;
    stream >> thresholdY;
    B_DESERIALIZE_END
}

void TrackGroup::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << index;
    stream << moveX;
    stream << moveY;
    B_SERIALIZE_RETURN
}

void TrackGroup::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> index;
    stream >> moveX;
    stream >> moveY;
    B_DESERIALIZE_END
}

void TrackerInfo::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << classIndex;
    stream << className;
    stream << matching;
    stream << ttl;
    stream << validateTime;
    stream << validateSaturation;
    stream << rectsTimeLimit;
    stream << rectsCountLimit;
    stream << rectsAverageDepth;
    stream << group;
    B_SERIALIZE_RETURN
}

void TrackerInfo::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> classIndex;
    stream >> className;
    stream >> matching;
    stream >> ttl;
    stream >> validateTime;
    stream >> validateSaturation;
    stream >> rectsTimeLimit;
    stream >> rectsCountLimit;
    stream >> rectsAverageDepth;
    stream >> group;
    B_DESERIALIZE_END
}

void TrackersInfoA::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << items;
    B_SERIALIZE_RETURN
}

void TrackersInfoA::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> items;
    B_DESERIALIZE_END
}

QUuidEx TrackObject::globalUuid() const
{
    return QUuidEx(globalId, globalIdExt);
}

void TrackObject::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << id;
    stream << globalId;
    stream << globalIdExt;
    stream << isValidated;
    stream << timeDetect;
    stream << timeComplete;
    stream << rects;
    stream << averageRect;
    B_SERIALIZE_RETURN
}

void TrackObject::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> id;
    stream >> globalId;
    stream >> globalIdExt;
    stream >> isValidated;
    stream >> timeDetect;
    stream >> timeComplete;
    stream >> rects;
    stream >> averageRect;
    B_DESERIALIZE_END
}

void TrackerEvent::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << classIndex;
    stream << className;
    stream << event;
    stream << object;
    B_SERIALIZE_RETURN
}

void TrackerEvent::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> classIndex;
    stream >> className;
    stream >> event;
    stream >> object;
    B_DESERIALIZE_END
}

void TrackersEventsInfoA::Item::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << classIndex;
    stream << className;
    stream << events;
    B_SERIALIZE_RETURN
}

void TrackersEventsInfoA::Item::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> classIndex;
    stream >> className;
    stream >> events;
    B_DESERIALIZE_END
}

void TrackersEventsInfoA::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << items;
    B_SERIALIZE_RETURN
}

void TrackersEventsInfoA::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> items;
    B_DESERIALIZE_END
}

void TrackersEventsSubscript::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << classIndexes;
    stream << events;
    B_SERIALIZE_RETURN
}

void TrackersEventsSubscript::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> classIndexes;
    stream >> events;
    B_DESERIALIZE_END
}

void TrackerSnapshot::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << classIndex;
    stream << className;
    stream << objects;
    B_SERIALIZE_RETURN
}

void TrackerSnapshot::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> classIndex;
    stream >> className;
    stream >> objects;
    B_DESERIALIZE_END
}

void TrackersSnapshotA::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << items;
    B_SERIALIZE_RETURN
}

void TrackersSnapshotA::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> items;
    B_DESERIALIZE_END
}

void TrackRect::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << id;
    stream << visible;
    stream << caption;
    stream << border;
    stream << center;
    stream << graphset;
    stream << onlyValidated;
    B_SERIALIZE_RETURN
}

void TrackRect::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> id;
    stream >> visible;
    stream >> caption;
    stream >> border;
    stream >> center;
    stream >> graphset;
    stream >> onlyValidated;
    B_DESERIALIZE_END
}

void TrackRects::ModelClass::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << index;
    stream << rect;
    B_SERIALIZE_RETURN
}

void TrackRects::ModelClass::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> index;
    stream >> rect;
    B_DESERIALIZE_END
}

void TrackRects::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << visible;
    stream << classes;
    B_SERIALIZE_RETURN
}

void TrackRects::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> visible;
    stream >> classes;
    B_DESERIALIZE_END
}

void TrackRectsInfoA::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << value;
    B_SERIALIZE_RETURN
}

void TrackRectsInfoA::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> value;
    B_DESERIALIZE_END
}

void TrackRectsDynamicInfoA::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << items;
    B_SERIALIZE_RETURN
}

void TrackRectsDynamicInfoA::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> items;
    B_DESERIALIZE_END
}

void TrackRectsDynamicUpdate::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << items;
    B_SERIALIZE_RETURN
}

void TrackRectsDynamicUpdate::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> items;
    B_DESERIALIZE_END
}

void TrackRectsDynamicRemove::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << items;
    B_SERIALIZE_RETURN
}

void TrackRectsDynamicRemove::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> items;
    B_DESERIALIZE_END
}

void TrackGlobalIdUpdate::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << id;
    stream << globalId;
    stream << globalIdExt;
    B_SERIALIZE_RETURN
}

void TrackGlobalIdUpdate::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> id;
    stream >> globalId;
    stream >> globalIdExt;
    B_DESERIALIZE_END
}

void Graphset::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << visible;
    stream << items;
    B_SERIALIZE_RETURN
}

void Graphset::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> visible;
    stream >> items;
    B_DESERIALIZE_END
}

void GraphsetInfoA::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << value;
    B_SERIALIZE_RETURN
}

void GraphsetInfoA::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> value;
    B_DESERIALIZE_END
}

void GraphsetDynamicInfoA::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << items;
    B_SERIALIZE_RETURN
}

void GraphsetDynamicInfoA::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> items;
    B_DESERIALIZE_END
}

void GraphsetDynamicUpdate::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << items;
    B_SERIALIZE_RETURN
}

void GraphsetDynamicUpdate::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> items;
    B_DESERIALIZE_END
}

void GraphsetDynamicRemove::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << items;
    B_SERIALIZE_RETURN
}

void GraphsetDynamicRemove::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> items;
    B_DESERIALIZE_END
}

void GraphsetVisible::Set::Item::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << identity;
    stream << visible;
    B_SERIALIZE_RETURN
}

void GraphsetVisible::Set::Item::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> identity;
    stream >> visible;
    B_DESERIALIZE_END
}

void GraphsetVisible::Set::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << texts;
    stream << lines;
    stream << rectangles;
    stream << circles;
    B_SERIALIZE_RETURN
}

void GraphsetVisible::Set::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> texts;
    stream >> lines;
    stream >> rectangles;
    stream >> circles;
    B_DESERIALIZE_END
}

void GraphsetVisible::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << visible;
    stream << items;
    B_SERIALIZE_RETURN
}

void GraphsetVisible::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> visible;
    stream >> items;
    B_DESERIALIZE_END
}

void RoisInfo::ModelClass::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << index;
    stream << skipGlobalRois;
    stream << rois;
    B_SERIALIZE_RETURN
}

void RoisInfo::ModelClass::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> index;
    stream >> skipGlobalRois;
    stream >> rois;
    B_DESERIALIZE_END
}

void RoisInfo::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << visible;
    stream << globalRois;
    stream << classes;
    B_SERIALIZE_RETURN
}

void RoisInfo::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> visible;
    stream >> globalRois;
    stream >> classes;
    B_DESERIALIZE_END
}

void FpFnInfo::ModelClass::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << index;
    stream << active;
    stream << roi;
    stream << fpTiming;
    stream << fnTiming;
    stream << fnTtlDown;
    B_SERIALIZE_RETURN
}

void FpFnInfo::ModelClass::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> index;
    stream >> active;
    stream >> roi;
    stream >> fpTiming;
    stream >> fnTiming;
    stream >> fnTtlDown;
    B_DESERIALIZE_END
}

void FpFnInfo::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << active;
    stream << path;
    stream << prefix;
    stream << visibleRois;
    stream << markupBrush;
    stream << classes;
    B_SERIALIZE_RETURN
}

void FpFnInfo::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> active;
    stream >> path;
    stream >> prefix;
    stream >> visibleRois;
    stream >> markupBrush;
    stream >> classes;
    B_DESERIALIZE_END
}

void SaverInfoA::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << fileName;
    stream << maxSaveTime;
    stream << isMarkup;
    B_SERIALIZE_RETURN
}

void SaverInfoA::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> fileName;
    stream >> maxSaveTime;
    stream >> isMarkup;
    B_DESERIALIZE_END
}

void SaverBufferInfoA::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << bufferSizeClear;
    stream << bufferSizeMarkup;
    B_SERIALIZE_RETURN
}

void SaverBufferInfoA::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> bufferSizeClear;
    stream >> bufferSizeMarkup;
    B_DESERIALIZE_END
}

void SetSaverBuffer::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << bufferSizeClear;
    stream << bufferSizeMarkup;
    B_SERIALIZE_RETURN
}

void SetSaverBuffer::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> bufferSizeClear;
    stream >> bufferSizeMarkup;
    B_DESERIALIZE_END
}

void SaverStart::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << saverIndex;
    stream << isMarkup;
    stream << maxSaveTime;
    stream << autoStop;
    stream << fileName;
    B_SERIALIZE_RETURN
}

void SaverStart::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> saverIndex;
    stream >> isMarkup;
    stream >> maxSaveTime;
    stream >> autoStop;
    stream >> fileName;
    B_DESERIALIZE_END
}

void SaverStop::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << saverIndex;
    stream << fileName;
    B_SERIALIZE_RETURN
}

void SaverStop::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> saverIndex;
    stream >> fileName;
    B_DESERIALIZE_END
}

} // namespace data
} // namespace pproto
