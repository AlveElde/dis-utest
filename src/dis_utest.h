#ifndef __DIS_UTEST_H__
#define __DIS_UTEST_H__

#include <stdlib.h>
#include <stdio.h>

#include <infiniband/verbs.h>

#ifdef DEBUG
#define DEBUG_PRINT 1
#else
#define DEBUG_PRINT 0
#endif


#define printf_debug(fmt, ...) \
do { \
    if (DEBUG_PRINT) { \
        fprintf(stderr, "%s:%d:%s(): " fmt, __FILE__, __LINE__,  __func__, \
                    ##__VA_ARGS__); \
    } \
} while (0)

#define DIS_STATUS_START    "Started.\n"
#define DIS_STATUS_COMPLETE "Completed.\n"
#define DIS_STATUS_FAIL     "Failed.\n"

#define DIS_MAX_PD      1
#define DIS_MAX_CQ      1
#define DIS_MAX_QP      1
#define DIS_MAX_SGE     2
#define DIS_MAX_SQE     1
#define DIS_MAX_RQE     1
#define DIS_MAX_CQE     DIS_MAX_SQE + DIS_MAX_RQE
#define DIS_MAX_SGE_SIZE 128

#define DIS_POLL_SLEEP_MS    200
#define DIS_POLL_TIMEOUT_SEC 20
#define DIS_POLL_TIMEOUT_MS  DIS_POLL_TIMEOUT_SEC * 1000


struct dev_ctx {
    struct ibv_device       *ibv_dev;
    struct ibv_device_attr  dev_attr;
    struct ibv_port_attr    port_attr;
    int                     port_num;
};

struct pd_ctx {
    struct ibv_pd       *ibv_pd;
    struct ibv_device   *ibv_dev;
    int                 flags;
};


struct send_receive_ctx {
    struct dev_ctx  dev;
    struct pd_ctx   pd[DIS_MAX_PD];
    int             pd_c;
};

#endif /* __DIS_UTEST_H__ */