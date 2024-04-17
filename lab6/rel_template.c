#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_REL_SIZE 100
#define MAX_RANGE 100

typedef struct {
    int first;
    int second;
} pair;

// Add pair to existing relation if not already there
int add_relation(pair *tab, int n, pair new_pair) {

}

// Read number of pairs, n, and then n pairs of ints
int read_relation(pair *relation) {
    int n;
    scanf("%d", &n);
    for (int i = 0; i < n; ++i) {
        scanf("%d %d", &relation[i].first, &relation[i].second);
    }
    return n;

}

void print_int_array(int *array, int n) {
    printf("%d\n", n);
    for (int i = 0; i < n; ++i) {
        printf("%d ", array[i]);
    }
    printf("\n");
}

//------------------------------------------------
void instert(int *domain, int n, int val){
    int i = 0;
    while (i < n && domain[i] < val){
        i += 1;
    }
    memmove(&domain[i]+1,&domain[i], sizeof(int) * (n-i));
    domain[i] = val;
}


int get_domain(pair *tab, int n, int *domain) {
    int cnt = 0;
    for (int i = 0; i < n; ++i) {
        int in1 = 0, in2 = 0, j = 0;
        int first = tab[i].first, second = tab[i].second;
        while (j < cnt) {
            if (in1 == 0 && first == domain[j]) in1 = 1;
            if (in2 == 0 && second == domain[j]) in2 = 1;
            if (in1 == 1 && in2 == 1) break;
            j += 1;
        }
        if (in1 == 0) {
            instert(domain,cnt,first);
            cnt += 1;
        }
        if (first != second && in2 == 0) {
            instert(domain,cnt,second);
            cnt += 1;
        }
    }

    return cnt;
}

// Case 1:

// The relation R is reflexive if xRx for every x in X
int is_reflexive(pair *tab, int n) {
    int domain[MAX_REL_SIZE];
    int len_dom = get_domain(tab, n, domain);
    for (int i = 0; i < len_dom; ++i) {
        int x = domain[i];
        int flag = 0;
        for (int j = 0; j < n; ++j) {
            if (x == tab[j].first && x == tab[j].second) flag = 1;
        }
        if (flag == 0) return 0;
    }
    return 1;
}

// The relation R on the set X is called irreflexive
// if xRx is false for every x in X
int is_irreflexive(pair *tab, int n) {
    int domain[MAX_REL_SIZE];
    int len_dom = get_domain(tab, n, domain);
    for (int i = 0; i < len_dom; ++i) {
        int x = domain[i];
        for (int j = 0; j < n; ++j) {
            if (x == tab[j].first && x == tab[j].second) return 0;
        }
    }
    return 1;
}

// A binary relation R over a set X is symmetric if:
// for all x, y in X xRy <=> yRx
int is_symmetric(pair *tab, int n) {
    for (int i = 0; i < n; ++i) {
        int x = tab[i].first;
        int y = tab[i].second;
        int flag = 0;
        for (int j = 0; j < n; ++j) {
            if (tab[j].first == y && tab[j].second == x) flag = 1;
        }
        if (flag == 0) return 0;
    }
    return 1;
}

// A binary relation R over a set X is antisymmetric if:
// for all x,y in X if xRy and yRx then x=y
int is_antisymmetric(pair *tab, int n) {
    for (int i = 0; i < n; ++i) {
        int x = tab[i].first;
        int y = tab[i].second;
        for (int j = 0; j < n; ++j) {
            if (tab[j].first == y && tab[j].second == x) {
                if (x != y) return 0;
            }
        }
    }
    return 1;
}

// A binary relation R over a set X is asymmetric if:
// for all x,y in X if at least one of xRy and yRx is false
int is_asymmetric(pair *tab, int n) {
    for (int i = 0; i < n; ++i) {
        int x = tab[i].first;
        int y = tab[i].second;
        for (int j = 0; j < n; ++j) {
            if (tab[j].first == y && tab[j].second == x) return 0;
        }
    }
    return 1;
}

// A homogeneous relation R on the set X is a transitive relation if:
// for all x, y, z in X, if xRy and yRz, then xRz
int is_transitive(pair *tab, int n) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i != j && tab[i].second == tab[j].first) {
                int k = 0, flag = 0;
                while (flag == 0 && k < n) {
                    if (tab[k].first == tab[i].first && tab[k].second == tab[j].second) flag = 1;
                    k += 1;
                }
                if (flag == 0) return 0;
            }
        }
    }
    return 1;
}

//------------------------------------------------

// Case 2:



// A partial order relation is a homogeneous relation that is
// reflexive, transitive, and antisymmetric
int is_partial_order(pair *tab, int n) {
    if (is_reflexive(tab, n) == 1 && is_transitive(tab, n) == 1 && is_antisymmetric(tab, n) == 1) return 1;
    return 0;
}

