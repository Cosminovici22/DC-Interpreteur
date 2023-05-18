#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    FILE *in = fopen(argv[1], "rt");
    int n = strlen(argv[1]);
    char fisc[n];
    strcpy(fisc, argv[1]);
    strcat(fisc, ".c");
    FILE *out = fopen(fisc, "wt");
    char instructiune[80];
    int m = strlen(instructiune) - 1;

    fprintf(out, "int main() {\n");
    while (fgets(instructiune, 80, in)) {
        //fprintf(out, "#include <stdio.h>\n\n");
        
        instructiune[strlen(instructiune) - 1] = '\0';
        fprintf(out, "%s;\n", instructiune);

        char *p = strtok(instructiune, " ");

        while(p) {
            
            if (p[strlen(p)-1] == '\n') {
                strcpy(p + strlen(p) - 1, "\0");
            }
            if (strcmp(p, "if") == 0) {
                fprintf(out, "if");
            } else if (strcmp(p, "for") == 0) {
                fprintf(out, "for");
            } else if (strcmp(p, "while") == 0) {
                fprintf(out, "while");
            }
            fprintf(out, "(%s)\n", p);
            
            p = strtok(NULL, " ");
        }
        //fprintf(out, "printf(\"egale\");\n");
        //system("make iupi.c");
    }
    fprintf(out, "    return 0;\n}");

    fclose(in);
    fclose(out);
    return 0;
}

