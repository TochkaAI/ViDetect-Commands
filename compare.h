#pragma once

#include "pproto/commands/base.h"

namespace pproto {

template<typename T> struct CompareId
{
    // Сортировка по id
    int operator() (const T* item1, const T* item2) const
        {return QUuidEx::compare(item1->id, item2->id);}

    // Поиск по id
    int operator() (const QUuidEx* id, const T* item2) const
        {return QUuidEx::compare(*id, item2->id);}
};

template<typename T> struct ModelClassFind
{
    // Поиск по index
    int operator() (const qint32* index, const T* item2) const
        {return LIST_COMPARE_ITEM(*index, item2->index);}
};

} // namespace pproto
