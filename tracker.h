#pragma once

#include "shared/container_ptr.h"
#include "pproto/commands/base.h"
#include "pproto/commands/time_spec.h"

#include "model.h"

namespace pproto {
namespace command {

//----------------------------- Список команд --------------------------------

/**
  Команда возвращает информацию по используемым трекерам
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
  Команда возвращает информацию об эмитируемых событиях для всех трекеров
*/
extern const QUuidEx TrackersEventsInfo;

/**
  Команда выполняет подписку на получение событий TrackerEvent от системы
  трекирования
*/
extern const QUuidEx TrackersEventsSubscript;

/**
  Команда возвращает текущее состояние трекируемых объектов во всех трекерах
*/
extern const QUuidEx TrackersSnapshot;

/**
  Команда возвращает информацию о параметрах отображения рамок трекирования
*/
extern const QUuidEx TrackRectsInfo;

/**
  Событие эмитируется при перечитывании информации о параметрах отображения
  рамок трекирования из конфигурационного файла (videtect.rect)
*/
extern const QUuidEx TrackRectsChanged;

/**
  Команда возвращает информацию об индивидуальных параметрах отображения рамок
  трекирования
*/
extern const QUuidEx TrackRectsDynamicInfo;

/**
  Команда обновляет или добавляет элементы TrackRect в список индивидуальных
  параметров отображения для рамок трекируемых объектов
*/
extern const QUuidEx TrackRectsDynamicUpdate;

/**
  Команда удаляет элементы TrackRect из списка индивидуальных параметров
  отображения для рамок трекируемых объектов
*/
extern const QUuidEx TrackRectsDynamicRemove;

/**
  Событие эмитируется при изменении списока индивидуальных параметров отображе-
  ния для рамок трекируемых объектов
*/
extern const QUuidEx TrackRectsDynamicChanged;

/**
  Команда обновляет или устанавливает глобальный идентификатор для трекируемого
  объекта
*/
extern const QUuidEx TrackGlobalIdUpdate;

} // namespace command

/**
  Коды событий трекера. Коды соотносятся с полями структуры tochka::TrackEvents
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

/**
  Координаты бокса (области детектированного объекта)
*/
struct DetectRect
{
    qint32 x_min = {0}; // Координата Top-Left
    qint32 y_min = {0}; // Координата Top-Left
    qint32 x_max = {0}; // Координата Bottom-Right
    qint32 y_max = {0}; // Координата Bottom-Right

    float    score  = {0}; // Оценка детектированной области
    TimeSpec time;         // Время получения кадра (UTC)
    quint64  number = quint64(-1); // Номер кадра

    struct Segment
    {
        typedef container_ptr<Segment> Ptr;
        QVector<graph::Point> contour;

        DECLARE_B_SERIALIZE_FUNC
        DECLARE_J_SERIALIZE_FUNC
    };
    Segment::Ptr segment;


#ifdef OPENCV_IS_PRESENT
    // Точка с координатами point1 (синоним top/left)
    cv::Point p1(const cv::Point& offset = {}) const;
    void setP1(const cv::Point&);

    // Точка с координатами point2 (синоним bottom/right)
    cv::Point p2(const cv::Point& offset = {}) const;
    void setP2(const cv::Point&);

    // Прямоугольник в cv::Rect представлении
    cv::Rect rect(const cv::Point& offset = {}) const;
    void setRect(const cv::Rect&);
#endif

    graph::Point center() const; // Центр прямоугольника

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "x_min"   , x_min   )
        J_SERIALIZE_MAP_ITEM( "y_min"   , y_min   )
        J_SERIALIZE_MAP_ITEM( "x_max"   , x_max   )
        J_SERIALIZE_MAP_ITEM( "y_max"   , y_max   )
        J_SERIALIZE_MAP_ITEM( "score"   , score   )
        J_SERIALIZE_MAP_ITEM( "time"    , time    )
        J_SERIALIZE_MAP_ITEM( "number"  , number  )
        J_SERIALIZE_MAP_ITEM( "segment" , segment )
    J_SERIALIZE_END
};

