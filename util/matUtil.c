// copy from
// http://www.lighthouse3d.com/cg-topics/code-samples/opengl-3-3-glsl-1-5-sample/
// used mostly for testing assembly version

// ----------------------------------------------------
// MATRIX STUFF
//

#define _GNU_SOURCE
#include <math.h>
#include <string.h>
#include <fdebug.h>


// sets the square matrix mat to the identity matrix,
// size refers to the number of rows (or columns)
void setIdentityMatrix( float *mat, int size) {

	// fill matrix with 0s
	for (int i = 0; i < size * size; ++i)
			mat[i] = 0.0f;

	// fill diagonal with 1s
	for (int i = 0; i < size; ++i)
		mat[i + i * size] = 1.0f;
}

//
// a = a * b;
//
void multMatrix(float *a, float *b) {

	float res[16];

	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			res[j*4 + i] = 0.0f;
			for (int k = 0; k < 4; ++k) {
				res[j*4 + i] += a[k + i*4] * b[j + k*4];
			}
		}
	}
	memcpy(a, res, 16 * sizeof(float));
}

void multVector(float *v, float *m) {
	float res[4] = {0};

	for(int i = 0; i < 4; ++i) {
		for(int j = 0; j < 4; ++j) {
			debug("+= %f * %f", v[j], m[i*4 + j]);
			res[i] += v[j] * m[i*4 + j];
		}
		debug("");
	}
//        debug("r: %f, %f, %f, %f", res[0], res[1], res[2], res[3]);
	memcpy(v, res, 4*sizeof(*v));
}

void normalizeTo3d(float *v) {
        debug("p: %f, %f, %f, %f", v[0], v[1], v[2], v[3]);
	v[0] = v[0]/v[3];
	v[1] = v[1]/v[3];
	v[2] = v[2]/v[3];
	v[3] = 1;

        debug("n: %f, %f, %f, %f", v[0], v[1], v[2], v[3]);
}

void doTranslationMatrix(float x, float y, float z, float* m) {
	memset(m, 0, 16);
	m[0] = 1;
	m[5] = 1;
	m[10] = 1;
	m[15] = 1;

	m[3] = x;
	m[7] = y;
	m[11] = z;
}

void doRotationMatrix1(float degree, float *m) {
	memset(m, 0, 16);

	m[0] = cos(degree);
	m[2] = sin(degree);
	m[5] = 1;
	m[8] = -sin(degree);
	m[10] = cos(degree);
	m[15] = 1;
}

float viewMatrix[16];
float projMatrix[16];

void buildProjectionMatrix(float fov, float ratio, float nearP, float farP) {

	float f = 1.0f / tan (fov * (M_PI / 360.0));

	setIdentityMatrix(projMatrix,4);

	projMatrix[0] = f / ratio;
	projMatrix[1 * 4 + 1] = f;
	projMatrix[2 * 4 + 2] = (farP + nearP) / (nearP - farP);
	projMatrix[3 * 4 + 2] = (2.0f * farP * nearP) / (nearP - farP);
	projMatrix[2 * 4 + 3] = -1.0f;
	projMatrix[3 * 4 + 3] = 0.0f;
}

// ----------------------------------------------------
// View Matrix
//
// note: it assumes the camera is not tilted,
// i.e. a vertical up vector (remmeber gluLookAt?)
//

#if 0
void setCamera(float posX, float posY, float posZ,
			   float lookAtX, float lookAtY, float lookAtZ) {

	float dir[3], right[3], up[3];

	up[0] = 0.0f;	up[1] = 1.0f;	up[2] = 0.0f;

	dir[0] =  (lookAtX - posX);
	dir[1] =  (lookAtY - posY);
	dir[2] =  (lookAtZ - posZ);
	normalize(dir);

	crossProduct(dir,up,right);
	normalize(right);

	crossProduct(right,dir,up);
	normalize(up);

	float aux[16];

	viewMatrix[0]  = right[0];
	viewMatrix[4]  = right[1];
	viewMatrix[8]  = right[2];
	viewMatrix[12] = 0.0f;

	viewMatrix[1]  = up[0];
	viewMatrix[5]  = up[1];
	viewMatrix[9]  = up[2];
	viewMatrix[13] = 0.0f;

	viewMatrix[2]  = -dir[0];
	viewMatrix[6]  = -dir[1];
	viewMatrix[10] = -dir[2];
	viewMatrix[14] =  0.0f;

	viewMatrix[3]  = 0.0f;
	viewMatrix[7]  = 0.0f;
	viewMatrix[11] = 0.0f;
	viewMatrix[15] = 1.0f;

	setTranslationMatrix(aux, -posX, -posY, -posZ);

	multMatrix(viewMatrix, aux);
}

#endif
