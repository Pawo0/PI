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

    int find_block_close = 0; // checks if "/*" is currently open
    int open_comment = 0; // checks if any comm is open
    int lin_comm = 0; // checks if "//" is open

    int d_quote = 0;
    int quote = 0;
    int open_quote = 0;

    char string[MAX_ID_LEN];
    while ((digit = getc(stdin)) != EOF) {
        int change_last = 1;
        // quote handle
        if (open_comment == 0 && digit == '"' && open_quote == 0) {
            d_quote = 1;
            open_quote = 1;

        } else if (open_comment == 0 && digit == '"' && d_quote == 1) {
            d_quote = 0;
            open_quote = 0;
            change_last = 0;
        } else if (open_comment == 0 && digit == '\'' && open_quote == 0) {
            quote = 1;
            open_quote = 1;
        } else if (open_comment == 0 && digit == '\'' && quote == 1) {
            quote = 0;
            open_quote = 0;
            change_last = 0;
        }
        if (open_quote == 0) {
            if (last_char == '/' && digit == '/' && open_comment == 0) { // "//" start
                open_comment = 1;
                lin_comm = 1;

                last_char = 0;
                change_last = 0;
            } else if (last_char == '/' && digit == '*') { // "/*" start
                find_block_close += 1;
                open_comment = 1;
            } else if (last_char == '*' && digit == '/' && find_block_close >= 1) { // "*/" close
                if (--find_block_close == 0) {
                    open_comment = 0;
                }

                last_char = 0;
                change_last = 0;
            }
            if (digit == '\n' && lin_comm == 1) { // "//" close
                open_comment = 0;
                lin_comm = 0;
            }
            if (open_comment == 0 && last_char != 92) { // 92 = "\"
                if (i == 0 && ((digit >= 'a' && digit <= 'z') || (digit >= 'A' && digit <= 'Z') || digit == '_')) { // starts with a letter
                    string[i] = (char) digit;
                    i++;
                } else if (i > 0 && ((digit >= 'a' && digit <= 'z') || (digit >= 'A' && digit <= 'Z') || (digit >= '0' && digit <= '9') || digit == '_')) {
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
            }
        }

        if (change_last == 1) last_char = digit;
        else last_char = 0;
    }
//    for (int j = 0; j < id_checked; ++j) {
//        printf("%s, ", identifiers[j]);
//    }
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