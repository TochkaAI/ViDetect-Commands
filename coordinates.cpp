#include "coordinates.h"
#include "pproto/commands/pool.h"

namespace pproto {
namespace command {

#define REGISTRY_COMMAND_SINGLPROC(COMMAND, UUID) \
    const QUuidEx COMMAND = command::Pool::Registry{UUID, #COMMAND, false};

#define REGISTRY_COMMAND_MULTIPROC(COMMAND, UUID) \
    const QUuidEx COMMAND = command::Pool::Registry{UUID, #COMMAND, true};

REGISTRY_COMMAND_SINGLPROC(Coordinates, "fcaca1f1-e59a-40e4-bbe5-e34cddad656b")

#undef REGISTRY_COMMAND_SINGLPROC
#undef REGISTRY_COMMAND_MULTIPROC

} // namespace command

namespace data {

Coordinates::Coordinates()
{
    memset(data, 0, sizeof(data));
}

void Coordinates::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    // stream << x;
    // stream << y;
    // stream << z;
    // stream << rx;
    // stream << ry;
    // stream << rz;
    stream.writeRawData((char*)data, sizeof(data));
    stream << time;
    B_SERIALIZE_RETURN
}

void Coordinates::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    // stream >> x;
    // stream >> y;
    // stream >> z;
    // stream >> rx;
    // stream >> ry;
    // stream >> rz;
    stream.readRawData((char*)data, sizeof(data));
    stream >> time;
    B_DESERIALIZE_END
}

} // namespace data
} // namespace pproto
