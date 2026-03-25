#include "classifier.h"
#include "pproto/commands/pool.h"

namespace pproto {
namespace command {

#define REGISTRY_COMMAND_SINGLPROC(COMMAND, UUID) \
    const QUuidEx COMMAND = command::Pool::Registry{UUID, #COMMAND, false};

#define REGISTRY_COMMAND_MULTIPROC(COMMAND, UUID) \
    const QUuidEx COMMAND = command::Pool::Registry{UUID, #COMMAND, true};

REGISTRY_COMMAND_SINGLPROC(ClassifModelsInfo, "43c98cce-928d-4203-9acb-de396fd27b4e")

#undef REGISTRY_COMMAND_SINGLPROC
#undef REGISTRY_COMMAND_MULTIPROC

} // namespace command

namespace data {

void ClassifModelClass::Threshold::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << score;
    B_SERIALIZE_RETURN
}

void ClassifModelClass::Threshold::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> score;
    B_DESERIALIZE_END
}

void ClassifModelClass::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << index;
    stream << name;
    stream << threshold;
    B_SERIALIZE_RETURN
}

void ClassifModelClass::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> index;
    stream >> name;
    stream >> threshold;
    B_DESERIALIZE_END
}

void ClassifModel::MainModel::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << classes;
    B_SERIALIZE_RETURN
}

void ClassifModel::MainModel::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> classes;
    B_DESERIALIZE_END
}

void ClassifModel::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << index;
    stream << name;
    stream << path;
    stream << file;
    stream << imageChannels;
    stream << inputSize;
    stream << inputSize2;
    stream << gpuIndex;
    stream << mainModel;
    stream << classes;
    B_SERIALIZE_RETURN
}

void ClassifModel::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> index;
    stream >> name;
    stream >> path;
    stream >> file;
    stream >> imageChannels;
    stream >> inputSize;
    stream >> inputSize2;
    stream >> gpuIndex;
    stream >> mainModel;
    stream >> classes;
    B_DESERIALIZE_END
}

void ClassifModelsInfoA::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << active;
    stream << items;
    B_SERIALIZE_RETURN
}

void ClassifModelsInfoA::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> active;
    stream >> items;
    B_DESERIALIZE_END
}

} // namespace data
} // namespace pproto
