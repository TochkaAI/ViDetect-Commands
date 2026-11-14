#pragma once

#include "pproto/commands/base.h"

namespace pproto {
namespace error {

//--- 10 Ошибки общего плана ---
DECL_ERROR_CODE(product_license,           10, "acf70ccf-569c-43eb-903f-43da08139338", u8"Лицензия для сервиса ViDetect не обнаружена")
DECL_ERROR_CODE(protect_key,               10, "5d43ca8c-1e7a-4fa9-947a-4624150baa59", u8"Ключ аппаратной защиты для сервиса ViDetect не обнаружен")

DECL_ERROR_CODE(trackers_event_subscript,  10, "c8ed073f-e005-43fa-9d39-f97f7e5ee556", u8"Индекс %1 класса модели не принадлежит диапазону [%2-%3)")

//--- 60 Ошибки работы с сейвером ---
DECL_ERROR_CODE(saver_start,               60, "11eed98e-03a2-4820-9edf-e9c4da92dce9", u8"Не удалось запустить запись видео Dynamic Saver")
DECL_ERROR_CODE(saver_stop,                60, "928156ae-ed91-4405-9382-a8afbfaed070", u8"Не удалось остановить запись видео Dynamic Saver")
DECL_ERROR_CODE(saver_already_running,     60, "df12e1cb-6558-49a3-86c2-1add01c54884", u8"Запись видео Dynamic Saver уже запущена")
DECL_ERROR_CODE(saver_no_space_left,       60, "42b56d0c-d59e-4100-a4be-b70a2dd9d3bf", u8"Недостаточно памяти для работы Dynamic Saver. Свободно %1 : необходимо %2 (Гб)")

} // namespace error
} // namespace pproto
