#pragma once

#include "shared/container_ptr.h"
#include "pproto/commands/base.h"
#include "pproto/commands/time_spec.h"

#include "compare.h"
#include "graphics.h"

#include <tuple>

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

struct PaintClassif
{
    qint32 modelIndex = {-1};
    qint32 classIndex = {-1};

    PaintRect rect;

    struct Compare
    {
        int operator() (const PaintClassif* item1, const PaintClassif* item2) const
        {
            LIST_COMPARE_MULTI_ITEM(item1->modelIndex, item2->modelIndex)
            LIST_COMPARE_MULTI_ITEM(item1->classIndex, item2->classIndex);
            return 0;
        }
        int operator() (const std::tuple<qint32 /*model index*/, qint32 /*class index*/>* item1,
                        const PaintClassif* item2) const
        {
            LIST_COMPARE_MULTI_ITEM(std::get<0>(*item1), item2->modelIndex)
            LIST_COMPARE_MULTI_ITEM(std::get<1>(*item1), item2->classIndex)
            return 0;
        }
    };

    typedef lst::List<PaintClassif, Compare> List;

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "model_index" , modelIndex )
        J_SERIALIZE_MAP_OPT ( "class_index" , classIndex )
        J_SERIALIZE_MAP_OPT ( "rect"        , rect       )
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

        // Параметры отображения рамок трекируемого объекта или логического
        // объекта
        PaintRect rect;

        PaintClassif::List classifiers;

        typedef lst::List<ModelClass, ModelClassFind<ModelClass>> List;

        DECLARE_B_SERIALIZE_FUNC

        J_SERIALIZE_BEGIN
            J_SERIALIZE_MAP_ITEM( "index"       , index       )
            J_SERIALIZE_MAP_ITEM( "rect"        , rect        )
            J_SERIALIZE_MAP_ITEM( "classifiers" , classifiers )
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
