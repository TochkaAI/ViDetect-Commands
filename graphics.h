#pragma once

#include "shared/container_ptr.h"
#include "pproto/commands/base.h"
#include "pproto/commands/identity.h"

#ifdef OPENCV_IS_PRESENT
#include <opencv2/opencv.hpp>
#endif

namespace pproto {

//---------------- Структуры данных используемые в сообщениях ----------------

namespace data {

/**
  Фиктивные структуры, требуется для корректного поиска потоковых операторов
  используемых для сериализации данных
*/
struct VDPoint {};

namespace graph {

using namespace std;

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

struct Point : data::VDPoint
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
    IdentityS identity;

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
    //   {id}            - идентификатор трекируемого объекта;
    //   {global_id}     - глобальный идентификатор трекируемого объекта;
    //   {class}         - наименование класса трекируемого/детектируемого объекта;
    //   {score}         - скор трекируемого/детектируемого объекта;
    //   {history}       - количество рамок в истории трекирования;
    //   {classif_name}  - наименование/индекс классификатора;
    //   {classif_score} - скор классификатора
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
    IdentityS identity;

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
    IdentityS identity;

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
    IdentityS identity;

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
    IdentityS identity;

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


/**
  Набор графических примитиров
*/
struct Set
{
    typedef container_ptr<Set> Ptr;

    // Идентификационный признак
    IdentityS identity;

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

} // namespace data
} // namespace pproto
