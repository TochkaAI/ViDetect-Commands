#pragma once

#include "shared/container_ptr.h"
#include "pproto/commands/base.h"
#include "qutils/commands/identity.h"

#include "graphics.h"

namespace pproto {
namespace command {

//----------------------------- Список команд --------------------------------


/**
  Возвращает информацию о наборе графических примитивов полученном из конфигу-
  рационного файла (videtect.graph)
*/
extern const QUuidEx GraphsetInfo;

/**
  Событие эмитируется при перечитывании набора графических примитивов из конфи-
  гурационного файла (videtect.graph)
*/
extern const QUuidEx GraphsetChanged;

/**
  Команда возвращает информацию о наборе графических примитивов созданных дина-
  мически при помощи команды GraphsetDynamicUpdate
*/
extern const QUuidEx GraphsetDynamicInfo;

/**
  Команда создает/обновляет динамический набор графических примитивов
*/
extern const QUuidEx GraphsetDynamicUpdate;

/**
  Команда удаляет элементы graph::Set из динамического набора графических
  примитивов
*/
extern const QUuidEx GraphsetDynamicRemove;

/**
  Событие эмитируется при обновлении динамического набора графических примитивов
*/
extern const QUuidEx GraphsetDynamicChanged;

/**
  Команда для конфигурирования видимости набора графических примитивов
  TODO Не реализовано.
*/
extern const QUuidEx GraphsetVisible;

} // namespace command

//---------------- Структуры данных используемые в сообщениях ----------------

namespace data {

struct Graphset
{
    typedef container_ptr<Graphset> Ptr;

    // Признак отображения всех наборов графических примитивов
    bool visible = {true};

    // Список наборов графических примитивов
    graph::Set::List items;

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "visible" , visible )
        J_SERIALIZE_MAP_ITEM( "items"   , items   )
    J_SERIALIZE_END
};

struct GraphsetInfoA : Data<&command::GraphsetInfo,
                             Message::Type::Answer>
{
    Graphset::Ptr value;

    DECLARE_B_SERIALIZE_FUNC
    J_SERIALIZE_MAP_ONE( "value", value )
};

struct GraphsetDynamicInfoA : Data<&command::GraphsetDynamicInfo,
                                    Message::Type::Answer>
{
    graph::Set::List items;

    DECLARE_B_SERIALIZE_FUNC
    J_SERIALIZE_MAP_ONE( "items", items )
};

struct GraphsetDynamicUpdate : Data<&command::GraphsetDynamicUpdate,
                                     Message::Type::Command>
{
    graph::Set::List items;

    DECLARE_B_SERIALIZE_FUNC
    J_SERIALIZE_MAP_ONE( "items", items )
};

struct GraphsetDynamicRemove : Data<&command::GraphsetDynamicRemove,
                                     Message::Type::Command>
{
    QList<Identity> items;

    DECLARE_B_SERIALIZE_FUNC
    J_SERIALIZE_MAP_ONE( "items", items )
};

struct GraphsetVisible : Data<&command::GraphsetVisible,
                               Message::Type::Command>
{
    // Признак отображения всех наборов графических примитивов
    bool visible = {true};

    struct Set
    {
        struct Item
        {
            Identity identity;     // Идентификационный признак
            bool visible = {true}; // Признак отображения примитива в кадре

            DECLARE_B_SERIALIZE_FUNC

            J_SERIALIZE_BEGIN
                J_SERIALIZE_MAP_ITEM( "identity" , identity )
                J_SERIALIZE_MAP_ITEM( "visible"  , visible  )
            J_SERIALIZE_END
        };

        QList<Item> texts;      // Набор текстовых примитивов
        QList<Item> lines;      // Набор линий
        QList<Item> rectangles; // Набор прямоугольников
        QList<Item> circles;    // Набор окружностей

        DECLARE_B_SERIALIZE_FUNC

        J_SERIALIZE_BEGIN
            J_SERIALIZE_MAP_ITEM( "texts"      , texts      )
            J_SERIALIZE_MAP_ITEM( "lines"      , lines      )
            J_SERIALIZE_MAP_ITEM( "rectangles" , rectangles )
            J_SERIALIZE_MAP_ITEM( "circles"    , circles    )
        J_SERIALIZE_END
    };

    QList<Set> items;

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "visible" , visible )
        J_SERIALIZE_MAP_ITEM( "items"   , items   )
    J_SERIALIZE_END
};

} // namespace data
} // namespace pproto
