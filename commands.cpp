#include "commands.h"
#include "pproto/commands/pool.h"

namespace pproto {
namespace command {

#define REGISTRY_COMMAND_SINGLPROC(COMMAND, UUID) \
    const QUuidEx COMMAND = command::Pool::Registry{UUID, #COMMAND, false};

#define REGISTRY_COMMAND_MULTIPROC(COMMAND, UUID) \
    const QUuidEx COMMAND = command::Pool::Registry{UUID, #COMMAND, true};

REGISTRY_COMMAND_SINGLPROC(ImageDetect,  "a17e5c5b-8a44-494b-85ff-ab68809148e3")

#undef REGISTRY_COMMAND_SINGLPROC
#undef REGISTRY_COMMAND_MULTIPROC

} // namespace command

namespace data {

bserial::RawVector DetectRect::toRaw() const
{
    B_SERIALIZE_V1(stream)
    stream << x_min;
    stream << y_min;
    stream << x_max;
    stream << y_max;
    stream << score;
    B_SERIALIZE_RETURN
}

void DetectRect::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect, stream)
    stream >> x_min;
    stream >> y_min;
    stream >> x_max;
    stream >> y_max;
    stream >> score;
    B_DESERIALIZE_END
}

bserial::RawVector DetectClass::toRaw() const
{
    B_SERIALIZE_V1(stream)
    stream << name;
    stream << rects;
    B_SERIALIZE_RETURN
}

void DetectClass::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect, stream)
    stream >> name;
    stream >> rects;
    B_DESERIALIZE_END
}

bserial::RawVector ImageItem::toRaw() const
{
    B_SERIALIZE_V1(stream)
    stream << name;
    stream << data;
    stream << tag;
    stream << classes;
    B_SERIALIZE_RETURN
}

void ImageItem::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect, stream)
    stream >> name;
    stream >> data;
    stream >> tag;
    stream >> classes;
    B_DESERIALIZE_END
}

bserial::RawVector ImageDetect::toRaw() const
{
    B_SERIALIZE_V1(stream)
    stream << images;
    stream << returnImages;
    B_SERIALIZE_RETURN
}

void ImageDetect::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect, stream)
    stream >> images;
    stream >> returnImages;
    B_DESERIALIZE_END
}

} // namespace data
} // namespace pproto
