#include <nds.h>
#include <math.h>

#include "engine.h"
#include "assets.h"

int timer0_count = 0;

void timer0_handler(void)
{
    timer0_count++;
}

void initGame() {
    glInit();
    // consoleDemoInit();

    videoSetMode(MODE_0_3D);
    videoSetModeSub(MODE_5_2D);

    // vramSetBankA(VRAM_A_TEXTURE);
    // vramSetBankB(VRAM_B_TEXTURE);

    // Allocate some VRAM to be used for backgrounds in the main engine
    vramSetBankF(VRAM_F_MAIN_BG_0x06000000);

    vramSetPrimaryBanks(VRAM_A_TEXTURE, VRAM_B_TEXTURE,
       VRAM_C_SUB_BG_0x06200000, VRAM_D_LCD);

    consoleInit(NULL, 1, BgType_Text4bpp, BgSize_T_256x256, 7, 0, true, true);

    // Set layer 0 (3D layer) to priority 1 (lower priority than 0)
    bgSetPriority(0, 1);

    // Set layer 1 (console) to priority 0
    bgSetPriority(1, 0);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_ANTIALIAS);
    glEnable(GL_BLEND);

    // Setup the rear plane
    glClearColor(0, 0, 0, 0); // Set BG to black
    // The BG and polygons will have the same ID unless a polygon is highlighted
    glClearPolyID(0);
    glClearDepth(GL_MAX_DEPTH);
}

int32_t *calculateOffset(int32_t *position, int32_t *offset) {
    int32_t *result = malloc(sizeof(int32_t) * 3);
    result[0] = position[0] + offset[0];
    result[1] = position[1] + offset[1];
    result[2] = position[2] + offset[2];
    return result;
}

short int *generateRandom() {
    short int *result = malloc(sizeof(short int) * 3);
    int random = rand();

    for (int i = 0; i < 100; i++) {
        random = rand() % 3;
    }
    result[0] = random == 0 ? 4 : 5;
    result[1] = random == 1 ? 4 : 5;
    result[2] = random == 2 ? 4 : 5;
    return result;
}

