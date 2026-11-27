#pragma once

#include "shared/container_ptr.h"
#include "pproto/commands/base.h"

namespace pproto {
namespace command {

//----------------------------- Список команд --------------------------------

/**
  Возвращает информацию о параметрах сейвера
*/
DECLARE_PPROTO_COMMAND(SaverInfo)

/**
  Возвращает информацию о параметрах сейвера
*/
DECLARE_PPROTO_COMMAND(SaverBufferInfo)

/**
  Команда устанавливающая размер буффера кадров динамического сейвера
  Размеченные и неразмеченные буфферы регулируются независимо.
  Кадры из буффера будут записаны в начало видео для просмотра
  предыстории события.
*/
DECLARE_PPROTO_COMMAND(SetSaverBuffer)

/**
  Команда для запуска записи динамическим сейвером
*/
DECLARE_PPROTO_COMMAND(SaverStart)

/**
  Команда для завершения записи динамическим сейвером
*/
DECLARE_PPROTO_COMMAND(SaverStop)

} // namespace command

//---------------- Структуры данных используемые в сообщениях ----------------

namespace data {

struct SaverInfo : Data<&command::SaverInfo,
                         Message::Type::Command>
{
    bool origin = {true};
    bool markup = {false};

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "origin" , origin )
        J_SERIALIZE_MAP_ITEM( "markup" , markup )
    J_SERIALIZE_END
};

struct SaverInfoA : Data<&command::SaverInfo,
                          Message::Type::Answer>
{
    bool origin = {true};
    bool markup = {false};

    // Флаг обозначающий тип записи - с разметкой или без
    QString fileName;

    // Максимальное время записи видеофрагмента (сек.)
    quint32 maxSaveTime = {0};

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "origin"        , origin      )
        J_SERIALIZE_MAP_ITEM( "markup"        , markup      )
        J_SERIALIZE_MAP_ITEM( "file_name"     , fileName    )
        J_SERIALIZE_MAP_ITEM( "max_save_time" , maxSaveTime )
    J_SERIALIZE_END
};

struct SaverBufferInfo : Data<&command::SaverBufferInfo,
                               Message::Type::Command>
{
    bool origin = {true};
    bool markup = {false};

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "origin" , origin )
        J_SERIALIZE_MAP_ITEM( "markup" , markup )
    J_SERIALIZE_END
};

struct SaverBufferInfoA : Data<&command::SaverBufferInfo,
                                Message::Type::Answer>
{
    bool origin = {true};
    bool markup = {false};

    // Число кадров в буффере. Максимальное значение ~500 кадров
    quint32 bufferSize = {0};

    // Число кадров в буффере без разметки
    // Максимальное значение ~ 500 кадров
    //quint32 bufferSizeClear = {0};

    // Число кадров в буффере с разметкой
    //quint32 bufferSizeMarkup = {0};

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "origin"      , origin     )
        J_SERIALIZE_MAP_ITEM( "markup"      , markup     )
        J_SERIALIZE_MAP_ITEM( "buffer_size" , bufferSize )
    J_SERIALIZE_END
};

struct SetSaverBuffer : Data<&command::SetSaverBuffer,
                              Message::Type::Command>
{
    bool origin = {true};
    bool markup = {false};

    // Число кадров в буффере. Максимальное значение 500 кадров
    quint32 bufferSize = {0};

    // // Число кадров в буффере без разметки
    // // Максимальное значение ~ 500 кадров
    // quint32 bufferSizeClear = {0};

    // // Число кадров в буффере с разметкой
    // quint32 bufferSizeMarkup = {0};

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "origin"      , origin     )
        J_SERIALIZE_MAP_ITEM( "markup"      , markup     )
        J_SERIALIZE_MAP_ITEM( "buffer_size" , bufferSize )
    J_SERIALIZE_END
};

struct SaverStart : Data<&command::SaverStart,
                          Message::Type::Command,
                          Message::Type::Answer>
{
    bool origin = {true};
    bool markup = {false};

    // Максимальное время записи видеофрагмента (сек.)
    quint32 maxSaveTime = {60};

    // Флаг обозначающий необходимость завершить запись
    // без дополнительной команды
    bool autoStop = {false};

    // Название файла записи
    // Если поле пусто, то будет использоваться стандартное именование
    QString fileName;

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "origin"        , origin      )
        J_SERIALIZE_MAP_ITEM( "markup"        , markup      )
        J_SERIALIZE_MAP_ITEM( "max_save_time" , maxSaveTime )
        J_SERIALIZE_MAP_ITEM( "auto_stop"     , autoStop    )
        J_SERIALIZE_MAP_ITEM( "file_name"     , fileName    )
    J_SERIALIZE_END
};

//struct SaverStop : Data<&command::SaverStop,
//                         Message::Type::Command>
//{
//    // Название видеозаписи которая будет остановлена.
//    // Это поле для информирования клиентской стороны
//    QString fileName;

//    DECLARE_B_SERIALIZE_FUNC

//    J_SERIALIZE_BEGIN
//        J_SERIALIZE_MAP_ITEM( "file_name" , fileName )
//    J_SERIALIZE_END
//};

struct SaverStop : Data<&command::SaverStop,
                         Message::Type::Command,
                         Message::Type::Answer>
{
    bool origin = {true};
    bool markup = {false};

    // Название видеозаписи которая будет остановлена.
    // Это поле для информирования клиентской стороны
    QString fileName;

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "origin"    , origin     )
        J_SERIALIZE_MAP_ITEM( "markup"    , markup     )
        J_SERIALIZE_MAP_OPT ( "file_name" , fileName   )
    J_SERIALIZE_END
};

} // namespace data
} // namespace pproto
