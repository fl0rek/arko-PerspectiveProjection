#include "util/matUtil.c"
#include <cairo.h>

#include <fdebug.h>

float vertices[] = {
        -1.f, -.2f, -1.f,  1.f,
         1.f, -.2f, -1.f,  1.f,
         0.f, -.2f,  1.f,  1.f,
         0.f, .8f,  0.f,  1.f
};

float projectedVertices[16];

const float distance = 3;

float projectionMatrix[] = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, -.5f, 1 // -1/distance
};

int lines[] = {
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


void drawLine(cairo_t *cr, float *ufrom, float *uto) {
        cairo_set_source_rgb(cr, 100,0,0);
        cairo_set_line_width(cr, 0.5);

        debug("from\t:%f, %f", ufrom[0], ufrom[1]);
        debug("to\t:%f, %f", uto[0], uto[1]);

        int from[2];
        int to[2];

        fromUniformCoords(ufrom, from);
        fromUniformCoords(uto, to);

        cairo_move_to(cr, from[0], from[1]);
        cairo_line_to(cr, to[0], to[1]);

        cairo_stroke(cr);
}


void transformPoint(const float* origin, float* dest) {
        memcpy(dest, origin, 4*sizeof(float));

        multVector(dest, projectionMatrix);

        debug("o: %f, %f, %f, %f", origin[0], origin[1], origin[2], origin[3]);
        debug("d: %f, %f, %f, %f", dest[0], dest[1], dest[2], dest[3]);
}

int main(int argc, char const *argv[]) {
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
