#include "video.h"
#include "pproto/commands/pool.h"

namespace pproto {
namespace command {

REGISTRY_PPROTO_COMMAND(VideoStreamOpened,   "69448105-ed53-4307-8436-a549861c56a2", SINGLPROC)
REGISTRY_PPROTO_COMMAND(VideoStreamClosed,   "6d0ba6b5-fcec-425a-92bb-2937c725fa3e", SINGLPROC)
REGISTRY_PPROTO_COMMAND(VideoLostManyFrames, "756ae276-70d1-4c77-ae69-12ddcc2e2b89", SINGLPROC)
REGISTRY_PPROTO_COMMAND(VideoFailedCapture,  "0520d9ed-97ba-492c-8072-80427cd79d8c", SINGLPROC)
REGISTRY_PPROTO_COMMAND(VideoStreamIsOpen,   "5b855d5b-d23f-488f-b237-b9b5685dfaf7", SINGLPROC)
REGISTRY_PPROTO_COMMAND(VideoSnapshot,       "e45f4c80-f142-11f0-9a81-171f19f91f1f", SINGLPROC)

} // namespace command

namespace data {

void VideoSnapshot::Request::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << type;
    stream << number;
    stream << time;
    stream << origin;
    B_SERIALIZE_RETURN
}

void VideoSnapshot::Request::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> type;
    stream >> number;
    stream >> time;
    stream >> origin;
    B_DESERIALIZE_END
}

void VideoSnapshot::Frame::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << cvType;
    stream << width;
    stream << height;
    stream << data;
    B_SERIALIZE_RETURN
}

void VideoSnapshot::Frame::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> cvType;
    stream >> width;
    stream >> height;
    stream >> data;
    B_DESERIALIZE_END
}

void VideoSnapshot::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << request;
    stream << frame;
    B_SERIALIZE_RETURN
}

void VideoSnapshot::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> request;
    stream >> frame;
    B_DESERIALIZE_END
}

// --- Оставлено для примера ---
// void VideoSnapshotError::toRaw(bserial::DataStream& stream) const
// {
//     B_SERIALIZE_V1

//     // Структуры унаследованные от MessageError сериализуются в особом порядке,
//     // поэтому для них сериализация/десериализация базового класса не обязательна
//     // stream << B_BASE_CLASS(MessageError);

//     stream << number;
//     stream << time;
//     B_SERIALIZE_RETURN
// }

// void VideoSnapshotError::fromRaw(const bserial::RawVector& vect)
// {
//     B_DESERIALIZE_V1(vect)

//     // Структуры унаследованные от MessageError сериализуются в особом порядке,
//     // поэтому для них сериализация/десериализация базового класса не обязательна
//     // stream << B_BASE_CLASS(MessageError);

//     stream >> number;
//     stream >> time;
//     B_DESERIALIZE_END
// }

} // namespace data
} // namespace pproto
