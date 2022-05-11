#include <tyche.cl>
#include <xorshift1024.cl>
#include <mt19937.cl>
void UniformSample(PRNG_STATE* state, float min, float max, uint N, float* res)
{
    for (int i = 0; i < N; i++)
    {
        res[i] = PRNG_FLOAT_FN(*state);
    }
}

bool BernoulliTrial(PRNG_STATE* state, float p)
{
    float r;
    UniformSample(state, 0.0f, 1.0f, 1, &r);
    return r < p;
}


uint BinomialSample(PRNG_STATE* state, const uint N, float p)
{
    float uSample = 1.f;
    uint count = 0;
    for (uint i = 0; i < N; i++)
    {
        UniformSample(state, 0, 1, 1, &uSample);
        // printf("%f\n", uSample);
        if (uSample < p)
        {
            count++;
        }
    }
    return count;
}

float PoissonSample(PRNG_STATE* state, float lambda)
{
    float x = 0;
    float p = -lambda;
    float s = 1;
    float u;
    // printf("p: %e, lbd: %f", p, lambda);
    while(s > p)
    {
        UniformSample(state, 0, 1, 1, &u);
        x++;
        s = s + log(u);
    }
    // printf("x: %f\n", x);
    return x-1;
}

