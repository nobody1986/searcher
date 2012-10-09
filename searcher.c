/* 
 * File:   main.c
 * Author: hp1
 *
 * Created on 2012年2月3日, 下午3:27
 */

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include "zlib.h"
#include "searcher.h"



#if DEBUG

void showList(void *c) {
    int i = 0;
    unsigned int *head = (unsigned int *) c;
    printf("====List start=====\n");
    for (i = 0; i < 256; ++i) {
        if (*(head + i) > 0) {
            printf("[%c](%d) -> [%d]\n", (unsigned char) i, i, *(head + i));
        }
    }
    printf("====List end=====\n");
}
#endif

int init(void **c, long len) {
    size_t full_block = sizeof (unsigned int) * 256 + sizeof (node) * ((len - 256) + 1 + 256);
    *c = (unsigned char *) malloc(full_block);
    memset(*c, 0, full_block);
    return full_block;
}

int release(void *c) {
    free(c);
}

void* parseRuleFile(unsigned char* fileName, int *nn) {
    void *c = NULL;
    FILE *fp = fopen(fileName, "rt");
    unsigned char line[1024];
    //
    fseek(fp, 0, SEEK_END);
    long len = ftell(fp);
#if DEBUG
    printf("filesize:%d\n", len);
#endif
    rewind(fp);
#if DEBUG
    printf("eof:%d\n", feof(fp));
#endif
    init(&c, len);
    int np = 1;
    while (fgets(line, 124, fp) != NULL) {
#if DEBUG
        printf("line: %s \n", line);
#endif
        parseRuleLine(c, line, strlen(line), &np);
#if DEBUG
        printf("np : %d\n", np);
#endif
    }
    fclose(fp);
    *nn = np;
    return c;
}

int parseRuleLine(void *c, unsigned char* line, int len, int *np) {
    //
    int i = 0;
    unsigned char n = 0;
    node* p = NULL;
    int tmp = 0;
    unsigned int point = 0;
    unsigned int *head = (unsigned int *) c;
    node *nhead = (node *) (head + 256);
    int stat = 0;
    for (i = 0; i < len; ++i) {
        if (*(line + i) == '#') {
            break;
        }
        if (*(line + i) == ' ') {
            stat = 1;
            continue;
        }
        if (stat == 1) {
            p->data = atoi(line + i);
#if DEBUG
            printf("levels : %d\n", p->data);
            printf("index : %d\n", p - nhead);
            printf("char : %c\n", p->c);
#endif
#if DEBUG
            //showList(c);
#endif
            break;
        }
        n = *(line + i);
        if (p == NULL) {
            point = *(head + n);
            if (point == 0) {
                *(head + n) = *np;
                p = (nhead + *np - 1);
#if DEBUG
                printf("+head : [%c] -> %d\n", n, *np + 1);
                showList(c);
#endif
                p->c = n;
                *np += 1;
            } else {
                p = (nhead + point - 1);
#if DEBUG
                printf("levels : %d\n", p->data);
                printf("index : %d\n", p - nhead);
                printf("char : %c\n", p->c);
#endif
            }
        } else {
            if (p->next == 0) {
                p->next = *np;
                *np += 1;
            }
            if (p->c > 0) {
                p = nhead + p->next - 1;
            }
            if (p->c == n) {
                /*
                if (p->next == 0) {
                    p->next = *np;
                    //p->c = n;
                 *np += 1;
                }
                p = nhead + p->next - 1;
                 * */
#if DEBUG
                printf("levels : %d\n", p->data);
                printf("index : %d\n", p - nhead);
                printf("char : %c\n", p->c);
#endif
            } else {
                if (p->c == 0) {
                    p->c = n;
                    if (p->next == 0) {
                        p->next = *np;
                        //p->c = n;
                        *np += 1;
                    }
                    //p = nhead + p->next - 1;
                } else {
                    tmp = p->friend;
                    while (tmp != 0) {
                        if ((nhead + tmp - 1)->c == n) {
                            break;
                        }
                        p = (nhead + tmp - 1);
#if DEBUG
                        printf("levels : %d\n", p->data);
                        printf("index : %d\n", p - nhead);
                        printf("char : %c\n", p->c);
#endif
                        tmp = p->friend;
                    }
                    if (tmp != 0) {
                        p = (nhead + tmp - 1);
#if DEBUG
                        printf("levels : %d\n", p->data);
                        printf("index : %d\n", p - nhead);
                        printf("char : %c\n", p->c);
#endif
                    } else {
                        //添加friend node
                        p->friend = *np;
                        (nhead + *np - 1)->c = n;
                        p = (nhead + *np - 1);
                        *np += 1;
#if DEBUG
                        printf("levels : %d\n", p->data);
                        printf("index : %d\n", p - nhead);
                        printf("char : %c\n", p->c);
#endif
                    }
                    /*
                    if (p->next == 0) {
                        p->next = *np;
                        //p->c = n;
                     *np += 1;
                    }
                    p = nhead + p->next - 1;
                     */
                }
            }
#if DEBUG
            showList(c);
#endif
        }
    }
}

