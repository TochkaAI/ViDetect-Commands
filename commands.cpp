#include "commands.h"
#include "pproto/commands/pool.h"

namespace pproto {
namespace command {

#define REGISTRY_COMMAND_SINGLPROC(COMMAND, UUID) \
    const QUuidEx COMMAND = command::Pool::Registry{UUID, #COMMAND, false};

#define REGISTRY_COMMAND_MULTIPROC(COMMAND, UUID) \
    const QUuidEx COMMAND = command::Pool::Registry{UUID, #COMMAND, true};

REGISTRY_COMMAND_SINGLPROC(ModelInfo,              "d8f7f3f5-2965-4b1a-9508-3e18f85d0fcc")
REGISTRY_COMMAND_SINGLPROC(TrackersInfo,           "d6272e50-b3e7-4d7d-b45e-e22440c78315")
REGISTRY_COMMAND_SINGLPROC(TrackerEvent,           "50dc6d0c-a0aa-48e6-81be-a10e2998dee3")
REGISTRY_COMMAND_SINGLPROC(TrackersEventInfo,      "04bd5a2f-c298-4aac-90a9-00b158eae8f6")
REGISTRY_COMMAND_SINGLPROC(TrackersEventSubscript, "33bac850-3f26-4816-b1a1-055e02921227")
REGISTRY_COMMAND_SINGLPROC(TrackersSnapshot,       "0d629902-bc81-4ace-a5c0-7cab38ec240d")
REGISTRY_COMMAND_SINGLPROC(TrackRectsInfo,         "0bc0246f-ef01-42a5-adb2-c724a8ea9327")
REGISTRY_COMMAND_SINGLPROC(PostprocessingMarkup,   "036f0683-29ee-4fd8-b612-1e103ace9331")
REGISTRY_COMMAND_SINGLPROC(GraphsetInfo,           "53aedbb1-ee08-4c01-8f45-499e5c099a68")
REGISTRY_COMMAND_SINGLPROC(GraphsetConfig,         "d184366a-a48e-43a6-8c7e-8561c9a43184")
REGISTRY_COMMAND_SINGLPROC(GraphsetVisible,        "dac925e7-8abe-49cb-98d3-15535131850a")
REGISTRY_COMMAND_SINGLPROC(GraphsetAdd,            "197d538b-c9e9-4328-bca3-5f573e76229d")
REGISTRY_COMMAND_SINGLPROC(GraphsetRemove,         "0581afc9-464e-4978-8ad7-5005bd623331")
REGISTRY_COMMAND_SINGLPROC(RoisInfo,               "49ee172b-2954-4f90-9965-de3804fe3f14")
REGISTRY_COMMAND_SINGLPROC(FpFnInfo,               "0db958aa-d392-4157-9ed8-901f258e69d9")

#undef REGISTRY_COMMAND_SINGLPROC
#undef REGISTRY_COMMAND_MULTIPROC

} // namespace command

namespace data {
namespace graph {

bool Identity::isNull() const
{
    return (id.isNull() && (index < 0) && name.isEmpty());
}

bserial::RawVector Identity::toRaw() const
{
    B_SERIALIZE_V1(stream)
    stream << id;
    stream << index;
    stream << name;
    stream << findField;
    B_SERIALIZE_RETURN
}

void Identity::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect, stream)
    stream >> id;
    stream >> index;
    stream >> name;
    stream >> findField;
    B_DESERIALIZE_END
}

bserial::RawVector Color::toRaw() const
{
    B_SERIALIZE_V1(stream)
    stream << r;
    stream << g;
    stream << b;
    B_SERIALIZE_RETURN
}

void Color::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect, stream)
    stream >> r;
    stream >> g;
    stream >> b;
    B_DESERIALIZE_END
}

bserial::RawVector Brush::toRaw() const
{
    B_SERIALIZE_V1(stream)
    stream << thickness;
    stream << color;
    B_SERIALIZE_RETURN
}

void Brush::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect, stream)
    stream >> thickness;
    stream >> color;
    B_DESERIALIZE_END
}

