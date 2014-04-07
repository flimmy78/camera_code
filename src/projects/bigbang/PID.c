

#include "PID.h"

void PIDInit(PID * sptr)
{
    sptr->LastError  = 0;			//Error[-1]
    sptr->PrevError  = 0;			//Error[-2]
	sptr->Proportion = 0;			//比例常数 Proportional Const
    sptr->Integral   = 0;			//积分常数 Integral Const
    sptr->Derivative = 0;			//微分常数 Derivative Const
    sptr->SetPoint   = 0;
	sptr->SumError   = 0;
}



void PIDSetPoint(int setpoint,PID * sptr)
{	sptr->SetPoint = setpoint;	}

int PIDGetSetpoint(PID *sptr)
{	return(sptr->SetPoint);	}

void PIDSetKp(double dKpp,PID * sptr)
{	sptr->Proportion = dKpp;	}

double PIDGetKp(PID * sptr)
{	return(sptr->Proportion);	}
void PIDSetKi(double dKii,PID * sptr)
{	sptr->Integral = dKii;	}

double PIDGetKi(PID * sptr)
{	return(sptr->Integral);	}

void PIDSetKd(double dKdd,PID *sptr)
{	sptr->Derivative = dKdd;	}

double PIDGetKd(PID *sptr)
{	return(sptr->Derivative);	}




//增量式PID
int IncPIDCalc(int NextPoint,PID *sptr)
{
	register int iError, iIncpid;

	iError = sptr->SetPoint - NextPoint;

	iIncpid = sptr->Proportion * iError				//E[0]
            - sptr->Integral   * sptr->LastError	//E[-1]
            + sptr->Derivative * sptr->PrevError;	//E[-2]

	sptr->PrevError = sptr->LastError;
	sptr->LastError = iError;

	return(iIncpid);
}



//位置式PID
float LocPIDCalc(float NextPoint,PID * sptr)
{
    register float  iError,dError;
	
	iError = sptr->SetPoint - NextPoint;      	// 偏差
	sptr->SumError += iError;					// 积分
	dError = iError - sptr->LastError; 			// 当前微分
	sptr->LastError = iError;

	return(sptr->Proportion * iError           	// 比例项
           + sptr->Integral * sptr->SumError 	// 积分项
           + sptr->Derivative * dError);
}

