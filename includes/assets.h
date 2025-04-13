#pragma once

#include "room_wall_bin.h"
#include "room.h"
#include "room_floor_bin.h"
#include "sand.h"
#include "bottom_screen.h"
#include "metal.h"
#include "pipe_bin.h"
#include "treadmill_bin.h"
#include "penguin_bin.h"
#include "cannon_bin.h"
#include "timer_bin.h"
#include "wrong_bin.h"
#include "right_bin.h"

/// Structure containing the name of a model as a string, a pointer to the model and the pointer to the texture.
///
/// @param name
///     A string containing the name of the model.
///
/// @param model
///     A pointer to the model.
///
/// @param texture
///     A pointer to the texture.
typedef struct {
    char *name;
    const void *model;
    int *texture;
} model;

/// Function to initialize objects.
model *initializeModels();

/// Function to initialize the textures.
///
/// @return
///     textureID list with all the references to the textures.
int *initializeTextures();
