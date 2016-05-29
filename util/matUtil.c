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
			res[j*4 + i] = .0f;
			for (int k = 0; k < 4; ++k) {
				res[j*4 + i] += a[k + j*4] * b[i + k*4];
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
		debug1("");
	}
//        debug("r: %f, %f, %f, %f", res[0], res[1], res[2], res[3]);
	memcpy(v, res, 4*sizeof(*v));
}

void normalizeTo3d(float *v) {
	v[0] = v[0]/v[3];
	v[1] = v[1]/v[3];
	v[2] = v[2]/v[3];
	v[3] = 1;
}
void dumpMatrix(float *m);
void doTranslationMatrix(float x, float y, float z, float* m) {
	memset(m, 0, 16 *sizeof(*m));

	setIdentityMatrix(m, 4);

	m[3] = x;
	m[7] = y;
	m[11] = z;
}

void doRotationMatrixY(float degree, float *m) {
	memset(m, 0, 16 *sizeof(*m));

	setIdentityMatrix(m, 4);

	m[0] = cos(degree);
	m[2] = sin(degree);
	m[8] = -sin(degree);
	m[10] = cos(degree);
}

void doRotationMatrixX(float degree, float *m) {
	memset(m, 0, 16 *sizeof(*m));

	setIdentityMatrix(m, 4);

	m[5] = cos(degree);
	m[6] = -sin(degree);
	m[9] = sin(degree);
	m[10] = cos(degree);
}

float viewMatrix[16];
static float projMatrix[16];

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

void transpose(float *m) {
	float tmp[16];
	tmp[0]  = m[0];  // argv[1],
	tmp[1]  = m[4];  // argv[5],
	tmp[2]  = m[8];  // argv[9],
	tmp[3]  = m[12]; // argv[13],
	tmp[4]  = m[1];  // argv[2],
	tmp[5]  = m[5];  // argv[6],
	tmp[6]  = m[9];  // argv[10],
	tmp[7]  = m[13]; // argv[14],
	tmp[8]  = m[2];  // argv[3],
	tmp[9]  = m[6];  // argv[7],
	tmp[10] = m[10]; // argv[11],
	tmp[11] = m[14]; // argv[15],
	tmp[12] = m[3];  // argv[4],
	tmp[13] = m[7];  // argv[8],
	tmp[14] = m[11]; // argv[12],
	tmp[15] = m[15]; // argv[16]

	memcpy(m, tmp, 16*sizeof(float));
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
