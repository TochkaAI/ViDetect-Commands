#include "model.h"
#include "pproto/commands/pool.h"

namespace pproto {
namespace command {

#define REGISTRY_COMMAND_SINGLPROC(COMMAND, UUID) \
    const QUuidEx COMMAND = command::Pool::Registry{UUID, #COMMAND, false};

#define REGISTRY_COMMAND_MULTIPROC(COMMAND, UUID) \
    const QUuidEx COMMAND = command::Pool::Registry{UUID, #COMMAND, true};

REGISTRY_COMMAND_SINGLPROC(ModelInfo, "d8f7f3f5-2965-4b1a-9508-3e18f85d0fcc")

#undef REGISTRY_COMMAND_SINGLPROC
#undef REGISTRY_COMMAND_MULTIPROC

} // namespace command

namespace data {

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

void ModelSegment::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << active;
    stream << outputSize;
    stream << output;
    B_SERIALIZE_RETURN
}

void ModelSegment::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> active;
    stream >> outputSize;
    stream >> output;
    B_DESERIALIZE_END
}

void ModelClassBase::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << index;
    stream << name;
    stream << skip;
    B_SERIALIZE_RETURN
}

void ModelClassBase::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> index;
    stream >> name;
    stream >> skip;
    B_DESERIALIZE_END
}

void RealModelClass::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << B_BASE_CLASS(ModelClassBase);
    stream << cuttingThresh;
    stream << scoreThresh;
    stream << nmsThresh;
    stream << segmentThresh;
    B_SERIALIZE_RETURN
}

void RealModelClass::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> B_BASE_CLASS(ModelClassBase);
    stream >> cuttingThresh;
    stream >> scoreThresh;
    stream >> nmsThresh;
    stream >> segmentThresh;
    B_DESERIALIZE_END
}

void MetaModelClass::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << B_BASE_CLASS(ModelClassBase);
    stream << realModelClasses;
    stream << iouThresh;
    B_SERIALIZE_RETURN
}

void MetaModelClass::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> B_BASE_CLASS(ModelClassBase);
    stream >> realModelClasses;
    stream >> iouThresh;
    B_DESERIALIZE_END
}

void RealModel::toRaw(bserial::DataStream& stream) const
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
    stream << inputSize2;
    stream << segmentation;
    B_SERIALIZE_RETURN
}

void RealModel::fromRaw(const bserial::RawVector& vect)
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
    stream >> inputSize2;
    stream >> segmentation;
    B_DESERIALIZE_END
}

void MetaModel::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << classes;
    B_SERIALIZE_RETURN
}

void MetaModel::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> classes;
    B_DESERIALIZE_END
}

void ModelInfoA::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << usingModel;
    stream << realModel;
    stream << metaModel;
    B_SERIALIZE_RETURN
}

void ModelInfoA::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> usingModel;
    stream >> realModel;
    stream >> metaModel;
    B_DESERIALIZE_END
}

} // namespace data
} // namespace pproto
