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

t_phil *last_phil(t_phil *lst)
{
    while (lst)
    {
        if (!lst->next)
            return (lst);
        lst = lst->next;
    }
    return (NULL);
}

t_fork *last_fork(t_fork *lst)
{
    while (lst)
    {
        if (!lst->next)
            return (lst);
        lst = lst->next;
    }
    return (NULL);
}

t_phil *bfr_this(t_phil *lst, t_phil *this)
{
    while (lst->next)
    {
        if (lst->next == this)
            return (lst);
        lst = lst->next;
    }
    return (NULL);
}
