#pragma once

#include "shared/container_ptr.h"
#include "pproto/commands/base.h"

#ifdef INFERENCE_IS_PRESENT
#include "inference/detect_rect.h"
#endif

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
  Возвращает информацию о параметрах отрисоки рамок трекирования
*/
extern const QUuidEx TrackRectsInfo;

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

/**
  Возвращает информацию о ROIs
  TODO Не реализовано.
*/
extern const QUuidEx RoisInfo;

/**
  Возвращает информацию настройках FP/FN
  TODO Не реализовано.
*/
extern const QUuidEx FpFnInfo;

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
namespace graph {

/**
  Структура используется для идентификации графического примитива. Идентификация
  может осуществляться по одному из трех полей:  id, index, name. За выбор поля
  для идентификации примитива отвечает пользователь API
*/
struct Identity
{
    QUuidEx id;
    qint32  index = {-1};
    QString name;

    // Определяет поле, по которому нужно выполнять поиск/идентификацию
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

        int operator() (const qint32* index, const T* item2) const
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

struct Point
{
    qint32 x = {0};
    qint32 y = {0};

    bool isNull() const;

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "x" , x )
        J_SERIALIZE_MAP_ITEM( "y" , y )
    J_SERIALIZE_END
};

struct Offset
{
    qint32 dx = {0};
    qint32 dy = {0};

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "dx" , dx )
        J_SERIALIZE_MAP_ITEM( "dy" , dy )
    J_SERIALIZE_END
};

struct Caption
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

    // Вспомогательный параметр,  используется для привязки надписи к одному из
    // углов прямоугольной рамки ROI или рамки детектирования. Параметр привязки
    // может принимать значения в диапазоне [1, 4], где 1 соответствует верхнему
    // левому углу прямоугольника. Обход углов происходит  по  часовой  стрелке.
    // Функция привязки  имеет  приоритет  над  параметром  position.  Механизм
    // деактивируется когда cornerBind меньше 1. Значение параметра по умолчанию
    // равно 1 (привязка к верхнему левому углу)
    qint32 cornerBind = {1};

#ifdef OPENCV_IS_PRESENT
    cv::Point position_() const;
    void setPosition(const cv::Point&);

    cv::Point offset_() const;
    void setOffset(const cv::Point&);
#endif

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "identity"    , identity   )
        J_SERIALIZE_MAP_OPT ( "visible"     , visible    )
        J_SERIALIZE_MAP_OPT ( "brush"       , brush      )
        J_SERIALIZE_MAP_ITEM( "position"    , position   )
        J_SERIALIZE_MAP_OPT ( "offset"      , offset     )
        J_SERIALIZE_MAP_OPT ( "scale"       , scale      )
        J_SERIALIZE_MAP_ITEM( "text"        , text       )
        J_SERIALIZE_MAP_ITEM( "corner_bind" , cornerBind )
    J_SERIALIZE_END

    typedef lst::List<Caption, Identity::Find<Caption>> List;
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

    bool isNull() const;

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

    Point point1; // Верхняя левая точка (синоним top/left)
    Point point2; // Правая нижняя точка (синоним bottom/right)

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

    bool isNull() const;

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

struct Roi
{
    // Идентификационный признак
    Identity identity;

    // Признак отображения примитива в кадре
    bool visible = {true};

    // Надпись ROI с привязкой к одному из углов геометрической области,
    // по умолчанию к верхнему левому углу
    Caption caption;

    // Геометрическая область ROI
    Rectangle border;

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "identity" , identity )
        J_SERIALIZE_MAP_OPT ( "visible"  , visible  )
        J_SERIALIZE_MAP_OPT ( "caption"  , caption  )
        J_SERIALIZE_MAP_ITEM( "border"   , border   )
    J_SERIALIZE_END

    typedef lst::List<Roi, Identity::Find<Roi>> List;
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

    // Граница области набора графических примитивов
    Rectangle border;

    // Набор текстовых примитивов
    Caption::List captions;

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
        J_SERIALIZE_MAP_OPT ( "captions"   , captions   )
        J_SERIALIZE_MAP_OPT ( "lines"      , lines      )
        J_SERIALIZE_MAP_OPT ( "rectangles" , rectangles )
        J_SERIALIZE_MAP_OPT ( "circles"    , circles    )
    J_SERIALIZE_END

    typedef lst::List<Set, Identity::Find<Set>> List;
};

struct TrackRect
{
    // Идентификатор трекируемого объекта, используется для динамической
    // раскраски объекта
    QUuidEx id;

    // Глобальный идентификатор трекирования
    // QUuidEx globalId;

