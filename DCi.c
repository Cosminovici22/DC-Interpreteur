#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void trimLeft(char *instr) {
    int i = 0;
    while (instr[i] == ' ' || instr[i] == '\t') {
        i++;
    }
    strcpy(instr, instr + i);
}

int main(int argc, char *argv[]) {
    FILE *in = fopen(argv[1], "rt");
    strcat(argv[1], ".c");
    FILE *out = fopen(argv[1], "wt");

    char matchVar[20];

    char instr[80];
    while (fgets(instr, 80, in)) {
        trimLeft(instr);
        if (strcmp(instr, "\n") == 0) {
            continue;
        }
        if (instr[strlen(instr) - 1] == '\n') {
            instr[strlen(instr) - 1] = '\0';
        }
        char aux[80];
        strcpy(aux, instr);

        char *p = strtok(aux, " ");
        if (strcmp(p, "if") == 0) {
            fprintf(out, "if(%s) {\n", instr + strlen(p) + 1);
        } else if (strcmp(p, "for") == 0) {
            fprintf(out, "for(%s) {\n", instr + strlen(p) + 1);
        } else if (strcmp(p, "match") == 0) {
            strcpy(matchVar, instr + strlen(p) + 1);
        } else if (strcmp(p, "case") == 0) {
            p = strtok(NULL, " :");
            if(strchr(p, ',')) {
                char str[20];
                for (int i = 0; p[i] != ','; i++) {
                    str[i] = p[i];
                }
                fprintf(out, "if (%s >= %s && ", matchVar, str);
                strcpy(str , strchr(p, ','));
                fprintf(out, "%s <= %s) {\n", matchVar, str+1);
            } else {
                fprintf(out, "if (%s == ", matchVar);
                fprintf(out, "%s) {\n", p);
            }
        } else if (strcmp(p, "while") == 0) {
            fprintf(out, "while(%s) {\n", instr + strlen(p) + 1);
        } else if (strcmp(p, "end") == 0) {
            fprintf(out, "}\n");
        } else {
            fprintf(out, "%s;\n", instr);
        }
    }

    fclose(in);
    fclose(out);
    return 0;
}
