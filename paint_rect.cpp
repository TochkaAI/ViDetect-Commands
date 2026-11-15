#include "paint_rect.h"

namespace pproto {
namespace data {

void PaintRect::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << id;
    stream << visible;
    stream << caption;
    stream << border;
    stream << center;
    stream << graphset;
    stream << onlyValidated;
    B_SERIALIZE_RETURN
}

void PaintRect::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> id;
    stream >> visible;
    stream >> caption;
    stream >> border;
    stream >> center;
    stream >> graphset;
    stream >> onlyValidated;
    B_DESERIALIZE_END
}

void PaintClassif::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << modelIndex;
    stream << classIndex;
    stream << rect;
    B_SERIALIZE_RETURN
}

void PaintClassif::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> modelIndex;
    stream >> classIndex;
    stream >> rect;
    B_DESERIALIZE_END
}

void PaintRects::ModelClass::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << index;
    stream << rect;
    stream << classifiers;
    B_SERIALIZE_RETURN
}

void PaintRects::ModelClass::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> index;
    stream >> rect;
    stream >> classifiers;
    B_DESERIALIZE_END
}

void PaintRects::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << visible;
    stream << classes;
    B_SERIALIZE_RETURN
}

void PaintRects::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> visible;
    stream >> classes;
    B_DESERIALIZE_END
}

} // namespace data
} // namespace pproto
