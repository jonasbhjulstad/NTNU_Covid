#ifndef ERK_CL
#define ERK_CL
#ifndef N_ERK_STAGES
#define N_ERK_STAGES 4
#endif
#include "../Epidemiological/SIR_Deterministic.cl"
//Requires
// #define NX
// #define N_ERK_STAGES
// declaration of void f_ODE(float t, float* x, float* P)

void f_ODE(float t, float* x, float* P, float* x_next)
{
	SIR_Deterministic(t, x, P, x_next);
}

void ERK(float* x_current, 
		float* x_next,
		float t, 
		float dt, 
		float* A, 
		float* b, 
		float* c,
		float* P)
{
	float K[N_ERK_STAGES*NX];
	f_ODE(f, x_current, P, K);

	for(size_t i = 1; i < N_ERK_STAGES; i++)
	{
		float* k_i = &K[i*NX];
		float c_i = c[i];
		
		float ak_sum[NX];
		for(size_t j = 0; j < NX; j++)
		{
			ak_sum[j] = x_current[j];
		}	
		for (size_t j = 0; j < i; j++)
		{
			ak_sum += A[i*N_ERK_STAGES + j] * K[j*NX];
		}
		f_ODE(t + c_i*dt, x_current + dt*(ak_sum), P, k_i);

		for (size_t j = 0; j < NX; j++)
		{
			x_next[j] += dt*b[i]*k_i[j];
		}
	}
}

#endif
	
