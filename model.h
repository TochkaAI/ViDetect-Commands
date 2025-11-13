#pragma once

#include "shared/container_ptr.h"
#include "pproto/commands/base.h"
#include "pproto/commands/time_spec.h"

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
    float scoreThresh   = {0.70};
    float nmsThresh     = {0.50};
    float segmentThresh = {0.75};

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
        J_SERIALIZE_MAP_ITEM( "segment_thresh" , segmentThresh )
    J_SERIALIZE_END
};

struct ModelInfo
{
    typedef container_ptr<ModelInfo> Ptr;

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
    ModelClass::List classes;

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

struct ModelInfoA : Data<&command::ModelInfo,
                          Message::Type::Answer>
{
    ModelInfo::Ptr value;

    DECLARE_B_SERIALIZE_FUNC
    J_SERIALIZE_MAP_ONE( "value", value )
};

} // namespace data
} // namespace pproto