    // Порядковый индекс расположения класса в выходном слое модели
    // qint32 classIndex = {-1};

    // Признак отображения примитива в кадре
    bool visible = {true};

    // Надпись для рамки трекирования с привязкой к одному из углов геометри-
    // ческой области, по умолчанию к верхнему левому углу
    graph::Caption caption;

    // Геометрическая область рамки трекирования.  Значимыми  являются  только
    // признак видимости и параметры кисти, геометрия рамки берется от трекера
    graph::Rectangle border;

    // TODO на проработку
    // graph::Set graphset;

    typedef lst::List<TrackRect, lst::CompareItemDummy> List;

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "id"      , id      )
        J_SERIALIZE_MAP_OPT ( "visible" , visible )
        J_SERIALIZE_MAP_OPT ( "caption" , caption )
        J_SERIALIZE_MAP_ITEM( "border"  , border  )
    J_SERIALIZE_END
};

} // namespace graph

struct ModelClass
{
    // Порядковый индекс класса модели
    qint32 index = {-1};

    // Наименование класса модели
    QString name;

    // Не обрабатывать выход нейросети для данного класса
    bool skip = {false};

    // Индивидуальные пороговые значения для класса модели
    float cuttingThresh = {0.10};
    float scoreThresh = {0.70};
    float nmsThresh = {0.50};

    // TODO возможно здесь надо добавить поля для конфигурирования отрисовки рамок

    typedef lst::List<ModelClass, lst::CompareItemDummy> List;

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "index"          , index         )
        J_SERIALIZE_MAP_ITEM( "name"           , name          )
        J_SERIALIZE_MAP_ITEM( "skip"           , skip          )
        J_SERIALIZE_MAP_ITEM( "cutting_thresh" , cuttingThresh )
        J_SERIALIZE_MAP_ITEM( "score_thresh"   , scoreThresh   )
        J_SERIALIZE_MAP_ITEM( "nms_thresh"     , nmsThresh     )
    J_SERIALIZE_END
};

struct ModelInfo
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

    // Количество записей в выходном слое модели. Для yolov9 количество столбцов
    // выходного слоя
    qint32 recordCount = {-1};

    // Список классов модели
    ModelClass::List classes;

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "file"           , file          )
        J_SERIALIZE_MAP_ITEM( "type"           , type          )
        J_SERIALIZE_MAP_ITEM( "type_str"       , typeStr       )
        J_SERIALIZE_MAP_ITEM( "image_channels" , imageChannels )
        J_SERIALIZE_MAP_ITEM( "size"           , inputSize     )
        J_SERIALIZE_MAP_ITEM( "input"          , input         )
        J_SERIALIZE_MAP_ITEM( "output"         , output        )
        J_SERIALIZE_MAP_ITEM( "record_count"   , recordCount   )
        J_SERIALIZE_MAP_ITEM( "classes"        , classes       )
    J_SERIALIZE_END
};

struct ModelInfoA : ModelInfo, Data<&command::ModelInfo,
                                     Message::Type::Answer>
{
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

    typedef lst::List<TrackerInfo, lst::CompareItemDummy> List;

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
    TrackerInfo::List items;

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

    typedef lst::List<TrackObject, lst::CompareItemDummy> List;

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
    TrackObject::List objects;

    typedef lst::List<TrackerSnapshot, lst::CompareItemDummy> List;

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
    TrackerSnapshot::List items;

    DECLARE_B_SERIALIZE_FUNC
    J_SERIALIZE_MAP_ONE( "items", items )
};

struct TrackRectsInfo
{
    // Признак отображения всех рамок трекирования
    bool visible = {false};

    struct ModelClass
    {
        // Порядковый индекс расположения класса в выходном слое модели
        qint32 index = {0};

        // Параметры отображения рамок трекирования для класса модели
        graph::TrackRect rect;

