#pragma once

#include "shared/container_ptr.h"
#include "pproto/commands/base.h"
#include "pproto/commands/time_spec.h"

#include "detect_rect.h"
#include "model.h"

namespace pproto {
namespace command {

//----------------------------- Список команд --------------------------------

/**
  Команда возвращает информацию по критериям отбора для логических объектов
*/
extern const QUuidEx LogicSetsInfo;

/**
  Команда для конфигурирования критериев отбора для логических объектов
  TODO не реализовано
*/
//extern const QUuidEx LogicSetsConfig;

/**
  Событие для логического объекта
  TODO не реализовано
*/
//extern const QUuidEx LogicSetEvent;

/**
  Команда возвращает информацию об эмитируемых событиях
  TODO не реализовано
*/
//extern const QUuidEx LogicSetsEventsInfo;

/**
  Команда выполняет подписку на получение событий LogicSetEvent от системы
  контроля логических объектов
  TODO не реализовано
*/
//extern const QUuidEx LogicSetsEventsSubscript;

/**
  Команда возвращает текущее состояние логических объектов
*/
extern const QUuidEx LogicSetsSnapshot;

} // namespace command


//---------------- Структуры данных используемые в сообщениях ----------------

namespace data {

struct LogicSetInfo
{
    // Порядковый индекс расположения класса в выходном слое модели
    int classIndex = {-1};

    // Наименование класса модели
    QString className;

    // Глубина буфера  кадров  определяет  количество  рамок  детектирования
    // у трекируемого объекта на основании которых будет приниматься решение
    // о создании логического объекта. Трекируемые объекты с меньшим количе-
    // ством рамок не будут принимать участия в создании логических объектов.
    // Значение параметра по умолчанию 100 (4секунды X 25fps)
    int framesDepth = {100};

    // Порог насыщения (количество рамок) от параметра frames_depth по достиже-
    // нии которого будет приниматься решение о создании  логического  объекта
    int saturate = {70};

    // Целевой порог классов модели
    float score = {0.70};

    // Список индексов реальных классов модели из которых собирается метакласс,
    // используется как критерий создания логического объекта
    QSet<int> realModelClasses;

    typedef lst::List<LogicSetInfo, lst::CompareItemDummy> List;

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "class_index"         , classIndex       )
        J_SERIALIZE_MAP_ITEM( "class_name"          , className        )
        J_SERIALIZE_MAP_ITEM( "frames_depth"        , framesDepth      )
        J_SERIALIZE_MAP_ITEM( "saturate"            , saturate         )
        J_SERIALIZE_MAP_ITEM( "score"               , score            )
        J_SERIALIZE_MAP_ITEM( "real_mModel_classes" , realModelClasses )
    J_SERIALIZE_END
};

struct LogicSetsInfoA : Data<&command::LogicSetsInfo,
                              Message::Type::Answer>
{
    LogicSetInfo::List items;

    DECLARE_B_SERIALIZE_FUNC
    J_SERIALIZE_MAP_ONE( "items", items )
};

template<typename T> struct LogicCompare
{
    // Сортировка по id
    int operator() (const T* item1, const T* item2) const
        {return QUuidEx::compare(item1->id, item2->id);}

    // Поиск по id
    int operator() (const QUuidEx* id, const T* item2) const
        {return QUuidEx::compare(*id, item2->id);}
};

/**
  Описание логического объекта для внешней системы
*/
struct LogicObject
{
    // Идентификатор логического объекта
    QUuidEx id;

    // Идентификатор трекера, на основе которого был создан логический объект
    QUuidEx trackId;

    // Индекс класса реальной модели соответствующий параметру score
    int classIndex = {-1};

    // Порог насыщения (количество рамок) от параметра frames_depth
    int saturate = {0};

    // Вермя создания объекта, соответствует времени получения последней рамки
    // детектирования в списке rects
    QDateTime timeCreate;

    // Список рамок трекирования, на основе которых был создан объект
    QList<DetectRect> rects;

    // Усредненная рамка/скор. Глубина усреднения определяется параметром
    // average_depth в файле конфигурирования трекеров (videtect.track)
    // DetectRect averageRect;

    typedef lst::List<LogicObject, LogicCompare<LogicObject>> List;

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "id"          , id         )
        J_SERIALIZE_MAP_ITEM( "track_id"    , trackId    )
        J_SERIALIZE_MAP_ITEM( "class_index" , classIndex )
        J_SERIALIZE_MAP_ITEM( "saturate"    , saturate   )
        J_SERIALIZE_MAP_ITEM( "time_create" , timeCreate )
        J_SERIALIZE_MAP_ITEM( "rects"       , rects      )
        //J_SERIALIZE_MAP_ITEM( "average_rect"  , averageRect  )
    J_SERIALIZE_END
};

struct LogicSetSnapshot
{
    // Порядковый индекс расположения класса в выходном слое модели
    qint32 classIndex = {-1};

    // Наименование класса модели
    QString className;

    // Список логических объектов
    LogicObject::List objects;

    struct Find
    {
        // Поиск по classIndex
        int operator() (const qint32* classIndex, const LogicSetSnapshot* item2) const
            {return LIST_COMPARE_ITEM(*classIndex, item2->classIndex);}
    };

    typedef lst::List<LogicSetSnapshot, Find> List;

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "class_index" , classIndex )
        J_SERIALIZE_MAP_ITEM( "class_name"  , className  )
        J_SERIALIZE_MAP_ITEM( "objects"     , objects    )
    J_SERIALIZE_END
};

struct LogicSetsSnapshotA : Data<&command::LogicSetsSnapshot,
                                  Message::Type::Answer>
{
    LogicSetSnapshot::List items;

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "items" , items )
    J_SERIALIZE_END
};

} // namespace data
} // namespace pproto
