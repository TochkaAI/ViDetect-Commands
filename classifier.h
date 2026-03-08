#pragma once

#include "shared/defmac.h"
#include "shared/container_ptr.h"
#include "pproto/commands/base.h"
#include "pproto/commands/time_spec.h"

#include "graphics.h"

namespace pproto {
namespace command {

//----------------------------- Список команд --------------------------------

/**
  Команда возвращает информацию об используемых моделях для классификации
*/
extern const QUuidEx ClassifModelsInfo;

} // namespace command

//---------------- Структуры данных используемые в сообщениях ----------------

namespace data {

struct ClassifModelClass
{
    // Порядковый индекс класса в выходном слое модели классификации
    qint32 index = {0};

    // Наименование выходного класса модели классификации
    QString name;

    // Пороговое значение для класса модели классификации
    float scoreThresh = {0.10};

    struct Find
    {
        // Поиск по index
        int operator() (const qint32* index, const ClassifModelClass* item2) const
            {return LIST_COMPARE_ITEM(*index, item2->index);}
    };

    typedef lst::List<ClassifModelClass, Find> List;

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "index"        , index       )
        J_SERIALIZE_MAP_ITEM( "name"         , name        )
        J_SERIALIZE_MAP_ITEM( "score_thresh" , scoreThresh )
    J_SERIALIZE_END
};

struct ClassifModel
{
    //typedef container_ptr<ClassifModel> Ptr;

    // Порядковый индекс модели классификации
    qint32 index = {0};

    // Наименование модели (информационный параметр)
    QString name;

    // Корневая директория для субдиректорий model и cache
    QString path;

    // Наименование файла модели
    QString file;

    // Количество каналов во входном изображении. Допускается значение '1' для
    // чернобелого изображения, и '3' - для цветного изображения
    qint8 imageChannels = {3};

    // Размер входного слоя модели
    qint32 inputSize = {640};

    // Второй размер входного слоя модели. Соответствует ширине (width)
    qint32 inputSize2 = {640};

    // Индекс GPU на котором будет работать модель для классификации.
    // Значение параметра по умолчанию равно 0
    qint32 gpuIndex = {0};

    // Список индексов классов основной модели (реальной модели или мета-модели)
    // для которых будет выполняться классификация
    QSet<qint32>  mainModelClasses;

    ClassifModelClass::List classes;

    typedef lst::List<ClassifModel, lst::CompareItemDummy> List;

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "index"              , index            )
        J_SERIALIZE_MAP_ITEM( "name"               , name             )
        J_SERIALIZE_MAP_ITEM( "path"               , path             )
        J_SERIALIZE_MAP_ITEM( "file"               , file             )
        J_SERIALIZE_MAP_ITEM( "image_channels"     , imageChannels    )
        J_SERIALIZE_MAP_ITEM( "input_size"         , inputSize        )
        J_SERIALIZE_MAP_ITEM( "input_size2"        , inputSize2       )
        J_SERIALIZE_MAP_ITEM( "gpu_index"          , gpuIndex         )
        J_SERIALIZE_MAP_ITEM( "main_model_classes" , mainModelClasses )
        J_SERIALIZE_MAP_ITEM( "classes"            , classes          )
    J_SERIALIZE_END
};

struct ClassifModelsInfoA : Data<&command::ClassifModelsInfo,
                                  Message::Type::Answer>
{
    // Признак активности механизма классификации
    bool active = {false};

    // Список моделей классификации
    ClassifModel::List items;

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "active", active )
        J_SERIALIZE_MAP_ITEM( "items",  items  )
    J_SERIALIZE_END
};

} // namespace data
} // namespace pproto
