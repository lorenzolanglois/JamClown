#include <nds.h>
#include "assets.h"

#define NUMBEROFTEXTURES 3
#define TEXTURELIST {&sandBitmap, &roomBitmap, &metalBitmap}
#define NUMBEROFMODEL 7
#define MODELLIST {{"room_floor", &room_floor_bin}, {"room_wall", &room_wall_bin}, {"pipe", &pipe_bin}, {"treadmill", &treadmill_bin}, {"penguin", &penguin_bin}, {"cannon", &cannon_bin}, {"timer", &timer_bin}}

model *initializeModels(int *textureID) {
    static model _models[] = MODELLIST;
    _models[0].texture = &textureID[0];
    _models[1].texture = &textureID[1];
    _models[2].texture = &textureID[2];
    _models[3].texture = &textureID[2];
    _models[4].texture = &textureID[2];
    _models[5].texture = &textureID[2];
    _models[6].texture = &textureID[2];
    return _models;
}

int *initializeTextures() {
    const void *_textureList[] = TEXTURELIST;
    int *textureID = malloc(NUMBEROFTEXTURES * sizeof(int));

    for (int i = 0; i < NUMBEROFTEXTURES; i += 1) {
        glGenTextures(1, &textureID[i]);
        glBindTexture(0, textureID[i]);

        if (glTexImage2D(0, 0, GL_RGBA, 128, 128, 0,
            TEXGEN_TEXCOORD | GL_TEXTURE_WRAP_S | GL_TEXTURE_WRAP_T,
            _textureList[i]) == 0) {
        printf("Failed to load texture stanleyBitmap\n");
        while (1)
            swiWaitForVBlank();
        }
    }
    return textureID;
}
