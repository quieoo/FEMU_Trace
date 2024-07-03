#include "record_lpn.h"

// 初始化Recorder
Recorder* init_recorder() {
    Recorder *rec = (Recorder *)malloc(sizeof(Recorder));
    if (rec == NULL) {
        perror("Failed to allocate memory for Recorder");
        return NULL;
    }
    rec->lpns = (__uint64_t *)malloc(MAX_LPN_COUNT * sizeof(__uint64_t));
    if (rec->lpns == NULL) {
        perror("Failed to allocate memory for LPNs");
        free(rec);
        return NULL;
    }
    rec->count = 0;
    rec->last_write_time = time(NULL);
    return rec;
}

// 释放Recorder
void free_recorder(Recorder *rec) {
    if (rec == NULL) return;
    free(rec->lpns);
    free(rec);
}

void read_check_file(){
    FILE *fp = fopen("/home/quieoo/ftl/trace/femu", "r");
    if (fp != NULL) {
        // 以8字节读取文件数据并输出
        __uint64_t lpn;
        while (fread(&lpn, sizeof(__uint64_t), 1, fp) == 1) {
            printf("%lx\n", lpn);
        }
        fclose(fp);
    } else {
        perror("Failed to open LPN log file");
    }
}

// 将收集的LPN写入文件
void write_to_file(Recorder *rec) {
    if (rec == NULL) return;

    // printf("Writing %d LPNs to file\n", rec->count);
    // time_t now = time(NULL);
    // struct tm *tm = localtime(&now);

    
    // for (int i = 0; i < rec->count; i++) {
    //     fprintf(file, "%04d-%02d-%02d %02d:%02d:%02d %lu\n", tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec, rec->lpns[i]);
    // }
    
    FILE *file = fopen("/home/quieoo/ftl/trace/femu", "a");
    if (!file) {
        perror("Failed to open file");
        return;
    }
    // 将lpn数组中的数据批量写入文件，每个lpn占据8字节
    fwrite(rec->lpns, sizeof(__uint64_t), rec->count, file);


    fflush(file); // 同步缓冲区
    fclose(file);
    
    rec->count = 0; // 重置LPN计数
    rec->last_write_time = time(NULL); // 更新最后写入时间
}

// 添加LPN到Recorder
void add_lpn(Recorder *rec, __uint64_t lpn) {
    if (rec == NULL) return;

    rec->lpns[rec->count++] = lpn;

    // 检查是否需要写入文件
    if (rec->count >= MAX_LPN_COUNT || difftime(time(NULL), rec->last_write_time) >= TIME_THRESHOLD) {
        write_to_file(rec);
    }
}

// int main() {
//     Recorder *rec = init_recorder();
//     if (rec == NULL) {
//         perror("Failed to initialize Recorder");
//         return 1;
//     }
    
//     int n = 105000;
//     // 模拟访问LPN
//     // 统计平均每次循环执行时间，以纳秒为单位
//     struct timespec start, end;
//     clock_gettime(CLOCK_MONOTONIC, &start);

//     for (int i = 0; i < n; i++) {
//         add_lpn(rec, i);
//     }

//     clock_gettime(CLOCK_MONOTONIC, &end);
//     double elapsed = (end.tv_sec - start.tv_sec) * 1000000000.0 + (end.tv_nsec - start.tv_nsec);
//     printf("Average execution time: %.6f nanoseconds\n", elapsed / n);

//     // 在程序结束前确保所有数据被写入文件
//     if (rec->count > 0) {
//         write_to_file(rec);
//     }

//     free_recorder(rec);
//     return 0;
// }
