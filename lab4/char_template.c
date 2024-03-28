#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

// consider chars from [FIRST_CHAR, LAST_CHAR)
#define FIRST_CHAR 33
#define LAST_CHAR 127
#define MAX_CHARS (LAST_CHAR - FIRST_CHAR)
#define MAX_BIGRAMS ((LAST_CHAR - FIRST_CHAR) * (LAST_CHAR - FIRST_CHAR))

#define NEWLINE '\n'
#define IN_WORD 1

#define IN_LINE_COMMENT 1
#define IN_BLOCK_COMMENT 2

int count[MAX_BIGRAMS] = {0};

// sort indices according to their respective counts.
// sort alphabetically if counts equal
int cmp(const void *a, const void *b) {
    int va = *(int *) a;
    int vb = *(int *) b;
    if (count[va] == count[vb]) return va - vb;
    return count[vb] - count[va];
}

// sort indices according to their respective counts.
// sort alphabetically if counts equal
int cmp_di(const void *a, const void *b) {
    int va = *(int *) a;
    int vb = *(int *) b;
    // sort according to second char if counts and the first char equal
    if (count[va] == count[vb] && va / MAX_CHARS == vb / MAX_CHARS) return va % MAX_CHARS - vb % MAX_CHARS;
    // sort according to first char if counts equal
    if (count[va] == count[vb]) return va / MAX_CHARS - vb / MAX_CHARS;
    return count[vb] - count[va];
}

// count lines, words & chars in a given text file
void wc(int *nl, int *nw, int *nc) {
    int digit;
    int new_word = 0;
    *nl = 0;
    *nw = 0;
    *nc = 0;

    while ((digit = getc(stdin)) != EOF) {
        if (digit < FIRST_CHAR) {
            new_word = 1;
        } else if (new_word == 1) {
            *nw = *nw + 1;
            new_word = 0;
        }
        if (digit == NEWLINE) {
            *nl = *nl + 1;
        }
        *nc = *nc + 1;
//        printf("%d ",digit);
    }
    if (new_word == 1) *nw = *nw + 1;

}

void char_count(int char_no, int *n_char, int *cnt) {
    int digit;
    int digits[MAX_CHARS] = {0};
    while ((digit = getc(stdin)) != EOF) {
        if (digit >= FIRST_CHAR && digit <= LAST_CHAR) {
            digits[digit - FIRST_CHAR] += 1;
        }
    }
//    print tab with multiples
//    for (int i = 0; i < MAX_CHARS; ++i) {
//        printf("%d: %d\n", i, digits[i]);
//    }

    int decreasing_id[MAX_CHARS] = {0};
    for (int i = 0; i < MAX_CHARS; ++i) {
        decreasing_id[i] = i;
    }

//    bubble bul bul bul ;p
    for (int i = 0; i < MAX_CHARS; ++i) {
        for (int j = 0; j < MAX_CHARS - i - 1; ++j) {
            if (digits[decreasing_id[j]] == digits[decreasing_id[j + 1]]) {
                if (decreasing_id[j] > decreasing_id[j + 1]) {
                    int tmp = decreasing_id[j];
                    decreasing_id[j] = decreasing_id[j + 1];
                    decreasing_id[j + 1] = tmp;
                }
            } else if (digits[decreasing_id[j]] < digits[decreasing_id[j + 1]]) {
                int tmp = decreasing_id[j];
                decreasing_id[j] = decreasing_id[j + 1];
                decreasing_id[j + 1] = tmp;
            }
        }
    }

//    print sorted tab with idx
//    for (int i = 0; i < MAX_CHARS; ++i) {
//        printf("%d: %d\n", i, decreasing_id[i]);
//    }
    *n_char = decreasing_id[char_no - 1] + FIRST_CHAR;
    *cnt = digits[decreasing_id[char_no - 1]];

}

