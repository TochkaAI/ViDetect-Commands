#pragma once

#include "shared/container_ptr.h"
#include "pproto/commands/base.h"
#include "pproto/commands/time_spec.h"

#include "coordinates.h"
#include "graphics.h"

namespace pproto {
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

    // Скор прямоугольника. Если прямоугольнику соответствуют несколько классов
    // рельной модели, то значение берется из класса с максимальным скором
    float score = {0};

    // Индекс класса реальной модели с максимальным скором, для которого опре-
    // делены координаты прямоугольника
    qint32 realClassIndex = {-1};

    // Список индексов и скоров классов соответствующих координатам прямоугольника
    QMap<qint32 /*real-class index*/, float /*real-class score*/> classes;

    // Время получения кадра (UTC)
    TimeSpec time;

    // Номер кадра
    quint64 number = quint64(-1);

    struct Segment
    {
        typedef container_ptr<Segment> Ptr;
        QVector<graph::Point> contour;

        DECLARE_B_SERIALIZE_FUNC
        DECLARE_J_SERIALIZE_FUNC
    };

    // Координаты контура сегментации
    Segment::Ptr segment;

    // Координаты камеры
    Coordinates::Ptr coordinates;

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
        J_SERIALIZE_MAP_ITEM( "x_min"            , x_min          )
        J_SERIALIZE_MAP_ITEM( "y_min"            , y_min          )
        J_SERIALIZE_MAP_ITEM( "x_max"            , x_max          )
        J_SERIALIZE_MAP_ITEM( "y_max"            , y_max          )
        J_SERIALIZE_MAP_ITEM( "score"            , score          )
        J_SERIALIZE_MAP_ITEM( "real_class_index" , realClassIndex )
        J_SERIALIZE_MAP_ITEM( "classes"          , classes        )
        J_SERIALIZE_MAP_ITEM( "time"             , time           )
        J_SERIALIZE_MAP_ITEM( "number"           , number         )
        J_SERIALIZE_MAP_ITEM( "segment"          , segment        )
        J_SERIALIZE_MAP_ITEM( "coordinates"      , coordinates    )
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

} // namespace data
} // namespace pproto
