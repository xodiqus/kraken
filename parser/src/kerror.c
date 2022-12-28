#include "kerror.h"

struct KError no_KError()
{
    struct KError r; 

    r.num = EN_OK; 
    r.msg = "no error";

    return r;
}