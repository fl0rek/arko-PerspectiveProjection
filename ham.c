#include <stdio.h>

int main(int argc, char** argv) {
        if(argc != 17)
                return -1;

        printf(
                " %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s \n ",
                argv[1],
                argv[5],
                argv[9],
                argv[13],
                argv[2],
                argv[6],
                argv[10],
                argv[14],
                argv[3],
                argv[7],
                argv[11],
                argv[15],
                argv[4],
                argv[8],
                argv[12],
                argv[16]
        );
}
