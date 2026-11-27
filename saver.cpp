#include "saver.h"
#include "pproto/commands/pool.h"

namespace pproto {
namespace command {

REGISTRY_PPROTO_COMMAND(SaverInfo,       "fa09f31b-d883-4a53-b807-6a557a82fc44", MULTIPROC)
REGISTRY_PPROTO_COMMAND(SaverBufferInfo, "506f9bf4-c111-4805-bf63-eace1d84dc33", MULTIPROC)
REGISTRY_PPROTO_COMMAND(SaverStart,      "4fced308-5509-4e71-9e8f-1d23d827c261", MULTIPROC)
REGISTRY_PPROTO_COMMAND(SaverStop,       "80e30f34-b86c-41b4-ba89-8884e74ad615", MULTIPROC)
REGISTRY_PPROTO_COMMAND(SetSaverBuffer,  "754c1dbc-6502-4714-9995-c1b13cd9ec13", MULTIPROC)

OBSOLETE_PPROTO_COMMAND(SaverInfo,       "774271e3-e3c5-40ff-b9e4-b9ec822eef18")
OBSOLETE_PPROTO_COMMAND(SaverInfo2,       "774271e3-e3c5-40ff-b9e4-b9ec822eef18")
OBSOLETE_PPROTO_COMMAND(SaverBufferInfo, "8e7012ea-c237-426c-a6b0-8c8b1acfd7c1")
OBSOLETE_PPROTO_COMMAND(SaverStart,      "b0340619-0513-4ca1-9149-99b3704c6d51")
OBSOLETE_PPROTO_COMMAND(SaverStop,       "bd5b7bdf-4fa2-4a9b-8e2c-f3df59490303")
OBSOLETE_PPROTO_COMMAND(SetSaverBuffer,  "5a01508d-16ae-468a-a872-59729a50f3cb")

} // namespace command

namespace data {

void SaverInfo::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << origin;
    stream << markup;
    B_SERIALIZE_RETURN
}

void SaverInfo::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> origin;
    stream >> markup;
    B_DESERIALIZE_END
}

void SaverInfoA::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << origin;
    stream << markup;
    stream << fileName;
    stream << maxSaveTime;
    B_SERIALIZE_RETURN
}

void SaverInfoA::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> origin;
    stream >> markup;
    stream >> fileName;
    stream >> maxSaveTime;
    B_DESERIALIZE_END
}

void SaverBufferInfo::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << origin;
    stream << markup;
    B_SERIALIZE_RETURN
}

void SaverBufferInfo::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> origin;
    stream >> markup;
    B_DESERIALIZE_END
}

void SaverBufferInfoA::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << origin;
    stream << markup;
    stream << bufferSize;
    B_SERIALIZE_RETURN
}

void SaverBufferInfoA::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> origin;
    stream >> markup;
    stream >> bufferSize;
    B_DESERIALIZE_END
}

void SetSaverBuffer::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << origin;
    stream << markup;
    stream << bufferSize;
    B_SERIALIZE_RETURN
}

void SetSaverBuffer::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> origin;
    stream >> markup;
    stream >> bufferSize;
    B_DESERIALIZE_END
}

void SaverStart::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << origin;
    stream << markup;
    stream << maxSaveTime;
    stream << autoStop;
    stream << fileName;
    B_SERIALIZE_RETURN
}

void SaverStart::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> origin;
    stream >> markup;
    stream >> maxSaveTime;
    stream >> autoStop;
    stream >> fileName;
    B_DESERIALIZE_END
}

void SaverStop::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << origin;
    stream << markup;
    stream << fileName;
    B_SERIALIZE_RETURN
}

void SaverStop::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> origin;
    stream >> markup;
    stream >> fileName;
    B_DESERIALIZE_END
}

} // namespace data
} // namespace pproto
