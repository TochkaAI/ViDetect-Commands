#pragma once

#include "shared/container_ptr.h"
#include "pproto/commands/base.h"

#ifdef DETECTION_IS_PRESENT
#include "detection/detect_rect.h"
#endif

#ifdef OPENCV_IS_PRESENT
#include <opencv2/opencv.hpp>
#endif

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
  Команда возвращает информацию по используемой модели и классам
*/
extern const QUuidEx ModelInfo;

/**
  Команда для конфигурирования классов модели. Допускается изменять наименование
  класса модели и параметр skip
  TODO не реализовано
*/
extern const QUuidEx ModelConfig;

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

/**
  Возвращает информацию о наборе графических примитивов полученном из конфигу-
  рационного файла (videtect.graph)
*/
extern const QUuidEx GraphsetInfo;

/**
  Событие эмитируется при перечитывании набора графических примитивов из конфи-
  гурационного файла (videtect.graph)
*/
extern const QUuidEx GraphsetChanged;

/**
  Команда возвращает информацию о наборе графических примитивов созданных дина-
  мически при помощи команды GraphsetDynamicUpdate
*/
extern const QUuidEx GraphsetDynamicInfo;

/**
  Команда создает/обновляет динамический набор графических примитивов
*/
extern const QUuidEx GraphsetDynamicUpdate;

/**
  Команда удаляет элементы graph::Set из динамического набора графических
  примитивов
*/
extern const QUuidEx GraphsetDynamicRemove;

/**
  Событие эмитируется при обновлении динамического набора графических примитивов
*/
extern const QUuidEx GraphsetDynamicChanged;

/**
  Команда для конфигурирования видимости набора графических примитивов
  TODO Не реализовано.
*/
extern const QUuidEx GraphsetVisible;

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

/**
  Возвращает информацию о параметрах сейвера
*/
extern const QUuidEx SaverInfo;

/**
  Возвращает информацию о параметрах сейвера
*/
extern const QUuidEx SaverBufferInfo;

/**
  Команда устанавливающая размер буффера кадров динамического сейвера
  Размеченные и неразмеченные буфферы регулируются независимо.
  Кадры из буффера будут записаны в начало видео для просмотра
  предыстории события.
*/
extern const QUuidEx SetSaverBuffer;

/**
  Команда для запуска записи динамическим сейвером
*/
extern const QUuidEx SaverStart;

/**
  Команда для завершения записи динамическим сейвером
*/
extern const QUuidEx SaverStop;


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

enum class ModelType : quint32
{
    Unknown   = 0,
    YoloX     = 1,
    Yolov5    = 5,
    Yolov7    = 7,
    Yolov9    = 9,
    Emulation = 100,
};

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
  Фиктивные структуры, требуется для корректного поиска потоковых операторов
  используемых для сериализации данных
*/
struct Point {};
struct Identity {};

namespace graph {

using namespace std;

/**
  Структура используется для идентификации графического примитива. Идентификация
  может осуществляться по одному из трех полей:  id, index, name. За выбор поля
  для идентификации примитива отвечает пользователь API
*/
struct Identity : data::Identity
{
    Identity() = default;
    Identity(const Identity&) = default;

    Identity(const QUuidEx& id ) : id(id),       findField(FindField::Id)    {}
    Identity(const int index   ) : index(index), findField(FindField::Index) {}
    Identity(const QString name) : name(name),   findField(FindField::Name)  {}
    Identity(const char*   name) : name(name),   findField(FindField::Name)  {}

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
            if (item1->findField == FindField::Id) {
                return operator() (&item1->id, item2);
            }
            else if (item1->findField == FindField::Index) {
                return operator() (&item1->index, item2);
            }
            else if (item1->findField == FindField::Name) {
                return operator() (&item1->name, item2);
            }
            return -1;
        }
    };

    bool isNull() const;

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

struct Point : data::Point
{
    Point() = default;
    Point(qint32 x, qint32 y) : x(x), y(y) {}

