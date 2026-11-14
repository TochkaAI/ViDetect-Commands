#include "commands/fpfn.h"
#include "pproto/commands/pool.h"

namespace pproto {
namespace command {

REGISTRY_PPROTO_COMMAND(FpFnInfo, "0db958aa-d392-4157-9ed8-901f258e69d9", SINGLPROC)

} // namespace command

namespace data {

void FpFnInfo::ModelClass::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << index;
    stream << active;
    stream << roi;
    stream << fpTiming;
    stream << fnTiming;
    stream << fnTtlDown;
    B_SERIALIZE_RETURN
}

void FpFnInfo::ModelClass::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> index;
    stream >> active;
    stream >> roi;
    stream >> fpTiming;
    stream >> fnTiming;
    stream >> fnTtlDown;
    B_DESERIALIZE_END
}

void FpFnInfo::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << active;
    stream << path;
    stream << prefix;
    stream << visibleRois;
    stream << markupBrush;
    stream << classes;
    stream << fileFormat;
    stream << markupSave;
    B_SERIALIZE_RETURN
}

void FpFnInfo::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> active;
    stream >> path;
    stream >> prefix;
    stream >> visibleRois;
    stream >> markupBrush;
    stream >> classes;
    stream >> fileFormat;
    stream >> markupSave;
    B_DESERIALIZE_END
}

} // namespace data
} // namespace pproto
