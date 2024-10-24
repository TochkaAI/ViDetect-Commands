#pragma once

#include "shared/container_ptr.h"
#include "pproto/commands/base.h"

namespace pproto {
namespace command {

//----------------------------- Список команд --------------------------------

/**
  Команда для отправки информации обо всех объектах в текущий момент
*/
extern const QUuidEx TrackInformation;

/**
  Команда для получения информации в виде снимка в данный момент
*/
extern const QUuidEx SnapshotInfo;

/**
  Команда для передачи информации по настройкам классов - текущие и новые (в будущем)
*/
extern const QUuidEx MetaSettings;

} // namespace command

//---------------- Структуры данных используемые в сообщениях ----------------

namespace data {

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

/**
  Описание объекта трекирования для внешней системы
*/
struct TrackObject
{
    typedef container_ptr<TrackObject> Ptr;

    enum class StatusObject : quint32
    {
        UNKNOWN = 0,
        Create = 10,
        AppendRect = 11,
        Validate = 20,
        DowngadeTTL = 21,
        RemoveOddObject = 22,
        ResetObject = 23,
        RecoveryTTL = 24,
        Complete = 30
    };

    // Идентификатор трекируемого объекта
    QUuidEx id;

    // Глобальный идентификатор трекирования
    QUuidEx globalId;

    // Состояние объекта на момент передачи
    StatusObject status = {StatusObject::UNKNOWN};

    // Признак вальдации объекта
    bool isValidated = {false};

    // Время начала уверенного трекирования объекта (валидация пройдена)
    QDateTime timeDetect;

    // Время завершения (потери) трекирования объекта
    QDateTime timeComplete;

    // История трекирования
    QList<DetectRect> rects;

    // Журавлев: Да, я знаю, что он есть в DetectRect, но проще будет выдать его
    // и тут, что бы верхняя система имела представление об объекте сразу
    // Усредненный по истории score
    double score = {0};

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "id"           , id           )
        J_SERIALIZE_MAP_ITEM( "globalId"     , globalId     )
        J_SERIALIZE_MAP_ITEM( "status"       , status       )
        J_SERIALIZE_MAP_ITEM( "isValidated"  , isValidated  )
        J_SERIALIZE_MAP_ITEM( "timeDetect"   , timeDetect   )
        J_SERIALIZE_MAP_ITEM( "timeComplete" , timeComplete )
        J_SERIALIZE_MAP_ITEM( "rects"        , rects        )
        J_SERIALIZE_MAP_ITEM( "score"        , score        )
    J_SERIALIZE_END
};

/**
  Структура метаинфорамции о классе для внешней системы
*/
struct MetaClass
{
    typedef container_ptr<MetaClass> Ptr;

    // Наименование класса детектирования
    QString name;

    // Признак чтения выхода нейросети для данного класса
    bool isSkip = {false};

    // Пороговое значение score для класса
    double threshScore = {0};

    // Пороговое значение отсечения nms для класса
    double threshNMS = {0};

    // Пороговое значение отсечения iou для класса
    double threshIOU = {0};

    // Пороговое значение отсечения nms для класса
    double threshTTL = {0};

    // Время валидации (в мс)
    int validateTime = {0};

    // Кол-во рамок для валидации
    int validateSaturate = {0};

    // Максимальное кол-во рамок в трекере (значени -1 - без ограничений)
    int rectsLimits = {-1};

    // Признак рисования объектов класса
    bool isPaint = {true};

    // Цвет отрисовки объектов класса на кадре
    QVector<int> color = {0,0,0};

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "name"             , name             )
        J_SERIALIZE_MAP_ITEM( "isSkip"           , isSkip           )
        J_SERIALIZE_MAP_ITEM( "threshScore"      , threshScore      )
        J_SERIALIZE_MAP_ITEM( "threshNMS"        , threshNMS        )
        J_SERIALIZE_MAP_ITEM( "threshIOU"        , threshIOU        )
        J_SERIALIZE_MAP_ITEM( "threshTTL"        , threshTTL        )
        J_SERIALIZE_MAP_ITEM( "validateTime"     , validateTime     )
        J_SERIALIZE_MAP_ITEM( "validateSaturate" , validateSaturate )
        J_SERIALIZE_MAP_ITEM( "rectsLimits"      , rectsLimits      )
        J_SERIALIZE_MAP_ITEM( "color"            , color            )
    J_SERIALIZE_END
};

/**
  Описание класса трекирования для внешней системы
*/
struct TrackClass
{
    typedef container_ptr<TrackClass> Ptr;

    // Индекс класса трекирования
    int index = {0}; // TODO: Не увестили это в meta?

    // Метаинформация о классе
    MetaClass meta; // TODO: может быть назвать information?

    // Список объектов трекирования
    QList<TrackObject> objects;

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "index"   , index   )
        J_SERIALIZE_MAP_ITEM( "meta"    , meta    )
        J_SERIALIZE_MAP_ITEM( "objects" , objects )
    J_SERIALIZE_END
};

struct TrackInformation : Data<&command::TrackInformation,
                                Message::Type::Command>
{
    typedef container_ptr<TrackInformation> Ptr;

    // Временная метка на момент которой актуальна информация
    QDateTime timeResponse;

    // Список классов со всеми объектами
    QList<TrackClass> classes;

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "timeResponse" , timeResponse )
        J_SERIALIZE_MAP_ITEM( "classes"      , classes     )
    J_SERIALIZE_END
};

struct SnapshotInfo : Data<&command::SnapshotInfo,
                            Message::Type::Command,
                            Message::Type::Answer>
{
    typedef container_ptr<SnapshotInfo> Ptr;

    /// Тело запроса

    // Временная метка отправки запроса
    QDateTime timeRequest;

    /// Тело ответа

    // Временная метка отправки ответа
    QDateTime timeResponse;

    // Информация по классам
    TrackInformation information;

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "timeRequest"     , timeRequest  )
        J_SERIALIZE_MAP_ITEM( "timeResponse"    , timeResponse )
        J_SERIALIZE_MAP_ITEM( "information"     , information  )
    J_SERIALIZE_END
};

struct MetaSettings : Data<&command::MetaSettings,
                            Message::Type::Command,
                            Message::Type::Answer>
{
    typedef container_ptr<MetaSettings> Ptr;

    enum class MethodRequest : quint32
    {
        UNKNOWN = 0,
        Set = 1,
        Update = 2,
        Read = 3,
        Delete = 4,
        Default = 5
    };

    MethodRequest method = {MethodRequest::UNKNOWN};

    /// Тело запроса

    QList<MetaClass> newSettings;

    /// Тело ответа

    QList<MetaClass> actualSettings;

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "method"         , method         )
        J_SERIALIZE_MAP_ITEM( "newSettings"    , newSettings    )
        J_SERIALIZE_MAP_ITEM( "actualSettings" , actualSettings )
    J_SERIALIZE_END
};

} // namespace data
} // namespace pproto
