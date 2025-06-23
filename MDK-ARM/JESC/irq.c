#include "irq.h"

FOC foc;
float addtheta = 0.01;
float adcRaw[3];
uint8_t Run_Flag = 0;
void HAL_ADCEx_InjectedConvCpltCallback(ADC_HandleTypeDef *hadc)
{
	//HAL_TIM_OC_Stop_IT(&htim1, TIM_CHANNEL_4);              /*关闭捕获中断*/
	
	 if (!Run_Flag)
	 {		
		htim1.Instance->CCR3 = 0;
		htim1.Instance->CCR2 = 0;
		htim1.Instance->CCR1 = 0;
		foc.VfAngleAdd  = 0;
		foc.VfAngle     = 0;	
		foc.StartVfVq   = 0;
		foc.VfAngleTimeCount = 0;
		foc.VfVqTimeCount = 0;
		foc.tick = 0;
		return;
	 }

	// 三相电流采样
	foc.RawA = hadc1.Instance->JDR1;
	foc.RawB = hadc1.Instance->JDR2;
	foc.RawC = hadc1.Instance->JDR3;
	
	// 用于输出串口波形
	// adcRaw[0] = foc.RawA - 2045.0;
	// adcRaw[1] = foc.RawB - 2045.0;
	// adcRaw[2] = foc.RawC - 2045.0;
	// 电流转换
	// 3.3V 12bitADC 4096 0.0005R
	foc.ia = (float)(foc.RawA - 2045.0f) / 4095.0f * 3.3f * 0.0005f / 20.0f;
	foc.ib = (float)(foc.RawB - 2045.0f) / 4095.0f * 3.3f * 0.0005f / 20.0f;
	foc.ic = (float)(foc.RawC - 2045.0f) / 4095.0f * 3.3f * 0.0005f / 20.0f;

	


	fast_sin_cos(foc.theta, &foc.cos, &foc.sin);

	// clarke
	foc.alpha = foc.ia;
	foc.beta = (foc.ia + 2 * foc.ib) * ONE_BY_SQRT3;

	// park
	foc.d = foc.alpha * foc.cos + foc.beta * foc.sin;
	foc.q = -foc.alpha * foc.sin + foc.beta * foc.cos;

	// q pid
	// q pid

	VF_Run(&foc);
	
	
	// ipark
	foc.u_alpha = foc.outd * foc.cos - foc.outq * foc.sin;
	foc.u_beta = foc.outq * foc.cos + foc.outd * foc.sin;

	foc.PWMFullDutyCycle = TIM1->ARR;
	// svpwm
	foc_svm(foc.u_alpha, foc.u_beta, foc.PWMFullDutyCycle,
			&foc.tAout, &foc.tBout, &foc.tCout, &foc.svm_sector);

	htim1.Instance->CCR3 = foc.tAout;
	htim1.Instance->CCR2 = foc.tBout;
	htim1.Instance->CCR1 = foc.tCout;

	//	Vofa_JustFloat(adcRaw,3);
	//HAL_TIM_OC_Start_IT(&htim1, TIM_CHANNEL_4); /*开启捕获中断*/
}