void bigram_count(int bigram_no, int bigram[]) {
    int digit;
    int diagrams[MAX_BIGRAMS] = {0};
    int last_char = 0;
    while ((digit = getc(stdin)) != EOF) {
        if ((last_char >= FIRST_CHAR && last_char <= LAST_CHAR) &&
            (digit >= FIRST_CHAR && digit <= LAST_CHAR)) {
            diagrams[(last_char - FIRST_CHAR) * MAX_CHARS + (digit - FIRST_CHAR)] += 1;
            last_char = digit;
        } else if (digit < FIRST_CHAR) last_char = 0;
        else last_char = digit;
    }


    int decreasing_id[MAX_BIGRAMS] = {0};
    for (int i = 0; i < MAX_BIGRAMS; ++i) {
        decreasing_id[i] = i;
    }

//    bubble bul bul bul ;p
    for (int i = 0; i < MAX_BIGRAMS; ++i) {
        for (int j = 0; j < MAX_BIGRAMS - i - 1; ++j) {
            if (diagrams[decreasing_id[j]] == diagrams[decreasing_id[j + 1]]) {
                if (decreasing_id[j] > decreasing_id[j + 1]) {
                    int tmp = decreasing_id[j];
                    decreasing_id[j] = decreasing_id[j + 1];
                    decreasing_id[j + 1] = tmp;
                }
            } else if (diagrams[decreasing_id[j]] < diagrams[decreasing_id[j + 1]]) {
                int tmp = decreasing_id[j];
                decreasing_id[j] = decreasing_id[j + 1];
                decreasing_id[j + 1] = tmp;
            }
        }
    }

//    print sorted tab with idx
//    for (int i = 0; i < MAX_BIGRAMS; ++i) {
//        printf("%d: |%d %d| %c%c, %d\n", i, (decreasing_id[i]/MAX_CHARS+FIRST_CHAR), (decreasing_id[i]%MAX_CHARS+FIRST_CHAR),
//               (decreasing_id[i]/MAX_CHARS+FIRST_CHAR), (decreasing_id[i]%MAX_CHARS+FIRST_CHAR),diagrams[decreasing_id[i]]);
//    }
    bigram[0] = decreasing_id[bigram_no - 1] / MAX_CHARS + FIRST_CHAR;
    bigram[1] = decreasing_id[bigram_no - 1] % MAX_CHARS + FIRST_CHAR;
    bigram[2] = diagrams[decreasing_id[bigram_no - 1]];

}

void find_comments(int *line_comment_counter, int *block_comment_counter) {
    *line_comment_counter = 0;
    *block_comment_counter = 0;
    int digit;
    int last_char = '\n';
    int find_block_close = 0; // checks if "/*" is currently open
    int open_comment = 0; // checks if any comm is open
    int lin_comm = 0; // checks if "//" is open
    while ((digit = getc(stdin)) != EOF) {
        int change_last = 1;
        if (last_char == '/' && digit == '/' && open_comment == 0){ // "//" start
            open_comment = 1;
            lin_comm = 1;
            *line_comment_counter = *line_comment_counter + 1;

            last_char = 0;
            change_last = 0;
        } else if (last_char == '/' && digit == '*') { // "/*" start
            find_block_close += 1;
            open_comment = 1;
        } else if (last_char == '*' && digit == '/' && find_block_close >= 1) { // "*/" close
            if (--find_block_close == 0){
                *block_comment_counter = *block_comment_counter + 1;
                open_comment = 0;
            }

            last_char = 0;
            change_last = 0;
        }
        if (digit == NEWLINE && lin_comm == 1){ // "//" close
            open_comment = 0;
            lin_comm = 0;
        }

        if (change_last == 1) last_char = digit;

    }
}

#define MAX_LINE 128

int read_int() {
    char line[MAX_LINE];
    fgets(line, MAX_LINE, stdin); // to get the whole line
    return (int) strtol(line, NULL, 10);
}

int main(void) {
    int to_do;
    int nl, nw, nc, char_no, n_char, cnt;
    int line_comment_counter, block_comment_counter;
    int bigram[3];

    to_do = read_int();
    switch (to_do) {
        case 1: // wc()
            wc(&nl, &nw, &nc);
            printf("%d %d %d\n", nl, nw, nc);
            break;
        case 2: // char_count()
            char_no = read_int();
            char_count(char_no, &n_char, &cnt);
            printf("%c %d\n", n_char, cnt);
            break;
        case 3: // bigram_count()
            char_no = read_int();
            bigram_count(char_no, bigram);
            printf("%c%c %d\n", bigram[0], bigram[1], bigram[2]);
            break;
        case 4:
            find_comments(&line_comment_counter, &block_comment_counter);
            printf("%d %d\n", block_comment_counter, line_comment_counter);
            break;
        default:
            printf("NOTHING TO DO FOR %d\n", to_do);
            break;
    }
    return 0;
}

