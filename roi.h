#pragma once

#include "shared/container_ptr.h"
#include "pproto/commands/base.h"

#include "graphics.h"

#ifdef DETECTION_IS_PRESENT
#include "detection/detect_rect.h"
#endif

namespace pproto {
namespace command {}

//---------------- Структуры данных используемые в сообщениях ----------------

namespace data {

struct Roi
{
    // Идентификационный признак
    graph::Identity identity;

    // Признак отображения примитива в кадре
    bool visible = {true};

    // Инвертирует принадлежность объекта детектирования к области ROI
    bool inverse = {false};

    // Надпись ROI с привязкой к одному из углов геометрической области,
    // по умолчанию к верхнему левому углу
    graph::Caption caption;

    // Геометрическая область ROI
    graph::Rectangle border;

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "identity" , identity )
        J_SERIALIZE_MAP_OPT ( "visible"  , visible  )
        J_SERIALIZE_MAP_OPT ( "inverse"  , inverse  )
        J_SERIALIZE_MAP_OPT ( "caption"  , caption  )
        J_SERIALIZE_MAP_ITEM( "border"   , border   )
    J_SERIALIZE_END

    typedef lst::List<Roi, graph::Identity::Find<Roi>> List;
};

} // namespace data
} // namespace pproto
