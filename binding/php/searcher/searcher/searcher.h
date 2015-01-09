#define DATA_ELEMENT_LEN 64

typedef struct {
    unsigned char c;
    unsigned int friend;
    unsigned int next;
    int data;
} node;

typedef struct {
    unsigned char d[DATA_ELEMENT_LEN];
} data;

typedef struct {
    unsigned char *head;
    node *body;
    void *data;
} content;

#define HAS_NO_ORDER 1
#define HAS_ORDER_BUT_NEEDNT_CAT 2
#define HAS_ORDER 3

#define DEBUG 0

typedef void (*callback_t)(unsigned char* s,int start,int end,int level);

int init(void **c, long len) ;
int release(void *c) ;
void* parseRuleFile(unsigned char* fileName, int *nn) ;
int parseRuleLine(void *c, unsigned char* line, int len, int *np);
int search(void *c, unsigned char *s, int len, int level,callback_t cbk);
int searchOne(void *c, unsigned char *s, int pos, int len, int *m);
int bestResult(unsigned char *s, int pos, int len, node *head, node *n, int * best_pos, int *best_level);
int saveToFile(void *c, int nn, unsigned char *fileName) ;
long releaseFromFile(unsigned char *fileName, void **c);
