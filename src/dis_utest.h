#ifndef __DIS_UTEST_H__
#define __DIS_UTEST_H__

#include <stdlib.h>
#include <stdio.h>

#include <infiniband/verbs.h>

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
    struct ibv_device       **ibv_dev_list;
    struct ibv_context      *ibv_ctx;
    struct ibv_device       *ibv_dev;
    struct ibv_device_attr  dev_attr;
    struct ibv_port_attr    port_attr;
    int                     num_devices;
    int                     port_num;
};

struct pd_ctx {
    struct ibv_pd       *ibv_pd;
    // struct ibv_context  *ibv_ctx;
};

struct cq_ctx {
    struct ibv_cq           *ibv_cq;
    struct ibv_context      *ibv_ctx;
    struct ibv_comp_channel *comp_ch;
    void                    *ctx;
    int                     cqe_max;
    int                     comp_vec;
};

struct qp_ctx {
    struct ibv_qp           *ibv_qp;
    struct ibv_pd           *ibv_pd;
    struct ibv_qp_init_attr init_attr;
    struct ibv_qp_attr      attr;
    struct cq_ctx           *send_cq;
    struct cq_ctx           *recv_cq;
    int                     attr_mask;
};

struct send_receive_ctx {
    struct dev_ctx  dev;
    struct pd_ctx   pd[DIS_MAX_PD];
    struct cq_ctx   cq[DIS_MAX_CQ];
    struct qp_ctx   qp[DIS_MAX_QP];
    int             pd_c;
    int             cq_c;
    int             qp_c;
};

#endif /* __DIS_UTEST_H__ */