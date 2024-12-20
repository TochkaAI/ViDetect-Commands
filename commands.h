#pragma once

#include "shared/container_ptr.h"
#include "pproto/commands/base.h"

#ifdef OPENCV_IS_PRESENT
#include <opencv2/opencv.hpp>
#endif

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

/**
  Возвращает информацию о наборе графических примитивов
*/
extern const QUuidEx GraphsetInfo;

/**
  Команда для конфигурирования набора графических примитивов
  TODO Не реализовано. Команда должна полностью заменять список
  графических примитивов. Прорабатать вопрос с опциональным полем brush
*/
extern const QUuidEx GraphsetConfig;

/**
  Команда для конфигурирования видимости набора графических примитивов
  TODO Не реализовано.
*/
extern const QUuidEx GraphsetVisible;

/**
  Команда для добавления новых графических примитивов
  TODO Не реализовано.
*/
extern const QUuidEx GraphsetAdd;

/**
  Команда для удаления существующих графических примитивов
  TODO Не реализовано.
*/
extern const QUuidEx GraphsetRemove;

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

namespace graph {

struct Identity
{
    QUuidEx id;
    quint32 index = {0};
    QString name;

    // Определяет поле, по которому нужно выполнять поиск
    enum class FindField : quint32
    {
        Id    = 0,
        Index = 1,
        Name  = 2
    };
    FindField findField = {FindField::Index};

    template<typename T> struct Find
    {
        int operator() (const QUuidEx* id, const T* item2) const
            {return QUuidEx::compare(*id, item2->identity.id);}

        int operator() (const quint32* index, const T* item2) const
            {return LIST_COMPARE_ITEM(*index, item2->identity.index);}

        int operator() (const QString* name, const T* item2) const
            {return name->compare(item2->identity.name);}

        int operator() (const Identity* item1, const T* item2) const
        {
            if (item1->findField == FindField::Id)
                return operator() (&item1->id, item2);

            else if (item1->findField == FindField::Index)
                return operator() (&item1->index, item2);

            else if (item1->findField == FindField::Name)
                return operator() (&item1->name, item2);

            return -1;
        }
    };

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_OPT( "id"         , id        )
        J_SERIALIZE_MAP_OPT( "index"      , index     )
        J_SERIALIZE_MAP_OPT( "name"       , name      )
        J_SERIALIZE_MAP_OPT( "find_field" , findField )
    J_SERIALIZE_END
};

struct Point
{
    qint32 x = {0};
    qint32 y = {0};

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "x" , x )
        J_SERIALIZE_MAP_ITEM( "y" , y )
    J_SERIALIZE_END
};

struct Offset
{
    qint32 x = {0};
    qint32 y = {0};

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "x" , x )
        J_SERIALIZE_MAP_ITEM( "y" , y )
    J_SERIALIZE_END
};

struct Color
{
    quint8 r = {255};
    quint8 g = {255};
    quint8 b = {255};

#ifdef OPENCV_IS_PRESENT
    operator cv::Scalar () const {return CV_RGB(r, g, b);}
#endif

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "r" , r )
        J_SERIALIZE_MAP_ITEM( "g" , g )
        J_SERIALIZE_MAP_ITEM( "b" , b )
    J_SERIALIZE_END
};

struct Brush
{
    qint32 thickness = {1};
    Color  color;

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_OPT( "thickness" , thickness )
        J_SERIALIZE_MAP_OPT( "color"     , color )
    J_SERIALIZE_END
};

struct Text
{
    // Идентификационный признак
    Identity identity;

    // Признак отображения примитива в кадре
    bool visible = {true};

    // Кисть
    Brush brush;

    // Начальная позиция рисования текста
    Point position;

    // Смещения начальной позиции рисования текста по осям X, Y. Может принимать
    // отрицательные значения
    Offset offset;

    // Масштаб текста (в процентах)
    quint32 scale = {100};

    // Текст
    QString text;

#ifdef OPENCV_IS_PRESENT
    cv::Point position_() const;
    void setPosition(const cv::Point&);

    cv::Point offset_() const;
    void setOffset(const cv::Point&);
#endif

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "identity" , identity )
        J_SERIALIZE_MAP_OPT ( "visible"  , visible  )
        J_SERIALIZE_MAP_OPT ( "brush"    , brush    )
        J_SERIALIZE_MAP_ITEM( "position" , position )
        J_SERIALIZE_MAP_OPT ( "offset"   , offset   )
        J_SERIALIZE_MAP_OPT ( "scale"    , scale    )
        J_SERIALIZE_MAP_ITEM( "text"     , text     )
    J_SERIALIZE_END

    typedef lst::List<Text, Identity::Find<Text>> List;
};

struct Line
{
    // Идентификационный признак
    Identity identity;

    // Признак отображения примитива в кадре
    bool visible = {true};

    // Кисть
    Brush brush;

    Point point1; // Первая точка линии
    Point point2; // Вторая точка линии

#ifdef OPENCV_IS_PRESENT
    // Точка с координатами point1
    cv::Point p1() const;
    void setP1(const cv::Point&);

    // Точка с координатами point2
    cv::Point p2() const;
    void setP2(const cv::Point&);
#endif

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "identity" , identity )
        J_SERIALIZE_MAP_OPT ( "visible"  , visible  )
        J_SERIALIZE_MAP_OPT ( "brush"    , brush    )
        J_SERIALIZE_MAP_ITEM( "point1"   , point1   )
        J_SERIALIZE_MAP_ITEM( "point2"   , point2   )
    J_SERIALIZE_END

    typedef lst::List<Line, Identity::Find<Line>> List;
};

