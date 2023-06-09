#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#define prod(p) do { fprintf(out, "%s(%s) {\n", p, instr + strlen(p) + 1); } while(0)

typedef struct {
    char type[10];
    char name[20];
} TVariables;

void trimLeft(char *instr) {
    int i = 0;
    while (instr[i] == ' ' || instr[i] == '\t') {
        i++;
    }
    int len = strlen(instr);
    memmove(instr, instr + i, len); 
}

int main(int argc, char *argv[]) {
    FILE *in = fopen(argv[1], "rt");
    FILE *out = fopen("/tmp/DCic.c", "wt");
    FILE *vout = fopen("/tmp/DCih.h", "wt");

    char matchVar[20] = "\0", structVar[20] = "\0";
    TVariables *variables = (TVariables *)malloc(200*sizeof(TVariables));
    int varCount = 0;
    fprintf(out, "#include <stdio.h>\n");
    fprintf(out, "#include <stdlib.h>\n");
    fprintf(out, "#include <math.h>\n");
    fprintf(out, "#include \"DCih.h\"\n\n");

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
        if (strcmp(p, "if") == 0 || strcmp(p, "for") == 0 || strcmp(p, "while") == 0) {
            prod(p);
        } else if (strcmp(p, "else") == 0) {
            fprintf(out, "} else {\n");
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
        } else if (strcmp(p, "end") == 0) {
            fprintf(out, "}\n");
            if(structVar[0] != '\0') {
                fprintf(out, " %s;\n", structVar);
                structVar[0] = '\0';
            }
        } else if (strcmp(p, "struct") == 0) {
            p = strtok(NULL, " ");
            fprintf(out, "typedef struct %s {\n", p);
            strcpy(structVar, p);
        } else if (strcmp(p, "function") == 0) {
            p = strtok(NULL, " ");
            fprintf(out, "void %s() {\n", p);
        } else if (strcmp(p, "call") == 0) {
            fprintf(out, "%s();\n", instr + strlen(p) + 1);
        } else if (strchr(instr, '=')) {
            FILE *auxFile = vout;
            strcpy(aux, instr);
            char *q = strtok(aux, " =");
            q = strtok(NULL, " =");
            bool ok = 0, arr = 0;
            
            for (int i = 0; i < varCount; i++) {
                if (strcmp(p, variables[i].name) == 0) {
                    ok = 1;
                    auxFile = out;
                    break;
                }
            }

            if (strchr(p, '[') != NULL) {
                ok = 1;
            }

            if (ok == 0) {
                char *s;
                strcpy(variables[varCount].name, p);
                if (q[0] == 39) {
                    strcpy(variables[varCount].type, "char");
                    fprintf(auxFile, "%s ", variables[varCount].type);
                } else if (strchr(q, '.')) {
                    strcpy(variables[varCount].type, "float");
                    fprintf(auxFile, "%s ", variables[varCount].type);
                } else if (q[0] >= '0' && q[0] <= '9') {
                    strcpy(variables[varCount].type, "int");
                    fprintf(auxFile, "%s ", variables[varCount].type);
                } else if (q[0] == '{' && !arr) {
                    strcpy(variables[varCount].type, "int");
                    if (strchr(q + 1, '{') != NULL) {
                        char *r = strtok(q, "{ }");
                        fprintf(auxFile, "%s %s[%s][%s];\n", variables[varCount].type, p, r, strtok(NULL, "{ }"));
                    } else {
                        fprintf(auxFile, "%s %s[%s];\n", variables[varCount].type, p, strtok(q, "{ }"));
                    }
                    arr = 1;
                } else if (q[0] == '<' && !arr) {
                    strcpy(variables[varCount].type, "float");
                    if (strchr(q + 1, '<') != NULL) {
                        char *r = strtok(q, "< >");
                        fprintf(auxFile, "%s %s[%s][%s];\n", variables[varCount].type, p, r, strtok(NULL, "< >"));
                    } else {
                        fprintf(auxFile, "%s %s[%s];\n", variables[varCount].type, p, strtok(q, "< >"));
                    }
                    arr = 1;
                } else if (q[0] == '"' && !arr) {
                    strcpy(variables[varCount].type, "char");
                    if (strchr(strchr(q + 1, '"') + 1, '"') != NULL) {
                        char *r = strtok(q, "\"");
                        fprintf(auxFile, "%s %s[%s][%s];\n", variables[varCount].type, p, r, strtok(NULL, "\""));
                    } else {
                        fprintf(auxFile, "%s %s[%s];\n", variables[varCount].type, p, strtok(q, "\""));
                    }
                    arr = 1;
                }
                if(!arr) {
                    fprintf(auxFile, "%s = %s;\n", p, strstr(instr, q));
                }
                varCount++;
            } else {
                fprintf(out, "%s;\n", instr);
            }
        } else {
            fprintf(out, "%s;\n", instr);
        }
    }
    free(variables);

    fclose(in);
    fclose(out);
    fclose(vout);
    return 0;
}
