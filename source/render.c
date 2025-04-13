#include "nds.h"
#include "assets.h"

void renderObject(int32_t position[], model *_model) {
    glTranslatef32(position[0], position[1], position[2]);
    glBindTexture(GL_TEXTURE_2D, *_model->texture);
    glCallList(_model->model);
    glTranslatef32(-position[0], -position[1], -position[2]);
}
