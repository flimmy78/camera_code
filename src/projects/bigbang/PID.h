

#ifndef __PID_H__
#define __PID_H__

typedef struct PID
{
	float  SetPoint; 			//  �趨Ŀ�� Desired Value
	float SumError;			//	����ۼ� 
		
	double  Proportion;     //  �������� Proportional Const
	double  Integral;       //  ���ֳ��� Integral Const
	double  Derivative;     //  ΢�ֳ��� Derivative Const

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



//����ʽPID
int IncPIDCalc(int NextPoint,PID *sptr);


//λ��ʽPID
float LocPIDCalc(float NextPoint,PID *sptr);






#endif
