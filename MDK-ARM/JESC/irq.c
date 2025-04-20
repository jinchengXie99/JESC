#include "irq.h"

FOC foc;
float addtheta=0.01;
float adcRaw[3];
uint8_t start;
void HAL_ADCEx_InjectedConvCpltCallback(ADC_HandleTypeDef *hadc)
{

  foc.RawA = hadc1.Instance->JDR1;
  foc.RawB = hadc1.Instance->JDR2;
  foc.RawC = hadc1.Instance->JDR3;
	adcRaw[0]= foc.RawA-2045.0;
	adcRaw[1]= foc.RawB-2045.0;
	adcRaw[2]= foc.RawC-2045.0; 
	
	foc.ia = (foc.RawA-2045.0)/4095*3.3f*0.0005/20;
	foc.ib = (foc.RawB-2045.0)/4095*3.3f*0.0005/20;
	foc.ic = (foc.RawC-2045.0)/4095*3.3f*0.0005/20;
	

	if(foc.tick++ < 0) // 初始位置定位 可提供初始位置的最大扭矩 在强拖中可做可不做
	{
		foc.theta = 0;
		foc.outd = 0.03;
		foc.outq = 0;
	}else if (foc.tick < 0)
	{
		foc.theta = 3.1415926535897f;
		foc.outd = 0.03;
		foc.outq = 0;
	
	}else  // 斜坡强拉
	{
		++foc.VfAngleTimeCount;
		++foc.VfVqTimeCount;
		if(foc.VfAngleTimeCount % 6==0)
		{
			foc.VfAngleTimeCount = 0;
			foc.VfAngleAdd += 0.0001;
		}
		if(foc.VfAngleAdd > 0.048f){
			foc.VfAngleAdd = 0.048f;
		}
		
		
		foc.VfAngle += foc.VfAngleAdd;
		
		
		if(foc.VfVqTimeCount % 56==0)
		{
			foc.VfVqTimeCount = 0;
			foc.StartVfVq += 0.002;
		}

		if(foc.StartVfVq > 0.12){
			foc.StartVfVq = 0.12;
		}
		
		foc.theta = foc.VfAngle;
		foc.outd = 0.0;
		foc.outq = foc.StartVfVq;
//		angle = motorParams->VfAngle;
//		vd_out = 0;
//		vq_out = motorParams->StartVfVq;
	}
	 
	
//	foc.theta +=addtheta;
	
	fast_sin_cos(foc.theta,&foc.cos,&foc.sin);
	
	// clarke
	foc.alpha = foc.ia;
	foc.beta = (foc.ib + 2 * foc.ib) * ONE_BY_SQRT3;
	
	
	
	
	//park
	foc.d = foc.alpha * foc.cos + foc.beta * foc.sin;
	foc.q = -foc.alpha * foc.sin + foc.beta * foc.cos;

	// q pid
	// q pid
	
	
	
	// ipark
	  foc.u_alpha = foc.outd * foc.cos - foc.outq * foc.sin;
    foc.u_beta = foc.outq * foc.cos + foc.outd * foc.sin;
		
		
	foc.PWMFullDutyCycle = TIM1->ARR;
	// svpwm
	 foc_svm(foc.u_alpha, foc.u_beta,  foc.PWMFullDutyCycle,
                &foc.tAout,  &foc.tBout,  &foc.tCout,  &foc.svm_sector);
	
	htim1.Instance->CCR3 = foc.tAout;
	htim1.Instance->CCR2 = foc.tBout;
	htim1.Instance->CCR1 = foc.tCout;

//	Vofa_JustFloat(adcRaw,3);
}