    qint32 x = {0};
    qint32 y = {0};

#ifdef OPENCV_IS_PRESENT
    Point(const cv::Point&   p) : x(p.x), y(p.y) {}
    Point(const cv::Point2f& p) : Point((cv::Point)p) {}

    operator cv::Point   () const {return {x, y};}
    operator cv::Point2f () const {return cv::Point2f(x, y);}
#endif

    bool isNull() const;

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "x" , x )
        J_SERIALIZE_MAP_ITEM( "y" , y )
    J_SERIALIZE_END
};

struct Size
{
    qint32 width = {0};
    qint32 height = {0};

    bool isNull() const;

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "width"  , width  )
        J_SERIALIZE_MAP_ITEM( "height" , height )
    J_SERIALIZE_END
};

struct Offset
{
    qint32 dx = {0};
    qint32 dy = {0};

#ifdef OPENCV_IS_PRESENT
    operator cv::Point () const {return cv::Point(dx, dy);}
#endif

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

    // Текст надписи. Если надпись используется для идентификации рамки трекиро-
    // вания или детектирования допускается  использование  следующих  подстано-
    // вочных  параметров:
    //   {id}        - идентификатор трекируемого объекта
    //   {global_id} - глобальный идентификатор трекируемого объекта
    //   {class}     - наименование класса трекируемого/детектируемого объекта;
    //   {score}     - скор трекируемого/детектируемого объекта.
    //   {history}   - количество рамок в истории трекирования
    QString text;

    // Вспомогательный параметр,  используется для привязки надписи к одному из
    // углов прямоугольной рамки ROI или рамки детектирования. Параметр привязки
    // может принимать значения [1,2,3,4], где 1 соответствует  верхнему  левому
    // углу прямоугольника. Обход углов происходит по часовой  стрелке.  Функция
    // привязки имеет приоритет над параметром position. Механизм деактивируется
    // когда cornerBind меньше 1
    qint32 cornerBind = {-1};

#ifdef OPENCV_IS_PRESENT
    cv::Point position_(const cv::Point& offset = {}) const;
    void setPosition(const cv::Point&);

    cv::Point offset_() const;
    void setOffset(const cv::Point&);
#endif

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_OPT( "identity"    , identity   )
        J_SERIALIZE_MAP_OPT( "visible"     , visible    )
        J_SERIALIZE_MAP_OPT( "brush"       , brush      )
        J_SERIALIZE_MAP_OPT( "position"    , position   )
        J_SERIALIZE_MAP_OPT( "offset"      , offset     )
        J_SERIALIZE_MAP_OPT( "scale"       , scale      )
        J_SERIALIZE_MAP_OPT( "text"        , text       )
        J_SERIALIZE_MAP_OPT( "corner_bind" , cornerBind )
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
    cv::Point p1(const cv::Point& offset = {}) const;
    void setP1(const cv::Point&);

    // Точка с координатами point2
    cv::Point p2(const cv::Point& offset = {}) const;
    void setP2(const cv::Point&);
#endif

    bool isNull() const;

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_OPT( "identity" , identity )
        J_SERIALIZE_MAP_OPT( "visible"  , visible  )
        J_SERIALIZE_MAP_OPT( "brush"    , brush    )
        J_SERIALIZE_MAP_OPT( "point1"   , point1   )
        J_SERIALIZE_MAP_OPT( "point2"   , point2   )
    J_SERIALIZE_END

    typedef lst::List<Line, Identity::Find<Line>> List;
};

struct Polyline
{
    // Идентификационный признак
    Identity identity;

    // Признак отображения примитива в кадре
    bool visible = {true};

    // Кисть
    Brush brush;

    // Точки полилинии
    QList<Point> points;

    // Признак замкнутой полилинии
    bool closed = {true};

#ifdef OPENCV_IS_PRESENT
    vector<cv::Point> points_(const cv::Point& offset = {}) const;
#endif

