#pragma once

#include "shared/container_ptr.h"
#include "pproto/commands/base.h"
#include "pproto/commands/time_spec.h"

#include "compare.h"
#include "model.h"
#include "detect_rect.h"
#include "paint_rect.h"

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
  Команда возвращает информацию о параметрах отображения рамок логических
  объектов
*/
extern const QUuidEx LogicRectsInfo;

/**
  Событие эмитируется при перечитывании информации о параметрах отображения
  рамок логических объектов из конфигурационного файла (videtect.rect)
*/
extern const QUuidEx LogicRectsChanged;

/**
  Команда возвращает текущее состояние логических объектов
*/
extern const QUuidEx LogicSetsSnapshot;

/**
  Возвращает информацию о дистанциях между логическими объектами
*/
extern const QUuidEx LogicDistances;

/**
  Команда выполняет сброс всех логических объектов и дистанций между ними.
  После выполнения команды логические объекты начнут нарождаться заново
  на основе существующих объектов трекирования
*/
extern const QUuidEx LogicObjectsReset;

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

    struct RealModel
    {
        typedef container_ptr<RealModel> Ptr;

        // Список индексов реальных классов модели из которых собирается мета-
        // класс, используется как критерий создания логического объекта
        QSet<int> classes;

        // Индекс класса реальной модели, соответствующий годному объекту. © Ганин
        int classAccept = {-1};

        DECLARE_B_SERIALIZE_FUNC

        J_SERIALIZE_BEGIN
            J_SERIALIZE_MAP_ITEM( "classes"      , classes     )
            J_SERIALIZE_MAP_ITEM( "class_accept" , classAccept )
        J_SERIALIZE_END

    };

    // Критерии реальной модели на основе  которых  происходит  создание  логи-
    // ческого объекта. Если параметр classifModel определен, то он будет иметь
    // приоритет над параметром realModel
    RealModel::Ptr realModel;

    struct ClassifModel
    {
        typedef container_ptr<ClassifModel> Ptr;

        // Порядковый индекс модели классификации в файле videtect.clsf.0
        int index = {-1};

        // Список индексов классов модели классификации, используется как
        // критерий создания логического объекта
        QSet<int> classes;

        // Индекс класса модели классификации, соответствующий годному объекту
        int classAccept = {-1};

        DECLARE_B_SERIALIZE_FUNC

        J_SERIALIZE_BEGIN
            J_SERIALIZE_MAP_ITEM( "index"        , index       )
            J_SERIALIZE_MAP_ITEM( "classes"      , classes     )
            J_SERIALIZE_MAP_ITEM( "class_accept" , classAccept )
        J_SERIALIZE_END
    };

    // Критерии модели классификации на основе которых происходит создание логи-
    // ческого объекта. Если параметр classifModel определен, то он будет иметь
    // приоритет над параметром realModel
    ClassifModel::Ptr classifModel;

    typedef lst::List<LogicSetInfo, lst::CompareItemDummy> List;

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "class_index"        , classIndex   )
        J_SERIALIZE_MAP_ITEM( "class_name"         , className    )
        J_SERIALIZE_MAP_ITEM( "frames_depth"       , framesDepth  )
        J_SERIALIZE_MAP_ITEM( "saturate"           , saturate     )
        J_SERIALIZE_MAP_ITEM( "score"              , score        )
        J_SERIALIZE_MAP_ITEM( "real_model"         , realModel    )
        J_SERIALIZE_MAP_ITEM( "classifier_model"   , classifModel )
    J_SERIALIZE_END
};

struct LogicSetsInfoA : Data<&command::LogicSetsInfo,
                              Message::Type::Answer>
{
    LogicSetInfo::List items;

    DECLARE_B_SERIALIZE_FUNC
    J_SERIALIZE_MAP_ONE( "items", items )
};

struct LogicRectsInfoA : Data<&command::LogicRectsInfo,
                               Message::Type::Answer>
{
    PaintRects::Ptr value;

    DECLARE_B_SERIALIZE_FUNC
    J_SERIALIZE_MAP_ONE( "value", value )
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

    // Индекс класса реальной модели
    int realClassIndex = {-1};

    // Индекс модели классификации и класса модели классификации
    int classifModelIndex = {-1};
    int classifClassIndex = {-1};

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

    typedef lst::List<LogicObject, CompareId<LogicObject>> List;

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "id"                  , id                )
        J_SERIALIZE_MAP_ITEM( "track_id"            , trackId           )
        J_SERIALIZE_MAP_ITEM( "real_class_index"    , realClassIndex    )
        J_SERIALIZE_MAP_ITEM( "classif_model_index" , classifModelIndex )
        J_SERIALIZE_MAP_ITEM( "classif_class_index" , classifClassIndex )
        J_SERIALIZE_MAP_ITEM( "saturate"            , saturate          )
        J_SERIALIZE_MAP_ITEM( "time_create"         , timeCreate        )
        J_SERIALIZE_MAP_ITEM( "rects"               , rects             )
        //J_SERIALIZE_MAP_ITEM( "average_rect"      , averageRect       )
    J_SERIALIZE_END
};

