

// sets the square matrix mat to the identity matrix,
// size refers to the number of rows (or columns)
void a_setIdentityMatrix( float *mat);
void a_multMatrix(float *a, float *b);
void a_multVector(float *v, float *m);
void a_normalizeTo3d(float *v);
void a_doTranslationMatrix(float x, float y, float z, float* m);

void a_doRotationMatrixY(float degree, float *m);
void a_doRotationMatrixX(float degree, float *m);

void a_transpose(float *m);