void game() {
    float angle = 4.7f;

    float cameraPos[] = {0.0f, 2.2f, 4.0f};
    float lookAt[] = {cameraPos[0] + cosf(angle), 2.1f, cameraPos[2] + sinf(angle)};
    const float up[] = {0.0f, 1.0f, 0.0f};

    initGame();
    soundEnable();

    gluLookAt(cameraPos[0], cameraPos[1], cameraPos[2],
        lookAt[0], lookAt[1], lookAt[2],
        up[0], up[1], up[2]);

    int *textureID = initializeTextures();
    model *models = initializeModels(textureID);

    glMaterialShinyness();

    // Set the viewport to fullscreen
    glViewport(0, 0, 255, 191);

    // Setup the projection matrix for regular drawing
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, 256.0 / 192.0, 0.1, 20);

    // Use the modelview matrix while drawing
    glMatrixMode(GL_MODELVIEW);

    // Setup some material properties
    glMaterialf(GL_AMBIENT, RGB15(22, 20, 20));
    glMaterialf(GL_DIFFUSE, RGB15(31, 31, 31));
    glMaterialf(GL_SPECULAR, RGB15(0, 0, 0));
    glMaterialf(GL_EMISSION, RGB15(0, 0, 0));

    int32_t scene_pos[1][3] = {
        { floattof32(0), floattof32(0), floattof32(0) },
    };

    int32_t pipe_pos[3][3] = {
        { floattof32(-1.75), floattof32(-0.6), floattof32(-1.3) },
        { floattof32(1.25), floattof32(-0.6), floattof32(-1.3) },
        { floattof32(4.25), floattof32(-0.6), floattof32(-1.3) },
    };

    int32_t trademill_pos[1][3] = {
        { floattof32(-0.75), floattof32(-0.6), floattof32(-1.5) },
    };

    int32_t random_pos[3][3] = {
        { floattof32(-1.45), floattof32(1.2), floattof32(-1.4) },
        { floattof32(1.45), floattof32(1.2), floattof32(-1.4) },
        { floattof32(4.45), floattof32(1.2), floattof32(-1.4) },
    };

    int32_t timer_pos[3] = {
        floattof32(-1.5), floattof32(-0.3), floattof32(-1.5)
    };

    light_info light[1] = {
        {
            { floattof32(0), floattof32(1.5), floattof32(5) },
            50, 50, 50, floattof32(1.0)
        },
    };

    // Bottom_screen image
    int bg = bgInitSub(3, BgType_Bmp16, BgSize_B16_256x256, 0, 0);
    dmaCopy(bottom_screenBitmap, bgGetGfxPtr(bg), bottom_screenBitmapLen);

    int32_t offset[3] = {
        floattof32(0.0), floattof32(0.0), floattof32(0.0)
    };

    int32_t timer_offset[3] = {
        floattof32(0.0), floattof32(0.0), floattof32(0.0)
    };

    short int *random = generateRandom();

    int high_score = 0;
    int score = 0;
    int ch_wav = -1;

    bool moving = false;
    bool falling = true;
    bool ended = false;

    uint16_t keys_held;
    touchPosition touch_pos;

    // Timer 0 will be called every second. This is a long time to wait, so a
    // big divider is required so that the timer counter can fit enough ticks to
    // count up to a second.
    timerStart(0, ClockDivider_1024, timerFreqToTicks_1024(1), timer0_handler);

    while (1)
    {
        // Handle key input
        scanKeys();
        keys_held = keysHeld();

        consoleClear();
        if (ended) {
            printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nPress Start to restart\n");
        } else {
            printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
        }
        printf("Score: %i\nHigh Score: %i\n", score, high_score);

        timer_offset[0] -= timer0_count / 3.5;
        if (score > high_score) {
            high_score = score;
        }

        if (timer0_count >= 60 && !ended) {
            timerStop(0);
            ended = true;
            moving = true;
        }

        // If pen down, update touch input.
        if (keys_held & KEY_TOUCH && !moving && !falling && !ended) {
            touchRead(&touch_pos);
            if (touch_pos.rawy >= 800 && touch_pos.rawy <= 2000) {
                if (touch_pos.rawx >= 150 && touch_pos.rawx < 1300) {
                    moving = true;
                    if (random[0] == 4) {
                        score += 1;
                        if (ch_wav != -1)
                            soundKill(ch_wav);

                        ch_wav = soundPlaySample(right_bin, SoundFormat_16Bit,
                                                right_bin_size, 44100, 127, 64,
                                                false, 0);
                    } else {
                        score -= 1;
                        if (ch_wav != -1)
                            soundKill(ch_wav);

                        ch_wav = soundPlaySample(wrong_bin, SoundFormat_16Bit,
                                                wrong_bin_size, 11025, 127, 64,
                                                false, 0);
                    }
                }
                if (touch_pos.rawx >= 1300 && touch_pos.rawx < 2600) {
                    moving = true;
                    if (random[1] == 4) {
                        score += 1;
                        if (ch_wav != -1)
                            soundKill(ch_wav);

                        ch_wav = soundPlaySample(right_bin, SoundFormat_16Bit,
                                                right_bin_size, 44100, 127, 64,
                                                false, 0);
                    } else {
                        score -= 1;
                        if (ch_wav != -1)
                            soundKill(ch_wav);

                        ch_wav = soundPlaySample(wrong_bin, SoundFormat_16Bit,
                                                wrong_bin_size, 11025, 127, 64,
                                                false, 0);
                    }
                }
                if (touch_pos.rawx >= 2600 && touch_pos.rawx <= 4000) {
                    moving = true;
                    if (random[2] == 4) {
                        score += 1;
                        if (ch_wav != -1)
                            soundKill(ch_wav);

                        ch_wav = soundPlaySample(right_bin, SoundFormat_ADPCM,
                                                right_bin_size, 44100, 60, 64,
                                                false, 0);
                    } else {
                        score -= 1;
                        if (ch_wav != -1)
                            soundKill(ch_wav);

                        ch_wav = soundPlaySample(wrong_bin, SoundFormat_16Bit,
                                                wrong_bin_size, 44100, 60, 64,
                                                false, 0);
                    }
                }
            }
        }

        if (moving) {
            if (offset[0] < floattof32(7.0)) {
                offset[0] += floattof32(0.2);
            } else {
                moving = false;
                if (!ended) {
                    offset[0] = 0;
                    offset[1] = floattof32(0.0);
                    random = generateRandom();
                    falling = true;
                } else {
                    offset[0] += floattof32(1.0);
                }
            }
        }

        if (falling) {
            if (offset[1] > floattof32(-1.5)) {
                offset[1] += floattof32(-0.3);
            } else {
                falling = false;
            }
        }

        glPolyFmt(POLY_ALPHA(31) | POLY_CULL_BACK | POLY_FORMAT_LIGHT0 | POLY_FORMAT_LIGHT1);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        gluLookAt(cameraPos[0], cameraPos[1], cameraPos[2],
            lookAt[0], lookAt[1], lookAt[2],
            up[0], up[1], up[2]);

        glPushMatrix();
        {
            setup_point_light(0, light[0].pos, scene_pos[0],
                              light[0].attenuation, light[0].r, light[0].g, light[0].b);

            renderObject(scene_pos[0], &models[0]);
            renderObject(scene_pos[0], &models[1]);

            renderObject(pipe_pos[0], &models[2]);
            renderObject(pipe_pos[1], &models[2]);
            renderObject(pipe_pos[2], &models[2]);

            renderObject(trademill_pos[0], &models[3]);

            for (int i = 0; i < 3; i++) {
                renderObject(calculateOffset(random_pos[i], offset), &models[random[i]]);
            }

            renderObject(calculateOffset(timer_pos, timer_offset), &models[6]);
        }

        glPopMatrix(1);

        glFlush(0);

        swiWaitForVBlank();

        if (keys_held & KEY_START && ended) {
            random = generateRandom();

            score = 0;

            moving = false;
            falling = true;
            ended = false;

            offset[0] = 0;
            offset[1] = floattof32(0.0);

            timer0_count = 0;

            timer_offset[0] = floattof32(0.0);
            timerStart(0, ClockDivider_1024, timerFreqToTicks_1024(1), timer0_handler);
        }
    }
}
