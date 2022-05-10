#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

int main () {
    FILE* f = fopen("./test/predict4.txt", "rt");
    FILE* g = fopen("./test/result2.txt", "rt");
    int currentP, currentR;
    currentP = getc(f);
    currentR = getc(g);
    while (currentP != EOF && currentR != EOF) {
        printf("%c", currentP);
        if (currentP != currentR) {printf("Hai file khong giong nhau\n", currentP,currentR); return 0;}
        currentP = getc(f);
        currentR = getc(g);
    }  
    
    fclose(f);
    fclose(g);

    printf("Hai file giong nhau\n");

    return 0;
}