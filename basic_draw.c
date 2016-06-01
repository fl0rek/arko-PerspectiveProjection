#include "util/matUtil.c"
#include "util/asmatUtil.h"

#include <inttypes.h>

#include <fdebug.h>


float vertices1[] = {
        -1.f, -.2f, -1.f,  1.f,
         1.f, -.2f, -1.f,  1.f,
         0.f, -.2f,  1.82842712, 1.f,
         0.f, .8f,  0.f,  1.f
};

unsigned vertice_num = 4;
float vertices[] = {
         1.f, -1.f,  1.f, 1.f,
        -1.f, -1.f, -1.f, 1.f,
        -1.f,  1.f,  1.f, 1.f,
         1.f,  1.f, -1.f, 1.f
};

float projectedVertices[16];

const float distance = 3;

float projectionMatrixOriginal[] = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, -.5f, 1 // -1/distance
};
float projectionMatrix[16];

unsigned line_num = 6;
unsigned lines[] = {
        0, 1,
        0, 2,
        0, 3,

        1, 2,
        1, 3,

        2, 3
};

int lines1[] = {
        0, 1,
        1, 2,
        2, 0,
        0, 3,
        1, 3,
        2, 3
};

float testSquare[] = {
        -.5f,  .5f,
         .5f,  .5f,
         .5f, -.5f,
        -.5f, -.5f
};

int w = 480;
int h = 240;

void fromUniformCoords(float *xy, int *rxy) {
        log_warn1("calling c implementation");
        rxy[0] = ((xy[0]+1) * w)/2;
        rxy[1] = ((xy[1]+1) * h)/2;
}

void transformPoint(const float* origin, float* dest) {
        log_warn1("calling c implementation");
        memcpy(dest, origin, 4*sizeof(float));

        a_multVector(dest, projectionMatrix);

        debug("o: %f, %f, %f, %f", origin[0], origin[1], origin[2], origin[3]);
        debug("d: %f, %f, %f, %f", dest[0], dest[1], dest[2], dest[3]);
}

void
drawlineXD(unsigned char *p, int x0, int y0, int x1, int y1, int width);

extern uint64_t window_width_int;
extern uint64_t window_height_int;

unsigned scale(float v, uint64_t max) {
        log_warn1("calling c implementation");
        if(v < -1) {
                v = -1;
                debug1("< -1");
        }
        if(v > 1) {
                v = 1;
                debug1(">  1");
        }
        return ((v+1)*max)/2;
}

void drawLineI(unsigned char *p, float* ufrom, float *uto) {
        log_warn1("calling c implementation");
        int from[2];
        int to[2];

        from[0] = scale(ufrom[0], window_width_int);
        from[1] = scale(ufrom[1], window_height_int);

        to[0] = scale(uto[0], window_width_int);
        to[1] = scale(uto[1], window_height_int);

        drawlineXD(p, from[0], from[1], to[0], to[1], window_width_int);
}

void dumpMatrix(float *m) {
        debug("\n%5.3f %5.3f %5.3f %5.3f\n%5.3f %5.3f %5.3f %5.3f\n%5.3f %5.3f %5.3f %5.3f\n%5.3f %5.3f %5.3f %5.3f\n",
        m[0], m[1], m[2], m[3], m[4], m[5], m[6], m[7], m[8], m[9], m[10],
        m[11], m[12], m[13], m[14], m[15]
        );
}

void a_redraw(unsigned char *p, float angleX, float angleY);

void sa_redraw(unsigned char *p, float angleX, float angleY) {
        debug1("I'm beeing called LUL");

        drawLineI(p, &testSquare[0], &testSquare[2]);
        drawLineI(p, &testSquare[2], &testSquare[4]);
        drawLineI(p, &testSquare[4], &testSquare[6]);
        drawLineI(p, &testSquare[6], &testSquare[0]);

        float tmp[16];

        memcpy(projectionMatrix, projectionMatrixOriginal, 16*sizeof(float));
        dumpMatrix(projectionMatrix);

        a_doTranslationMatrix(0, 0, -3, tmp);
        dumpMatrix(tmp);
        a_multMatrix(projectionMatrix, tmp);

        dumpMatrix(projectionMatrix);

        a_doRotationMatrixY(angleY, tmp);
        a_multMatrix(projectionMatrix, tmp);

        a_doRotationMatrixX(angleX, tmp);
        a_multMatrix(projectionMatrix, tmp);

        dumpMatrix(projectionMatrix);

        a_transpose(projectionMatrix);

        for (size_t i = 0; i < 4; i++) {
                debug("%f", vertices[i*4]);
                transformPoint(&vertices[i *4], &projectedVertices[i *4]);
                a_normalizeTo3d(&projectedVertices[i *4]);
        }

        for (size_t i = 0; i < 6; i++) {
                drawLineI(p,
                        &projectedVertices[lines[i *2] *4],
                        &projectedVertices[lines[i *2 +1] *4]
                );
        }
}

void redraw(unsigned char *p, float angleX, float angleY) {

        debug1("I'm beeing called LUL");

        drawLineI(p, &testSquare[0], &testSquare[2]);
        drawLineI(p, &testSquare[2], &testSquare[4]);
        drawLineI(p, &testSquare[4], &testSquare[6]);
        drawLineI(p, &testSquare[6], &testSquare[0]);

        float tmp[16];

        memcpy(projectionMatrix, projectionMatrixOriginal, 16*sizeof(float));
        dumpMatrix(projectionMatrix);

        doTranslationMatrix(0, 0, -3, tmp);
        dumpMatrix(tmp);
        multMatrix(projectionMatrix, tmp);

        dumpMatrix(projectionMatrix);

        doRotationMatrixY(angleY, tmp);
        multMatrix(projectionMatrix, tmp);

        doRotationMatrixX(angleX, tmp);
        multMatrix(projectionMatrix, tmp);

        dumpMatrix(projectionMatrix);

        for (size_t i = 0; i < 4; i++) {
                debug("%f", vertices[i*4]);
                transformPoint(&vertices[i *4], &projectedVertices[i *4]);
                normalizeTo3d(&projectedVertices[i *4]);
        }

        for (size_t i = 0; i < 6; i++) {
                drawLineI(p,
                        &projectedVertices[lines[i *2] *4],
                        &projectedVertices[lines[i *2 +1] *4]
                );
        }
}
