/*
 * Copyright (c) 2005 Gregor Richards
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdio.h>
#include <string.h>

void implClass(char *line, FILE *pinp);
void declClass(char *line, FILE *pinp);
void implMain(char *line, FILE *pinp);
void implFunc(char *line, FILE *pinp);
void implGenFunc(char *cname, char *fname, FILE *pinp);
void removeOwnership(char *from);
void gr_tok(char *line, char delim, char **linee, int lec);
int findElem(char **les, int from, char *find);
void concatUntil(char **les, int from, char *to, int *tonum);
void concatAll(char **les, int from);
char *orkVarToC(char *ovar);
int getLine(char *line, FILE *pinp);

int main(int argc, char **argv)
{
    FILE *pinp;
    char line[1024];
    
    if (argc <= 1) {
        fprintf(stderr, "Use: %s <program>\n", argv[0]);
        return 1;
    }
    
    line[1023] = '\0';
    
    pinp = fopen(argv[1], "r");
    if (!pinp) {
        perror(argv[1]);
        return 1;
    }
    
    printf("#include <iostream>\n#include <string>\nusing namespace std;\n");
    printf("#include \"libork.h\"\n");
    printf("#define arg_O arg.\n");
    
    while (!feof(pinp) && !ferror(pinp)) {
      if (!getLine(line, pinp))
	continue;
      
        if (!strncmp(line, "There is such a thing as a", 26)) {
	  declClass(line, pinp);
        }
    }
    
    rewind(pinp);

    while (!feof(pinp) && !ferror(pinp)) {
        if (!getLine(line, pinp))
            continue;
        
        if (!strncmp(line, "There is such a thing as a", 26)) {
            implClass(line, pinp);
        } else if (!strcmp(line, "When this program starts")) {
            implMain(line, pinp);
        } else if (!strncmp(line, "When a", 6)) {
            implFunc(line, pinp);
        }
    }
    
    return 0;
}

void implClass(char *line, FILE *pinp)
{
    /* There is such a thing as a[n] {class} */
    char *cname;
    char *lineelem[50];
    
    if (line[26] == 'n') {
        cname = line + 28;
    } else {
        cname = line + 27;
    }
    
    removeOwnership(cname);
    
    /* OK, we've got our class name, output it */
    printf("class %s ", cname);
    
    /* if our next line is "Every {class} is a {class},
     * we need ": {class}" */
    if (getLine(line, pinp)) {
        if (!strncmp(line, "Every ", 6)) {
            gr_tok(line, ' ', lineelem, 50);
            if (findElem(lineelem, 2, "is")) {
                int isloc;
                concatUntil(lineelem, 1, "is", &isloc);
                removeOwnership(lineelem[1]);
                
                concatAll(lineelem, isloc + 2);
                removeOwnership(lineelem[isloc + 2]);
                
                printf(": public %s ", lineelem[isloc + 2]);
            } else {
                concatAll(lineelem, 0);
            }
        }
    }
    
    printf("{\npublic:\n");

    if (line[0] == '\0') {
        printf("};\n");
        return;
    }
    
    /* we have one leftover line, so do first */
    do {
        gr_tok(line, ' ', lineelem, 50);
        
        if (!strcmp(lineelem[0], "A") ||
            !strcmp(lineelem[0], "An")) {
            /* "A {class} {can/has} ... */
            
            if (findElem(lineelem, 2, "can")) {
                /* A {class} can {function} {variables} */
                int canloc;
                concatUntil(lineelem, 1, "can", &canloc);
                
                /* Or it could be A {class} can have [a[n]] {variable} which is a[n] {type} */
                if (!strcmp(lineelem[canloc + 1], "have")) {
                    int next = canloc + 2;
                    char *cvar;
                    if (!strcmp(lineelem[next], "a") ||
                        !strcmp(lineelem[next], "an")) {
                        next++;
                    }
                
                    int whichloc;
                    concatUntil(lineelem, next, "which", &whichloc);
                    removeOwnership(lineelem[next]);
                
                    concatAll(lineelem, whichloc + 3);
                    removeOwnership(lineelem[whichloc + 3]);
                    
                    cvar = orkVarToC(lineelem[whichloc + 3]);
                    printf("%s *%s;\n"
                           "#undef %s_P\n"
                           "#define %s_P %s\n"
                           "#undef %s_O\n"
                           "#define %s_O %s->\n", cvar, lineelem[next],
                           lineelem[next], lineelem[next], lineelem[next],
                           lineelem[next], lineelem[next], lineelem[next]);
                }
                
                /* or it could be A {class} can say {blah} */
                else if (!strcmp(lineelem[canloc + 1], "say")) {
                    /* make an int verbatum */
                    concatAll(lineelem, canloc + 2);
                    removeOwnership(lineelem[canloc + 2]);
                    printf("int %s;\n", lineelem[canloc + 2]);
                } else {
                    int aloc = 0;
                    if (findElem(lineelem, canloc + 2, "a")) {
                        /* A {class} can {function} a {type} */
                        concatUntil(lineelem, canloc + 1, "a", &aloc);
                        removeOwnership(lineelem[canloc + 1]);
                    }
                    if (findElem(lineelem, canloc + 1, "an")) {
                        /* A {class} can {function} an {type} */
                        concatUntil(lineelem, canloc + 1, "an", &aloc);
                        removeOwnership(lineelem[canloc + 1]);
                    }
                
                    if (aloc) {
                        /* fix our type and we're done */
                        concatAll(lineelem, aloc + 1);
                        removeOwnership(lineelem[aloc + 1]);
                    }
                
                    /* output */
                    if (aloc) {
                        char *cvar;
                        cvar = orkVarToC(lineelem[aloc + 1]);
                        if (!strcmp(cvar, "orknumber")) {
                            printf("void ORK_%s(double arg);\n", lineelem[canloc + 1]);
                        }
                        printf("void ORK_%s(%s &arg);\n", lineelem[canloc + 1], cvar);
                    } else {
                        printf("void ORK_%s();\n", lineelem[canloc + 1]);
                    }
                }
            }
            
            if (findElem(lineelem, 2, "has")) {
                /* A {class} has [a[n]] {variable} which is a[n] {type} */
                char *cvar;
                int hasloc;
                concatUntil(lineelem, 1, "has", &hasloc);
                
                int next = hasloc + 1;
                if (!strcmp(lineelem[next], "a") ||
                    !strcmp(lineelem[next], "an")) {
                    next++;
                }
                
                int whichloc;
                concatUntil(lineelem, next, "which", &whichloc);
                removeOwnership(lineelem[next]);
                
                concatAll(lineelem, whichloc + 3);
                removeOwnership(lineelem[whichloc + 3]);
                
                cvar = orkVarToC(lineelem[whichloc + 3]);
                printf("%s %s;\n"
                       "#undef %s_P\n"
                       "#define %s_P (&%s)\n"
                       "#undef %s_O\n"
                       "#define %s_O %s.\n", cvar, lineelem[next],
                       lineelem[next], lineelem[next], lineelem[next], 
                       lineelem[next], lineelem[next], lineelem[next]);
            }
        }
    } while (!feof(pinp) &&
             !ferror(pinp) &&
             getLine(line, pinp) &&
             line[0] != '\0');
    
    /* done with that class! */
    printf("};\n");
}

