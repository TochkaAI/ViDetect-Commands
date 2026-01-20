#include "roi.h"
#include "pproto/commands/pool.h"

namespace pproto {
namespace command {}

namespace data {

void Roi::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << identity;
    stream << visible;
    stream << caption;
    stream << border;
    stream << inverse;
    B_SERIALIZE_RETURN
}

void Roi::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> identity;
    stream >> visible;
    stream >> caption;
    stream >> border;
    stream >> inverse;
    B_DESERIALIZE_END
}

} // namespace data
} // namespace pproto
