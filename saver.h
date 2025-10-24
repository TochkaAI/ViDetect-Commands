#pragma once

#include "shared/container_ptr.h"
#include "pproto/commands/base.h"

namespace pproto {
namespace command {

//----------------------------- Список команд --------------------------------

/**
  Возвращает информацию о параметрах сейвера
*/
extern const QUuidEx SaverInfo;

/**
  Возвращает информацию о параметрах сейвера
*/
extern const QUuidEx SaverBufferInfo;

/**
  Команда устанавливающая размер буффера кадров динамического сейвера
  Размеченные и неразмеченные буфферы регулируются независимо.
  Кадры из буффера будут записаны в начало видео для просмотра
  предыстории события.
*/
extern const QUuidEx SetSaverBuffer;

/**
  Команда для запуска записи динамическим сейвером
*/
extern const QUuidEx SaverStart;

/**
  Команда для завершения записи динамическим сейвером
*/
extern const QUuidEx SaverStop;


} // namespace command

//---------------- Структуры данных используемые в сообщениях ----------------

namespace data {

//struct SaverInfo : Data<&command::SaverInfo,
//                         Message::Type::Command>
//{
//    // Флаг обозначающий тип записи - с разметкой или без
//    QString fileName;

//    // Максимальное время записи видеофрагмента (сек.)
//    quint32 maxSaveTime = {0};

//    //TODO информация об ограничениях?
//    bool isMarkup = {false};


//    DECLARE_B_SERIALIZE_FUNC

//    J_SERIALIZE_BEGIN
//        J_SERIALIZE_MAP_ITEM( "fileName"    , fileName    )
//        J_SERIALIZE_MAP_ITEM( "maxSaveTime" , maxSaveTime )
//    J_SERIALIZE_END
//};

struct SaverInfoA : Data<&command::SaverInfo,
                          Message::Type::Answer>
{
    // Флаг обозначающий тип записи - с разметкой или без
    QString fileName;

    // Максимальное время записи видеофрагмента (сек.)
    quint32 maxSaveTime = {0};

    //TODO информация об ограничениях?
    bool isMarkup = {false};


    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "file_name"     , fileName    )
        J_SERIALIZE_MAP_ITEM( "max_save_time" , maxSaveTime )
        J_SERIALIZE_MAP_ITEM( "is_markup"     , isMarkup    )
    J_SERIALIZE_END
};

//struct SaverBufferInfo : Data<&command::SaverBufferInfo,
//                               Message::Type::Command>
//{
//    // Число кадров в буффере без разметки
//    // Максимальное значение ~ 500 кадров
//    quint32 bufferSizeClear = {0};

//    // Число кадров в буффере с разметкой
//    quint32 bufferSizeMarkup = {0};

//    DECLARE_B_SERIALIZE_FUNC

//    J_SERIALIZE_BEGIN
//        J_SERIALIZE_MAP_ITEM( "bufferSizeClear"  , bufferSizeClear  )
//        J_SERIALIZE_MAP_ITEM( "bufferSizeMarkup" , bufferSizeMarkup )
//    J_SERIALIZE_END
//};

struct SaverBufferInfoA : Data<&command::SaverBufferInfo,
                                Message::Type::Answer>
{
    // Число кадров в буффере без разметки
    // Максимальное значение ~ 500 кадров
    quint32 bufferSizeClear = {0};

    // Число кадров в буффере с разметкой
    quint32 bufferSizeMarkup = {0};

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "buffer_size_clear"  , bufferSizeClear  )
        J_SERIALIZE_MAP_ITEM( "buffer_size_markup" , bufferSizeMarkup )
    J_SERIALIZE_END
};

struct SetSaverBuffer : Data<&command::SetSaverBuffer,
                              Message::Type::Command>
{
    // Число кадров в буффере без разметки
    // Максимальное значение ~ 500 кадров
    quint32 bufferSizeClear = {0};

    // Число кадров в буффере с разметкой
    quint32 bufferSizeMarkup = {0};

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "buffer_size_clear"  , bufferSizeClear  )
        J_SERIALIZE_MAP_ITEM( "buffer_size_markup" , bufferSizeMarkup )
    J_SERIALIZE_END
};

struct SaverStart : Data<&command::SaverStart,
                          Message::Type::Command,
                          Message::Type::Answer>
{
    int saverIndex = {-1};

    // Флаг обозначающий тип записи - с разметкой или без
    bool isMarkup = {true};

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
        J_SERIALIZE_MAP_ITEM( "saver_index"   , saverIndex  )
        J_SERIALIZE_MAP_ITEM( "is_markup"     , isMarkup    )
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
    int saverIndex = {-1};

    // Название видеозаписи которая будет остановлена.
    // Это поле для информирования клиентской стороны
    QString fileName;

    DECLARE_B_SERIALIZE_FUNC

    J_SERIALIZE_BEGIN
        J_SERIALIZE_MAP_ITEM( "saver_index" , saverIndex )
        J_SERIALIZE_MAP_ITEM( "file_name"   , fileName   )
    J_SERIALIZE_END
};

} // namespace data
} // namespace pproto
