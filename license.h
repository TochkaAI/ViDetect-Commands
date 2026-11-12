#pragma once

#include "pproto/commands/base.h"

namespace pproto {
namespace command {

//----------------------------- Список команд --------------------------------

/**
  Команда возвращает статус лицензии в поле tag(0). Значение 1 соответствует
  активной  лицензии,  значение 0 возвращается, когда лицензия неактивна или
  отсутствует
*/
extern const QUuidEx LicenseActive;

} // namespace command
} // namespace pproto
