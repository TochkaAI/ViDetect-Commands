#pragma once

#include "shared/container_ptr.h"
#include "pproto/commands/base.h"

#include "roi.h"

#ifdef DETECTION_IS_PRESENT
#include "detection/detect_rect.h"
#endif

namespace pproto {
namespace command {

//----------------------------- Список команд --------------------------------

/**
  Возвращает информацию о ROIs
  TODO Не реализовано.
*/
extern const QUuidEx RoisInfo;

} // namespace command

//---------------- Структуры данных используемые в сообщениях ----------------

namespace data {

struct RoisInfo
{
    // Признак отображения всех наборов ROI
    bool visible = {false};

    // Глобальные области ROI
    Roi::List globalRois;

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
        Roi::List rois;

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

} // namespace data
} // namespace pproto