void declClass(char *line, FILE *pinp)
{
    /* There is such a thing as a[n] {class} */
    char *cname;
    
    if (line[26] == 'n') {
        cname = line + 28;
    } else {
        cname = line + 27;
    }
    
    removeOwnership(cname);
    
    /* The class name is found, forward reference */
    printf("class %s;\n", cname);
}

void implMain(char *line, FILE *pinp)
{
    printf("int main() {\nfunc_main:\n");
    implGenFunc("main", "main", pinp);
    printf("}\n");
}

void implFunc(char *line, FILE *pinp)
{
    char *lineelem[50];
    
    gr_tok(line, ' ', lineelem, 50);
    
    int isloc;
    concatUntil(lineelem, 2, "is", &isloc);
    removeOwnership(lineelem[2]);
    
    int aloc = 0;
    if (findElem(lineelem, isloc + 3, "a")) {
        concatUntil(lineelem, isloc + 2, "a", &aloc);
    } else if (findElem(lineelem, isloc + 3, "an")) {
        concatUntil(lineelem, isloc + 2, "an", &aloc);
    } else {
        concatAll(lineelem, isloc + 2);
    }
    removeOwnership(lineelem[isloc + 2]);
    
    if (aloc) {
        concatAll(lineelem, aloc + 1);
        removeOwnership(lineelem[aloc + 1]);
        char *cvar = orkVarToC(lineelem[aloc + 1]);
        if (!strcmp(cvar, "orknumber")) {
            printf("DTON(%s, ORK_%s)\n", lineelem[2], lineelem[isloc + 2]);
        }
        printf("void %s::ORK_%s (%s &arg)\n{\n", lineelem[2], lineelem[isloc + 2], cvar);
    } else {
        printf("void %s::ORK_%s ()\n{\n", lineelem[2], lineelem[isloc + 2]);
    }
    
    printf("func_%s_%s:\n", lineelem[2], lineelem[isloc + 2]);
    
    implGenFunc(lineelem[2], lineelem[isloc + 2], pinp);
    
    printf("}\n");
}