bool Point::isNull() const
{
    return (x == 0) && (y == 0);
}

bserial::RawVector Point::toRaw() const
{
    B_SERIALIZE_V1(stream)
    stream << x;
    stream << y;
    B_SERIALIZE_RETURN
}

void Point::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect, stream)
    stream >> x;
    stream >> y;
    B_DESERIALIZE_END
}

bool Size::isNull() const
{
    return (width == 0) && (height == 0);
}

bserial::RawVector Size::toRaw() const
{
    B_SERIALIZE_V1(stream)
    stream << width;
    stream << height;
    B_SERIALIZE_RETURN
}

void Size::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect, stream)
    stream >> width;
    stream >> height;
    B_DESERIALIZE_END
}

bserial::RawVector Offset::toRaw() const
{
    B_SERIALIZE_V1(stream)
    stream << dx;
    stream << dy;
    B_SERIALIZE_RETURN
}

void Offset::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect, stream)
    stream >> dx;
    stream >> dy;
    B_DESERIALIZE_END
}

#ifdef OPENCV_IS_PRESENT

cv::Point Caption::position_() const
{
    return cv::Point(position.x, position.y);
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

bserial::RawVector Caption::toRaw() const
{
    B_SERIALIZE_V1(stream)
    stream << identity;
    stream << brush;
    stream << visible;
    stream << position;
    stream << offset;
    stream << scale;
    stream << text;
    stream << cornerBind;
    B_SERIALIZE_RETURN
}

void Caption::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect, stream)
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

cv::Point Line::p1() const
{
    return cv::Point(point1.x, point1.y);
}

void Line::setP1(const cv::Point& p)
{
    point1.x = p.x;
    point1.y = p.y;
}

cv::Point Line::p2() const
{
    return cv::Point(point2.x, point2.y);
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

bserial::RawVector Line::toRaw() const
{
    B_SERIALIZE_V1(stream)
    stream << identity;
    stream << visible;
    stream << brush;
    stream << point1;
    stream << point2;
    B_SERIALIZE_RETURN
}

void Line::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect, stream)
    stream >> identity;
    stream >> visible;
    stream >> brush;
    stream >> point1;
    stream >> point2;
    B_DESERIALIZE_END
}

#ifdef OPENCV_IS_PRESENT

cv::Point Rectangle::p1() const
{
    return cv::Point(point1.x, point1.y);
}

void Rectangle::setP1(const cv::Point& p)
{
    point1.x = p.x;
    point1.y = p.y;
}

cv::Point Rectangle::p2() const
{
    if (!point2.isNull())
        return cv::Point(point2.x, point2.y);
    else
        return cv::Point(point1.x + size.width, point1.y + size.height);
}

void Rectangle::setP2(const cv::Point& p)
{
    point2.x = p.x;
    point2.y = p.y;
}

cv::Rect Rectangle::rect() const
{
    if (!point2.isNull())
        return cv::Rect(point1.x, point1.y,
                        point2.x - point1.x /*width*/,
                        point2.y - point1.y /*height*/);
    else
        return cv::Rect(point1.x, point1.y, size.width, size.height);
}

void Rectangle::setRect(const cv::Rect& rect)
{
    point1.x = rect.x;
    point1.y = rect.y;
    point2.x = rect.x + rect.width;
    point2.y = rect.y + rect.height;

    size.width  = rect.width;
    size.height = rect.height;
}

#endif // OPENCV_IS_PRESENT

bool pproto::data::graph::Rectangle::isNull() const
{
    return (point1.isNull() && point2.isNull() && size.isNull());
}

bserial::RawVector Rectangle::toRaw() const
{
    B_SERIALIZE_V1(stream)
    stream << identity;
    stream << visible;
    stream << brush;
    stream << point1;
    stream << point2;
    stream << size;
    B_SERIALIZE_RETURN
}

void Rectangle::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect, stream)
    stream >> identity;
    stream >> visible;
    stream >> brush;
    stream >> point1;
    stream >> point2;
    stream >> size;
    B_DESERIALIZE_END
}

