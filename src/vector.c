#include "vector.h"
#include <math.h>

vect3_t vect3_rotate_x(vect3_t v, float angle) 
{
    vect3_t rotated_vactor = {
        v.x,
        v.y * cos(angle) - v.z * sin(angle),
        v.y * sin(angle) + v.z * cos(angle)
    };

    return rotated_vactor;
}

vect3_t vect3_rotate_y(vect3_t v, float angle)
{
    vect3_t rotated_vactor = {
        v.x * cos(angle) - v.z * sin(angle),
        v.y,
        v.x * sin(angle) + v.z * cos(angle)
    };

    return rotated_vactor;
}

vect3_t vect3_rotate_z(vect3_t v, float angle)
{
    vect3_t rotated_vactor = {
        v.x * cos(angle) - v.y * sin(angle),
        v.x * sin(angle) + v.y * cos(angle),
        v.z
    };

    return rotated_vactor;
}