void implGenFunc(char *cname, char *fname, FILE *pinp)
{
    char line[1024];
    char *lineelem[50];
    line[1023] = '\0';
    
    while (!feof(pinp) && !ferror(pinp)) {
        if (!getLine(line, pinp))
            continue;
        
redoloop:
        
        if (line[0] == '\0') return;
        
        gr_tok(line, ' ', lineelem, 50);
        
        if ((!strcmp(lineelem[0], "I") &&
            !strcmp(lineelem[1], "have") &&
            lineelem[2][0] == 'a') ||
            (!strcmp(lineelem[0], "There") &&
             !strcmp(lineelem[1], "is") &&
             lineelem[2][0] == 'a')) {
            int calledloc;
            concatUntil(lineelem, 3, "called", &calledloc);
            removeOwnership(lineelem[3]);
            
            concatAll(lineelem, calledloc + 1);
            removeOwnership(lineelem[calledloc + 1]);
            
            printf("%s %s;\n"
                   "#undef %s_P\n"
                   "#define %s_P (&%s)\n"
                   "#undef %s_O\n"
                   "#define %s_O %s.\n", orkVarToC(lineelem[3]), lineelem[calledloc + 1],
                   lineelem[calledloc + 1], lineelem[calledloc + 1], lineelem[calledloc + 1],
                   lineelem[calledloc + 1], lineelem[calledloc + 1], lineelem[calledloc + 1]);
        } else if (!strcmp(lineelem[0], "If")) {
            int thenloc;
            concatUntil(lineelem, 1, "then", &thenloc);
            removeOwnership(lineelem[1]);
            
            printf("if (%s) ", lineelem[1]);
            
            /* then do this whole loop for the part after "if" */
            concatAll(lineelem, thenloc + 1);
            memmove(line, lineelem[thenloc + 1], strlen(lineelem[thenloc + 1]) + 1);
            goto redoloop;
        } else if (!strcmp(lineelem[0], "I") &&
                   !strcmp(lineelem[1], "am") &&
                   !strcmp(lineelem[2], "to")) {
            /* call a function of this object */
            /* FIXME:
             * this logic is wrong, but the grammar is confusing, since it's:
             * I am to {function} {variable}
             */
            int isloc = 1;
            if (lineelem[isloc + 3]) {
                concatAll(lineelem, isloc + 3);
                removeOwnership(lineelem[isloc + 3]);
                if (!strcmp(lineelem[isloc + 2], "say")) {
                    /* set a variable to 1 */
                    printf("%s = 1;\n", lineelem[isloc + 3]);
                } else {
                    printf("ORK_%s(%s);\n", lineelem[isloc + 2], lineelem[isloc + 3]);
                }
            } else {
                /* check for specials */
                if (!strcmp(lineelem[isloc + 2], "loop")) {
                    printf("goto func_%s_%s;\n", cname, fname);
                } else if (!strcmp(lineelem[isloc + 2], "quit")) {
                    printf("exit(0);\n");
                } else {
                    printf("ORK_%s();\n", lineelem[isloc + 2]);
                }
            }
        } else if (!strcmp(lineelem[0], "I") &&
                   !strcmp(lineelem[1], "am") &&
                   !strcmp(lineelem[2], "not") &&
                   !strcmp(lineelem[3], "to") &&
                   !strcmp(lineelem[4], "say")) {
            /* set a variable to 0 */
            concatAll(lineelem, 5);
            removeOwnership(lineelem[5]);
            printf("%s = 0;\n", lineelem[5]);
        } else if (!strcmp(lineelem[0], "I") &&
                   !strcmp(lineelem[1], "have")) {
            /* I have [a[n]] {variable} which is a {type}.
             * this becomes  variable = new type; */
            char *cvar;
                
            int next = 2;
            if (!strcmp(lineelem[next], "a") ||
                !strcmp(lineelem[next], "an")) {
                next++;
            }
                
            int whichloc;
            concatUntil(lineelem, next, "which", &whichloc);
            removeOwnership(lineelem[next]);
                
            concatAll(lineelem, whichloc + 3);
            removeOwnership(lineelem[whichloc + 3]);
                
            cvar = orkVarToC(lineelem[whichloc + 3]);
            printf("%s = new %s();\n", lineelem[next], cvar);
        } else {
            /* {this} has [a[n] {that} which is a[n] {type} */
            if (findElem(lineelem, 1, "has")) {
                int hasloc;
                concatUntil(lineelem, 0, "has", &hasloc);
                removeOwnership(lineelem[0]);
                /* lineelem[0] is our object */
                
                char *cvar;
                
                int next = hasloc + 1;
                if (!strcmp(lineelem[next], "a") ||
                    !strcmp(lineelem[next], "an")) {
                    next++;
                }
                
                int whichloc;
                concatUntil(lineelem, next, "which", &whichloc);
                removeOwnership(lineelem[next]);
                
                concatAll(lineelem, whichloc + 3);
                removeOwnership(lineelem[whichloc + 3]);
                
                cvar = orkVarToC(lineelem[whichloc + 3]);
                printf("%s_O %s = new %s();\n", lineelem[0], lineelem[next], cvar);
            }
            
            /* this is [to] that */
            else if (findElem(lineelem, 1, "is")) {
                int isloc;
                concatUntil(lineelem, 0, "is", &isloc);
                removeOwnership(lineelem[0]);
                /* lineelem[0] is our object */
                
                /* check if it's "is to" which calls a function */
                if (!strcmp(lineelem[isloc + 1], "to")) {
                    /* FIXME:
                     * this logic is wrong, but the grammar is confusing, since it's:
                     * {object} is to {function} {variable}
                     */
                    if (lineelem[isloc + 3]) {
                        concatAll(lineelem, isloc + 3);
                        removeOwnership(lineelem[isloc + 3]);
                        printf("%s_O ORK_%s(%s);\n", lineelem[0], lineelem[isloc + 2], lineelem[isloc + 3]);
                    } else {
                        printf("%s_O ORK_%s();\n", lineelem[0], lineelem[isloc + 2]);
                    }
                } else {
                    /* just setting x to y */
                    concatAll(lineelem, isloc + 1);
                    removeOwnership(lineelem[isloc + 1]);
                    printf("%s = %s;\n", lineelem[0], lineelem[isloc + 1]);
                }
            }
        }
    }
}