#ifdef OPENCV_IS_PRESENT

cv::Point Circle::center_() const
{
    return cv::Point(center.x, center.y);
}

void Circle::setCenter(const cv::Point& p)
{
    center.x = p.x;
    center.y = p.y;
}

#endif // OPENCV_IS_PRESENT

bserial::RawVector Circle::toRaw() const
{
    B_SERIALIZE_V1(stream)
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
    B_DESERIALIZE_V1(vect, stream)
    stream >> identity;
    stream >> visible;
    stream >> brush;
    stream >> center;
    stream >> radius;
    stream >> filled;
    B_DESERIALIZE_END
}

bserial::RawVector Roi::toRaw() const
{
    B_SERIALIZE_V1(stream)
    stream << identity;
    stream << visible;
    stream << caption;
    stream << border;
    B_SERIALIZE_RETURN
}

void Roi::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect, stream)
    stream >> identity;
    stream >> visible;
    stream >> caption;
    stream >> border;
    B_DESERIALIZE_END
}

bserial::RawVector Set::toRaw() const
{
    B_SERIALIZE_V1(stream)
    stream << identity;
    stream << visible;
    stream << border;
    stream << captions;
    stream << lines;
    stream << rectangles;
    stream << circles;
    B_SERIALIZE_RETURN
}

void Set::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect, stream)
    stream >> identity;
    stream >> visible;
    stream >> border;
    stream >> captions;
    stream >> lines;
    stream >> rectangles;
    stream >> circles;
    B_DESERIALIZE_END
}

bserial::RawVector TrackRect::toRaw() const
{
    B_SERIALIZE_V1(stream)
    stream << id;
    stream << visible;
    stream << caption;
    stream << border;
    stream << center;
    B_SERIALIZE_RETURN
}

void TrackRect::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect, stream)
    stream >> id;
    stream >> visible;
    stream >> caption;
    stream >> border;
    stream >> center;
    B_DESERIALIZE_END
}

} // namespace graph

bserial::RawVector FrameCrop::toRaw() const
{
    B_SERIALIZE_V1(stream)
    stream << active;
    stream << border;
    B_SERIALIZE_RETURN
}

void FrameCrop::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect, stream)
    stream >> active;
    stream >> border;
    B_DESERIALIZE_END
}

bserial::RawVector ModelClass::toRaw() const
{
    B_SERIALIZE_V1(stream)
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
    B_DESERIALIZE_V1(vect, stream)
    stream >> index;
    stream >> name;
    stream >> skip;
    stream >> cuttingThresh;
    stream >> scoreThresh;
    stream >> nmsThresh;
    B_DESERIALIZE_END
}

bserial::RawVector ModelInfo::toRaw() const
{
    B_SERIALIZE_V1(stream)
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
    B_SERIALIZE_RETURN
}

void ModelInfo::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect, stream)
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
    B_DESERIALIZE_END
}

bserial::RawVector DetectRect::toRaw() const
{
    B_SERIALIZE_V1(stream)
    stream << x_min;
    stream << y_min;
    stream << x_max;
    stream << y_max;
    stream << score;
    B_SERIALIZE_RETURN
}

void DetectRect::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect, stream)
    stream >> x_min;
    stream >> y_min;
    stream >> x_max;
    stream >> y_max;
    stream >> score;
    B_DESERIALIZE_END
}

bserial::RawVector TrackerInfo::toRaw() const
{
    B_SERIALIZE_V1(stream)
    stream << classIndex;
    stream << className;
    stream << iouThresh;
    stream << ttlThresh;
    stream << validateTime;
    stream << validateSaturation;
    stream << rectsTimeLimit;
    stream << rectsCountLimit;
    B_SERIALIZE_RETURN
}

void TrackerInfo::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect, stream)
    stream >> classIndex;
    stream >> className;
    stream >> iouThresh;
    stream >> ttlThresh;
    stream >> validateTime;
    stream >> validateSaturation;
    stream >> rectsTimeLimit;
    stream >> rectsCountLimit;
    B_DESERIALIZE_END
}

