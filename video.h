#pragma once

#include "shared/container_ptr.h"
#include "pproto/commands/base.h"
#include "pproto/commands/time_spec.h"
//#include "video/transport/compression.h"

namespace pproto {
namespace command {

//----------------------------- Список команд --------------------------------

/**
  Событие эмитируется при попытке открытия видеопотока. При удачном открытии по-
  тока поле сообщения tag(0) будет содержать значение 1 (TRUE),  если  открытие
  потока было неудачным поле tag(0) будет содержать значение 0 (FALSE)
*/
extern const QUuidEx VideoStreamOpened;

/**
  Событие эмитируется при закрытиии видеопотока
*/
extern const QUuidEx VideoStreamClosed;

/**
  Команда возвращает статус видеопотока открыт/закрыт в поле tag(0). Значение 0
  сигнализирует о том, что видеопоток недоступен (закрыт),  значение 1 сигнали-
  зирует об активном видеопотоке (открыт)
*/
extern const QUuidEx VideoStreamIsOpen;

/**
  Команда для получения кадра по запросу из временного буфера кадров
*/
extern const QUuidEx VideoSnapshot;


} // namespace command

//---------------- Структуры данных используемые в сообщениях ----------------

namespace data {

struct VideoSnapshot : Data<&command::VideoSnapshot,
                             Message::Type::Command,
                             Message::Type::Answer>
{
    struct Request
    {
        enum class Type : quint32
        {
            Number = 0, // Запрос кадра по номеру
            Time   = 1, // Запрос кадра по времени
            Last   = 2  // Запрос последнего кадра
        };

        Type     type   = {Type::Number};
        quint64  number = quint64(-1);
        TimeSpec time;

        DECLARE_B_SERIALIZE_FUNC

        J_SERIALIZE_BEGIN
            J_SERIALIZE_MAP_ITEM( "type"   , type   )
            J_SERIALIZE_MAP_ITEM( "number" , number )
            J_SERIALIZE_MAP_ITEM( "time"   , time   )
        J_SERIALIZE_END
    };

    // Параметры запроса кадра из временного буфера кадров
    Request request;

    struct Frame
    {
        typedef container_ptr<Frame> Ptr;

        qint32  cvType = {0}; // Тип матрицы изображения в представлении OpenCV
        qint32  width  = {0}; // Ширина кадра
        qint32  height = {0}; // Высота кадра

        SByteArray data; // Матрица кадра

        DECLARE_B_SERIALIZE_FUNC

        J_SERIALIZE_BEGIN
            J_SERIALIZE_MAP_ITEM( "cv_type" , cvType )
            J_SERIALIZE_MAP_ITEM( "width"   , width  )
            J_SERIALIZE_MAP_ITEM( "height"  , height )
            J_SERIALIZE_MAP_ITEM( "data"    , data   )
        J_SERIALIZE_END
    };

    Frame::Ptr frame;

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_OPT( "request" , request )
        J_SERIALIZE_MAP_OPT( "frame"   , frame   )
    J_SERIALIZE_END
};

// --- Оставлено для примера ---
// struct VideoSnapshotError : MessageError, Data<&command::VideoSnapshot,
//                                                 Message::Type::Answer>
// {
//     // Номер запрашиваемого кадра
//     quint64 number = quint64(-1);

//     // Время запрашиваемого кадра (UTC)
//     TimeSpec time;

//     DECLARE_B_SERIALIZE_FUNC

//     J_SERIALIZE_BEGIN
//         J_SERIALIZE_BASE   ( MessageError      )
//         J_SERIALIZE_MAP_OPT( "number" , number )
//         J_SERIALIZE_MAP_OPT( "time"   , time   )
//     J_SERIALIZE_END
// };

} // namespace data
} // namespace pproto

