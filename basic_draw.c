#include "util/matUtil.c"

#include <fdebug.h>

float vertices1[] = {
        -1.f, -.2f, -1.f,  1.f,
         1.f, -.2f, -1.f,  1.f,
         0.f, -.2f,  1.82842712, 1.f,
         0.f, .8f,  0.f,  1.f
};

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

int lines[] = {
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
        rxy[0] = ((xy[0]+1) * w)/2;
        rxy[1] = ((xy[1]+1) * h)/2;
}

void transformPoint(const float* origin, float* dest) {
        memcpy(dest, origin, 4*sizeof(float));

        multVector(dest, projectionMatrix);

        debug("o: %f, %f, %f, %f", origin[0], origin[1], origin[2], origin[3]);
        debug("d: %f, %f, %f, %f", dest[0], dest[1], dest[2], dest[3]);
}

void
drawlineXD(unsigned char *p, int x0, int y0, int x1, int y1, int width);

extern unsigned window_width;
extern unsigned window_height;

unsigned scale(float v, unsigned max) {
        if(v < -1) {
                v = -1;
                debug("< -1");
        }
        if(v > 1) {
                v = 1;
                debug(">  1");
        }
        return ((v+1)*max)/2;
}

void drawLineI(unsigned char *p, float* ufrom, float *uto) {
        int from[2];
        int to[2];

        from[0] = scale(ufrom[0], window_width);
        from[1] = scale(ufrom[1], window_height);

        to[0] = scale(uto[0], window_width);
        to[1] = scale(uto[1], window_height);

        drawlineXD(p, from[0], from[1], to[0], to[1], window_width);
}

void dumpMatrix(float *m) {
        debug("\n%5.3f %5.3f %5.3f %5.3f\n%5.3f %5.3f %5.3f %5.3f\n%5.3f %5.3f %5.3f %5.3f\n%5.3f %5.3f %5.3f %5.3f\n",
        m[0], m[1], m[2], m[3], m[4], m[5], m[6], m[7], m[8], m[9], m[10],
        m[11], m[12], m[13], m[14], m[15]
        );
}

void redraw(unsigned char *p, float angle) {
        drawLineI(p, &testSquare[0], &testSquare[2]);
        drawLineI(p, &testSquare[2], &testSquare[4]);
        drawLineI(p, &testSquare[4], &testSquare[6]);
        drawLineI(p, &testSquare[6], &testSquare[0]);

        float tmp[16];

        memcpy(projectionMatrix, projectionMatrixOriginal, 16*sizeof(float));
        dumpMatrix(projectionMatrix);

        doTranslationMatrix(0, 0, -5, tmp);
        dumpMatrix(tmp);
        multMatrix(projectionMatrix, tmp);

        dumpMatrix(projectionMatrix);

        doRotationMatrix1(angle, tmp);
        debug("rotation matrix:");
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

#if 0
int main1(int argc, char const *argv[]) {
        // INIT
        cairo_surface_t *surface  =
                cairo_image_surface_create(CAIRO_FORMAT_RGB24, w, h);
        cairo_t *cr = cairo_create(surface);

        log_info("init done");

        cairo_save(cr);
        cairo_set_source_rgb(cr, 100, 100, 0);
        cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);
        cairo_paint(cr);
        cairo_restore(cr);

        // DRAW LINE
        drawLine(cr, &testSquare[0], &testSquare[2]);
        drawLine(cr, &testSquare[2], &testSquare[4]);
        drawLine(cr, &testSquare[4], &testSquare[6]);
        drawLine(cr, &testSquare[6], &testSquare[0]);

        float tmp[16];
        doRotationMatrix1(.0, tmp);
        multMatrix(projectionMatrix, tmp);
        doTranslationMatrix(0, 0, 5, tmp);
        multMatrix(projectionMatrix, tmp);


        //multMatrix(rot, projectionMatrix);
        //memcpy(projectionMatrix, rot, 16);

        for (size_t i = 0; i < 4; i++) {
                debug("%f", vertices[i*4]);
                transformPoint(&vertices[i *4], &projectedVertices[i *4]);
                normalizeTo3d(&projectedVertices[i *4]);
        }

        for (size_t i = 0; i < 6; i++) {
                drawLine(cr,
                        &projectedVertices[lines[i *2] *4],
                        &projectedVertices[lines[i *2 +1] *4]
                );
        }

        // CLEANUP
        cairo_destroy(cr);
        cairo_surface_write_to_png(surface, "test0.png");
        cairo_surface_destroy(surface);
        return 0;
}
#endif