    bool isNull() const;

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_OPT( "identity" , identity )
        J_SERIALIZE_MAP_OPT( "visible"  , visible  )
        J_SERIALIZE_MAP_OPT( "brush"    , brush    )
        J_SERIALIZE_MAP_OPT( "points"   , points   )
        J_SERIALIZE_MAP_OPT( "closed"   , closed   )
    J_SERIALIZE_END

    typedef lst::List<Polyline, Identity::Find<Polyline>> List;
};

struct Rectangle
{
    // Идентификационный признак
    Identity identity;

    // Признак отображения примитива в кадре
    bool visible = {true};

    // Кисть
    Brush brush;

    // Верхняя левая точка (синоним top/left)
    Point point1;

    // Правая нижняя точка (синоним bottom/right)
    Point point2;

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

    bool isNull() const;

    // Ширина/высота прямоугольника
    Size size() const;
    void setSize(const Size&);

    // Центр прямоугольника
    Point center() const;

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_OPT( "identity" , identity )
        J_SERIALIZE_MAP_OPT( "visible"  , visible  )
        J_SERIALIZE_MAP_OPT( "brush"    , brush    )
        J_SERIALIZE_MAP_OPT( "point1"   , point1   )
        J_SERIALIZE_MAP_OPT( "point2"   , point2   )
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

    // Признак заполнения круга сплошным цветом
    bool filled = {false};

#ifdef OPENCV_IS_PRESENT
    cv::Point center_(const cv::Point& offset = {}) const;
    void setCenter(const cv::Point&);
#endif

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_OPT( "identity" , identity )
        J_SERIALIZE_MAP_OPT( "visible"  , visible  )
        J_SERIALIZE_MAP_OPT( "brush"    , brush    )
        J_SERIALIZE_MAP_OPT( "center"   , center   )
        J_SERIALIZE_MAP_OPT( "radius"   , radius   )
        J_SERIALIZE_MAP_OPT( "filled"   , filled   )
    J_SERIALIZE_END

    typedef lst::List<Circle, Identity::Find<Circle>> List;
};

struct Roi
{
    // Идентификационный признак
    Identity identity;

    // Признак отображения примитива в кадре
    bool visible = {true};

    // Инвертирует принадлежность объекта детектирования к области ROI
    bool inverse = {false};

    // Надпись ROI с привязкой к одному из углов геометрической области,
    // по умолчанию к верхнему левому углу
    Caption caption;

    // Геометрическая область ROI
    Rectangle border;

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "identity" , identity )
        J_SERIALIZE_MAP_OPT ( "visible"  , visible  )
        J_SERIALIZE_MAP_OPT ( "inverse"  , inverse  )
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
    typedef container_ptr<Set> Ptr;

    // Идентификационный признак
    Identity identity;

    // Признак отображения всего набора графических примитивов
    bool visible = {true};

    // Вспомогательный параметр, определяет будет ли выводиться в лог информация
    // по графическому примитиву.  Используется  для  минимизации  лог-сообщений
    // при частом обновлении динамических наборов примитивов
    bool printLog = {false};

    // Граница области набора графических примитивов
    Rectangle border;

    // Набор текстовых примитивов
    Caption::List captions;

    // Набор линий
    Line::List lines;

    // Набор линий
    Polyline::List polylines;

    // Набор прямоугольников
    Rectangle::List rectangles;

    // Набор окружностей
    Circle::List circles;

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "identity"   , identity   )
        J_SERIALIZE_MAP_OPT ( "visible"    , visible    )
        J_SERIALIZE_MAP_OPT ( "print_log"  , printLog   )
        J_SERIALIZE_MAP_OPT ( "border"     , border     )
        J_SERIALIZE_MAP_OPT ( "captions"   , captions   )
        J_SERIALIZE_MAP_OPT ( "lines"      , lines      )
        J_SERIALIZE_MAP_OPT ( "polylines"  , polylines  )
        J_SERIALIZE_MAP_OPT ( "rectangles" , rectangles )
        J_SERIALIZE_MAP_OPT ( "circles"    , circles    )
    J_SERIALIZE_END

    typedef lst::List<Set, Identity::Find<Set>> List;
};

} // namespace graph

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

