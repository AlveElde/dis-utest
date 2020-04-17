#ifndef __DIS_UTEST_H__
#define __DIS_UTEST_H__

#include <stdlib.h>
#include <stdio.h>

#include <infiniband/verbs.h>

#define PD_NUM  1
#define CQ_NUM  1
#define QP_NUM  1
#define WQE_PER_QP 1
#define SGE_PER_WQE 2

#define SGE_NUM     QP_NUM * WQE_PER_QP * SGE_PER_WQE
#define MR_NUM      SGE_NUM * 2
#define CQE_PER_CQ  WQE_PER_QP * 2

#define SGE_LENGTH  100000

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
    struct ibv_wc           cqe[CQE_PER_CQ];
    int                     cqe_max;
    int                     cqe_expected;
    int                     cqe_c;
    int                     comp_vec;
    void                    *ctx;
};

struct rqe_ctx {
    struct ibv_qp       *ibv_qp;
    struct ibv_recv_wr  ibv_wr;
    struct ibv_sge      ibv_sge[SGE_PER_WQE];
    struct ibv_recv_wr  *ibv_badwr;
};

struct sqe_ctx {
    struct ibv_qp       *ibv_qp;
    struct ibv_send_wr  ibv_wr;
    struct ibv_sge      ibv_sge[SGE_PER_WQE];
    struct ibv_send_wr  *ibv_badwr;
};

struct qp_ctx {
    struct ibv_qp           *ibv_qp;
    struct ibv_pd           *ibv_pd;
    struct ibv_qp_init_attr init_attr;
    struct ibv_qp_attr      attr;
    struct sqe_ctx          sqe[WQE_PER_QP];
    struct rqe_ctx          rqe[WQE_PER_QP];
    struct cq_ctx           *send_cq;
    struct cq_ctx           *recv_cq;
    int                     attr_mask;
    int                     sqe_c;
    int                     rqe_c;
};

struct sge_ctx {
    char    send_sge[SGE_LENGTH];
    char    recv_sge[SGE_LENGTH];
    int     length;
    int     lkey;
};

struct mr_ctx {
    struct ibv_mr   *ibv_mr;
    struct ibv_pd   *ibv_pd;
    int             access;
    int             length;
    void            *buf;
};

struct send_receive_ctx {
    struct dev_ctx  dev;
    struct pd_ctx   pd[PD_NUM];
    struct cq_ctx   cq[CQ_NUM];
    struct qp_ctx   qp[QP_NUM];
    struct sge_ctx  sge[SGE_NUM];
    struct mr_ctx   mr[MR_NUM];
    int             pd_c;
    int             cq_c;
    int             qp_c;
    int             sge_c;
    int             mr_c;
};

#endif /* __DIS_UTEST_H__ */