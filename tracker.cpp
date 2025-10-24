#include "tracker.h"
#include "pproto/commands/pool.h"

namespace pproto {
namespace command {

#define REGISTRY_COMMAND_SINGLPROC(COMMAND, UUID) \
    const QUuidEx COMMAND = command::Pool::Registry{UUID, #COMMAND, false};

#define REGISTRY_COMMAND_MULTIPROC(COMMAND, UUID) \
    const QUuidEx COMMAND = command::Pool::Registry{UUID, #COMMAND, true};

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

#undef REGISTRY_COMMAND_SINGLPROC
#undef REGISTRY_COMMAND_MULTIPROC

} // namespace command

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

void TrackersSnapshot::VideoFrame::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << requestData;
    stream << sharedMem;
    B_SERIALIZE_RETURN
}

void TrackersSnapshot::VideoFrame::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> requestData;
    stream >> sharedMem;
    B_DESERIALIZE_END
}

void TrackersSnapshot::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << videoFrame;
    B_SERIALIZE_RETURN
}

void TrackersSnapshot::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> videoFrame;
    B_DESERIALIZE_END
}

void TrackersSnapshotA::VideoFrame::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << number;
    stream << cvType;
    stream << width;
    stream << height;
    stream << data;
    B_SERIALIZE_RETURN
}

void TrackersSnapshotA::VideoFrame::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> number;
    stream >> cvType;
    stream >> width;
    stream >> height;
    stream >> data;
    B_DESERIALIZE_END
}

void TrackersSnapshotA::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << items;
    stream << videoFrame;
    B_SERIALIZE_RETURN
}

void TrackersSnapshotA::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> items;
    stream >> videoFrame;
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

} // namespace data
} // namespace pproto
