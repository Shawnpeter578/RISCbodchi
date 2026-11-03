#include <string.h>
#include <time.h>
#include "algorithms.h"

const char *algorithm_name(int a) {
    switch(a) {
        case ALG_FIFO: return "FIFO";
        case ALG_LRU: return "LRU";
        case ALG_OPTIMAL: return "OPTIMAL";
        default: return "UNKNOWN";
    }
}

int run_algorithm(Message *m) {
    if (!m || m->page_count < 0 || m->page_count > W4_MAX_PAGES ||
        m->frame_count <= 0 || m->frame_count > W4_MAX_FRAMES) return -1;

    int frames[W4_MAX_FRAMES], last_used[W4_MAX_FRAMES];
    int count=0, faults=0, hits=0;
    for (int i=0;i<W4_MAX_FRAMES;i++) { frames[i]=-1; last_used[i]=-1; }

    clock_t start=clock();
    for (int i=0;i<m->page_count;i++) {
        int page=m->pages[i], found=-1;
        for (int j=0;j<count;j++) if (frames[j]==page) { found=j; break; }

        if (found>=0) {
            hits++;
            last_used[found]=i;
            continue;
        }

        faults++;
        if (count < m->frame_count) {
            frames[count]=page;
            last_used[count]=i;
            count++;
            continue;
        }

        int victim=0;
        if (m->algorithm==ALG_FIFO) {
            /* FIFO pointer is faults modulo frame count. */
            victim=(faults-1) % m->frame_count;
        } else if (m->algorithm==ALG_LRU) {
            for (int j=1;j<count;j++)
                if (last_used[j] < last_used[victim]) victim=j;
        } else if (m->algorithm==ALG_OPTIMAL) {
            int farthest=-1;
            for (int j=0;j<count;j++) {
                int next=m->page_count+1;
                for (int k=i+1;k<m->page_count;k++)
                    if (m->pages[k]==frames[j]) { next=k; break; }
                if (next > farthest) { farthest=next; victim=j; }
            }
        } else return -1;

        frames[victim]=page;
        last_used[victim]=i;
    }
    clock_t end=clock();
    m->page_faults=faults;
    m->hits=hits;
    m->execution_seconds=(double)(end-start)/(double)CLOCKS_PER_SEC;
    m->type=MSG_RESULT;
    return 0;
}
