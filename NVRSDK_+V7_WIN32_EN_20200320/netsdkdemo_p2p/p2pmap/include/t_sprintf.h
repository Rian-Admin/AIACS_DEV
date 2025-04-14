//
//  t_sprintf.h
//  HKSPlayerDemo
//
//  Created by fred on 2017/7/21.
//

#ifndef t_sprintf_h
#define t_sprintf_h

#ifdef __cplusplus
extern "C" {
#endif

    int t_printf(const char *format, ...);

    int t_sprintf(char *buf, const char *format, ...);

    int t_snprintf( char *buf, unsigned int count, const char *format, ... );

#ifdef __cplusplus
}
#endif

#endif /* t_sprintf_h */
