/**
* @file         main.c
* @brief		主函数运行
* @details	    None
* @author		JW
* @date		    date
* @par Copyright(c):2003-2023, Hangzhou SYSTEM Technology Co., Ltd.
	www.systech.com.cn
*/

/* Includes ------------------------------------------------------------------*/
/* 头文件引用 */
#include "n32g45x.h"
#include "N32_devInit.h"
#include "f_Main.h"
#include "MotorMain.h"
#include "CpuTime.h"
#include "UserCtrlParaList.h"



/*******************************************************************************
  函数名:  main
  输入:    
  输出:    
  子函数:         
  描述:  主函数
********************************************************************************/
int main(void)
{
    uint8_t  u8LoopFlag;
    int16_t  s16BaseTime,s16DeltaTime;
	
	//关中断
	DINT;
	
    /* 外设初始化P1 */
    BSP_DeviceInitPart1(); 
	
	/* 初始化延迟 给外部电平建立时间 大概60MS */
    for(uint32_t delay = 0; delay < 1000000; delay++){};

	/* FUNCODE Init */
    vInitFuncCode();

    /* 后台任务参数初始化 */
    vBackgroundTaskParaInit();
		
	/* 标幺化 */
	strFuncParaList.pvSystemUnitInit(&strSystemPerUint);

	/* 外设初始化2 */
	BSP_DeviceInitPart2();

    /* 电机控制参数初始化 */
	for(uint8_t i = 0; i < MOTOR_AXIS; i++)
	{
		strMotorMainFun.pvMotorFocInit(&strMotorFoc[i], i);
	}
	
    /*!< 取当前时间计数值，用于计算任务时间 */    
	s16BaseTime = GetTime();
		
	/*初始化完成，开启看门狗*/
//    BSP_WtdInit();
		
    /* 开启所有中断 */
	EINT;
			
    /* 主循环 */
    while(1)
    {
        s16DeltaTime = s16BaseTime - GetTime();
        if(s16DeltaTime > C_TIME_500US)
        {
            strCpuTimeFun.pvCpuTimeInit(&strCpuTime.Main05Ms);//任务运行花费时间的检测

            u8LoopFlag ++;
            s16BaseTime -= C_TIME_500US;
            
            /** 4*0.5ms 组成2ms周期 函数 */
			if((u8LoopFlag & 0x03) == 0)
			{
                strAppMainFun.pvMain05msFuncPA();
            }	
			else if((u8LoopFlag & 0x03) == 1)
			{
                strAppMainFun.pvMain05msFuncPB();
			}
			else if((u8LoopFlag & 0x03) == 2)
			{
                strAppMainFun.pvMain05msFuncPC();
			}
			else if((u8LoopFlag & 0x03) == 3)
			{
                strAppMainFun.pvMain05msFuncPD();
			}
            /* 0.5Ms 任务 */
            strAppMainFun.pvMain05msFunc();
            
            strCpuTimeFun.pvCpuTimeTaskCalc(&strCpuTime.Main05Ms);
        }
    }
}


// EOF

