#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAXWORDS 3000
#define MAXLEN 300

typedef struct {
    int ignore_case; // -i 옵션 (0 또는 1)
    int top_n;       // -n 옵션 (출력 개수 제한, 기본값 0: 전체)
    int min_len;     // -m 옵션 (최소 단어 길이, 기본값 0)
} Options;

typedef struct {
    char word[64]; // 단어 문자열
    int count;     // 빈도수
} WordCount;

Options init_Options(Options o) {
    o.ignore_case = 0;
    o.top_n = 0;
    o.min_len = 0;

    return o;
}

void swap(WordCount *, int, int);
void qsort_c(WordCount *, int, int);
void frequency_counter(FILE *fp, Options o, int min, int top_n);
// int min_len(WordCount *, int, int);
char* ignore_case(char *);

int main(int argc, char *argv[]) {
    int c;
    int min;
    int n;
    Options options;
    options = init_Options(options);
    FILE *fp;
    while (--argc > 0 && (*++argv)[0] == '-') {
        while (c = *++argv[0]) {
            switch (c)
            {
            case 'i':
                options.ignore_case = 1;
                break;
            case 'n':
                options.top_n = 1;
                break;
            case 'm':
                options.min_len = 1;
                break;

            default:
                printf("word-counter : illegal option \'%c\'\n", c);
                    argc = 0;
                break;
            }
        }
    }

    // if (argc != 1 && options.min_len != 0  && options.top_n != 0) {
    //     printf("Usage : ./main -i -n -m count min-len filename\n");
    //     return -1;
    // }
    // if (argc != 2 && (options.min_len == options.top_n)) {
        
    //     return -1;
    // }
    
    if (argc == 1 && options.top_n == 0 && options.min_len == 0) {
        fp = fopen(*argv, "r");
        frequency_counter(fp, options, 0, 0);
    }
    else if (argc == 2 && (options.top_n != options.min_len)) {
        if (options.min_len == 1) {
            min = atoi(*argv++);
            fp = fopen(*argv, "r");
            frequency_counter(fp, options, min, 0);
        }
        else {
            n = atoi(*argv++);
            fp = fopen(*argv, "r");
            frequency_counter(fp, options, 0, n);
        }

    }
    else if (argc == 3 && options.top_n == 1 && options.min_len == 1) {
        n = atoi(*argv++);
        min = atoi(*argv++);
        fp = fopen(*argv, "r");
        frequency_counter(fp, options, min, n);
    }
    else {
        printf("Usage : ./main -i -n -m count min-len filename\n");
        printf("options.top_n : %d | options.min : %d | argc : %d", options.top_n, options.min_len, argc);
        return -1;
    }
    if (fp == NULL) {
        perror("Error opening file.\n");
        return -1;
    }
    fclose(fp);

    return 0;
}

void frequency_counter(FILE *fp, Options o, int min, int n) {
    char line[MAXLEN];
    WordCount word[MAXWORDS] = { 0 };
    int i = 0;
    int processed = 0;
    
    while (fgets(line, sizeof(line), fp) != NULL)
    {
        char *ptr = strtok(line, " \t\n\r.,!?:;\"'()-");

        while(ptr != NULL) {
            int is_duplicate = 0;
            char *w;
            if (o.ignore_case == 1) {
                w = ignore_case(ptr);
            }
            else {
                w = ptr;
            }
            
            if (o.min_len == 1 && min > strlen(w)) {
                ptr = strtok(NULL, " \t\n\r.,!?:;\"'()-");
                continue;;
            }

            for (int j = 0; j < i; j++) {
                if (strcmp(word[j].word, w) == 0) {
                    word[j].count++;
                    is_duplicate = 1;
                    processed++;
                    break;
                }
            }
            
            if (!is_duplicate) {
                strcpy(word[i].word, w);
                word[i++].count = 1;
                processed++;
            }

            
            ptr = strtok(NULL, " \t\n\r.,!?:;\"'()-");
        }
    }

    qsort_c(word, 0, i - 1);
    printf("----------------------------------\n");
    printf("RANK               WORD      COUNT\n");
    printf("----------------------------------\n");
    
    int k;
    // if (o.ignore_case == 1) {
    //     if (o.min_len != o.top_n) {
    //         if (o.min_len == 1) {

    //         }
    //         else {

    //         }
    //     }
    //     else if (o.min_len == 1 && o.top_n == 1) {

    //     }
    //     else {

    //     }
    // }
    // else {
    //     if (o.min_len != o.top_n) {
    //         if (o.min_len == 1) {
    //             min_len(word, min, i);    
    //         }
    //         else {
    //             for (k = 0; k < n && word[k].count != 0; k++) {
    //                 printf("%3d%21s%9d\n", k+1,word[k].word, word[k].count);
    //             }
    //         }
    //     }
    //     else if (o.min_len == 1 && o.top_n == 1) {

    //     }
    //     else {
    //         for (k = 0; k < i; k++) {
    //             printf("%3d%21s%9d\n", k+1,word[k].word, word[k].count);
    //         }
    //     }
    // }
    
    // if (o.min_len == 1) {
    //     i = min_len(word, min, i);
    // }
    if (o.top_n == 1 && n < i) {
        i = n;
    }
    for (k = 0; k < i && word[k].count != 0; k++) {
        printf("%3d%21s%9d\n", k+1,word[k].word, word[k].count);
    }
    printf("----------------------------------\n");
    printf("Total Unique Words: %d\nTotal Words Processed: %d\n", k, processed);
}

void qsort_c(WordCount *wptr, int left, int right) {
    int i, last;

    if (left >= right) {
        return;
    }

    swap(wptr, left, (left + right) / 2);
    last = left;

    for (i = left + 1; i <= right; i++) {
        if (wptr[i].count > wptr[left].count) {
            swap(wptr, ++last, i);
        }
    }

    swap(wptr, left, last);
    qsort_c(wptr, left, last - 1);
    qsort_c(wptr, last + 1, right);
}

void swap(WordCount *wptr, int left, int right) {
    WordCount temp;
    temp = wptr[left];
    wptr[left] = wptr[right];
    wptr[right] = temp;
}

// int min_len(WordCount *wptr, int min, int word_count) {
//     int new_count = 0;
//     for (int i = 0; i < word_count; i++, new_count++) {
//         if(strlen(wptr[i].word) < min) {
//             for (int j = i; j < word_count; j++) {
//                 if (&wptr[j + 1] != NULL) {
//                     wptr[j] = wptr[j + 1];
//                 }
//             }
//             new_count--;
//         }
//     }

//     return new_count;
// }

char* ignore_case(char *word) {
    for (int i = 0; word[i] != '\0' && word[i] != '\n'; i++) {
        word[i] = tolower(word[i]);
    }

    return word;
}