#pragma once

#include "shared/container_ptr.h"
#include "pproto/commands/base.h"

namespace pproto {
namespace command {

//----------------------------- Список команд --------------------------------

/**
  Событие эмитируется при попытке открытия видеопотока. При удачном открытии по-
  тока поле сообщения tag(0) будет содержать значение 1 (TRUE),  если  открытие
  потока было неудачным поле tag(0) будет содержать значение 0 (FALSE)
*/
extern const QUuidEx VideoStreamOpened;

/**
  Событие эмитируется при закрытиии видеопотока
*/
extern const QUuidEx VideoStreamClosed;

/**
  Команда возвращает статус видеопотока открыт/закрыт в поле tag(0). Значение 0
  сигнализирует о том, что видеопоток недоступен (закрыт),  значение 1 сигнали-
  зирует об активном видеопотоке (открыт)
*/
extern const QUuidEx VideoStreamIsOpen;

} // namespace command

//---------------- Структуры данных используемые в сообщениях ----------------

namespace data {

} // namespace data
} // namespace pproto

