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

t_phil *last_member(t_phil *lst)
{
    while (lst->next)
        lst = lst->next;
    return (lst);
}

t_phil *bfr_this(t_phil *lst, t_phil *this)
{
    while (lst)
    {
        if (lst->next == this)
            break;
        lst = lst->next;
    }
    return (lst);
}
