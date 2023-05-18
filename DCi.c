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

    fprintf(out, "#include <stdio.h>\n\n");
    fprintf(out, "int main() {\n");
    char matchVar[20];
    while (fgets(instructiune, 80, in)) {
        //fprintf(out, "#include <stdio.h>\n\n");
        if(strcmp(instructiune, "\n")) {
            instructiune[strlen(instructiune) - 1] = '\0';
            char aux[80];
            strcpy(aux, instructiune);

            char *p = strtok(aux, " ");
            if (strcmp(p, "if") == 0) {
                fprintf(out, "if(%s) {\n", instructiune + strlen(p) + 1);
            } else if (strcmp(p, "for") == 0) {
                fprintf(out, "for(%s) {\n", instructiune + strlen(p) + 1);
                /*fprintf(out, "for(");
                char *tmp = strstr(instructiune, " to ");
                for (int i = 4; i < tmp - instructiune; i++) {
                    fprintf(out, "%c", instructiune[i]);
                }*/
            } else if (strcmp(p, "match") == 0) {
                strcpy(matchVar, instructiune + strlen(p) + 1);
                //fprintf(out, "%s", instructiune + strlen(p) + 1);
                
            } else if (strcmp(p, "case") == 0) {
                p = strtok(NULL, " :");
                if(strchr(p, ',')){
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
                fprintf(out, "while(%s) {\n", instructiune + strlen(p) + 1);
            } else if (strcmp(p, "end") == 0) {
                fprintf(out, "}\n");
            } else {
                fprintf(out, "%s;\n", instructiune);
            }
            //system("make iupi.c");
        }
    }
    fprintf(out, "    return 0;\n}");
    //system("make iupi");
    //system("chmod +x ./iupi");
    //system("./iupi");

    fclose(in);
    fclose(out);
    return 0;
}
