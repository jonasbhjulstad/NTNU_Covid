#ifndef SIR_DETERMINISTIC_CL
#define SIR_DETERMINISTIC_CL

void SIR_Deterministic(float t, float* x, float* x_next, float* params);
{
    float alpha = params[0];
    float beta = params[1];
    float N_pop = params[2];

    x_next[0] = -beta*x[0]*x[1]/N_pop;
    x_next[1] = beta*x[0]*x[1]/N_pop - alpha*x[1];
    x_next[2] = alpha*x[1];
}



#endif