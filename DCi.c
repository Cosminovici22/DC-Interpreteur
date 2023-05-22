#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    char type[5];
    char name[20];
} TVariables;

void trimLeft(char *instr) {
    int i = 0;
    while (instr[i] == ' ' || instr[i] == '\t') {
        i++;
    }
    strcpy(instr, instr + i);
}

void parseOperation(char *operation, FILE *out) {
    int ok = 1;
    for (int i = 0; operation[i]; i++) {
        if (operation[i] >= 'a' && operation[i] <= 'z' || operation[i] >= 'A' && operation[i] <= 'Z' && ok == 1) {
            fprintf(out, "*");
            ok = 0;
        }
        if (strchr(" +-*/%", operation[i])) {
            ok = 1;
        }
        fprintf(out, "%c", operation[i]);
    }
}

int main(int argc, char *argv[]) {
    FILE *in = fopen(argv[1], "rt");
    strcat(argv[1], ".c");
    FILE *out = fopen(argv[1], "wt");

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
            fprintf(out, "}");
            if(structVar[0] != '\0') {
                fprintf(out, " %s;\n", structVar);
                structVar[0] = '\0';
            }
            fprintf(out, "\n");
        } else if (strcmp(p, "struct") == 0) {
            p = strtok(NULL, " ");
            fprintf(out, "typedef struct %s {\n", p);
            strcpy(structVar, p);
        } else if (strchr(instr, '=')) {
            strcpy(aux, instr);
            char *q = strtok(aux, " =");
            q = strtok(NULL, " =(");
            bool ok = 0;
            for (int i = 0; i < varCount; i++) {
                if (strcmp(p, variables[i].name) == 0) {
                    ok = 1;
                    break;
                }
            }
            if (ok == 0) {
                strcpy(variables[varCount].name, p);
                if (strchr(q, '.')) {
                    strcpy(variables[varCount].type, "float");
                } else if (q[0] >= '0' && q[0] <= '9') {
                    strcpy(variables[varCount].type, "int");
                } else if (q[0] == 39) {
                    strcpy(variables[varCount].type, "char");
                }
                fprintf(out, "%s *%s = ", variables[varCount].type, p);
                fprintf(out, "malloc(sizeof(%s));\n", variables[varCount].type);
            }
            fprintf(out, "*%s = ", p);
            char *tmp = strstr(instr, q);
            //printf("%s ", tmp);
            ok = 0;
            for (int i = 0; i < varCount; i++) {
                if (strstr(tmp, variables[i].name) != 0 && strcmp(variables[i].type, "func") != 0) {
                    fprintf(out, "%s;\n", tmp);
                    ok = 1;
                    break;
                }
            }
            if (ok == 0) {
                ///printf("%s ", variables[i].type);
                //fprintf(out, "%s;\n", tmp);
                parseOperation(tmp, out);
                fprintf(out, ";\n");
            }
            //fprintf(out, ";\n");
            varCount++;
        } else if (strcmp(p, "function") == 0) {
            //paramCount = 0;
            fprintf(out, "void *");
            p = strtok(NULL, "()");
            strcpy(variables[varCount].name, p);
            strcpy(variables[varCount].type, "func");
            varCount++;
            fprintf(out, "%s (", p);
            p = strtok(NULL, "()");
            char *q = strtok(p, " ,");
            fprintf(out, "void *%s", p);
            while (q != NULL) {
                q = strtok(NULL, " ,");
                if (q != NULL) {
                    fprintf(out, " ,void *%s", q);
                }
            }
            fprintf(out, ") {\n");
            //printf("%s\n", p);
            /*fprintf(out, "void *%s(",p);
            if (paramCount == 0)
                p = strtok(NULL," )");
            paramCount++;
            p = strtok(NULL, ", ");
            if (paramCount == 1)
                fprintf(out,"void *%s", p);
            else {
                fprintf(out,", void *%s", p);
            }*/
        } else {
            fprintf(out, "%s;\n", instr);
        }
    }

    for (int i = 0; i < varCount; i++) {
        printf("\n%s %s", variables[i].name, variables[i].type);
    }

    fclose(in);
    fclose(out);
    return 0;
}
