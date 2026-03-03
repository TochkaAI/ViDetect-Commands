#pragma once

#include "shared/container_ptr.h"
#include "pproto/commands/base.h"
#include "pproto/commands/time_spec.h"

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

/**
  Команда для получения кадра по номеру  или времени  создания  из  временного
  буфера кадров
*/
extern const QUuidEx FrameSnapshot;

/**
  Команда останавливает/возобновляет обработку  кадров  нейросетью.  Остановка
  осуществляется через передачу в поле tag(0)  значения 1.  Для  возобновления
  обработки кадров надо передать в tag(0) значения 0
*/
extern const QUuidEx ProcessingPause;

/**
  Команда возвращает информацию о процессе обработки
*/
extern const QUuidEx ProcessingInfo;

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
        Logic   = 8, // Конфиг-файл Logic
        Clsf    = 9, // Конфиг-файл классификаторов
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

struct FrameSnapshot : Data<&command::FrameSnapshot,
                             Message::Type::Command,
                             Message::Type::Answer>
{
    // Номер запрашиваемого кадра. Если номер кадра равен -1, то параметр будет
    // проигнорирован и поиск будет выполнен по полю time
    quint64 number = quint64(-1);

    // Время запрашиваемого кадра (UTC). Поиск по этому параметру выполняется,
    // когда значение поля number равно -1
    TimeSpec time;

    struct VideoFrame
    {
        typedef container_ptr<VideoFrame> Ptr;

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

    VideoFrame::Ptr videoFrame;

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_OPT( "number"      , number     )
        J_SERIALIZE_MAP_OPT( "time"        , time       )
        J_SERIALIZE_MAP_OPT( "video_frame" , videoFrame )
    J_SERIALIZE_END
};

// --- Оставлено для примера ---
// struct FrameSnapshotError : MessageError, Data<&command::FrameSnapshot,
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

struct ProcessingInfoA : Data<&command::ProcessingInfo,
                               Message::Type::Answer>
{
    // Признак приостановки обработки кадров
    bool paused = {false};

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "paused" , paused )
    J_SERIALIZE_END
};

} // namespace data
} // namespace pproto
