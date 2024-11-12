#pragma once

#include "shared/container_ptr.h"
#include "pproto/commands/base.h"

namespace pproto {
namespace command {

//----------------------------- Список команд --------------------------------

/**
  Возвращает информацию по используемой модели, по классам модели, по настройкам
  трекера, по настройкам отрисовки рамок детектирования (и возможно по зонам ROI,
  это еще на разработку)
*/
extern const QUuidEx ModelInfo;

/**
  Команда для конфигурирования классов модели. Допускается изменять наименование
  класса модели и параметр skip
  TODO не реализовано
*/
extern const QUuidEx ModelConfig;

/**
  Возвращает информацию по используемым трекерам
*/
extern const QUuidEx TrackersInfo;

/**
  Команда для конфигурирования трекеров
  TODO не реализовано
*/
extern const QUuidEx TrackersConfig;

/**
  Событие трекера
*/
extern const QUuidEx TrackerEvent;

/**
  Возвращает информацию об эмитируемых событиях для всех трекеров
*/
extern const QUuidEx TrackersEventInfo;

/**
  Команда выполняет подписку на получение событий TrackerEvent от системы
  трекирования
*/
extern const QUuidEx TrackersEventSubscript;

/**
  Возвращает текущее состояние трекируемых объектов во всех трекерах
*/
extern const QUuidEx TrackersSnapshot;

/**
  Передает информацию о дополнительно необходимых отрисовках на кадре от
  системы верхнего уровня к VDetect
*/
extern const QUuidEx PostprocessingMarkup;

} // namespace command

enum class ModelType : quint32
{
    Unknown = 0,
    YoloX   = 1,
    Yolov5  = 5,
    Yolov7  = 7,
    Yolov9  = 9,
};

/**
  Коды событий трекера. Коды соотносятся с полями структуры infer::TrackEvents
*/
enum class TrackEvent : quint32
{
    Unknown   = 0,
    Create    = 1, // Объект трекирования создан, но еще не валидирован
    Append    = 2, // В объект трекирования добавлена рамка
    Validate  = 3, // Объект трекирования валидирован
    Remove    = 4, // Удален случайный объект (не прошел валидацию)
    Downgrade = 5, // Для валидированного объекта рамка трекирования
                   // не найдена, TTL понижен на 1
    Recovery  = 6, // Для валидированного объекта найдена подтверждающая
                   // рамка трекирования. Значение TTL восстановлено до
                   // порогового значения ttlThresh (см. параметр трекера)
    Complete  = 7, // Трекирование объекта завершено
    Reset     = 8  // Трекирование объекта принудительно сброшено
};

inline uint qHash(TrackEvent te) {return static_cast<uint>(te);}

inline QDataStream& operator>> (QDataStream& s, pproto::TrackEvent& te)
    {return bserial::detail::getFromStreamEnum(s, te);}

inline QDataStream& operator<< (QDataStream& s, const pproto::TrackEvent te)
    {return bserial::detail::putToStreamEnum(s, te);}

//---------------- Структуры данных используемые в сообщениях ----------------

namespace data {

struct ModelClass
{
    // Порядковый индекс класса модели
    qint32 index = {-1};

    // Наименование класса модели
    QString name;

    // Не обрабатывать выход нейросети для данного класса
    bool skip = {false};

    // Индивидуальные пороговые значения для класса модели
    float scoreThresh = {0.70};
    float nmsThresh = {0.50};

    // TODO возможно здесь надо добавить поля для конфигурирования отрисовки рамок

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "index"        , index       )
        J_SERIALIZE_MAP_ITEM( "name"         , name        )
        J_SERIALIZE_MAP_ITEM( "skip"         , skip        )
        J_SERIALIZE_MAP_ITEM( "score_thresh" , scoreThresh )
        J_SERIALIZE_MAP_ITEM( "nms_thresh"   , nmsThresh   )
    J_SERIALIZE_END
};

struct ModelInfoA : Data<&command::ModelInfo,
                          Message::Type::Answer>
{
    // Наименование файла модели
    QString file;

    // Архитектура модели
    ModelType type = {ModelType::Unknown};

    // Архитектура модели (строковое представление)
    QString typeStr;

    // Количество каналов во входном изображении. Допускается значение '1' для
    // чернобелого изображения, и '3' - для цветного изображения
    qint8 imageChannels = {3};

    // Размер входного слоя модели
    quint32 inputSize = {640};

    // Наименование входного слоя модели
    QString input;

    // Наименование выходного слоя модели
    QString output;

    // Список классов модели
    QList<ModelClass> classes;

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "file"           , file          )
        J_SERIALIZE_MAP_ITEM( "type"           , type          )
        J_SERIALIZE_MAP_ITEM( "type_str"       , typeStr       )
        J_SERIALIZE_MAP_ITEM( "image_channels" , imageChannels )
        J_SERIALIZE_MAP_ITEM( "size"           , inputSize     )
        J_SERIALIZE_MAP_ITEM( "input"          , input         )
        J_SERIALIZE_MAP_ITEM( "output"         , output        )
        J_SERIALIZE_MAP_ITEM( "classes"        , classes       )
    J_SERIALIZE_END
};