// Relation R is connected if for each x, y in X:
// xRy or yRx (or both)
int is_connected(pair *tab, int n) {
    int domain[MAX_REL_SIZE];
    int len_dom = get_domain(tab, n, domain);
    for (int i = 0; i < len_dom - 1; ++i) {
        for (int j = 0; j < len_dom; ++j) {
            int flag = 0, k = 0;
            while (flag == 0 && k < n) {
                if ((tab[k].first == domain[i] && tab[k].second == domain[j]) ||
                    (tab[k].second == domain[i] && tab[k].first == domain[j]))
                    flag = 1;
                k += 1;
            }
            if (flag == 0) return 0;
        }
    }
    return 1;
}

// A total order relation is a partial order relation that is connected

int is_total_order(pair *tab, int n) {
    if (is_partial_order(tab, n) && is_connected(tab, n)) return 1;
    return 0;
}

int find_max_elements(pair *tab, int n, int *max_elements) { // tab - partial order
    int domain[MAX_REL_SIZE];
    int len_dom = get_domain(tab, n, domain);
    int len_max = 0;
    for (int i = 0; i < len_dom; ++i) {
        int flag = 1;
        for (int j = 0; j < n; ++j) {
            if (tab[j].first == domain[i] && tab[j].first != tab[j].second) {
                flag = 0;
                break;
            }
        }
        if (flag == 1) {
            max_elements[len_max] = domain[i];
            len_max += 1;
        }
    }
    return len_max;
}

int find_min_elements(pair *tab, int n, int *min_elements) { // tab - strong partial order
    int domain[MAX_REL_SIZE];
    int len_dom = get_domain(tab, n, domain);
    int len_min = 0;
    for (int i = 0; i < len_dom; ++i) {
        int flag = 1;
        for (int j = 0; j < n; ++j) {
            if (tab[j].second == domain[i] && tab[j].first != tab[j].second) {
                flag = 0;
                break;
            }
        }
        if (flag == 1) {
            min_elements[len_min] = domain[i];
            len_min += 1;
        }
    }
    return len_min;
}



//------------------------------------------------

// Case 3:

// x(S o R)z iff exists y: xRy and ySz
int composition(pair *rel_1, int n1, pair *rel_2, int n2, pair *rel_3) {
    int cnt = 0;
    for (int i = 0; i < n1; ++i) {
        for (int j = 0; j < n2; ++j) {

            if (rel_1[i].second == rel_2[j].first){
                int uniq = 1, k = 0;
                while (k < cnt && uniq == 1){
                    if (rel_3[i].first == rel_1[i].first && rel_3[i].second == rel_2[i].second) uniq = 0;
                    k += 1;
                }
                if (uniq == 1){
                    rel_3[cnt].first = rel_1[i].first;
                    rel_3[cnt].second = rel_2[i].second;
                    cnt += 1;
                }
            }
        }
    }
//    for (int i = 0; i < cnt; ++i) {
//        printf("%d %d\n", rel_3[i].first, rel_3[i].second);
//    }
    return cnt;
}

int main(void) {
    int to_do;
    pair relation[MAX_REL_SIZE];
    pair relation_2[MAX_REL_SIZE];
    pair comp_relation[MAX_REL_SIZE];
    int domain[MAX_REL_SIZE];
    int max_elements[MAX_REL_SIZE];
    int min_elements[MAX_REL_SIZE];

    scanf("%d", &to_do);
    int size = read_relation(relation);
    int ordered, size_2, n_domain;

    switch (to_do) {
        case 1:
            printf("%d ", is_reflexive(relation, size));
            printf("%d ", is_irreflexive(relation, size));
            printf("%d ", is_symmetric(relation, size));
            printf("%d ", is_antisymmetric(relation, size));
            printf("%d ", is_asymmetric(relation, size));
            printf("%d\n", is_transitive(relation, size));
            break;
        case 2:
            ordered = is_partial_order(relation, size);
            n_domain = get_domain(relation, size, domain);
            printf("%d %d\n", ordered, is_total_order(relation, size));
            print_int_array(domain, n_domain);
            if (!ordered) break;
            int no_max_elements = find_max_elements(relation, size, max_elements);
            int no_min_elements = find_min_elements(relation, size, min_elements);
            print_int_array(max_elements, no_max_elements);
            print_int_array(min_elements, no_min_elements);
            break;
        case 3:
            size_2 = read_relation(relation_2);
            printf("%d\n", composition(relation, size, relation_2, size_2, comp_relation));
            break;
        default:
            printf("NOTHING TO DO FOR %d\n", to_do);
            break;
    }

    return 0;
}

