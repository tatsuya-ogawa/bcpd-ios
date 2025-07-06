#include "lib_bcpd.h"
#include "lib_bcpd_private.h"
// execute: mainのロジックをバッファ受け渡しAPI化
// 必要に応じて引数を追加・整理してください
int bcpd_execute(
    double *X, int N, // target点群 (入力/正規化される)
    double *Y, int M, // source点群 (入力/正規化される)
    int D,            // 次元
    double *fx, int Df, // target特徴量 (NULL可)
    double *fy,         // source特徴量 (NULL可)
    bcpd_pwpm *bcpd_pm,           // パラメータ
    bcpd_pwsz *bcpd_sz,           // サイズ情報
    double *out_y,      // 結果y格納用 (M*D)
    double *out_x,      // 結果x格納用 (M*D)
    double *out_v,      // 結果v格納用 (M*D)
    double *out_a,      // 結果a格納用 (M)
    double *out_sgm,    // 結果sgm格納用 (M)
    double *out_R,      // 結果R格納用 (D*D)
    double *out_t,      // 結果t格納用 (D)
    double *out_s,      // 結果s格納用 (1)
    double *out_r,      // 結果r格納用 (1)
    double *out_Np,     // 結果Np格納用 (1)
    double *out_pf      // 結果pf格納用 (3*pm->nlp)
) {
    int l,lp; double s,r,Np,sgmX,sgmY,sgmfx,sgmfy,*muX,*muY,*mufx,*mufy; double *u,*v,*w,*R,*t,*a,*sgm,*e;
    double *x,*y,*wd; int *wi; int sd=sizeof(double),si=sizeof(int);
    int dsz,isz,ysz,xsz; int nx,ny,N0,M0=0; double rx,ry,*x0,*y0,*v0,*X0,*Y0=NULL; double *pf;
    double *LQ=NULL,*LQ0=NULL; int *Ux=NULL,*Uy=NULL; int K; int geok=0; double *fx0=NULL,*fy0=NULL; int nr,nc; int ssm;
    // 構造体値コピー
    pwsz sz_local = *(pwsz*)(void*)bcpd_sz;
    pwpm pm_local = *(pwpm*)(void*)bcpd_pm;
    pwsz *sz = &sz_local;
    pwpm *pm = &pm_local;
    // 正規化
    muX=calloc(D,sd); muY=calloc(D,sd);
    normalizer(muX,&sgmX,muY,&sgmY,X,Y,N,M,D,pm->nrm);
    normalize (X,muX,sgmX,N,D);
    normalize (Y,muY,sgmY,M,D);
    if(fx&&fy){ mufx=calloc(Df,sd); mufy=calloc(Df,sd);
        normalizer(mufx,&sgmfx,mufy,&sgmfy,fx,fy,N,M,Df,pm->nrf);
        normalize (fx,mufx,sgmfx,N,Df);
        normalize (fy,mufy,sgmfy,M,Df);
    }
    // geodesic kernel
    ssm=strlen(pm->fn[COV_LQ]);
    if(ssm){
        LQ=read_LQ(&nr,&nc,pm->fn[COV_LQ]); pm->K=K=nc;
        if(nr!=1+D*M) return -1;
    }
    if(pm->opt&PW_OPT_NONRG) goto skip_geok;
    geok=(pm->nnk||strlen(pm->fn[FACE_Y]))&&pm->tau>1e-5;
    if(geok){ sgraph* sg;
        if(pm->nnk) sg=sgraph_from_points(Y,D,M,pm->nnk,pm->nnr);
        else        sg=sgraph_from_mesh  (Y,D,M,pm->fn[FACE_Y]);
        LQ=geokdecomp(&K,Y,D,M,(const int**)sg->E,(const double**)sg->W,pm->K,pm->bet,pm->tau,pm->eps);
        sz->K=pm->K=K;
        sgraph_free(sg);
    }
skip_geok:
    // downsampling
    nx=pm->dwn[TARGET]; rx=pm->dwr[TARGET];
    ny=pm->dwn[SOURCE]; ry=pm->dwr[SOURCE];
    if(nx){X0=X;N0=N;N=sz->N=nx;X=calloc(D*N,sd);Ux=calloc(rx==0?N0:N,si);downsample(X,Ux,N,X0,D,N0,rx);}
    if(ny){Y0=Y;M0=M;M=sz->M=ny;Y=calloc(D*M,sd);Uy=calloc(ry==0?M0:M,si);downsample(Y,Uy,M,Y0,D,M0,ry);}
    if(nx&&fx){fx0=fx;fx=downsample_F (fx0,Df,N0,Ux,N);}
    if(ny&&fy){fy0=fy;fy=downsample_F (fy0,Df,M0,Uy,M);}
    if(ny&&LQ){LQ0=LQ;LQ=downsample_LQ(LQ0,M0,K, Uy,M,ssm?D:0);}
    // メモリ確保
    memsize(&dsz,&isz,*sz,*pm);
    ysz=D*M; ysz+=D*M*((pm->opt&PW_OPT_PATHY)?pm->nlp:0);
    xsz=D*M; xsz+=D*M*((pm->opt&PW_OPT_PATHX)?pm->nlp:0);
    wd=calloc(dsz,sd); x=calloc(xsz,sd); a=calloc(M,sd); u=calloc(D*M,sd); R=calloc(D*D,sd); sgm=calloc(M,sd);
    wi=calloc(isz,si); y=calloc(ysz,sd); w=calloc(M,sd); v=calloc(D*M,sd); t=calloc(D,sd); pf=calloc(3*pm->nlp,sd);
    e=fx?calloc(Df,sd):NULL;
    // メイン計算
    lp=bcpd(x,y,u,v,w,a,sgm,&s,R,t,&r,e,&Np,pf,wd,wi,X,Y,fx,fy,LQ,*sz,*pm);
    // 結果コピー
    if(out_y) memcpy(out_y, y, D*M*sizeof(double));
    if(out_x) memcpy(out_x, x, D*M*sizeof(double));
    if(out_v) memcpy(out_v, v, D*M*sizeof(double));
    if(out_a) memcpy(out_a, a, M*sizeof(double));
    if(out_sgm) memcpy(out_sgm, sgm, M*sizeof(double));
    if(out_R) memcpy(out_R, R, D*D*sizeof(double));
    if(out_t) memcpy(out_t, t, D*sizeof(double));
    if(out_s) *out_s = s;
    if(out_r) *out_r = r;
    if(out_Np) *out_Np = Np;
    if(out_pf) memcpy(out_pf, pf, 3*pm->nlp*sizeof(double));
    // メモリ解放
    free(muX); free(muY); if(fx&&fy){free(mufx); free(mufy);}
    if(ssm) free(LQ); if(LQ0) free(LQ0);
    if(Ux) free(Ux); if(Uy) free(Uy);
    free(wd); free(x); free(a); free(u); free(R); free(sgm);
    free(wi); free(y); free(w); free(v); free(t); free(pf); if(e) free(e);
    return lp;
}