template<typename This, typename Packer>
Packer& DetectRect::Segment::jserialize(const This* ct, Packer& p)
{
    This* t = const_cast<This*>(ct);

    p.startObject();

    if (p.isWriter())
    {
        QVector<int> vect;
        for (int i = 0; i < t->contour.count(); ++i)
        {
            vect.append(t->contour[i].x);
            vect.append(t->contour[i].y);
        }
        p.member("contour") & vect;
    }
    else if (p.isReader())
    {
        QVector<int> vect;
        p.member("contour") & vect;

        t->contour.clear();
        for (int i = 0; i < vect.count(); i += 2)
        {
            if ((i + 1) >= vect.count())
                break;
            qint32 x = vect[i];
            qint32 y = vect[i + 1];
            t->contour.append({x, y});
        }
    }
    return p.endObject();
}

/**
  Механизмы сопоставления/матчинга объектов
*/
struct TrackMatching
{
    enum class Type : quint32
    {
        Unknown = 0,
        Iou = 1, // Intersection over Union
        Atd = 2, // Asymmetric Triangle distance
    };

    struct Iou
    {
        typedef container_ptr<Iou> Ptr;

        // Пороговое значение iou
        float threshold = {0.3};

        DECLARE_B_SERIALIZE_FUNC
        J_SERIALIZE_MAP_ONE( "threshold", threshold )
    };

    struct Atd
    {
        typedef container_ptr<Atd> Ptr;

        // f(x) ^
        //    1 |          *
        //      |         *| *
        //      |        * |   *
        //      |       *  |     *
        //      |      *   |       *
        //      |     *    |         *
        //    0 |* * *     |           * * * *
        //      +--------------------------> x
        //           |     |           |
        //           x1    x2          x3

        QString axis; // Допустимые значения: 'XY', 'X', 'Y'

        float x1 = {0}; // x left
        float x2 = {0}; // x center
        float x3 = {0}; // x right

        float y1 = {0}; // y top
        float y2 = {0}; // y center
        float y3 = {0}; // y bottom

        // Пороговые значения по осям X, Y
        float thresholdX = {0.3};
        float thresholdY = {0.3};

        DECLARE_B_SERIALIZE_FUNC

        J_SERIALIZE_BEGIN
            J_SERIALIZE_MAP_ITEM( "axis" , axis )
            J_SERIALIZE_MAP_ITEM( "x1"   , x1   )
            J_SERIALIZE_MAP_ITEM( "x2"   , x2   )
            J_SERIALIZE_MAP_ITEM( "x3"   , x3   )
            J_SERIALIZE_MAP_ITEM( "y1"   , y1   )
            J_SERIALIZE_MAP_ITEM( "y2"   , y2   )
            J_SERIALIZE_MAP_ITEM( "y3"   , y3   )
            J_SERIALIZE_MAP_ITEM( "threshold_x" , thresholdX )
            J_SERIALIZE_MAP_ITEM( "threshold_y" , thresholdY )
        J_SERIALIZE_END
    };

    Type type = {Type::Iou};

    Iou::Ptr iou;
    Atd::Ptr atd;

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "type", type )
        J_SERIALIZE_MAP_OPT ( "iou" , iou  )
        J_SERIALIZE_MAP_OPT ( "atd" , atd  )
    J_SERIALIZE_END
};

/**
  Механизм группировки трекируемых объектов
*/
struct TrackGroup
{
    typedef container_ptr<TrackGroup> Ptr;

    // Индекс группы
    qint32 index = {-1};

    // Смещение трекируемых объектов принадлежащих группе по осям X, Y.
    // Размерность смещения: пиксель/кадр.
    float moveX = {0};
    float moveY = {0};

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "index"  , index )
        J_SERIALIZE_MAP_OPT ( "move_x" , moveX )
        J_SERIALIZE_MAP_OPT ( "move_y" , moveY )
    J_SERIALIZE_END
};

struct TrackerInfo
{
    // Порядковый индекс расположения класса в выходном слое модели
    qint32 classIndex = {-1};

    // Наименование класса модели
    QString className;

