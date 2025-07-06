typedef struct bcpd_pairwise_size  {int D; int Df; int M; int N; int K; int J;} bcpd_pwsz;
typedef struct bcpd_pairwise_param {char fn[8][256]; char nrm; char nrf; int opt;
  int nlp; int G; double dlt; double omg; double gma; double fpv;
  int llp; int J; double lim; double lmd; double bet;
  int rns; int K; double btn; double kpa; double cnv;
  int dwn[2]; double dwr[2]; int nnk; double nnr; double tau; double eps;
} bcpd_pwpm;
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
);