struct FrameCrop
{
    // Признак активности механизма frame crop
    bool active = {true};

    graph::Rectangle border;

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_OPT ( "active" , active )
        J_SERIALIZE_MAP_ITEM( "border" , border )
    J_SERIALIZE_END
};

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

    template<typename T> struct Find
    {
        // Поиск по index
        int operator() (const qint32* index, const T* item2) const
            {return LIST_COMPARE_ITEM(*index, item2->index);}
    };

    typedef lst::List<ModelClass, Find<ModelClass>> List;

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
    typedef container_ptr<ModelInfo> Ptr;

    // Корневая директория для субдиректорий models и cache. Если параметр path
    // не определен явно,  то  директории  расположения  модели  и  кеша  будут
    // следующие:
    //   /var/opt/tochkacv/models/videtect
    //   /var/opt/tochkacv/cache/videtect
    QString path;

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
    qint32 inputSize = {640};

    // Наименование входного слоя модели
    QString input;

    // Наименование выходного слоя модели
    QString output;

    // Количество записей в выходном слое модели. Для yolov9 количество столбцов
    // выходного слоя
    qint32 recordCount = {-1};

    // Определяет область кадра для отправки в нейросеть
    FrameCrop frameCrop;

    // Список классов модели
    ModelClass::List classes;

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "path"           , path          )
        J_SERIALIZE_MAP_ITEM( "file"           , file          )
        J_SERIALIZE_MAP_ITEM( "type"           , type          )
        J_SERIALIZE_MAP_ITEM( "type_str"       , typeStr       )
        J_SERIALIZE_MAP_ITEM( "image_channels" , imageChannels )
        J_SERIALIZE_MAP_ITEM( "size"           , inputSize     )
        J_SERIALIZE_MAP_ITEM( "input"          , input         )
        J_SERIALIZE_MAP_ITEM( "output"         , output        )
        J_SERIALIZE_MAP_ITEM( "record_count"   , recordCount   )
        J_SERIALIZE_MAP_ITEM( "frame_crop"     , frameCrop     )
        J_SERIALIZE_MAP_ITEM( "classes"        , classes       )
    J_SERIALIZE_END
};

struct ModelInfoA : Data<&command::ModelInfo,
                          Message::Type::Answer>
{
    ModelInfo::Ptr value;

    DECLARE_B_SERIALIZE_FUNC
    J_SERIALIZE_MAP_ONE( "value", value )
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
        J_SERIALIZE_MAP_ITEM( "x_min" , x_min )
        J_SERIALIZE_MAP_ITEM( "y_min" , y_min )
        J_SERIALIZE_MAP_ITEM( "x_max" , x_max )
        J_SERIALIZE_MAP_ITEM( "y_max" , y_max )
        J_SERIALIZE_MAP_ITEM( "score" , score )
    J_SERIALIZE_END
};

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
        // Запрос видео-фрейма при получении snapshot-а трекеров
        bool request = {false};

        // Передача видео-фрейма через shared memory
        // TODO зарезервировано для будущего использования
        bool sharedMem = {false};

        DECLARE_B_SERIALIZE_FUNC

        J_SERIALIZE_BEGIN
            J_SERIALIZE_MAP_OPT( "request"    , request   )
            J_SERIALIZE_MAP_OPT( "shared_mem" , sharedMem )
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

        SByteArray data; // Кадр

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

struct Graphset
{
    typedef container_ptr<Graphset> Ptr;

    // Признак отображения всех наборов графических примитивов
    bool visible = {true};

    // Список наборов графических примитивов
    graph::Set::List items;

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "visible" , visible )
        J_SERIALIZE_MAP_ITEM( "items"   , items   )
    J_SERIALIZE_END
};

struct GraphsetInfoA : Data<&command::GraphsetInfo,
                             Message::Type::Answer>
{
    Graphset::Ptr value;

    DECLARE_B_SERIALIZE_FUNC
    J_SERIALIZE_MAP_ONE( "value", value )
};