        typedef lst::List<ModelClass, lst::CompareItemDummy> List;

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

struct TrackRectsInfoA : TrackRectsInfo,  Data<&command::TrackRectsInfo,
                                                Message::Type::Answer>
{
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

struct RoisInfo
{
    // Признак отображения всех наборов ROI
    bool visible = {false};

    // Глобальные области ROI
    data::graph::Roi::List globalRois;

#ifdef INFERENCE_IS_PRESENT
    // Глобальные области ROI, используются для быстрой обработки
    infer::DetectRect::List globalRoisFast;
#endif

    struct ModelClass
    {
        // Порядковый индекс расположения класса в выходном слое модели
        qint32 index = {0};

        // Если параметр равен TRUE, то проверка принадлежности рамки
        // детектирования к глобальным ROIs выполняться не будет
        bool skipGlobalRois = {false};

        // Области ROI для класса
        data::graph::Roi::List rois;

#ifdef INFERENCE_IS_PRESENT
        // Области ROI, используются для быстрой обработки
        infer::DetectRect::List roisFast;
#endif
        typedef lst::List<ModelClass, lst::CompareItemDummy> List;

        DECLARE_B_SERIALIZE_FUNC

        J_SERIALIZE_BEGIN
            J_SERIALIZE_MAP_ITEM( "index"            , index          )
            J_SERIALIZE_MAP_ITEM( "skip_global_rois" , skipGlobalRois )
            J_SERIALIZE_MAP_ITEM( "rois"             , rois           )
        J_SERIALIZE_END
    };

    ModelClass::List classes;

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "visible"     , visible    )
        J_SERIALIZE_MAP_ITEM( "global_rois" , globalRois )
        J_SERIALIZE_MAP_ITEM( "classes"     , classes    )
    J_SERIALIZE_END
};

struct RoisInfoA : RoisInfo, Data<&command::RoisInfo,
                                   Message::Type::Answer>
{
};

/*** Алгорит работы FP/FN ***
  По FN:
  1. Сохраняем картинку при снижении TTL на заданную долю. Например, на 20%.
  2. Далее ждем заданный интервал времени (с привязкой к классу, а не треку).
     По истечении снова переходим к п.1.
  3. Вынести в настройки какие классы учитывать и задавать время для каждого
     класса.
  4. Сделать задание ROI, в котором работает алгоритм (один прямоугольник)
  5. Сохранять 2 картинки с идентичными именами - размеченную детектором
     (не трекером ???) и без разметки.

  По FP:
  1. Накапливаем картинки пока объект формируется.
  2. Если он не сформировался, сохраняем одну среднюю (по времени насыщения)
     картинку.
  3. Далее ждем заданный интервал времени (с привязкой к классу, а не треку).
     По истечении снова переходим к п.1.
  4. Вынести в настройки какие классы учитывать и задавать время для каждого
     класса.
  5. Сделать задание ROI, в котором работает алгоритм (один прямоугольник)
  6. Сохранять 2 картинки с идентичными именами - размеченную детектором
     (не трекером ???) и без разметки.
*/
struct FpFnInfo
{
    // Признак активности механизма FP/FN
    bool active = {false};

    QString path;
    QString prefix;

    // Признак отображения всех ROI для механизма FP/FN
    bool visibleRois = {false};

    // Кисть для отрисовки рамки трекирования в файле с разметкой
    data::graph::Brush markupBrush;

    struct ModelClass
    {
        // Порядковый индекс расположения класса в выходном слое модели
        qint32 index = {0};

        // Включение/отключение режима FP/FN для класса
        bool active = {true};

        // Область ROI для класса
        data::graph::Roi roi;

        // Время следующей итерации сохранения FP-изображения (в секундах)
        qint32 fpTiming = {30};

        // Время следующей итерации сохранения FN-изображения (в секундах)
        qint32 fnTiming = {30};

        // Уровень снижения текущего TTL для FN (в процентах)
        qint32 fnTtlDown = {20};

        typedef lst::List<ModelClass, lst::CompareItemDummy> List;

        DECLARE_B_SERIALIZE_FUNC

        J_SERIALIZE_BEGIN
            J_SERIALIZE_MAP_ITEM( "index"       , index     )
            J_SERIALIZE_MAP_ITEM( "active"      , active    )
            J_SERIALIZE_MAP_ITEM( "roi"         , roi       )
            J_SERIALIZE_MAP_ITEM( "fp_timing"   , fpTiming  )
            J_SERIALIZE_MAP_ITEM( "fn_timing"   , fnTiming  )
            J_SERIALIZE_MAP_ITEM( "fn_ttl_down" , fnTtlDown )
        J_SERIALIZE_END
    };

    ModelClass::List classes;

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "active"       , active      )
        J_SERIALIZE_MAP_ITEM( "path"         , path        )
        J_SERIALIZE_MAP_ITEM( "prefix"       , prefix      )
        J_SERIALIZE_MAP_ITEM( "visible_rois" , visibleRois )
        J_SERIALIZE_MAP_ITEM( "markup_brush" , markupBrush )
        J_SERIALIZE_MAP_ITEM( "classes"      , classes     )
    J_SERIALIZE_END
};

struct FpFnInfoA : FpFnInfo, Data<&command::FpFnInfo,
                                   Message::Type::Answer>
{
};

} // namespace data
} // namespace pproto
