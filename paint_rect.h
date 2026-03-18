#pragma once

#include "shared/container_ptr.h"
#include "pproto/commands/base.h"
#include "pproto/commands/time_spec.h"

#include "compare.h"
#include "graphics.h"

namespace pproto {
namespace data {

struct PaintRect
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

    typedef lst::List<PaintRect, CompareId<PaintRect>> List;

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

struct PaintRects
{
    typedef container_ptr<PaintRects> Ptr;

    // Признак отображения всех рамок трекирования
    bool visible = {false};

    struct ModelClass
    {
        // Порядковый индекс расположения класса в выходном слое модели
        qint32 index = {0};

        // Параметры отображения рамок трекирования для класса модели
        PaintRect rect;

        typedef lst::List<ModelClass, ModelClassFind<ModelClass>> List;

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

} // namespace data
} // namespace pproto
