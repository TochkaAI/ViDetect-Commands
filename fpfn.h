#pragma once

#include "shared/container_ptr.h"
#include "pproto/commands/base.h"

#include "graphics.h"
#include "roi.h"

namespace pproto {
namespace command {

//----------------------------- Список команд --------------------------------

/**
  Возвращает информацию настройках FP/FN
  TODO Не реализовано.
*/
extern const QUuidEx FpFnInfo;

} // namespace command

//---------------- Структуры данных используемые в сообщениях ----------------

namespace data {

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
    bool markupSave = {false};

    // Кисть для отрисовки рамки трекирования в файле с разметкой
    graph::Brush markupBrush;

    // Флаги сохранения оригинального и модифицированного изображений
    struct FrameSave
    {
        bool origin = {true};
        bool data = {false};

        DECLARE_B_SERIALIZE_FUNC

        J_SERIALIZE_BEGIN
            J_SERIALIZE_MAP_ITEM( "origin" , origin )
            J_SERIALIZE_MAP_ITEM( "data"   , data   )
        J_SERIALIZE_END
    };
    FrameSave frameSave;

    struct ModelClass
    {
        // Порядковый индекс расположения класса в выходном слое модели
        qint32 index = {0};

        // Включение/отключение режима FP/FN для класса
        bool active = {true};

        // Область ROI для класса
        Roi roi;

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
        J_SERIALIZE_MAP_ITEM( "frame_save"   , frameSave   )
        J_SERIALIZE_MAP_ITEM( "classes"      , classes     )
    J_SERIALIZE_END
};

struct FpFnInfoA : FpFnInfo, Data<&command::FpFnInfo,
                                   Message::Type::Answer>
{
};

} // namespace data
} // namespace pproto