struct GraphsetDynamicInfoA : Data<&command::GraphsetDynamicInfo,
                                    Message::Type::Answer>
{
    graph::Set::List items;

    DECLARE_B_SERIALIZE_FUNC
    J_SERIALIZE_MAP_ONE( "items", items )
};

struct GraphsetDynamicUpdate : Data<&command::GraphsetDynamicUpdate,
                                     Message::Type::Command>
{
    graph::Set::List items;

    DECLARE_B_SERIALIZE_FUNC
    J_SERIALIZE_MAP_ONE( "items", items )
};

struct GraphsetDynamicRemove : Data<&command::GraphsetDynamicRemove,
                                     Message::Type::Command>
{
    QList<graph::Identity> items;

    DECLARE_B_SERIALIZE_FUNC
    J_SERIALIZE_MAP_ONE( "items", items )
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
    graph::Roi::List globalRois;

#ifdef DETECTION_IS_PRESENT
    // Глобальные области ROI, используются для быстрой обработки
    tochka::DetectRect::List globalRoisFast;
#endif

    struct ModelClass
    {
        // Порядковый индекс расположения класса в выходном слое модели
        qint32 index = {0};

        // Если параметр равен TRUE, то проверка принадлежности рамки
        // детектирования к глобальным ROIs выполняться не будет
        bool skipGlobalRois = {false};

        // Области ROI для класса
        graph::Roi::List rois;

#ifdef DETECTION_IS_PRESENT
        // Области ROI, используются для быстрой обработки
        tochka::DetectRect::List roisFast;
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

    // Формат сохранения файла. Допустимые значения: jpg, png
    QString fileFormat = {"jpg"};

    // Признак отображения всех ROI для механизма FP/FN
    bool visibleRois = {false};

    // Сохранять файл с отрисованной разметкой. Файл используется как подсказка
    // для разметчика
    bool markupSave = {true};

    // Кисть для отрисовки рамки трекирования в файле с разметкой
    graph::Brush markupBrush;

    struct ModelClass
    {
        // Порядковый индекс расположения класса в выходном слое модели
        qint32 index = {0};

        // Включение/отключение режима FP/FN для класса
        bool active = {true};

        // Область ROI для класса
        graph::Roi roi;

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
        J_SERIALIZE_MAP_ITEM( "file_format"  , fileFormat  )
        J_SERIALIZE_MAP_ITEM( "visible_rois" , visibleRois )
        J_SERIALIZE_MAP_ITEM( "markup_save"  , markupSave  )
        J_SERIALIZE_MAP_ITEM( "markup_brush" , markupBrush )
        J_SERIALIZE_MAP_ITEM( "classes"      , classes     )
    J_SERIALIZE_END
};

struct FpFnInfoA : FpFnInfo, Data<&command::FpFnInfo,
                                   Message::Type::Answer>
{
};

//struct SaverInfo : Data<&command::SaverInfo,
//                         Message::Type::Command>
//{
//    // Флаг обозначающий тип записи - с разметкой или без
//    QString fileName;

//    // Максимальное время записи видеофрагмента (сек.)
//    quint32 maxSaveTime = {0};

//    //TODO информация об ограничениях?
//    bool isMarkup = {false};


//    DECLARE_B_SERIALIZE_FUNC

//    J_SERIALIZE_BEGIN
//        J_SERIALIZE_MAP_ITEM( "fileName"    , fileName    )
//        J_SERIALIZE_MAP_ITEM( "maxSaveTime" , maxSaveTime )
//    J_SERIALIZE_END
//};

struct SaverInfoA : Data<&command::SaverInfo,
                          Message::Type::Answer>
{
    // Флаг обозначающий тип записи - с разметкой или без
    QString fileName;

    // Максимальное время записи видеофрагмента (сек.)
    quint32 maxSaveTime = {0};

    //TODO информация об ограничениях?
    bool isMarkup = {false};


    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "file_name"     , fileName    )
        J_SERIALIZE_MAP_ITEM( "max_save_time" , maxSaveTime )
        J_SERIALIZE_MAP_ITEM( "is_markup"     , isMarkup    )
    J_SERIALIZE_END
};

//struct SaverBufferInfo : Data<&command::SaverBufferInfo,
//                               Message::Type::Command>
//{
//    // Число кадров в буффере без разметки
//    // Максимальное значение ~ 500 кадров
//    quint32 bufferSizeClear = {0};

//    // Число кадров в буффере с разметкой
//    quint32 bufferSizeMarkup = {0};

//    DECLARE_B_SERIALIZE_FUNC

//    J_SERIALIZE_BEGIN
//        J_SERIALIZE_MAP_ITEM( "bufferSizeClear"  , bufferSizeClear  )
//        J_SERIALIZE_MAP_ITEM( "bufferSizeMarkup" , bufferSizeMarkup )
//    J_SERIALIZE_END
//};

struct SaverBufferInfoA : Data<&command::SaverBufferInfo,
                                Message::Type::Answer>
{
    // Число кадров в буффере без разметки
    // Максимальное значение ~ 500 кадров
    quint32 bufferSizeClear = {0};

    // Число кадров в буффере с разметкой
    quint32 bufferSizeMarkup = {0};

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "buffer_size_clear"  , bufferSizeClear  )
        J_SERIALIZE_MAP_ITEM( "buffer_size_markup" , bufferSizeMarkup )
    J_SERIALIZE_END
};