void removeOwnership(char *from)
{
    int i, sl, inq;
    inq = 0;

    /* it can start with "my", which can be immediately remvoed */
    if (!strncmp(from, "my ", 3) ||
        !strncmp(from, "My ", 3)) {
        sl = strlen(from + 3) + 1;
        memmove(from, from + 3, sl);
    }
    
    for (i = 0; from[i]; i++) {
        if (from[i] == '"') {
            if (i == 0 || from[i - 1] != '\\') {
                inq = !inq;
            }
        }
        
        if (!inq) {
            if (from[i] == '\'' &&
                from[i + 1] == 's' &&
                from[i + 2] == ' ') {
                /* don't do it if it's "it's" */
                if (from[i - 1] == 't' &&
                    from[i - 2] == 'i') {
                    from[i] = '_';
                } else {
                    /* turn this into "_O " */
                    from[i] = '_';
                    from[i + 1] = 'O';
                    from[i + 2] = ' ';
                    i += 2;
                }
            }
            
            /* also turn " says " into "_O."
             * as in:
             * If ProfBob says it's less then ... */
            else if (!strncmp(from + i, " says ", 6)) {
                from[i] = '_';
                from[i + 1] = 'O';
                from[i + 2] = ' ';
                sl = strlen(from + i + 6) + 1;
                memmove(from + i + 3, from + i + 6, sl);
                i += 2;
            }
        
            /* and if it's ' ', make it '_' */
            else if (from[i] == ' ') {
                from[i] = '_';
            }
        }
    }
    
    /* now we fix "the", since it's easier here */
    for (i = 0; from[i]; i++) {
        /* "the" anything is inferred to be "arg" */
        /* note that this can get a bit screwy with 'the's in the middle of a variable name:
         * I have a mathematician called Bob the Mathematician
         * becomes:
         * mathematician Bob_arg
         * This is usually fine though */
        if (!strncmp(from, "The_", 4) ||
            !strncmp(from, "the_", 4)) {
            /* only up to a dot */
            int j, osl;
            for (j = i; from[j] && from[j] != '.' && strncmp(from + j, "_O", 2); j++);
            /* copy in "arg" */
            strcpy(from + i, "arg");
            /* then move in the end */
            osl = strlen(from + j) + 1;
            memmove(from + i + 3, from + j, osl);
        }
    }
}