/**
  Координаты бокса (области детектированного объекта)
*/
struct DetectRect
{
    qint32 x_min = {0}; // Координата Top-Left
    qint32 y_min = {0}; // Координата Top-Left
    qint32 x_max = {0}; // Координата Bottom-Right
    qint32 y_max = {0}; // Координата Bottom-Right

    float score  = {0}; // Оценка детектированной области

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "x_min" , x_min )
        J_SERIALIZE_MAP_ITEM( "y_min" , y_min )
        J_SERIALIZE_MAP_ITEM( "x_max" , x_max )
        J_SERIALIZE_MAP_ITEM( "y_max" , y_max )
        J_SERIALIZE_MAP_ITEM( "score" , score )
    J_SERIALIZE_END
};

struct TrackerInfo
{
    // Порядковый индекс расположения класса в выходном слое модели
    qint32 classIndex = {-1};

    // Наименованеи класса модели
    QString className;

    // Пороговые значения iou, ttl
    float  iouThresh = {0.3};
    qint32 ttlThresh = {12};

    // Порог валидации по времени (в миллисекундах)
    qint32 validateTime = {500};

    // Порог валидации по насыщению (количество рамок)
    qint32 validateSaturation = {10};

    // Ограничение числа трекируемых рамок в списке TrackBase::rects
    // по времени (в миллисекундах)
    qint32 rectsTimeLimit = {5*1000}; // 5 секунд

    // Ограничение числа трекируемых рамок в списке TrackBase::rects
    // по количеству
    qint32 rectsCountLimit = {-1};

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "class_index"         , classIndex         )
        J_SERIALIZE_MAP_ITEM( "class_name"          , className          )
        J_SERIALIZE_MAP_ITEM( "iou_thresh"          , iouThresh          )
        J_SERIALIZE_MAP_ITEM( "ttl_thresh"          , ttlThresh          )
        J_SERIALIZE_MAP_ITEM( "validate_time"       , validateTime       )
        J_SERIALIZE_MAP_ITEM( "validate_saturation" , validateSaturation )
        J_SERIALIZE_MAP_ITEM( "rects_time_limit"    , rectsTimeLimit     )
        J_SERIALIZE_MAP_ITEM( "rects_count_limit"   , rectsCountLimit    )
    J_SERIALIZE_END
};

struct TrackersInfoA : Data<&command::TrackersInfo,
                             Message::Type::Answer>
{
    QList<TrackerInfo> items;

    DECLARE_B_SERIALIZE_FUNC
    J_SERIALIZE_MAP_ONE( "items", items )
};

/**
  Описание объекта трекирования для внешней системы
*/
struct TrackObject
{
    // Идентификатор трекируемого объекта
    QUuidEx id;

    // Глобальный идентификатор трекирования
    QUuidEx globalId;

    // Признак вальдации объекта
    bool isValidated = {false};

    // Время начала уверенного трекирования объекта (валидация пройдена)
    QDateTime timeDetect;

    // Время завершения (потери) трекирования объекта
    QDateTime timeComplete;

    // История трекирования
    QList<DetectRect> rects;

    // Усредненная рамка/скор за 1-8 итераций
    DetectRect averageRect;

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "id"            , id           )
        J_SERIALIZE_MAP_ITEM( "global_id"     , globalId     )
        J_SERIALIZE_MAP_ITEM( "is_validated"  , isValidated  )
        J_SERIALIZE_MAP_ITEM( "time_detect"   , timeDetect   )
        J_SERIALIZE_MAP_ITEM( "time_complete" , timeComplete )
        J_SERIALIZE_MAP_ITEM( "rects"         , rects        )
        J_SERIALIZE_MAP_ITEM( "average_rect"  , averageRect  )
    J_SERIALIZE_END
};

struct TrackerEvent : Data<&command::TrackerEvent,
                            Message::Type::Event>
{
    // Порядковый индекс расположения класса в выходном слое модели
    qint32 classIndex = {-1};

    // Наименованеи класса модели
    QString className;

    // Эмитируемое событие
    TrackEvent event = {TrackEvent::Unknown};

    // Трекируемый объект
    TrackObject object;

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "class_index" , classIndex )
        J_SERIALIZE_MAP_ITEM( "class_name"  , className  )
        J_SERIALIZE_MAP_ITEM( "event"       , event      )
        J_SERIALIZE_MAP_ITEM( "object"      , object     )
    J_SERIALIZE_END
};