    // Matching-функция
    TrackMatching matching;

    // Пороговое значениея ttl
    qint32 ttl = {12};

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

    // Глубина усреднения координат и скоров для списка TrackBase::rects.
    // Используется для минимизации эффекта дрожания при отрисовке рамок
    qint32 rectsAverageDepth = {10};

    // Группировка трекируемых объектов
    TrackGroup::Ptr group;

    typedef lst::List<TrackerInfo, lst::CompareItemDummy> List;

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "class_index"         , classIndex         )
        J_SERIALIZE_MAP_ITEM( "class_name"          , className          )
        J_SERIALIZE_MAP_ITEM( "matching"            , matching           )
        J_SERIALIZE_MAP_ITEM( "ttl"                 , ttl                )
        J_SERIALIZE_MAP_ITEM( "validate_time"       , validateTime       )
        J_SERIALIZE_MAP_ITEM( "validate_saturation" , validateSaturation )
        J_SERIALIZE_MAP_ITEM( "rects_time_limit"    , rectsTimeLimit     )
        J_SERIALIZE_MAP_ITEM( "rects_count_limit"   , rectsCountLimit    )
        J_SERIALIZE_MAP_ITEM( "rects_average_depth" , rectsAverageDepth  )
        J_SERIALIZE_MAP_ITEM( "group"               , group              )
    J_SERIALIZE_END
};

struct TrackersInfoA : Data<&command::TrackersInfo,
                             Message::Type::Answer>
{
    TrackerInfo::List items;

    DECLARE_B_SERIALIZE_FUNC
    J_SERIALIZE_MAP_ONE( "items", items )
};

template<typename T> struct TrackCompare
{
    // Сортировка по id
    int operator() (const T* item1, const T* item2) const
        {return QUuidEx::compare(item1->id, item2->id);}

    // Поиск по id
    int operator() (const QUuidEx* id, const T* item2) const
        {return QUuidEx::compare(*id, item2->id);}
};

/**
  Описание объекта трекирования для внешней системы
*/
struct TrackObject
{
    // Идентификатор трекируемого объекта
    QUuidEx id;

    // Глобальный идентификатор трекирования, используется для прослеживания
    // объекта между разными зонами/камерами
    quint64 globalId  = {0};

    // Вспомогательное поле, используется для расширения глобального идентифи-
    // катора трекирования до UUID представления
    quint64 globalIdExt = {0};

    // Возвращает глобальный идентификатор трекирования в UUID представлении
    QUuidEx globalUuid() const;

    // Признак вальдации объекта
    bool isValidated = {false};

    // Время начала уверенного трекирования объекта (валидация пройдена)
    QDateTime timeDetect;

    // Время завершения (потери) трекирования объекта
    QDateTime timeComplete;

    // История трекирования
    QList<DetectRect> rects;

    // Усредненная рамка/скор. Глубина усреднения определяется параметром
    // average_depth в файле конфигурирования трекеров (videtect.track)
    DetectRect averageRect;

    typedef lst::List<TrackObject, TrackCompare<TrackObject>> List;

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "id"            , id           )
        J_SERIALIZE_MAP_ITEM( "global_id"     , globalId     )
        J_SERIALIZE_MAP_ITEM( "global_id_ext" , globalIdExt  )
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

    // Наименование класса модели
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

struct TrackersEventsInfoA : Data<&command::TrackersEventsInfo,
                                   Message::Type::Answer>
{
    struct Item
    {
        // Порядковый индекс расположения класса в выходном слое модели
        qint32 classIndex = {-1};

        // Наименование класса модели
        QString className;

        // Набор событий
        QSet<pproto::TrackEvent> events;

        typedef lst::List<Item, lst::CompareItemDummy> List;

        DECLARE_B_SERIALIZE_FUNC

        J_SERIALIZE_BEGIN
            J_SERIALIZE_MAP_ITEM( "class_index" , classIndex )
            J_SERIALIZE_MAP_ITEM( "class_name"  , className  )
            J_SERIALIZE_MAP_ITEM( "events"      , events     )
        J_SERIALIZE_END
    };

    Item::List items;

    DECLARE_B_SERIALIZE_FUNC
    J_SERIALIZE_MAP_ONE( "items", items )
};

