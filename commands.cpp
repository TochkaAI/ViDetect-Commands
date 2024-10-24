#include "commands.h"
#include "pproto/commands/pool.h"

namespace pproto {
namespace command {

#define REGISTRY_COMMAND_SINGLPROC(COMMAND, UUID) \
    const QUuidEx COMMAND = command::Pool::Registry{UUID, #COMMAND, false};

#define REGISTRY_COMMAND_MULTIPROC(COMMAND, UUID) \
    const QUuidEx COMMAND = command::Pool::Registry{UUID, #COMMAND, true};

REGISTRY_COMMAND_SINGLPROC(TrackInformation,  "50dc6d0c-a0aa-48e6-81be-a10e2998dee3")
REGISTRY_COMMAND_SINGLPROC(SnapshotInfo,      "0d629902-bc81-4ace-a5c0-7cab38ec240d")
REGISTRY_COMMAND_SINGLPROC(MetaSettings,      "4b04e2b8-6957-4c4a-949b-8a17f5f47cb5")

#undef REGISTRY_COMMAND_SINGLPROC
#undef REGISTRY_COMMAND_MULTIPROC

} // namespace command

namespace data {

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

bserial::RawVector TrackObject::toRaw() const
{
    B_SERIALIZE_V1(stream)
    stream << id;
    stream << globalId;
    stream << status;
    stream << isValidated;
    stream << timeDetect;
    stream << timeComplete;
    stream << score;
    B_SERIALIZE_RETURN
}

void TrackObject::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect, stream)
    stream >> id;
    stream >> globalId;
    stream >> status;
    stream >> isValidated;
    stream >> timeDetect;
    stream >> timeComplete;
    stream >> score;
    B_DESERIALIZE_END
}

bserial::RawVector MetaClass::toRaw() const
{
    B_SERIALIZE_V1(stream)
//    stream << index;
    stream << name;
    stream << isSkip;
    stream << threshScore;
    stream << threshNMS;
    stream << threshIOU;
    stream << threshTTL;
    stream << validateTime;
    stream << validateSaturate;
    stream << rectsLimits;
    stream << isPaint;
    stream << color;
    B_SERIALIZE_RETURN
}

void MetaClass::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect, stream)
//    stream >> index;
    stream >> name;
    stream >> isSkip;
    stream >> threshScore;
    stream >> threshNMS;
    stream >> threshIOU;
    stream >> threshTTL;
    stream >> validateTime;
    stream >> validateSaturate;
    stream >> rectsLimits;
    stream >> isPaint;
    stream >> color;
    B_DESERIALIZE_END
}

bserial::RawVector TrackClass::toRaw() const
{
    B_SERIALIZE_V1(stream)
    stream << index;
    stream << meta;
    stream << objects;
    B_SERIALIZE_RETURN
}

void TrackClass::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect, stream)
    stream >> index;
    stream >> meta;
    stream >> objects;
    B_DESERIALIZE_END
}

bserial::RawVector TrackInformation::toRaw() const
{
    B_SERIALIZE_V1(stream)
    stream << timeResponse;
    stream << classes;
    B_SERIALIZE_RETURN
}

void TrackInformation::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect, stream)
    stream >> timeResponse;
    stream >> classes;
    B_DESERIALIZE_END
}

bserial::RawVector SnapshotInfo::toRaw() const
{
    B_SERIALIZE_V1(stream)
    stream << timeRequest;
    stream << timeResponse;
    stream << information;
    B_SERIALIZE_RETURN
}

void SnapshotInfo::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect, stream)
    stream >> timeRequest;
    stream >> timeResponse;
    stream >> information;
    B_DESERIALIZE_END
}

bserial::RawVector MetaSettings::toRaw() const
{
    B_SERIALIZE_V1(stream)
    stream << method;
    stream << newSettings;
    stream << actualSettings;
    B_SERIALIZE_RETURN
}

void MetaSettings::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect, stream)
    stream >> method;
    stream >> newSettings;
    stream >> actualSettings;
    B_DESERIALIZE_END
}

} // namespace data
} // namespace pproto
