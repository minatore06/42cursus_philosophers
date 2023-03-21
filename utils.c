#include "philosophers.h"

t_fork  *get_next(t_fork *lst, t_fork *el)
{
    if (el->next)
        return (el->next);
    return (lst);
}

int last_id(t_fork *lst)
{
    int id;

    while (lst)
    {
        id = lst->id;
        lst = lst->next;
    }
    return (id);
}