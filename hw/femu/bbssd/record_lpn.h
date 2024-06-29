#ifndef record_lpn_h
#define record_lpn_h
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX_LPN_COUNT 1000  // 定义LPN数量阈值
#define TIME_THRESHOLD 5   // 定义时间间隔阈值（单位：秒）

typedef struct recorder {
    __uint64_t *lpns;               // 存储LPN的数组
    int count;               // 当前收集的LPN数量
    time_t last_write_time;  // 上次写入文件的时间
} Recorder;

Recorder* init_recorder();
void free_recorder(Recorder *rec);
void write_to_file(Recorder *rec);
void add_lpn(Recorder *rec, __uint64_t lpn);

#endif