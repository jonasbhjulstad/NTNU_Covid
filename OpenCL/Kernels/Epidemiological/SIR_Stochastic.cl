#define PRNG_STATE xorshift1024_state
#define PRNG_SEED_FN xorshift1024_seed
#define PRNG_FLOAT_FN xorshift1024_float
#include "../Distributions/Distributions.cl"
#define SIR_BINOMIAL_TOLERANCE 10

void SIR_Stochastic_Binomial(float* x, float* x_next, constant float* param, float dt, local PRNG_STATE* state)
{
    float alpha = param[0];
    float beta = param[1];
    float N_pop = param[2];

    float S = x[0];
    float I = x[1];
    float R = x[2];

    //print state
    // printf("S: %f, I: %f, R: %f\n", S, I, R);
    float p_I = 1-exp(-beta*I/N_pop*dt);
    float p_R = 1-exp(-alpha*dt);
    float k_SI;

    if (S*p_I > SIR_BINOMIAL_TOLERANCE)
    {
        k_SI = PoissonSample(state, S*p_I);
    }
    else
    {
        k_SI = BinomialSample(state, S, p_I);
    }
    // printf("k_SI: %.4f ", k_SI);

    float k_IR = 0;
    k_IR = BinomialSample(state, I, p_R);
    // printf("k_SI: %i\n", k_SI);
    // printf("I: %i\n", I);
    float S_1 = S - k_SI;
    float I_1 = I + k_SI - k_IR;
    float R_1 = R + k_IR;
    // printf("S: %i   I: %i   R: %i    S_1: %i   I_1: %i   R_1:%i\n", S, I, R, S_1, I_1, R_1);

    x_next[0] = S - k_SI;
    x_next[1] = I + k_SI - k_IR;
    x_next[2] = R + k_IR;

}