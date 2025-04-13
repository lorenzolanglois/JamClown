#pragma once

#include <nds.h>
#include "assets.h"

// Structures

typedef struct {
    int32_t pos[3];
    int r, g, b;
    int32_t attenuation;
} light_info;

// Lights

void setup_point_light(int light_id, int32_t *light_pos, int32_t *obj_pos,
    int32_t attenuation, int r, int g, int b);


// Render

/// Function to render objects.
///
/// @param position
///     Position of the object.
///
/// @param _model
///     Pointer to the structure of the model.
void renderObject(int32_t position[], model *_model);

// Game

void game();
