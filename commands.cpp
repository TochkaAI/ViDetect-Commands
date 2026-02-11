#include "commands.h"
#include "pproto/commands/pool.h"

namespace pproto {
namespace command {

#define REGISTRY_COMMAND_SINGLPROC(COMMAND, UUID) \
    const QUuidEx COMMAND = command::Pool::Registry{UUID, #COMMAND, false};

#define REGISTRY_COMMAND_MULTIPROC(COMMAND, UUID) \
    const QUuidEx COMMAND = command::Pool::Registry{UUID, #COMMAND, true};

REGISTRY_COMMAND_SINGLPROC(ConfigChanged,   "aaa0324a-aca7-4ec5-b94e-ada1716df083")
REGISTRY_COMMAND_SINGLPROC(FrameSnapshot,   "e45f4c80-f142-11f0-9a81-171f19f91f1f")
REGISTRY_COMMAND_SINGLPROC(ProcessingPause, "1395b089-9e7a-4f99-afc7-43cc2813cb5b")
REGISTRY_COMMAND_SINGLPROC(ProcessingInfo,  "b206217f-da09-4d94-a845-389ef002f494")

#undef REGISTRY_COMMAND_SINGLPROC
#undef REGISTRY_COMMAND_MULTIPROC

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

void FrameSnapshot::VideoFrame::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << cvType;
    stream << width;
    stream << height;
    stream << data;
    B_SERIALIZE_RETURN
}

void FrameSnapshot::VideoFrame::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> cvType;
    stream >> width;
    stream >> height;
    stream >> data;
    B_DESERIALIZE_END
}

void FrameSnapshot::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << number;
    stream << time;
    stream << videoFrame;
    B_SERIALIZE_RETURN
}

void FrameSnapshot::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> number;
    stream >> time;
    stream >> videoFrame;
    B_DESERIALIZE_END
}

// --- Оставлено для примера ---
// void FrameSnapshotError::toRaw(bserial::DataStream& stream) const
// {
//     B_SERIALIZE_V1

//     // Структуры унаследованные от MessageError сериализуются в особом порядке,
//     // поэтому для них сериализация/десериализация базового класса не обязательна
//     // stream << B_BASE_CLASS(MessageError);

//     stream << number;
//     stream << time;
//     B_SERIALIZE_RETURN
// }

// void FrameSnapshotError::fromRaw(const bserial::RawVector& vect)
// {
//     B_DESERIALIZE_V1(vect)

//     // Структуры унаследованные от MessageError сериализуются в особом порядке,
//     // поэтому для них сериализация/десериализация базового класса не обязательна
//     // stream << B_BASE_CLASS(MessageError);

//     stream >> number;
//     stream >> time;
//     B_DESERIALIZE_END
// }

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