struct TrackersEventsSubscript : Data<&command::TrackersEventsSubscript,
                                       Message::Type::Command>
{
    // Набор индексов классов в выходном слое модели, для которых будет выпол-
    // нена подписка на события. Если набор пустой - события будут  подписаны
    // для всех классов
    QSet<qint32> classIndexes;

    // Набор событий, на которые будет выполнена подписка. Если набор событий
    // оставить пустым, то будет выполнена отписка от всех событий
    QSet<pproto::TrackEvent> events;

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "class_indexes" , classIndexes )
        J_SERIALIZE_MAP_ITEM( "events"        , events       )
    J_SERIALIZE_END
};

struct TrackerSnapshot
{
    // Порядковый индекс расположения класса в выходном слое модели
    qint32 classIndex = {-1};

    // Наименование класса модели
    QString className;

    // Список трекируемых объектов
    TrackObject::List objects;

    struct Find
    {
        // Поиск по classIndex
        int operator() (const qint32* classIndex, const TrackerSnapshot* item2) const
            {return LIST_COMPARE_ITEM(*classIndex, item2->classIndex);}
    };

    typedef lst::List<TrackerSnapshot, Find> List;

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "class_index" , classIndex )
        J_SERIALIZE_MAP_ITEM( "class_name"  , className  )
        J_SERIALIZE_MAP_ITEM( "objects"     , objects    )
    J_SERIALIZE_END
};

struct TrackersSnapshot : Data<&command::TrackersSnapshot,
                                Message::Type::Command>
{
    struct VideoFrame
    {
        // Запрос матрицы видео-фрейма (поле VideoFrame::data в структуре TrackersSnapshotA)
        // при получении snapshot-а трекеров
        bool requestData = {false};

        // Передача матрицы видео-фрейма через shared memory
        // TODO зарезервировано для будущего использования
        bool sharedMem = {false};

        DECLARE_B_SERIALIZE_FUNC

        J_SERIALIZE_BEGIN
            J_SERIALIZE_MAP_OPT( "request_data" , requestData )
            J_SERIALIZE_MAP_OPT( "shared_mem"   , sharedMem   )
        J_SERIALIZE_END
    };

    VideoFrame videoFrame;

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_OPT( "video_frame" , videoFrame )
    J_SERIALIZE_END
};

struct TrackersSnapshotA : Data<&command::TrackersSnapshot,
                                 Message::Type::Answer>
{
    struct VideoFrame
    {
        typedef container_ptr<VideoFrame> Ptr;

        quint64 number = quint64(-1); // Номер кадра
        qint32  cvType = {0}; // Тип матрицы изображения в представлении OpenCV
        qint32  width  = {0}; // Ширина кадра
        qint32  height = {0}; // Высота кадра

        SByteArray data; // Матрица кадра

        DECLARE_B_SERIALIZE_FUNC

        J_SERIALIZE_BEGIN
            J_SERIALIZE_MAP_ITEM( "number"  , number )
            J_SERIALIZE_MAP_ITEM( "cv_type" , cvType )
            J_SERIALIZE_MAP_ITEM( "width"   , width  )
            J_SERIALIZE_MAP_ITEM( "height"  , height )
            J_SERIALIZE_MAP_ITEM( "data"    , data   )
        J_SERIALIZE_END
    };

    TrackerSnapshot::List items;
    VideoFrame::Ptr videoFrame;

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "items"       , items      )
        J_SERIALIZE_MAP_OPT ( "video_frame" , videoFrame )
    J_SERIALIZE_END
};

struct TrackRect
{
    // Идентификатор трекируемого объекта, используется для динамической
    // раскраски объекта
    QUuidEx id;

    // Признак отображения рамки трекирования в кадре
    bool visible = {true};

    // Отображать рамки трекирования только для валидированных объектов
    bool onlyValidated = {false};

    // Надпись для рамки трекирования с привязкой к одному из углов геометри-
    // ческой области, по умолчанию к верхнему левому углу
    graph::Caption caption;

