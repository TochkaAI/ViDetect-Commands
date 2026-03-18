#pragma once

#include "shared/defmac.h"
#include "shared/container_ptr.h"
#include "pproto/commands/base.h"
#include "pproto/commands/time_spec.h"

#include "compare.h"
#include "graphics.h"

namespace pproto {
namespace command {

//----------------------------- Список команд --------------------------------

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

} // namespace command

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
  Признак используемой модели
*/
enum class UsingModel : quint32
{
    RealModel = 0, // Реальная модель
    MetaModel = 1, // Мета-модель
};

//---------------- Структуры данных используемые в сообщениях ----------------

namespace data {

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

struct ModelSegment
{
    // Признак наличия матрицы сегментации в составе нейросети
    bool active = {false};

    // Размерность выходного слоя сегментации. Сейчас поддерживается только
    // одна размерность [160, 160, 32]
    QVector<qint32> outputSize {160, 160, 32};

    // Наименование выходного слоя с кубом коэффициентов сегментации
    QString output;

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_OPT ( "active"      , active     )
        J_SERIALIZE_MAP_ITEM( "output_size" , outputSize )
        J_SERIALIZE_MAP_ITEM( "output"      , output     )
    J_SERIALIZE_END
};

struct ModelClassBase
{
    // Порядковый индекс класса модели
    qint32 index = {-1};

    // Наименование класса модели
    QString name;

    // Не обрабатывать выход нейросети для данного класса
    bool skip = {false};

    typedef lst::List<ModelClassBase, ModelClassFind<ModelClassBase>> List;

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BASE_BEGIN
        J_SERIALIZE_MAP_ITEM( "index" , index )
        J_SERIALIZE_MAP_ITEM( "name"  , name  )
        J_SERIALIZE_MAP_ITEM( "skip"  , skip  )
    J_SERIALIZE_BASE_END
};

struct RealModelClass : ModelClassBase
{
    // Индивидуальные пороговые значения для класса модели
    float cuttingThresh = {0.10};
    float scoreThresh   = {0.70};
    float nmsThresh     = {0.50};
    float segmentThresh = {0.75};

    typedef lst::List<RealModelClass, ModelClassFind<ModelClassBase>> List;

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_BASE( ModelClassBase )
        J_SERIALIZE_MAP_ITEM( "cutting_thresh" , cuttingThresh )
        J_SERIALIZE_MAP_ITEM( "score_thresh"   , scoreThresh   )
        J_SERIALIZE_MAP_ITEM( "nms_thresh"     , nmsThresh     )
        J_SERIALIZE_MAP_ITEM( "segment_thresh" , segmentThresh )
    J_SERIALIZE_END
};

struct MetaModelClass : ModelClassBase
{
    // Список индексов реальных классов модели из которых собирается метакласс
    QSet<int> realModelClasses;

    // Пороговое значение IoU для агрегирования детектированных рамок реальных
    // классов в рамку метакласса
    float iouThresh = {0.70};

    typedef lst::List<MetaModelClass, ModelClassFind<ModelClassBase>> List;

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_BASE( ModelClassBase )
        J_SERIALIZE_MAP_ITEM( "real_model_classes" , realModelClasses )
        J_SERIALIZE_MAP_ITEM( "iou_thresh"         , iouThresh        )
    J_SERIALIZE_END
};

struct RealModel
{
    typedef container_ptr<RealModel> Ptr;

    // Корневая директория для субдиректорий model и cache. Если параметр path
    // не определен явно, то  директории  расположения  модели  и  кеша  будут
    // следующие:
    //   /var/opt/tochkacv/model/videtect
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

    // Второй размер входного слоя модели. Соответствует ширине (width)
    qint32 inputSize2 = {640};

    // Наименование входного слоя модели
    QString input;

    // Наименование выходного слоя модели
    QString output;

    // Количество записей в выходном слое модели. Для yolov9 количество столбцов
    // выходного слоя
    qint32 recordCount = {-1};

    // Механизм поддержки сегментации в нейросети. Сейчас эту возможность имеют
    // только сети yolov9
    ModelSegment segmentation;

    // Определяет область кадра для отправки в нейросеть
    FrameCrop frameCrop;

    // Список классов модели
    RealModelClass::List classes;

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "path"           , path          )
        J_SERIALIZE_MAP_ITEM( "file"           , file          )
        J_SERIALIZE_MAP_ITEM( "type"           , type          )
        J_SERIALIZE_MAP_ITEM( "type_str"       , typeStr       )
        J_SERIALIZE_MAP_ITEM( "image_channels" , imageChannels )
        J_SERIALIZE_MAP_ITEM( "input_size"     , inputSize     )
        J_SERIALIZE_MAP_ITEM( "input_size2"    , inputSize2    )
        J_SERIALIZE_MAP_ITEM( "input"          , input         )
        J_SERIALIZE_MAP_ITEM( "output"         , output        )
        J_SERIALIZE_MAP_ITEM( "record_count"   , recordCount   )
        J_SERIALIZE_MAP_ITEM( "segmentation"   , segmentation  )
        J_SERIALIZE_MAP_ITEM( "frame_crop"     , frameCrop     )
        J_SERIALIZE_MAP_ITEM( "classes"        , classes       )
    J_SERIALIZE_END
};

struct MetaModel
{
    typedef container_ptr<MetaModel> Ptr;

    // Список метаклассов
    MetaModelClass::List classes;

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "classes" , classes )
    J_SERIALIZE_END
};

struct ModelInfoA : Data<&command::ModelInfo,
                          Message::Type::Answer>
{
    UsingModel usingModel = {UsingModel::RealModel};

    RealModel::Ptr realModel;
    MetaModel::Ptr metaModel;

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "using_model" , usingModel )
        J_SERIALIZE_MAP_ITEM( "real_model"  , realModel  )
        J_SERIALIZE_MAP_ITEM( "meta_model"  , metaModel  )
    J_SERIALIZE_END
};

} // namespace data

// Вспомогательная функция, используется для получения списка классов модели
// в виде списка data::ModelClassBase::List
template<typename ModelT>
data::ModelClassBase::List modelClassesBase(const ModelT& model)
{
    data::ModelClassBase::List list {lst::Container::No};
    for (auto* modelCls : model.classes)
        list.add(modelCls);
    return list;
}

} // namespace pproto
