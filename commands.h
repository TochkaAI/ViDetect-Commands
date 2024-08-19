#pragma once

#include "shared/container_ptr.h"
#include "pproto/commands/base.h"

namespace pproto {
namespace command {

//----------------------------- Список команд --------------------------------

/**
  Команда детектирования сущностей/объектов на изображениях
*/
extern const QUuidEx ImageDetect;

} // namespace command

//---------------- Структуры данных используемые в сообщениях ----------------

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

    float score  = {0}; // Оценка детектированной области

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "x_min" , x_min )
        J_SERIALIZE_MAP_ITEM( "y_min" , y_min )
        J_SERIALIZE_MAP_ITEM( "x_max" , x_max )
        J_SERIALIZE_MAP_ITEM( "y_max" , y_max )
        J_SERIALIZE_MAP_ITEM( "score" , score )
    J_SERIALIZE_END
};

struct DetectClass
{
    // Наименование класса детектирования
    QString name;

    // Список детектированных рамок (объектов)
    QList<DetectRect> rects;

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "name"       , name  )
        J_SERIALIZE_MAP_ITEM( "rectangles" , rects )
    J_SERIALIZE_END
};

struct ImageItem
{
    // Наименование изображения. Вспомогательное поле, используется для
    // конкретизации сообщений об ошибках
    QString name;

    // Анализируемое изображение в формате JPEG. Для сообщения в формате json
    // изображение должно быть в представлено в BASE64
    QByteArray data;

    // Информационное поле общего назначения, может содержать любые текстовые
    // данные
    QString tag;

    // Результаты детектирования
    QList<DetectClass> classes;

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "name"    , name    )
        J_SERIALIZE_MAP_ITEM( "data"    , data    )
        J_SERIALIZE_MAP_ITEM( "tag"     , tag     )
        J_SERIALIZE_MAP_ITEM( "classes" , classes )
    J_SERIALIZE_END
};

struct ImageDetect : Data<&command::ImageDetect,
                           Message::Type::Command,
                           Message::Type::Answer>
{
    // Список изображений и результатов их обработки
    QList<ImageItem> images;

    // Определяет будут ли возвращаться в ответном сообщении анализируемые
    // изображения. По умолчанию изображения не возвращаются
    bool returnImages = {false};

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "images"        , images       )
        J_SERIALIZE_MAP_ITEM( "return_images" , returnImages )
    J_SERIALIZE_END
};

} // namespace data
} // namespace pproto
