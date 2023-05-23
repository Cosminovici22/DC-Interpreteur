#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    char type[10];
    char name[20];
} TVariables;

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
    //fpos_t posGlob;
    //fgetpos(out, &posGlob);

    char matchVar[20], structVar[20] = "\0";
    TVariables *variables = (TVariables *)malloc(200*sizeof(TVariables));
    int varCount = 0;
    int paramCount = 0;

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
            if (strchr(p, ',')) {
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
            if(structVar[0] != '\0') {
                fprintf(out, " %s;\n", structVar);
                structVar[0] = '\0';
            }
            fprintf(out, "\n");
        } else if (strcmp(p, "struct") == 0) {
            p = strtok(NULL, " ");
            fprintf(out, "typedef struct %s {\n", p);
            strcpy(structVar, p);
        } else if (strcmp(p, "function") == 0) {
            p = strtok(NULL, " ");
            fprintf(out, "void %s() {\n", p);
        } else if (strchr(instr, '=')) {
            strcpy(aux, instr);
            char *q = strtok(aux, " =");
            q = strtok(NULL, " =");
            bool ok = 0, arr = 0;
            p = strtok(p, "[");
            
            for (int i = 0; i < varCount; i++) {
                if (strcmp(p, variables[i].name) == 0) {
                    ok = 1;
                    break;
                }
            }
            if (ok == 0) {
                //fpos_t posCur;
                //fgetpos(out, &posCur);
                //fsetpos(out, &posGlob);
                //q = strstr(q, "><");
                printf("%s\n", q);
                char *s;
                strcpy(variables[varCount].name, p);
                if (strchr(q, '.')) {
                    strcpy(variables[varCount].type, "float");
                    fprintf(out, "%s ", variables[varCount].type);
                } else if (strstr(q, "><") && !arr) {
                    strcpy(variables[varCount].type, "float");
                    char *r = strtok(q, "< >");
                    fprintf(out, "%s %s[%s][%s];\n", variables[varCount].type, p, r, strtok(NULL, "< >"));
                    arr = 1;
                } else if (q[0] >= '0' && q[0] <= '9') {
                    strcpy(variables[varCount].type, "int");
                    fprintf(out, "%s ", variables[varCount].type);
                } else if (q[0] == 39) {
                    strcpy(variables[varCount].type, "char");
                    fprintf(out, "%s ", variables[varCount].type);
                } else if (strstr(q, "}{") && !arr) {
                    printf("bau");
                    strcpy(variables[varCount].type, "int");
                    char *r = strtok(q, "{ }");
                    fprintf(out, "%s %s[%s][%s];\n", variables[varCount].type, p, r, strtok(NULL, "{ }"));
                    arr = 1;
                } else if (strstr(q, """") && !arr) {
                    strcpy(variables[varCount].type, "char");
                    char *r = strtok(q, " \"");
                    fprintf(out, "%s %s[%s][%s];\n", variables[varCount].type, p, r, strtok(NULL, " \""));
                    arr = 1;
                } else if (q[0] == '{' && !arr) {
                    strcpy(variables[varCount].type, "int");
                    fprintf(out, "%s %s[%s];\n", variables[varCount].type, p, strtok(q, "{ }"));
                    arr = 1;
                } else if (q[0] == '"' && !arr) {
                    strcpy(variables[varCount].type, "char");
                    fprintf(out, "%s %s[%s];\n", variables[varCount].type, p, strtok(q, " \""));
                    arr = 1;
                } else if (q[0] == '<' && !arr) {
                    strcpy(variables[varCount].type, "float");
                    fprintf(out, "%s %s[%s];\n", variables[varCount].type, p, strtok(q, "< >"));
                    arr = 1;
                }
                if(!arr) {
                    fprintf(out, "%s = %s;\n", p, strstr(instr, q));
                }
                //fprintf(out, "\n");
                varCount++;
                //fgetpos(out, &posGlob);
                //fsetpos(out, &posCur);
            } else {
                fprintf(out, "%s;\n", instr);
            }
        } else {
            fprintf(out, "%s;\n", instr);
        }
    }
    
    /*for (int i = 0; i < varCount; i++) {
        printf("%s %s\n", variables[i].name, variables[i].type);
    }*/

    fclose(in);
    fclose(out);
    return 0;
}
