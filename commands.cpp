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

#undef REGISTRY_COMMAND_SINGLPROC
#undef REGISTRY_COMMAND_MULTIPROC

} // namespace command

namespace data {

bserial::RawVector ModelClass::toRaw() const
{
    B_SERIALIZE_V1(stream)
    stream << index;
    stream << name;
    stream << skip;
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
    stream >> scoreThresh;
    stream >> nmsThresh;
    B_DESERIALIZE_END
}

bserial::RawVector ModelInfoA::toRaw() const
{
    B_SERIALIZE_V1(stream)
    stream << file;
    stream << type;
    stream << typeStr;
    stream << imageChannels;
    stream << inputSize;
    stream << input;
    stream << output;
    stream << classes;
    B_SERIALIZE_RETURN
}

void ModelInfoA::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect, stream)
    stream >> file;
    stream >> type;
    stream >> typeStr;
    stream >> imageChannels;
    stream >> inputSize;
    stream >> input;
    stream >> output;
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

} // namespace data
} // namespace pproto