struct SetSaverBuffer : Data<&command::SetSaverBuffer,
                              Message::Type::Command>
{
    // Число кадров в буффере без разметки
    // Максимальное значение ~ 500 кадров
    quint32 bufferSizeClear = {0};

    // Число кадров в буффере с разметкой
    quint32 bufferSizeMarkup = {0};

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "buffer_size_clear"  , bufferSizeClear  )
        J_SERIALIZE_MAP_ITEM( "buffer_size_markup" , bufferSizeMarkup )
    J_SERIALIZE_END
};

struct SaverStart : Data<&command::SaverStart,
                          Message::Type::Command,
                          Message::Type::Answer>
{
    int saverIndex = {-1};

    // Флаг обозначающий тип записи - с разметкой или без
    bool isMarkup = {true};

    // Максимальное время записи видеофрагмента (сек.)
    quint32 maxSaveTime = {60};

    // Флаг обозначающий необходимость завершить запись
    // без дополнительной команды
    bool autoStop = {false};

    // Название файла записи
    // Если поле пусто, то будет использоваться стандартное именование
    QString fileName;

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "saver_index"   , saverIndex  )
        J_SERIALIZE_MAP_ITEM( "is_markup"     , isMarkup    )
        J_SERIALIZE_MAP_ITEM( "max_save_time" , maxSaveTime )
        J_SERIALIZE_MAP_ITEM( "auto_stop"     , autoStop    )
        J_SERIALIZE_MAP_ITEM( "file_name"     , fileName    )
    J_SERIALIZE_END
};

//struct SaverStop : Data<&command::SaverStop,
//                         Message::Type::Command>
//{
//    // Название видеозаписи которая будет остановлена.
//    // Это поле для информирования клиентской стороны
//    QString fileName;

//    DECLARE_B_SERIALIZE_FUNC

//    J_SERIALIZE_BEGIN
//        J_SERIALIZE_MAP_ITEM( "file_name" , fileName )
//    J_SERIALIZE_END
//};

struct SaverStop : Data<&command::SaverStop,
                         Message::Type::Command,
                         Message::Type::Answer>
{
    int saverIndex = {-1};

    // Название видеозаписи которая будет остановлена.
    // Это поле для информирования клиентской стороны
    QString fileName;

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "saver_index" , saverIndex )
        J_SERIALIZE_MAP_ITEM( "file_name"   , fileName   )
    J_SERIALIZE_END
};

} // namespace data
} // namespace pproto
