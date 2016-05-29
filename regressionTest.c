??=include "util/asmatUtil.h"
??=include "util/matUtil.c"

void dumpMatrix(float *m) {
        debug("\n%5.3f %5.3f %5.3f %5.3f\n%5.3f %5.3f %5.3f %5.3f\n%5.3f %5.3f %5.3f %5.3f\n%5.3f %5.3f %5.3f %5.3f\n",
        m[0], m[1], m[2], m[3], m[4], m[5], m[6], m[7], m[8], m[9], m[10],
        m[11], m[12], m[13], m[14], m[15]
        );
}

void dumpVector(float *v1) {
        debug("\n%5.3f %5.3f %5.3f %5.3f\n",
                v1[0], v1[1], v1[2], v1[3]
        );

}

int cmp_x(float* lhs, float* rhs, size_t count) {
        for(size_t i = 0; i < count; i++) {
                if(lhs[i] != rhs[i]) {
                        debug("%5.3f == %5.3f\n", lhs[i], rhs[i]);
                        return i+1;
                }
        }
        return 0;
}

int cmp_map(float* lhs, float* rhs) {
        return cmp_x(lhs, rhs, 16);
}

int cmp_vec(float* lhs, float* rhs) {
        return cmp_x(lhs, rhs, 4);
}

int main() {
        float chs1[16], ahs1[16];

        float chv[4] = {8.f, 6.f, 4.f, 2.f};
        float ahv[4] = {8.f, 6.f, 4.f, 2.f};

        float chs[16] =  {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
        float ahs[16] =  {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
        transpose(chs);
        a_transpose(ahs);
        check1(!cmp_map(chs, ahs), "transpose2");
        transpose(chs);
        a_transpose(ahs);

        check1(!cmp_map(chs, ahs), "transpose3");


        a_transpose(chs);
        transpose(ahs);
        check1(!cmp_map(ahs, chs), "a_transpose");

        memset(chs, 'C', 16*sizeof(float));
        memset(ahs, 'C', 16*sizeof(float));


        check1(1, "sanity checking");

        a_setIdentityMatrix(ahs);
        setIdentityMatrix(chs, 4);
        check1(!cmp_map(ahs, chs), "setIdentityMatrix");

        transpose(ahs);
        check1(!cmp_map(ahs, chs), "transpose");


        setIdentityMatrix(chs,4);
        setIdentityMatrix(chs1,4);
        setIdentityMatrix(ahs,4);
        setIdentityMatrix(ahs1,4);


        multMatrix(chs, chs1);
        a_multMatrix(ahs, ahs1);
        check1(!cmp_map(ahs, chs), "multMatrix");

        doTranslationMatrix(3, 2, 1, chs);
        doTranslationMatrix(3, 2, 1, ahs);

        doRotationMatrixY(1.f, chs1);
        doRotationMatrixY(1.f, ahs1);

        multMatrix(chs, chs1);
        //a_transpose(ahs);
        //a_transpose(ahs1);
        a_multMatrix(ahs, ahs1);
        check1(!cmp_map(chs, ahs), "multMatrix2");

        doTranslationMatrix(1.f, 2.f, 3.f, chs);
        a_doTranslationMatrix(1.f, 2.f, 3.f, ahs);
        check1(!cmp_map(ahs, chs), "doTranslationMatrix");

        doRotationMatrixX(0.f, chs);
        a_doRotationMatrixX(0.f, ahs);
        check1(!cmp_map(ahs, chs), "doRotationMatrixX");

        doRotationMatrixX(2.f, chs);
        a_doRotationMatrixX(2.f, ahs);
        check1(!cmp_map(ahs, chs), "doRotationMatrixX");

        doRotationMatrixY(0.f, chs);
        a_doRotationMatrixY(0.f, ahs);
        check1(!cmp_map(ahs, chs), "doRotationMatrixY");

        doRotationMatrixY(2.f, chs);
        a_doRotationMatrixY(2.f, ahs);
        check1(!cmp_map(ahs, chs), "doRotationMatrixY");


        normalizeTo3d(chv);
        a_normalizeTo3d(ahv);
        check1(!cmp_vec(ahv, chv), "normalizeTo3d");

multi:
        multVector(chv, chs);
        a_transpose(ahs);
        a_multVector(ahv, ahs);
        check1(!cmp_vec(chv, ahv), "multVector");

        log_info1("done");
        return 0;
error:
        if(cmp_vec(chv, ahv)) {
                debug1("should be:");
                dumpVector(chv);
                debug1("is:");
                dumpVector(ahv);
        }

        if(cmp_map(chs, ahs)) {
                debug1("should be:");
                dumpMatrix(chs);
                debug1("is:");
                dumpMatrix(ahs);
        }
        log_info1("done with errors");
        return -1;
}