struct TrackersEventInfoA : Data<&command::TrackersEventInfo,
                                  Message::Type::Answer>
{
    struct Item
    {
        // Порядковый индекс расположения класса в выходном слое модели
        qint32 classIndex = {-1};

        // Наименованеи класса модели
        QString className;

        // Набор событий
        QSet<pproto::TrackEvent> events;

        DECLARE_B_SERIALIZE_FUNC

        J_SERIALIZE_BEGIN
            J_SERIALIZE_MAP_ITEM( "class_index" , classIndex )
            J_SERIALIZE_MAP_ITEM( "class_name"  , className  )
            J_SERIALIZE_MAP_ITEM( "events"      , events     )
        J_SERIALIZE_END
    };

    QList<Item> items;

    DECLARE_B_SERIALIZE_FUNC
    J_SERIALIZE_MAP_ONE( "items", items )
};

struct TrackersEventSubscript : Data<&command::TrackersEventSubscript,
                                      Message::Type::Command>
{
    // Список индексов классов в выходном слое модели, для которых будет выпол-
    // нена подписка на события. Если список пустой - события будут  подписаны
    // для всех классов
    QList<qint32> classIndexes;

    // Набор событий
    QSet<pproto::TrackEvent> events;

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "class_indexes" , classIndexes )
        J_SERIALIZE_MAP_ITEM( "events"       , events        )
    J_SERIALIZE_END
};

struct TrackerSnapshot
{
    // Порядковый индекс расположения класса в выходном слое модели
    qint32 classIndex = {-1};

    // Наименованеи класса модели
    QString className;

    // Список трекируемых объектов
    QList<TrackObject> objects;

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "class_index" , classIndex )
        J_SERIALIZE_MAP_ITEM( "class_name"  , className  )
        J_SERIALIZE_MAP_ITEM( "objects"     , objects    )
    J_SERIALIZE_END
};

struct TrackersSnapshotA : Data<&command::TrackersSnapshot,
                                 Message::Type::Answer>
{
    QList<TrackerSnapshot> items;

    DECLARE_B_SERIALIZE_FUNC
    J_SERIALIZE_MAP_ONE( "items", items )
};

struct MarkupObject
{
    typedef container_ptr<MarkupObject> Ptr;

    enum class TypeFigure : quint32
    {
        Unknouwn = 0,
        Point = 1,
        Line = 2,
        Rectangle = 3,
        Circle = 4
    };

    enum class GroupFigure : quint32
    {
        Unknouwn = 0,
        OrdinaryFigure = 1,
        ROI = 2,
        CurrectionFigure = 3
    };

    struct Text
    {
        QString text;
        QList<int> position;
        int scale = {0};
        QList<int> color;
        int thickness = {0};
        float offset = {0};

        DECLARE_B_SERIALIZE_FUNC
        J_SERIALIZE_BEGIN
            J_SERIALIZE_MAP_ITEM( "text"      , text      )
            J_SERIALIZE_MAP_ITEM( "position"  , position  )
            J_SERIALIZE_MAP_ITEM( "scale"     , scale     )
            J_SERIALIZE_MAP_ITEM( "color"     , color     )
            J_SERIALIZE_MAP_ITEM( "thickness" , thickness )
            J_SERIALIZE_MAP_ITEM( "offset"    , offset    )
        J_SERIALIZE_END
    };

    GroupFigure group = {GroupFigure::Unknouwn};
    TypeFigure type = {TypeFigure::Unknouwn};
    QList<int> coorditane;
    float thickness = {0};
    QList<int> color;
    Text descriptor;

    DECLARE_B_SERIALIZE_FUNC
    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "group"      , group      )
        J_SERIALIZE_MAP_ITEM( "type"       , type       )
        J_SERIALIZE_MAP_ITEM( "coorditane" , coorditane )
        J_SERIALIZE_MAP_ITEM( "thickness"  , thickness  )
        J_SERIALIZE_MAP_ITEM( "descriptor" , descriptor )
    J_SERIALIZE_END
};

struct PostprocessingMarkup : Data<&command::PostprocessingMarkup,
                                    Message::Type::Command>
{

    QList<MarkupObject::Ptr> items;

    DECLARE_B_SERIALIZE_FUNC
    J_SERIALIZE_MAP_ONE( "items", items )
};

} // namespace data
} // namespace pproto
