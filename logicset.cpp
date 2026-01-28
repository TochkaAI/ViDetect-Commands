#include "logicset.h"
#include "pproto/commands/pool.h"

namespace pproto {
namespace command {

#define REGISTRY_COMMAND_SINGLPROC(COMMAND, UUID) \
    const QUuidEx COMMAND = command::Pool::Registry{UUID, #COMMAND, false};

#define REGISTRY_COMMAND_MULTIPROC(COMMAND, UUID) \
    const QUuidEx COMMAND = command::Pool::Registry{UUID, #COMMAND, true};

REGISTRY_COMMAND_SINGLPROC(LogicSetsInfo,     "4efd50d8-07cd-40e3-8531-6aa7599dd200")
REGISTRY_COMMAND_SINGLPROC(LogicSetsSnapshot, "8933a645-35cf-466e-b20f-45a2555295f6")

#undef REGISTRY_COMMAND_SINGLPROC
#undef REGISTRY_COMMAND_MULTIPROC

} // namespace command

namespace data {

void LogicSetInfo::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << classIndex;
    stream << className;
    stream << framesDepth;
    stream << saturate;
    stream << score;
    stream << realModelClasses;
    B_SERIALIZE_RETURN
}

void LogicSetInfo::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> classIndex;
    stream >> className;
    stream >> framesDepth;
    stream >> saturate;
    stream >> score;
    stream >> realModelClasses;
    B_DESERIALIZE_END
}

void LogicSetsInfoA::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << items;
    B_SERIALIZE_RETURN
}

void LogicSetsInfoA::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> items;
    B_DESERIALIZE_END
}

void LogicObject::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << id;
    stream << trackId;
    stream << classIndex;
    stream << saturate;
    stream << timeCreate;
    stream << rects;
    //stream << averageRect;
    B_SERIALIZE_RETURN
}

void LogicObject::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> id;
    stream >> trackId;
    stream >> classIndex;
    stream >> saturate;
    stream >> timeCreate;
    stream >> rects;
    //stream >> averageRect;
    B_DESERIALIZE_END
}

void LogicSetSnapshot::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << classIndex;
    stream << className;
    stream << objects;
    B_SERIALIZE_RETURN
}

void LogicSetSnapshot::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> classIndex;
    stream >> className;
    stream >> objects;
    B_DESERIALIZE_END
}

void LogicSetsSnapshot::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << mode;
    B_SERIALIZE_RETURN
}

void LogicSetsSnapshot::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> mode;
    B_DESERIALIZE_END
}

void LogicSetsSnapshotA::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << items;
    B_SERIALIZE_RETURN
}

void LogicSetsSnapshotA::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> items;
    B_DESERIALIZE_END
}

} // namespace data
} // namespace pproto
