#ifndef W4_MESSAGE_H
#define W4_MESSAGE_H

#define W4_MAX_PAGES 256
#define W4_MAX_FRAMES 64
#define W4_TEXT 128

typedef enum { MSG_REQUEST=1, MSG_RESULT, MSG_LOG, MSG_STOP, MSG_ERROR } MessageType;
typedef enum { ALG_FIFO=1, ALG_LRU, ALG_OPTIMAL } Algorithm;

typedef struct {
    MessageType type;
    int algorithm;
    int page_count;
    int frame_count;
    int pages[W4_MAX_PAGES];
    int page_faults;
    int hits;
    double execution_seconds;
    char text[W4_TEXT];
} Message;

#endif
