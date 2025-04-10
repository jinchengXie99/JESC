#ifndef FOC__H
#define FOC__H


#include  "main.h"
#include "fast_sin.h"

// Constants
#define ONE_BY_SQRT3			(0.57735026919)
#define TWO_BY_SQRT3			(2.0f * 0.57735026919)
#define SQRT3_BY_2				(0.86602540378)
#define COS_30_DEG				(0.86602540378)
#define SIN_30_DEG				(0.5)
#define COS_MINUS_30_DEG		(0.86602540378)
#define SIN_MINUS_30_DEG		(-0.5)
#define ONE_BY_SQRT2			(0.7071067811865475)

void foc_svm(float alpha, float beta, uint32_t PWMFullDutyCycle,
                uint32_t* tAout, uint32_t* tBout, uint32_t* tCout, uint32_t *svm_sector);

typedef struct  {

	  uint32_t RawU;
    uint32_t RawV;
    uint32_t RawW;

	  float alpha;
	  float beta;

    float u_alpha;
  	float u_beta;

    float d;
    float q;

    float sin;
    float cos;
    float theta;

    uint32_t PWMFullDutyCycle;
    uint32_t tAout;
    uint32_t tBout;
    uint32_t tCout;
    uint32_t svm_sector;

}FOC;


#endif