    // Геометрическая область рамки трекирования.  Значимыми  являются  только
    // признак видимости и параметры кисти. Геометрия рамки берется от трекера
    graph::Rectangle border;

    // Геометрический центр рамки трекирования. Значимыми являются только приз-
    // нак видимости, параметры кисти, радиус.  Координаты  центра  окружности
    // берутся от трекера
    graph::Circle center;

    // Набор графических примитивов, используется для отрисовки дополнительной/
    // вспомогательной информации по трекируемому объекту
    graph::Set::Ptr graphset;

    typedef lst::List<TrackRect, TrackCompare<TrackRect>> List;

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "id"             , id            )
        J_SERIALIZE_MAP_OPT ( "visible"        , visible       )
        J_SERIALIZE_MAP_OPT ( "only_validated" , onlyValidated )
        J_SERIALIZE_MAP_OPT ( "caption"        , caption       )
        J_SERIALIZE_MAP_OPT ( "border"         , border        )
        J_SERIALIZE_MAP_OPT ( "center"         , center        )
        J_SERIALIZE_MAP_OPT ( "graphset"       , graphset      )
    J_SERIALIZE_END
};

struct TrackRects
{
    typedef container_ptr<TrackRects> Ptr;

    // Признак отображения всех рамок трекирования
    bool visible = {false};

    struct ModelClass
    {
        // Порядковый индекс расположения класса в выходном слое модели
        qint32 index = {0};

        // Параметры отображения рамок трекирования для класса модели
        TrackRect rect;

        typedef lst::List<ModelClass, data::ModelClass::Find<ModelClass>> List;

        DECLARE_B_SERIALIZE_FUNC

        J_SERIALIZE_BEGIN
            J_SERIALIZE_MAP_ITEM( "index" , index )
            J_SERIALIZE_MAP_ITEM( "rect"  , rect  )
        J_SERIALIZE_END
    };

    ModelClass::List classes;

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "visible" , visible )
        J_SERIALIZE_MAP_ITEM( "classes" , classes )
    J_SERIALIZE_END
};

struct TrackRectsInfoA : Data<&command::TrackRectsInfo,
                               Message::Type::Answer>
{
    TrackRects::Ptr value;

    DECLARE_B_SERIALIZE_FUNC
    J_SERIALIZE_MAP_ONE( "value", value )
};

struct TrackRectsDynamicInfoA : Data<&command::TrackRectsDynamicInfo,
                                      Message::Type::Answer>
{
    TrackRect::List items;

    DECLARE_B_SERIALIZE_FUNC
    J_SERIALIZE_MAP_ONE( "items", items )
};

struct TrackRectsDynamicUpdate : Data<&command::TrackRectsDynamicUpdate,
                                       Message::Type::Command>
{
    TrackRect::List items;

    DECLARE_B_SERIALIZE_FUNC
    J_SERIALIZE_MAP_ONE( "items", items )
};

struct TrackRectsDynamicRemove : Data<&command::TrackRectsDynamicRemove,
                                       Message::Type::Command>
{
    // Список идентификаторов трекируемых объектов
    QSet<QUuidEx> items;

    DECLARE_B_SERIALIZE_FUNC
    J_SERIALIZE_MAP_ONE( "items", items )
};

struct TrackGlobalIdUpdate : Data<&command::TrackGlobalIdUpdate,
                                   Message::Type::Command>
{
    // Идентификатор трекируемого объекта
    QUuidEx id;

    // Глобальный идентификатор трекирования, используется для прослеживания
    // объекта между разными зонами/камерами
    quint64 globalId  = {0};

    // Вспомогательное поле, используется для расширения глобального идентифи-
    // катора трекирования до UUID представления
    quint64 globalIdExt = {0};

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "id"            , id          )
        J_SERIALIZE_MAP_ITEM( "global_id"     , globalId    )
        J_SERIALIZE_MAP_OPT ( "global_id_ext" , globalIdExt )
    J_SERIALIZE_END
};

} // namespace data
} // namespace pproto
