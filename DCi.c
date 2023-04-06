#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#define pentru() { int s = 0; for (i = 1; i < 10; i++) s+=i; printf("%d", f); }

int main(int argc, char *argv[]) {
    FILE *in = fopen(argv[1], "rt");
    int n = strlen(argv[1]);
    char fisc[n];
    strcpy(fisc, argv[1]);
    strcat(fisc, ".c");
    FILE *out = fopen(fisc, "wt");
    char instructiune[80];
    int m = strlen(instructiune) - 1;
    char exceptii[2];
    //exceptii[1] = ' ';
    //exceptii[2] = '\n';
    //while (instructiune[m] != EOF) {
        fprintf(out, "#include <stdio.h>\n\n");
        fprintf(out, "int main() {\n\n");
        fgets(instructiune, 80, in);
    
        char *p = strtok(instructiune, " ");
        fprintf(out, "int a = 10, b = 10;\n");
        while(p) {
            
            if (p[strlen(p)-1] == '\n') {
                strcpy(p + strlen(p) - 1, "\0");
            }
            if (strcmp(p, "daca") == 0) {
                fprintf(out, "if");
            } else {
                fprintf(out, "(%s)\n", p);
            }
            
            p = strtok(NULL, " ");
        }
        fprintf(out, "printf(\"egale\");\n");
        // fprintf(out, "%s", intrare);
        fprintf(out, "return 0;\n}");
        //char u = ""
        //system("make iupi.c");
    //}

    fclose(in);
    fclose(out);
    return 0;
}
