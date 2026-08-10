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

    Coordinates();

    union {
        struct {
            double x  = {0}; // Координата по оси  X (в метрах)
            double y  = {0}; // Координата по оси  Y (в метрах)
            double z  = {0}; // Координата по оси  Z (в метрах)
            double rx = {0}; // Поворот вокруг оси X (в радианах)
            double ry = {0}; // Поворот вокруг оси Y (в радианах)
            double rz = {0}; // Поворот вокруг оси Z (в радианах)
        };
        double data[6];
    };

    data::TimeSpec time; // Время получения координат (UTC)

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_ITEM( x    )
        J_SERIALIZE_ITEM( y    )
        J_SERIALIZE_ITEM( z    )
        J_SERIALIZE_ITEM( rx   )
        J_SERIALIZE_ITEM( ry   )
        J_SERIALIZE_ITEM( rz   )
        J_SERIALIZE_ITEM( time )
    J_SERIALIZE_END
};

} // namespace data
} // namespace pproto
