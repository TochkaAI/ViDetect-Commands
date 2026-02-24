#pragma once

#include "pproto/commands/base.h"
#include "pproto/commands/time_spec.h"

namespace pproto {
namespace command {

//----------------------------- Список команд --------------------------------

/**
  Команда используется для получения/передачи координат камеры
*/
extern const QUuidEx Coordinates;

} // namespace command

//---------------- Структуры данных используемые в сообщениях ----------------

namespace data {

struct Coordinates : Data<&command::Coordinates,
                           Message::Type::Command,
                           Message::Type::Answer>
{
    typedef container_ptr<Coordinates> Ptr;

    float x     = {0};
    float y     = {0};
    float z     = {0};
    float alpha = {0};
    float beta  = {0};
    float gamma = {0};

    data::TimeSpec time; // Время получения координат (UTC)

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "x"     , x     )
        J_SERIALIZE_MAP_ITEM( "y"     , y     )
        J_SERIALIZE_MAP_ITEM( "z"     , z     )
        J_SERIALIZE_MAP_ITEM( "alpha" , alpha )
        J_SERIALIZE_MAP_ITEM( "beta"  , beta  )
        J_SERIALIZE_MAP_ITEM( "gamma" , gamma )
        J_SERIALIZE_MAP_ITEM( "time"  , time  )
    J_SERIALIZE_END
};

} // namespace data
} // namespace pproto