int search(void *c, unsigned char *s, int len, int level,callback_t cbk) {
    int i = 0;
    int pos = 0;
    int ret = 0;
    for (i = 0; i < len; ++i) {
        ret = searchOne(c, s, i, len, &pos);
        if (ret > 0) {
            #if DEBUG
            printf("pos :%d - %d -level: %d\n", i, pos, ret);
            #endif
            if(cbk){
                cbk(s,i,pos,ret);
            }
            if (ret > level) {
                i = pos;
            }
            ret = 0;
        }
    }
    return 0;
}

int searchOne(void *c, unsigned char *s, int pos, int len, int *m) {
    int i = 0;
    node *p = NULL;
    node *tmp = NULL;
    unsigned char n = 0;
    unsigned int *head = (unsigned int *) c;
    node *nhead = (node *) (head + 256);
    int last_pos = 0;
    int last_level = 0;
    //for (i = pos; i < len; ++i) {
    i = pos;
    n = *(s + i);
#if DEBUG
    printf("char : [%c]\n", n);
#endif
    if (p == NULL) {
#if DEBUG
        printf("head : %d\n", *(head + n));
#endif
        if (*(head + n) > 0) {
            p = nhead + *(head + n) - 1;
#if DEBUG
            printf("-index : [%d]\n", *(head + n));
            printf("-char : [%c]\n", p->c);
            printf("-level : [%d]\n", p->data);
#endif
            /*
            while (p != NULL && i < len) {
                if (p->data > last_level) {
                    last_pos = i;
                    last_level = p->data;
                }
                if (p->next == 0) {
                    break;
                }
                //printf("-index : [%d]\n", p->next );
                tmp = nhead + p->next - 1;
                ++i;
                n = *(s + i);
                //#if DEBUG
                //printf("-char : [%c]\n", p->c);
                //#endif
                //friend
                p = NULL;
#if DEBUG
                printf("--char : [%c]\n", n);
                printf("--char =: [%c]\n", tmp->c);
                printf("--index =: [%d]\n", tmp - nhead);
#endif
                if (tmp->c == n) {
                    p = tmp;
                } else {
                    while (tmp->friend > 0) {
                        //printf("-index : [%d]\n", tmp->friend);
                        tmp = nhead + tmp->friend - 1;
                        if (tmp->c == n) {
                            p = tmp;
                            break;
                        }
                    }
                }
            }
             * */
            bestResult(s, pos, len, nhead, p, &last_pos, &last_level);
            if (last_level > 0) {
                *m = last_pos;
                return last_level;
            }
        } else {
            return 0;
        }
    }
    //}
    return 0;
}