struct Rectangle
{
    // Идентификационный признак
    Identity identity;

    // Признак отображения примитива в кадре
    bool visible = {true};

    // Кисть
    Brush brush;

    Point point1 = {0, 0};     // Верхняя левая точка (синоним top/left)
    Point point2 = {100, 100}; // Правая нижняя точка (синоним bottom/right)

#ifdef OPENCV_IS_PRESENT
    // Точка с координатами point1 (синоним top/left)
    cv::Point p1() const;
    void setP1(const cv::Point&);

    // Точка с координатами point2 (синоним bottom/right)
    cv::Point p2() const;
    void setP2(const cv::Point&);

    // Верхний левый угол прямоугольника (top/left)
    cv::Point tl() const {return p1();}
    void setTl(const cv::Point& p) {setP1(p);}

    // Нижний правый угол прямоугольника (bottom/right)
    cv::Point br() const {return p2();}
    void setBr(const cv::Point& p) {setP2(p);}

    // Прямоугольник в cv::Rect представлении
    cv::Rect rect() const;
    void setRect(const cv::Rect&);
#endif

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "identity" , identity )
        J_SERIALIZE_MAP_OPT ( "visible"  , visible  )
        J_SERIALIZE_MAP_OPT ( "brush"    , brush    )
        J_SERIALIZE_MAP_ITEM( "point1"   , point1   )
        J_SERIALIZE_MAP_ITEM( "point2"   , point2   )
    J_SERIALIZE_END

    typedef lst::List<Rectangle, Identity::Find<Rectangle>> List;
};

struct Circle
{
    // Идентификационный признак
    Identity identity;

    // Признак отображения примитива в кадре
    bool visible = {true};

    // Кисть
    Brush brush;

    Point center;
    quint32 radius = {0};

#ifdef OPENCV_IS_PRESENT
    cv::Point center_() const;
    void setCenter(const cv::Point&);
#endif

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "identity" , identity )
        J_SERIALIZE_MAP_OPT ( "visible"  , visible  )
        J_SERIALIZE_MAP_OPT ( "brush"    , brush    )
        J_SERIALIZE_MAP_ITEM( "center"   , center   )
        J_SERIALIZE_MAP_ITEM( "radius"   , radius   )
    J_SERIALIZE_END

    typedef lst::List<Circle, Identity::Find<Circle>> List;
};

/**
  Набор графических примитиров
*/
struct Set
{
    // Идентификационный признак
    Identity identity;

    // Признак отображения всего набора графических примитивов
    bool visible = {true};

    // Общая кисть
    Brush brush;

    // Граница области набора графических примитивов
    Rectangle border;

    // Набор текстовых примитивов
    Text::List texts;

    // Набор линий
    Line::List lines;

    // Набор прямоугольников
    Rectangle::List rectangles;

    // Набор окружностей
    Circle::List circles;

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "identity"   , identity   )
        J_SERIALIZE_MAP_OPT ( "visible"    , visible    )
        J_SERIALIZE_MAP_OPT ( "border"     , border     )
        J_SERIALIZE_MAP_OPT ( "texts"      , texts      )
        J_SERIALIZE_MAP_OPT ( "lines"      , lines      )
        J_SERIALIZE_MAP_OPT ( "rectangles" , rectangles )
        J_SERIALIZE_MAP_OPT ( "circles"    , circles    )
    J_SERIALIZE_END

    typedef lst::List<Set, Identity::Find<Set>> List;
};

} // namespace graph

struct Graphset
{
    // Признак отображения всех наборов графических примитивов
    bool visible = {true};

    // Список наборов графических примитивов
    data::graph::Set::List items;

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "visible" , visible )
        J_SERIALIZE_MAP_ITEM( "items"   , items   )
    J_SERIALIZE_END
};

struct GraphsetInfoA : Graphset, Data<&command::GraphsetInfo,
                                       Message::Type::Answer>
{
};

struct GraphsetVisible : Data<&command::GraphsetVisible,
                               Message::Type::Command>
{
    // Признак отображения всех наборов графических примитивов
    bool visible = {true};

    struct Set
    {
        struct Item
        {
            graph::Identity identity; // Идентификационный признак
            bool visible = {true};    // Признак отображения примитива в кадре

            DECLARE_B_SERIALIZE_FUNC

            J_SERIALIZE_BEGIN
                J_SERIALIZE_MAP_ITEM( "identity" , identity )
                J_SERIALIZE_MAP_ITEM( "visible"  , visible  )
            J_SERIALIZE_END
        };

        QList<Item> texts;      // Набор текстовых примитивов
        QList<Item> lines;      // Набор линий
        QList<Item> rectangles; // Набор прямоугольников
        QList<Item> circles;    // Набор окружностей

        DECLARE_B_SERIALIZE_FUNC

        J_SERIALIZE_BEGIN
            J_SERIALIZE_MAP_ITEM( "texts"      , texts      )
            J_SERIALIZE_MAP_ITEM( "lines"      , lines      )
            J_SERIALIZE_MAP_ITEM( "rectangles" , rectangles )
            J_SERIALIZE_MAP_ITEM( "circles"    , circles    )
        J_SERIALIZE_END
    };

    QList<Set> items;

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "visible" , visible )
        J_SERIALIZE_MAP_ITEM( "items"   , items   )
    J_SERIALIZE_END
};

} // namespace data
} // namespace pproto
