??=include "util/asmatUtil.h"
??=include "util/matUtil.c"

void dumpMatrix(float *m) {
        debug("\n%5.3f %5.3f %5.3f %5.3f\n%5.3f %5.3f %5.3f %5.3f\n%5.3f %5.3f %5.3f %5.3f\n%5.3f %5.3f %5.3f %5.3f\n",
        m[0], m[1], m[2], m[3], m[4], m[5], m[6], m[7], m[8], m[9], m[10],
        m[11], m[12], m[13], m[14], m[15]
        );
}

int cmp_x(float* lhs, float* rhs, size_t count) {
        for(size_t i = 0; i < count; i++)
                if(lhs[i] != rhs[i])
                        return i;
        return 0;
}

int cmp_map(float* lhs, float* rhs) {
        return cmp_x(lhs, rhs, 16);
}

int cmp_vec(float* lhs, float* rhs) {
        return cmp_x(lhs, rhs, 4);
}

int main() {
        float chs[16] = {3.0};
        float ahs[16] = {2.0};


        check1(1, "sanity checking");

        a_setIdentityMatrix(ahs);
        setIdentityMatrix(chs, 4);
        check1(!cmp_map(ahs, chs), "setIdentityMatrix");

        doTranslationMatrix(1.f, 2.f, 3.f, chs);
        a_doTranslationMatrix(1.f, 2.f, 3.f, ahs);
        check1(!cmp_map(ahs, chs), "doTranslationMatrix");

        float chv[4] = {8.f, 4.f, 2.f, 2.f};
        float ahv[4] = {8.f, 4.f, 2.f, 2.f};
        
        normalizeTo3d(chv);
        a_normalizeTo3d(ahv);
        check1(!cmp_vec(ahv, chv), "normalizeTo3d");

        log_info1("done");
        return 0;
error:
        dumpMatrix(chs);
        dumpMatrix(ahs);
        log_info1("done with errors");
        return -1;
}