int bestResult(unsigned char *s, int pos, int len, node *head, node *n, int * best_pos, int *best_level) {
    node *tmp = n;
    unsigned char c = *(s + pos);
    if (pos >= len) {
        return 0;
    }
    while (tmp != NULL) {
#if DEBUG
        printf("pos -> char :[%d] -> [%c] | [%c]\n", pos, c, tmp->c);
        printf("friend :[%d] \n", tmp->friend);
#endif
        if (c == tmp->c) {
#if DEBUG
            printf("pos -> char :[%d] -> [%c] | [%c]\n", pos, c, tmp->c);
            printf("friend :[%d] \n", tmp->friend);
            printf("level :[%d] \n", tmp->data);
            printf("index :[%d] \n", tmp - head);
            printf("next :[%d] \n", tmp->next);
#endif
            if (tmp->data > *best_level) {
                *best_level = tmp->data;
                *best_pos = pos;
            } else if (tmp->data == *best_level) {
                if (pos > *best_pos) {
                    *best_level = tmp->data;
                    *best_pos = pos;
                }
            }
            if (tmp->next > 0) {
                bestResult(s, pos + 1, len, head, head + tmp->next - 1, best_pos, best_level);
            }
        }
        if (tmp->friend > 0) {
            tmp = head + tmp->friend - 1;
#if DEBUG
            printf("pos -> char :[%d] -> [%c] | [%c]\n", pos, c, tmp->c);
            printf("friend :[%d] \n", tmp->friend);
            printf("level :[%d] \n", tmp->data);
            printf("index :[%d] \n", tmp->friend);
#endif
        } else {
            tmp = NULL;
        }
    }
    return 0;
}

int saveToFile(void *c, int nn, unsigned char *fileName) {
    size_t length = 256 * sizeof (unsigned int) +nn * sizeof (node);
    unsigned long lengthCompressed = compressBound(length);
    unsigned long *s = malloc(lengthCompressed + sizeof (unsigned long));
    unsigned long lengthCompressedR = lengthCompressed + sizeof (unsigned long);
    memset(s, 0, lengthCompressed + sizeof (unsigned long));
    if (compress((Bytef *) (s + 1), &lengthCompressedR, (Bytef *) c, length) != Z_OK) {
#if DEBUG
        printf("%d\n", length);
        printf("%d\n", lengthCompressed);
        printf("%d\n", lengthCompressed + sizeof (unsigned long));
        printf("%d\n", compress((Bytef *) s, &lengthCompressedR, "xx", 2));
#endif
        return 0;
    }
    *s = length;
#if DEBUG
    printf("length :[%d] \n", length);
    printf("before Compressed compute :[%d] \n", lengthCompressed);
    printf("after    Compressed true :[%d] \n", lengthCompressedR);
    printf("nn :[%d] \n", nn);
#endif
    FILE *fp = fopen(fileName, "wb");
    fwrite(s, lengthCompressedR + sizeof (unsigned long), 1, fp);
    fclose(fp);
    free(s);
}

long releaseFromFile(unsigned char *fileName, void **c) {
    FILE *fp = fopen(fileName, "rb");
    unsigned long *s = NULL;
    unsigned long beforeUncompress = 0;
    unsigned long afterUncompress = 0;
    fseek(fp, 0, SEEK_END);
    long len = ftell(fp);
    rewind(fp);
    s = malloc(len + 1);
    memset(s, 0, len + 1);
    fread(s, len, 1, fp);
    beforeUncompress = *s;
    *c = malloc(beforeUncompress);
    memset(*c, 0, beforeUncompress);
    afterUncompress = beforeUncompress;
    int ret = 0;
    if ((ret = uncompress((Bytef *) * c, &afterUncompress, (Bytef *) (s + 1), len - sizeof (unsigned long))) != Z_OK) {
        printf("ret :[%d] \n", ret);
        return 0;
    }
#if DEBUG
    printf("len :[%d] \n", len);
    printf("before Uncompressed compute :[%d] \n", beforeUncompress);
    printf("after    Uncompressed true :[%d] \n", afterUncompress);
#endif
    free(s);
    return len;
}

#if DEBUG
/*
 * 
 */
int main(int argc, char** argv) {
    if (argc < 2) {
        printf("argument error!\n");
        exit(1);
    }
    void *c = NULL;
    int nn = 0;
    unsigned char * compiled = "compiled.cd.gz";
    unsigned char* s = "abcdefghabdafacdadadas89dabcdeaafkasdasdasdas89";
    c = parseRuleFile(argv[1], &nn);
    saveToFile(c, nn, compiled);
    c = NULL;
    releaseFromFile(compiled, &c);
    showList(c);
    search(c, s, strlen(s), 2);
    return (EXIT_SUCCESS);
}
#endif
