#pragma once

#include "pproto/commands/base.h"

namespace pproto {
namespace error {

//--- 10 Ошибки общего плана ---
DECL_ERROR_CODE(product_license,           10, "1f46e8e0-0fdf-464c-b311-264c85977b96", u8"Лицензия на продукт ViDetect не обнаружена")
//DECL_ERROR_CODE(image_empty,             10, "3ca5827e-62ff-46ff-8228-7936a386d0f9", u8"Нет изображений в пакете")
//DECL_ERROR_CODE(image_unknown,           10, "b6b3afbd-63e4-482a-a80b-647a423642d1", u8"Неизвестная ошибка обработки изображения %1")
//DECL_ERROR_CODE(image_format,            10, "cc599a01-abba-4880-af16-08527d43a975", u8"Неизвестный формат изображения")
//DECL_ERROR_CODE(image_decode,            10, "85dc0536-b7be-45e4-a14b-f0eab2ffab2c", u8"Ошибка декодирования изображения %1. Подробнее: %2")
//DECL_ERROR_CODE(image_limit,             10, "331f4ca7-7564-4f64-9bfa-85760048ec54", u8"Превышен лимит (%1) на количество изображений в одном пакете")
//DECL_ERROR_CODE(cuda_engine_context,     10, "667b388a-3243-4106-868e-5e0bb9e2365b", u8"Ошибка обработки изображения %1 нейросетью")
//DECL_ERROR_CODE(cuda_engine_context_cpu, 10, "f9a0abb0-3d32-4381-8085-f7b20e05759d", u8"Ошибка обработки изображения %1 нейросетью на CPU")

DECL_ERROR_CODE(trackers_event_subscript,  10, "c8ed073f-e005-43fa-9d39-f97f7e5ee556", u8"Индекс %1 класса модели не принадлежит диапазону [%2-%3)")

} // namespace error
} // namespace pproto
