#include <SIR_Stochastic.cl>
#define X_TRAJECTORY_RANGE_MAX 100


void assignVec(float* a, constant float* b, uint N)
{
    for (int i = 0; i < N; i++)
    {
        a[i] = b[i];
    }
}

void assignVec_g(global float* a, float* b, uint N)
{
    for (int i = 0; i < N; i++)
    {
        a[i] = b[i];
    }
}

__kernel void SIR_Compute_Stochastic(constant ulong* seeds, constant float* x0, global float* x_traj_g, float dt, uint Nt, constant float* param)
{

    float x_traj[3*X_TRAJECTORY_RANGE_MAX+1];
    for (int i = 0; i < 3*Nt; i++)
    {
        x_traj[i]= 0;
    }

    assignVec(x_traj, x0, 3);

    uint id = get_local_id(0);
    uint work_dim = get_work_dim();
    local PRNG_STATE state;
    for (int i = 0; i < 10; i++)
    {
	    PRNG_SEED_FN(&state, seeds[id]);
    }

    for (uint i = 0; i < Nt; i++)
    {
        SIR_Stochastic_Binomial(&x_traj[3*i], &x_traj[3*(i+1)], param, dt, &state);
    }

    assignVec_g(&x_traj_g[3*(Nt+1)*id], x_traj, 3*(Nt+1));
}