void gr_tok(char *line, char delim, char **linee, int lec)
{
    int i, cle;
    linee[0] = line;
    cle = 1;
    
    for (i = 0; line[i] && cle < lec - 1; i++) {
        if (line[i] == delim) {
            line[i] = '\0';
            linee[cle] = line + i + 1;
            cle++;
        }
    }
    
    linee[cle] = NULL;
}

int findElem(char **les, int from, char *find)
{
    int i;
    
    for (i = from; les[i]; i++) {
        if (!strcmp(les[i], find)) {
            return 1;
        }
    }
    
    return 0;
}

void concatUntil(char **les, int from, char *to, int *tonum)
{
    int i;
    
    for (i = from; les[i + 1]; i++) {
        if (!strcmp(les[i + 1], to)) {
            *tonum = i + 1;
            return;
        }
        les[i][strlen(les[i])] = ' ';
    }
}

void concatAll(char **les, int from)
{
    int i;
    
    for (i = from; les[i + 1]; i++) {
        les[i][strlen(les[i])] = ' ';
    }
}

char *orkVarToC(char *ovar)
{
    if (!strcmp(ovar, "word") ||
        !strcmp(ovar, "sentence") ||
        !strcmp(ovar, "phrase")) {
        return "string";
    } else if (!strcmp(ovar, "number")) {
        return "orknumber";
    } else {
        return ovar;
    }
}

int getLine(char *line, FILE *pinp)
{
    int osl;
    
    line[0] = '#';
    while (line[0] == '#') {
        if (!fgets(line, 1023, pinp))
            return 0;
    }
        
    osl = strlen(line) - 1;
    while (line[osl] == '\n' ||
           line[osl] == '.' ||
           line[osl] == '!' ||
           line[osl] == ':') {
        line[osl] = '\0';
        osl--;
    }
    
    return 1;
}