bserial::RawVector TrackersInfoA::toRaw() const
{
    B_SERIALIZE_V1(stream)
    stream << items;
    B_SERIALIZE_RETURN
}

void TrackersInfoA::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect, stream)
    stream >> items;
    B_DESERIALIZE_END
}

bserial::RawVector TrackObject::toRaw() const
{
    B_SERIALIZE_V1(stream)
    stream << id;
    stream << globalId;
    stream << isValidated;
    stream << timeDetect;
    stream << timeComplete;
    stream << rects;
    stream << averageRect;
    B_SERIALIZE_RETURN
}

void TrackObject::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect, stream)
    stream >> id;
    stream >> globalId;
    stream >> isValidated;
    stream >> timeDetect;
    stream >> timeComplete;
    stream >> rects;
    stream >> averageRect;
    B_DESERIALIZE_END
}

bserial::RawVector TrackerEvent::toRaw() const
{
    B_SERIALIZE_V1(stream)
    stream << classIndex;
    stream << className;
    stream << event;
    stream << object;
    B_SERIALIZE_RETURN
}

void TrackerEvent::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect, stream)
    stream >> classIndex;
    stream >> className;
    stream >> event;
    stream >> object;
    B_DESERIALIZE_END
}

bserial::RawVector TrackersEventInfoA::Item::toRaw() const
{
    B_SERIALIZE_V1(stream)
    stream << classIndex;
    stream << className;
    stream << events;
    B_SERIALIZE_RETURN
}

void TrackersEventInfoA::Item::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect, stream)
    stream >> classIndex;
    stream >> className;
    stream >> events;
    B_DESERIALIZE_END
}

bserial::RawVector TrackersEventInfoA::toRaw() const
{
    B_SERIALIZE_V1(stream)
    stream << items;
    B_SERIALIZE_RETURN
}

void TrackersEventInfoA::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect, stream)
    stream >> items;
    B_DESERIALIZE_END
}

bserial::RawVector TrackersEventSubscript::toRaw() const
{
    B_SERIALIZE_V1(stream)
    stream << classIndexes;
    stream << events;
    B_SERIALIZE_RETURN
}

void TrackersEventSubscript::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect, stream)
    stream >> classIndexes;
    stream >> events;
    B_DESERIALIZE_END
}

bserial::RawVector TrackerSnapshot::toRaw() const
{
    B_SERIALIZE_V1(stream)
    stream << classIndex;
    stream << className;
    stream << objects;
    B_SERIALIZE_RETURN
}

void TrackerSnapshot::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect, stream)
    stream >> classIndex;
    stream >> className;
    stream >> objects;
    B_DESERIALIZE_END
}

bserial::RawVector TrackersSnapshotA::toRaw() const
{
    B_SERIALIZE_V1(stream)
    stream << items;
    B_SERIALIZE_RETURN
}

void TrackersSnapshotA::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect, stream)
    stream >> items;
    B_DESERIALIZE_END
}

bserial::RawVector TrackRectsInfo::ModelClass::toRaw() const
{
    B_SERIALIZE_V1(stream)
    stream << index;
    stream << rect;
    B_SERIALIZE_RETURN
}

void TrackRectsInfo::ModelClass::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect, stream)
    stream >> index;
    stream >> rect;
    B_DESERIALIZE_END
}

bserial::RawVector TrackRectsInfo::toRaw() const
{
    B_SERIALIZE_V1(stream)
    stream << visible;
    stream << classes;
    B_SERIALIZE_RETURN
}

void TrackRectsInfo::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect, stream)
    stream >> visible;
    stream >> classes;
    B_DESERIALIZE_END
}

bserial::RawVector MarkupObject::Text::toRaw() const
{
    B_SERIALIZE_V1(stream)
    stream << text;
    stream << position;
    stream << scale;
    stream << color;
    stream << thickness;
    stream << offset;
    B_SERIALIZE_RETURN
}

