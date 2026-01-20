#include "rois_info.h"
#include "pproto/commands/pool.h"

namespace pproto {
namespace command {

#define REGISTRY_COMMAND_SINGLPROC(COMMAND, UUID) \
    const QUuidEx COMMAND = command::Pool::Registry{UUID, #COMMAND, false};

#define REGISTRY_COMMAND_MULTIPROC(COMMAND, UUID) \
    const QUuidEx COMMAND = command::Pool::Registry{UUID, #COMMAND, true};

REGISTRY_COMMAND_SINGLPROC(RoisInfo, "49ee172b-2954-4f90-9965-de3804fe3f14")

#undef REGISTRY_COMMAND_SINGLPROC
#undef REGISTRY_COMMAND_MULTIPROC

} // namespace command

namespace data {

void RoisInfo::ModelClass::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << index;
    stream << skipGlobalRois;
    stream << rois;
    B_SERIALIZE_RETURN
}

void RoisInfo::ModelClass::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> index;
    stream >> skipGlobalRois;
    stream >> rois;
    B_DESERIALIZE_END
}

void RoisInfo::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << visible;
    stream << globalRois;
    stream << classes;
    B_SERIALIZE_RETURN
}

void RoisInfo::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> visible;
    stream >> globalRois;
    stream >> classes;
    B_DESERIALIZE_END
}

} // namespace data
} // namespace pproto
