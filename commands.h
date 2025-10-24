#pragma once

#include "shared/container_ptr.h"
#include "pproto/commands/base.h"

namespace pproto {
namespace command {

//----------------------------- Список команд --------------------------------

/**
  Событие эмитируется в моменты начала и окончания обновления  состояния  меха-
  низмов ViDetect. Обновление механизмов  ViDetect  происходит  после  фиксации
  факта изменения конфигурационных файлов. Описание отслеживаемых конфиг-файлов
  смотри в структуре ConfChange
*/
extern const QUuidEx ConfigChanged;

} // namespace command

struct ConfChange
{
    enum class File : quint32
    {
        Unknown = 0,
        Base    = 1, // Базовый конфиг-файл
        Model   = 2, // Конфиг-файл модели
        Track   = 3, // Конфиг-файл трекера
        Rect    = 4, // Конфиг-файл рамок трекируемых объектов
        Graph   = 5, // Конфиг-файл с наборами графических примитивов
        Roi     = 6, // Конфиг-файл ROI
        FpFn    = 7, // Конфиг-файл FpFn
    };

    enum class Phase : quint32
    {
        Unknown = 0,
        Begin   = 1, // Начало обновления состояния механизма
        End     = 2  // Окончание обновления состояния механизма
    };
};

//---------------- Структуры данных используемые в сообщениях ----------------

namespace data {

struct ConfigChanged : Data<&command::ConfigChanged,
                             Message::Type::Event>
{
    ConfChange::File  file  = {ConfChange::File::Unknown};
    ConfChange::Phase phase = {ConfChange::Phase::Unknown};

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "file" , file  )
        J_SERIALIZE_MAP_OPT ( "phase", phase )
    J_SERIALIZE_END
};

} // namespace data
} // namespace pproto
