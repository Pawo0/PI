#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define IN_LINE_COMMENT 1
#define IN_BLOCK_COMMENT 2
#define IN_STRING 4
#define IN_ID 8

#define MAX_ID_LEN 64
#define MAX_IDS 1024

int index_cmp(const void *, const void *);

int cmp(const void *, const void *);

char identifiers[MAX_IDS][MAX_ID_LEN];

const char *keywords[] = {
        "auto", "break", "case", "char",
        "const", "continue", "default", "do",
        "double", "else", "enum", "extern",
        "float", "for", "goto", "if",
        "int", "long", "register", "return",
        "short", "signed", "sizeof", "static",
        "struct", "switch", "typedef", "union",
        "unsigned", "void", "volatile", "while"
};


int unique(char *str, int how_much) {
    int flag = 1;
    for (int i = 0; i < how_much; ++i) {
        if (strcmp(str, identifiers[i]) == 0) {
            flag = 0;
            break;
        }
    }
    int i = 0;
    while (i < 32 && flag == 1) {
        if (strcmp(str, keywords[i]) == 0) flag = 0;
        i++;
    }
    return flag;
}

int find_idents() {
    int digit;
    int i = 0;
    int id_checked = 0;
    int last_char = 0;
    char string[MAX_ID_LEN];
    while ((digit = getc(stdin)) != EOF) {
        if ((digit >= 'a' && digit <= 'z') || (digit >= 'A' && digit <= 'Z')
            || (digit >= '0' && digit <= '9') || digit == '_') {
            string[i] = (char) digit;
            i++;
        } else {
            if (unique((char *) string, id_checked) == 1 && i > 0) { // if string appears first time
                sprintf(identifiers[id_checked], "%s", string);
                id_checked++;
            }
            i = 0;
            memset(string, 0, sizeof(string));
        }
        last_char = digit;
    }
    for (int j = 0; j < id_checked; ++j) {
        printf("%s, ", identifiers[j]);

    }
    return id_checked;

}

int cmp(const void *first_arg, const void *second_arg) {
    char *a = *(char **) first_arg;
    char *b = *(char **) second_arg;
    return strcmp(a, b);
}

int index_cmp(const void *first_arg, const void *second_arg) {
    int a = *(int *) first_arg;
    int b = *(int *) second_arg;
    return strcmp(identifiers[a], identifiers[b]);
}

int main(void) {
    printf("%d\n", find_idents());
    return 0;
}

