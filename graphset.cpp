#include "graphset.h"
#include "pproto/commands/pool.h"

namespace pproto {
namespace command {

#define REGISTRY_COMMAND_SINGLPROC(COMMAND, UUID) \
    const QUuidEx COMMAND = command::Pool::Registry{UUID, #COMMAND, false};

#define REGISTRY_COMMAND_MULTIPROC(COMMAND, UUID) \
    const QUuidEx COMMAND = command::Pool::Registry{UUID, #COMMAND, true};

REGISTRY_COMMAND_SINGLPROC(GraphsetInfo,           "53aedbb1-ee08-4c01-8f45-499e5c099a68")
REGISTRY_COMMAND_SINGLPROC(GraphsetChanged,        "d184366a-a48e-43a6-8c7e-8561c9a43184")
REGISTRY_COMMAND_SINGLPROC(GraphsetDynamicInfo,    "b4fdbb1c-dfdf-4b25-8479-49b0902fe9e0")
REGISTRY_COMMAND_SINGLPROC(GraphsetDynamicUpdate,  "056b6217-5919-4578-8b30-7c80bb52e8df")
REGISTRY_COMMAND_SINGLPROC(GraphsetDynamicRemove,  "164cde90-b098-4e7b-9927-f1945c91c62c")
REGISTRY_COMMAND_SINGLPROC(GraphsetDynamicChanged, "85072983-a893-48b4-a916-d7a362aea537")
REGISTRY_COMMAND_SINGLPROC(GraphsetVisible,        "dac925e7-8abe-49cb-98d3-15535131850a")

#undef REGISTRY_COMMAND_SINGLPROC
#undef REGISTRY_COMMAND_MULTIPROC

} // namespace command

namespace data {


void Graphset::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << visible;
    stream << items;
    B_SERIALIZE_RETURN
}

void Graphset::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> visible;
    stream >> items;
    B_DESERIALIZE_END
}

void GraphsetInfoA::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << value;
    B_SERIALIZE_RETURN
}

void GraphsetInfoA::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> value;
    B_DESERIALIZE_END
}

void GraphsetDynamicInfoA::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << items;
    B_SERIALIZE_RETURN
}

void GraphsetDynamicInfoA::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> items;
    B_DESERIALIZE_END
}

void GraphsetDynamicUpdate::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << items;
    B_SERIALIZE_RETURN
}

void GraphsetDynamicUpdate::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> items;
    B_DESERIALIZE_END
}

void GraphsetDynamicRemove::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << items;
    B_SERIALIZE_RETURN
}

void GraphsetDynamicRemove::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> items;
    B_DESERIALIZE_END
}

void GraphsetVisible::Set::Item::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << identity;
    stream << visible;
    B_SERIALIZE_RETURN
}

void GraphsetVisible::Set::Item::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> identity;
    stream >> visible;
    B_DESERIALIZE_END
}

void GraphsetVisible::Set::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << texts;
    stream << lines;
    stream << rectangles;
    stream << circles;
    B_SERIALIZE_RETURN
}

void GraphsetVisible::Set::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> texts;
    stream >> lines;
    stream >> rectangles;
    stream >> circles;
    B_DESERIALIZE_END
}

void GraphsetVisible::toRaw(bserial::DataStream& stream) const
{
    B_SERIALIZE_V1
    stream << visible;
    stream << items;
    B_SERIALIZE_RETURN
}

void GraphsetVisible::fromRaw(const bserial::RawVector& vect)
{
    B_DESERIALIZE_V1(vect)
    stream >> visible;
    stream >> items;
    B_DESERIALIZE_END
}

} // namespace data
} // namespace pproto