struct LogicSetSnapshot
{
    // Порядковый индекс расположения класса в выходном слое модели (в реальном
    // слое или в слое мета-модели)
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

struct LogicSetsSnapshot : Data<&command::LogicSetsSnapshot,
                                 Message::Type::Command>
{
    // Режим получения Snapshot-а логических объектов
    enum class Mode : quint32
    {
        Normal  = 0, // Нормальный режим. В этом режиме команда  делает  копию
                     // логических объектов. Логические  объекты  для  которых
                     // завершено трекирование (объект трекера завершил сущест-
                     // вование) будут удалены из пула объектов ViDetect после
                     // выполнения этого запроса.
        Extract = 1  // Режим извлечения. После выполнения команды все логичес-
                     // кие объекты будут удалены  из пула  объектов  ViDetect.
                     // Новые логические объекты на основе существующих объек-
                     // тов трекирования создаваться не будут
    };

    Mode mode = {Mode::Normal};

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_OPT( "mode" , mode )
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

struct LogicDistances : Data<&command::LogicDistances,
                              Message::Type::Command,
                              Message::Type::Answer>
{
    // Параметры запроса
    struct Request
    {
        typedef container_ptr<Request> Ptr;

        // Идентификатор логического объекта
        QUuidEx logicId;

        // Список индексов классов модели (реальной или мета-модели) для которых
        // возвращается дистанции до объекта с идентификатором logicId
        QSet<int> calcDistanceBetween;

        DECLARE_B_SERIALIZE_FUNC

        J_SERIALIZE_BEGIN
            J_SERIALIZE_MAP_ITEM( "logic_id"              , logicId             )
            J_SERIALIZE_MAP_ITEM( "calc_distance_between" , calcDistanceBetween )
        J_SERIALIZE_END
    };

    struct Request2
    {
        typedef container_ptr<Request2> Ptr;

        // Первое множество классов модели (logicset.model_classes) для которых
        // будет создаваться выборка дистанций
        QSet<int> classIndexes1;

        // Второе множество классов модели (logicset.model_classes) для которых
        // будет создаваться выборка дистанций
        QSet<int> classIndexes2;

        DECLARE_B_SERIALIZE_FUNC

        J_SERIALIZE_BEGIN
            J_SERIALIZE_MAP_ITEM( "class_indexes1" , classIndexes1 )
            J_SERIALIZE_MAP_ITEM( "class_indexes2" , classIndexes2 )
        J_SERIALIZE_END
    };

    // Информация о дистанции между двумя логическими объектами
    struct Item
    {
        qint32 classIndex1 = {-1}; // Индекс класса первого логического объекта
        qint32 classIndex2 = {-1}; // Индекс класса второго логического объекта

        QUuidEx logicId1; // Идентификатор первого логического объекта
        QUuidEx logicId2; // Идентификатор второго логического объекта

        float dx = {0};   // Дистанция между объектами по оси X
        float dy = {0};   // Дистанция между объектами по оси Y

        typedef lst::List<Item, lst::CompareItemDummy> List;

        DECLARE_B_SERIALIZE_FUNC

        J_SERIALIZE_BEGIN
            J_SERIALIZE_MAP_ITEM( "class_index1" , classIndex1 )
            J_SERIALIZE_MAP_ITEM( "class_index2" , classIndex2 )
            J_SERIALIZE_MAP_ITEM( "logic_id1"    , logicId1    )
            J_SERIALIZE_MAP_ITEM( "logic_id2"    , logicId2    )
            J_SERIALIZE_MAP_ITEM( "dx"           , dx          )
            J_SERIALIZE_MAP_ITEM( "dy"           , dy          )
        J_SERIALIZE_END
    };

    // Параметры запроса
    Request::Ptr  request;
    Request2::Ptr request2;

    // Результаты запроса
    Item::List items;

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_OPT( "request"  , request )
        J_SERIALIZE_MAP_OPT( "request2" , request2 )
        J_SERIALIZE_MAP_OPT( "items"    , items   )
    J_SERIALIZE_END
};

} // namespace data
} // namespace pproto