void MarkupObject::Text::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect, stream)
    stream >> text;
    stream >> position;
    stream >> scale;
    stream >> color;
    stream >> thickness;
    stream >> offset;
    B_DESERIALIZE_END
}

bserial::RawVector MarkupObject::toRaw() const
{
    B_SERIALIZE_V1(stream)
    stream << group;
    stream << type;
    stream << coorditane;
    stream << thickness;
    stream << color;
    stream << descriptor;
    B_SERIALIZE_RETURN
}

void MarkupObject::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect, stream)
    stream >> group;
    stream >> type;
    stream >> coorditane;
    stream >> thickness;
    stream >> color;
    stream >> descriptor;
    B_DESERIALIZE_END
}

bserial::RawVector PostprocessingMarkup::toRaw() const
{
    B_SERIALIZE_V1(stream)
    stream << items;
    B_SERIALIZE_RETURN
}

void PostprocessingMarkup::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect, stream)
    stream >> items;
    B_DESERIALIZE_END
}

bserial::RawVector Graphset::toRaw() const
{
    B_SERIALIZE_V1(stream)
    stream << visible;
    stream << items;
    B_SERIALIZE_RETURN
}

void Graphset::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect, stream)
    stream >> visible;
    stream >> items;
    B_DESERIALIZE_END
}

bserial::RawVector GraphsetVisible::Set::Item::toRaw() const
{
    B_SERIALIZE_V1(stream)
    stream << identity;
    stream << visible;
    B_SERIALIZE_RETURN
}

void GraphsetVisible::Set::Item::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect, stream)
    stream >> identity;
    stream >> visible;
    B_DESERIALIZE_END
}

bserial::RawVector GraphsetVisible::Set::toRaw() const
{
    B_SERIALIZE_V1(stream)
    stream << texts;
    stream << lines;
    stream << rectangles;
    stream << circles;
    B_SERIALIZE_RETURN
}

void GraphsetVisible::Set::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect, stream)
    stream >> texts;
    stream >> lines;
    stream >> rectangles;
    stream >> circles;
    B_DESERIALIZE_END
}

bserial::RawVector GraphsetVisible::toRaw() const
{
    B_SERIALIZE_V1(stream)
    stream << visible;
    stream << items;
    B_SERIALIZE_RETURN
}

void GraphsetVisible::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect, stream)
    stream >> visible;
    stream >> items;
    B_DESERIALIZE_END
}

bserial::RawVector RoisInfo::ModelClass::toRaw() const
{
    B_SERIALIZE_V1(stream)
    stream << index;
    stream << skipGlobalRois;
    stream << rois;
    B_SERIALIZE_RETURN
}

void RoisInfo::ModelClass::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect, stream)
    stream >> index;
    stream >> skipGlobalRois;
    stream >> rois;
    B_DESERIALIZE_END
}

bserial::RawVector RoisInfo::toRaw() const
{
    B_SERIALIZE_V1(stream)
    stream << visible;
    stream << globalRois;
    stream << classes;
    B_SERIALIZE_RETURN
}

void RoisInfo::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect, stream)
    stream >> visible;
    stream >> globalRois;
    stream >> classes;
    B_DESERIALIZE_END
}

bserial::RawVector FpFnInfo::ModelClass::toRaw() const
{
    B_SERIALIZE_V1(stream)
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
    B_DESERIALIZE_V1(vect, stream)
    stream >> index;
    stream >> active;
    stream >> roi;
    stream >> fpTiming;
    stream >> fnTiming;
    stream >> fnTtlDown;
    B_DESERIALIZE_END
}

bserial::RawVector FpFnInfo::toRaw() const
{
    B_SERIALIZE_V1(stream)
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
    B_DESERIALIZE_V1(vect, stream)
    stream >> active;
    stream >> path;
    stream >> prefix;
    stream >> visibleRois;
    stream >> markupBrush;
    stream >> classes;
    B_DESERIALIZE_END
}

} // namespace data
} // namespace pproto
