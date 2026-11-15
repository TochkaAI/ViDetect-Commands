#include "saver.h"
#include "pproto/commands/pool.h"

namespace pproto {
namespace command {

REGISTRY_PPROTO_COMMAND(SaverInfo,       "774271e3-e3c5-40ff-b9e4-b9ec822eef18", SINGLPROC)
REGISTRY_PPROTO_COMMAND(SaverBufferInfo, "8e7012ea-c237-426c-a6b0-8c8b1acfd7c1", SINGLPROC)
REGISTRY_PPROTO_COMMAND(SaverStart,      "b0340619-0513-4ca1-9149-99b3704c6d51", SINGLPROC)
REGISTRY_PPROTO_COMMAND(SaverStop,       "bd5b7bdf-4fa2-4a9b-8e2c-f3df59490303", SINGLPROC)
REGISTRY_PPROTO_COMMAND(SetSaverBuffer,  "5a01508d-16ae-468a-a872-59729a50f3cb", SINGLPROC)

} // namespace command

namespace data {

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
