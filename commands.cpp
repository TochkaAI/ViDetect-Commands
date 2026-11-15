#include "commands.h"
#include "pproto/commands/pool.h"

namespace pproto {
namespace command {

REGISTRY_PPROTO_COMMAND(ConfigChanged,   "aaa0324a-aca7-4ec5-b94e-ada1716df083", SINGLPROC)
REGISTRY_PPROTO_COMMAND(ProcessingPause, "1395b089-9e7a-4f99-afc7-43cc2813cb5b", SINGLPROC)
REGISTRY_PPROTO_COMMAND(ProcessingInfo,  "b206217f-da09-4d94-a845-389ef002f494", SINGLPROC)

} // namespace command

namespace data {

void ConfigChanged::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << file;
    stream << phase;
    B_SERIALIZE_RETURN
}

void ConfigChanged::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> file;
    stream >> phase;
    B_DESERIALIZE_END
}

void ProcessingInfoA::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << paused;
    B_SERIALIZE_RETURN
}

void ProcessingInfoA::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> paused;
    B_DESERIALIZE_END
}

} // namespace data
} // namespace pproto

