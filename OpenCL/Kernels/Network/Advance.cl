#include "../Distributions/Distributions.cl"
// Requires:
// #define NX
// #define NV

void Network_Advance_Discrete(float* v_current, 
                            float* v_next, 
                            constant bool* vec_A, 
                            size_t N_A, 
                            constant float seed,
                            constant float* P)
{
    PRNG_STATE state;
    PRNG_SEED_FN(&state, seed);

    for (int a = 0; a < N_A; a++)
    {
        size_t source_node_offset = vec_A[2*a]*NX;
        size_t target_node_offset = vec_A[2*a+1]*NX;
        for (size_t state_idx = 0; state_idx < NX; state_idx++)
        {
            if(P[state_idx] != 0.) && BernoulliTrial(&state, P[state_idx]))
            {
                v_next[source_node_offset + state_idx] -= 1.;
                v_next[target_node_offset + state_idx] += 1.;
            }
        }
        
    }
    
}



void Network_Advance_ODE(float* v_current, 
                            float* v_next, 
                            constant bool* vec_A, 
                            size_t N_A,
                            float dt,
                            constant float* P)
{
    PRNG_STATE state;
    PRNG_SEED_FN(&state, seed);

    for (int a = 0; a < N_A; a++)
    {
        size_t source_node_offset = vec_A[2*a]*NX;
        size_t target_node_offset = vec_A[2*a+1]*NX;

        float* source_node_current = &v_current[source_node_offset];
        float* source_node_next = &v_next[source_node_offset];
        float* source_node_current = &v_current[source_node_offset];
        float* source_node_next = &v_next[source_node_offset];

        

    }
    
}


void Network_Advance_Discrete(float* v_current, 
                            float* v_next, 
                            constant bool* vec_A, 
                            size_t N_A, 
                            size_t Nx, 
                            constant float seed,
                            constant float* P)
{
    PRNG_STATE state;
    PRNG_SEED_FN(&state, seed);

    for (int a = 0; a < N_A; a++)
    {
        size_t source_node_offset = vec_A[2*a]*NX;
        size_t target_node_offset = vec_A[2*a+1]*NX;
        for (size_t state_idx = 0; state_idx < NX; state_idx++)
        {
            if(P[state_idx] != 0.) && BernoulliTrial(&state, P[state_idx]))
            {
                v_next[source_node_offset + state_idx] -= 1.;
                v_next[target_node_offset + state_idx] += 1.;
            }
        }
        
    }
    
}