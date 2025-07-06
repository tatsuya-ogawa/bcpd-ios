//
//  pcpd.m
//  bcpd-ios-app
//
//  Created by Tatsuya Ogawa on 2025/07/06.
//

#include "lib_bcpd.h"
#import <Foundation/Foundation.h>

@interface BCPD : NSObject

- (int)execute_bcpWithTargetPoints:(double *)X targetCount:(int)N
                      sourcePoints:(double *)Y sourceCount:(int)M
                         dimension:(int)D
                   targetFeatures:(double *)fx featureDimension:(int)Df
                   sourceFeatures:(double *)fy
                        parameters:(bcpd_pwpm *)bcpd_pm
                         sizeInfo:(bcpd_pwsz *)bcpd_sz
                          results:(NSDictionary **)results;

@end

@implementation BCPD

- (int)execute_bcpWithTargetPoints:(double *)X targetCount:(int)N
                      sourcePoints:(double *)Y sourceCount:(int)M
                         dimension:(int)D
                   targetFeatures:(double *)fx featureDimension:(int)Df
                   sourceFeatures:(double *)fy
                        parameters:(bcpd_pwpm *)bcpd_pm
                         sizeInfo:(bcpd_pwsz *)bcpd_sz
                          results:(NSDictionary **)results {
    
    // 結果格納用のメモリを割り当て
    double *out_y = malloc(M * D * sizeof(double));
    double *out_x = malloc(M * D * sizeof(double));
    double *out_v = malloc(M * D * sizeof(double));
    double *out_a = malloc(M * sizeof(double));
    double *out_sgm = malloc(M * sizeof(double));
    double *out_R = malloc(D * D * sizeof(double));
    double *out_t = malloc(D * sizeof(double));
    double *out_s = malloc(sizeof(double));
    double *out_r = malloc(sizeof(double));
    double *out_Np = malloc(sizeof(double));
    double *out_pf = malloc(3 * bcpd_pm->nlp * sizeof(double));
    
    // BCPD実行
    int result = bcpd_execute(X, N, Y, M, D, fx, Df, fy, bcpd_pm, bcpd_sz,
                             out_y, out_x, out_v, out_a, out_sgm, out_R, out_t,
                             out_s, out_r, out_Np, out_pf);
    
    // 結果をNSDictionaryに格納
    if (results != NULL) {
        NSMutableDictionary *resultDict = [NSMutableDictionary dictionary];
        
        // NSDataとして結果を格納
        resultDict[@"y"] = [NSData dataWithBytes:out_y length:M * D * sizeof(double)];
        resultDict[@"x"] = [NSData dataWithBytes:out_x length:M * D * sizeof(double)];
        resultDict[@"v"] = [NSData dataWithBytes:out_v length:M * D * sizeof(double)];
        resultDict[@"a"] = [NSData dataWithBytes:out_a length:M * sizeof(double)];
        resultDict[@"sgm"] = [NSData dataWithBytes:out_sgm length:M * sizeof(double)];
        resultDict[@"R"] = [NSData dataWithBytes:out_R length:D * D * sizeof(double)];
        resultDict[@"t"] = [NSData dataWithBytes:out_t length:D * sizeof(double)];
        resultDict[@"s"] = [NSData dataWithBytes:out_s length:sizeof(double)];
        resultDict[@"r"] = [NSData dataWithBytes:out_r length:sizeof(double)];
        resultDict[@"Np"] = [NSData dataWithBytes:out_Np length:sizeof(double)];
        resultDict[@"pf"] = [NSData dataWithBytes:out_pf length:3 * bcpd_pm->nlp * sizeof(double)];
        
        *results = [resultDict copy];
    }
    
    // メモリ解放
    free(out_y);
    free(out_x);
    free(out_v);
    free(out_a);
    free(out_sgm);
    free(out_R);
    free(out_t);
    free(out_s);
    free(out_r);
    free(out_Np);
    free(out_pf);
    
    return result;
}

@end

