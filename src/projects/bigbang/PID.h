

#ifndef __PID_H__
#define __PID_H__

typedef struct PID
{
	float  SetPoint; 			//  设定目标 Desired Value
	float SumError;			//	误差累计 
		
	double  Proportion;     //  比例常数 Proportional Const
	double  Integral;       //  积分常数 Integral Const
	double  Derivative;     //  微分常数 Derivative Const

	float LastError;         	//  Error[-1]
	float PrevError;          //  Error[-2]

} PID;

//static PID	 sPID;
//static PID	 *sptr = &sPID;
void PIDInit(PID * sptr);


void PIDSetPoint(int setpoint,PID * sptr);

int PIDGetSetpoint(PID *sptr);
void PIDSetKp(double dKpp,PID * sptr);

double PIDGetKp(PID * sptr);
void PIDSetKi(double dKii,PID * sptr);
double PIDGetKi(PID * sptr);

void PIDSetKd(double dKdd,PID *sptr);
double PIDGetKd(PID *sptr);



//增量式PID
int IncPIDCalc(int NextPoint,PID *sptr);


//位置式PID
float LocPIDCalc(float NextPoint,PID *sptr);






#endif
