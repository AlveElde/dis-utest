#ifndef __DIS_UTEST_H__
#define __DIS_UTEST_H__

#include <stdlib.h>
#include <stdio.h>

#include <infiniband/verbs.h>

#define DIS_MAX_PD          1
#define DIS_MAX_CQ          1
#define DIS_MAX_QP          1
#define DIS_MAX_SGE         2
#define DIS_MAX_SQE_PER_SQ  1
#define DIS_MAX_RQE_PER_RQ  1
#define DIS_MAX_CQE_PER_CQ  DIS_MAX_SQE_PER_SQ + DIS_MAX_RQE_PER_RQ
#define DIS_MAX_SGE_SIZE    128

#define POLL_TIMEOUT_SEC    10
#define POLL_INTERVAL_MSEC  200



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
    struct ibv_wc           cqe[DIS_MAX_CQE_PER_CQ];
    int                     cqe_max;
    int                     cqe_expected;
    int                     cqe_c;
    int                     comp_vec;
    void                    *ctx;
};

struct rqe_ctx {
    struct ibv_qp            *ibv_qp;
    struct ibv_recv_wr       ibv_wr;
    struct ibv_sge           ibv_sge[DIS_MAX_SGE];
    struct ibv_recv_wr *ibv_badwr;
};

struct sqe_ctx {
    struct ibv_qp            *ibv_qp;
    struct ibv_send_wr       ibv_wr;
    struct ibv_sge           ibv_sge[DIS_MAX_SGE];
    struct ibv_send_wr *ibv_badwr;
};

struct qp_ctx {
    struct ibv_qp           *ibv_qp;
    struct ibv_pd           *ibv_pd;
    struct ibv_qp_init_attr init_attr;
    struct ibv_qp_attr      attr;
    struct sqe_ctx          sqe[DIS_MAX_SQE_PER_SQ];
    struct rqe_ctx          rqe[DIS_MAX_RQE_PER_RQ];
    struct cq_ctx           *send_cq;
    struct cq_ctx           *recv_cq;
    int                     attr_mask;
    int                     sqe_c;
    int                     rqe_c;
};


struct sge_ctx {
    char    send_sge[DIS_MAX_SGE_SIZE];
    char    recv_sge[DIS_MAX_SGE_SIZE];
    int     length;
    int     lkey;
};

struct send_receive_ctx {
    struct dev_ctx  dev;
    struct pd_ctx   pd[DIS_MAX_PD];
    struct cq_ctx   cq[DIS_MAX_CQ];
    struct qp_ctx   qp[DIS_MAX_QP];
    struct sge_ctx  sge[DIS_MAX_SGE];
    int             pd_c;
    int             cq_c;
    int             qp_c;
    int             sge_c;
};

#endif /* __DIS_UTEST_H__ */