#ifndef __VOFA_H
#define __VOFA_H
#include <stdint.h>
#include <stdio.h>
#include "usbd_cdc_if.h"
#include <stdarg.h>


// 按printf格式写，最后必须加\r\n
void Vofa_FireWater(const char *format, ...);
// 输入个数和数组地址
void Vofa_JustFloat(float *_data, uint8_t _num);

/*...........示例..............
    float f1=0.5,f2=114.514;
    Vofa_FireWater("%f,%f\r\n", f1, f2);

    float f3[3]={88.77,0.66,55.44};
    Vofa_JustFloat(f3, 3);
*/
#endif 

