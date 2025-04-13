#include "engine.h"

// Subtract vectors: r[] = a[] - b[]
static inline void vec_sub(int32_t *a, int32_t *b, int32_t *r)
{
    r[0] = a[0] - b[0];
    r[1] = a[1] - b[1];
    r[2] = a[2] - b[2];
}

// Calculate length of a vector
static inline int32_t vec_length(int32_t *v)
{
    return sqrtf32(mulf32(v[0], v[0]) + mulf32(v[1], v[1]) + mulf32(v[2], v[2]));
}

// Divide a vector by a constant: r[] = a[] / b
static inline void vec_div(int32_t *a, int32_t b, int32_t *r)
{
    r[0] = divf32(a[0], b);
    r[1] = divf32(a[1], b);
    r[2] = divf32(a[2], b);
}

// It setups hardware light "light_id" to point from "light_pos" to "obj_pos".
// It will use an attenuation factor of "attenuation", and the color of the
// light will be set to (r, g, b). This needs to be called for every light for
// every object.
void setup_point_light(int light_id, int32_t *light_pos, int32_t *obj_pos,
    int32_t attenuation, int r, int g, int b)
{
    int32_t light_to_obj[3];
    vec_sub(obj_pos, light_pos, light_to_obj);

    int32_t distance = vec_length(light_to_obj);

    int32_t light_to_obj_unit[3];
    vec_div(light_to_obj, distance, light_to_obj_unit);

    int32_t distance_square = mulf32(distance, distance);

    int32_t r_calc = divf32(inttof32(r), mulf32(attenuation, distance_square));
    int32_t g_calc = divf32(inttof32(g), mulf32(attenuation, distance_square));
    int32_t b_calc = divf32(inttof32(b), mulf32(attenuation, distance_square));

    if (r_calc > r)
    r_calc = r;
    if (g_calc > g)
    g_calc = g;
    if (b_calc > b)
    b_calc = b;

    // Setup the light
    glLight(light_id, RGB15(r_calc, g_calc, b_calc),
    floattov10(f32tofloat(light_to_obj_unit[0])),
    floattov10(f32tofloat(light_to_obj_unit[1])),
    floattov10(f32tofloat(light_to_obj_unit[2])));
}
