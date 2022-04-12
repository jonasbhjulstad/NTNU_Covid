
#include <cstdlib>
#include <cmath>
#include <iostream>

#include <cvode/cvode.h>               // access CVODE fcts., consts.
#include <sunlinsol/sunlinsol_spgmr.h> // access the SPGMR SUNLinearSolver
#include <CL/sycl.hpp>
#include <nvector/nvector_sycl.h>
#include <sundials/sundials_sycl_policies.hpp>

#if defined(SUNDIALS_EXTENDED_PRECISION)
#define GSYM "Lg"
#define ESYM "Le"
#define FSYM "Lf"
#else
#define GSYM "g"
#define ESYM "e"
#define FSYM "f"
#endif

#if defined(SUNDIALS_INT64_T)
#define DSYM "ld"
#else
#define DSYM "d"
#endif

const realtype R0 = .4;
const realtype S0 = 1000000.;
const realtype I0 = 100.;
const realtype N_pop = S0 + I0;
const sunindextype Nt = 10;
// User data sstructure contains model and discretization parameters
struct UserData
{
    realtype alpha = .7;
    realtype beta = (R0 * alpha) / N_pop;
    uint32_t NX = 3;
    uint32_t NEQ = 2;
    sycl::queue *queue;
};

// Functions Called by the Solver
static int f(realtype t, N_Vector u, N_Vector udot, void *user_data);

static void PrintSIR(realtype t, sunrealtype *p_x, int nst);

// Private function to check function return values
static int check_retval(void *returnvalue, const char *funcname, int opt);

/* ---------------------------------------------------------------------------
 * Main Program
 * ---------------------------------------------------------------------------*/

int main(int argc, char **argv)
{
    // SUNDIALS simulation context
    sundials::Context sunctx;

    int retval;

    sycl::cpu_selector selector;
    sycl::queue queue = sycl::queue(selector, sycl::property_list{sycl::property::queue::in_order{}});

    UserData data;
    data.queue = &queue;
    sycl::device dev = queue.get_device();
    int size = dev.get_info<sycl::info::device::max_work_group_size>();

    sundials::sycl::ThreadDirectExecPolicy policy(size / 2);

    // Create a SYCL vector
    N_Vector u = N_VNew_Sycl(data.NEQ*data.NX, &queue, sunctx);
    N_VSetKernelExecPolicy_Sycl(u, &policy, &policy);
    if (check_retval((void *)u, "N_VNew_Sycl", 0))
        return 1;
    // Extract host pointer to solution vector data on the host
    realtype *udata = N_VGetArrayPointer(u);

    // Load initial profile into u vector
    for (sunindextype eq_i = 0; eq_i < data.NEQ; eq_i++)
    {

        udata[data.NX * eq_i] = S0;
        udata[data.NX * eq_i + 1] = I0;
        udata[data.NX * eq_i + 2] = .0;
    }

    // Copy data to device
    N_VCopyToDevice_Sycl(u);

    realtype S, I, R;
    S = udata[0];
    I = udata[1];
    R = udata[2];

    // Create CVODE and specify the Backward Differentiation Formula
    void *cvode_mem = CVodeCreate(CV_BDF, sunctx);
    if (check_retval((void *)cvode_mem, "CVodeCreate", 0))
        return 1;

    const sunrealtype t0 = 0.;
    // Specify the right hand side function in f(t,u), initial condition (t0, u0)
    retval = CVodeInit(cvode_mem, f, t0, u);
    if (check_retval(&retval, "CVodeInit", 1))
        return 1;

    // Specify the scalar relative tolerance and scalar absolute tolerance
    realtype reltol = 1e-1;
    realtype abstol = 1e-1;
    retval = CVodeSStolerances(cvode_mem, reltol, abstol);
    if (check_retval(&retval, "CVodeSStolerances", 1))
        return 1;

    // Set the pointer to user-defined data
    retval = CVodeSetUserData(cvode_mem, &data);
    if (check_retval(&retval, "CVodeSetUserData", 1))
        return 1;

    // Create SPGMR solver without preconditioning and default Krylov dimension
    SUNLinearSolver LS = SUNLinSol_SPGMR(u, SUN_PREC_NONE, 0, sunctx);
    if (check_retval(&retval, "SUNLinSol_SPGMR", 1))
        return 1;

    // Attach the linear sovler to CVODE
    retval = CVodeSetLinearSolver(cvode_mem, LS, NULL);
    if (check_retval(&retval, "CVodeSetLinearSolver", 1))
        return 1;

    // Set the Jacobian-times-vector function
    // retval = CVodeSetJacTimes(cvode_mem, NULL, jtv);
    // if (check_retval(&retval, "CVodeSetJacTimesVecFn", 1))
    // return 1;
    const realtype dt = .1;
    realtype tout = dt; // output time
    realtype t;              // CVODE return time
    long int nst;            // number of time steps

    for (int iout = 0; iout < Nt; iout++)
    {
        // Advance in time
        retval = CVode(cvode_mem, tout, u, &t, CV_NORMAL);
        // if (check_retval(&retval, "CVode", 1))
        //     break;

        // Output status
        retval = CVodeGetNumSteps(cvode_mem, &nst);
        // if (check_retval(&retval, "CVodeGetNumSteps", 1))
        //     break;

        udata = N_VGetArrayPointer(u);

        PrintSIR(t, udata, nst);

        // Update output time
        tout += dt;
    }

    N_VDestroy(u);         // Free the u vector
    CVodeFree(&cvode_mem); // Free the integrator memory
    SUNLinSolFree(LS);     // Free linear solver memory

    return 0;
}

/* ---------------------------------------------------------------------------
 * Functions called by the solver
 * ---------------------------------------------------------------------------*/

static int f(realtype t, N_Vector y, N_Vector ydot, void *user_data)
{

    UserData *data = static_cast<UserData *>(user_data);

    realtype alpha = data->alpha;
    realtype beta = data->beta;

    const realtype *p_y = N_VGetArrayPointer(y);
    realtype *p_ydot = N_VGetArrayPointer(ydot);
    uint32_t NX = data->NX;
    uint32_t NEQ = data->NEQ;

    realtype S, I, R;
    S = p_y[0];
    I = p_y[1];
    R = p_y[2];

    
    data->queue->submit([&](sycl::handler &h)
                        { h.parallel_for(sycl::range{NEQ}, [=](sycl::id<1> idx)
                                         {
        realtype S = p_y[3*idx[0]];
        realtype I = p_y[3*idx[0] + 1];
        realtype R = p_y[3*idx[0] + 2];

        p_ydot[3*idx[0]] = -beta * S * I;
        p_ydot[3*idx[0] + 1] = beta * S * I - alpha * I;
        p_ydot[3*idx[0] + 2] = alpha * I; }); });
    return 0;
}


// Print current value
static void PrintSIR(realtype t, sunrealtype *p_x, int nst)
{
    std::cout << "At t = " << t
              << "\tS = " << p_x[0] << "\tI = " << p_x[1] << "\tR = " << p_x[2] << "\tStep = " << nst << std::endl;
    return;
}

/* Check function return value...
     opt == 0 means SUNDIALS function allocates memory so check if
              returned NULL pointer
     opt == 1 means SUNDIALS function returns an integer value so check if
              retval >= 0
     opt == 2 means function allocates memory so check if returned
              NULL pointer */
static int check_retval(void *returnvalue, const char *funcname, int opt)
{
    int *retval;

    if (opt == 0 && returnvalue == NULL)
    {
        // Check if SUNDIALS function returned NULL pointer - no memory allocated
        std::cerr << "\nSUNDIALS_ERROR: " << funcname
                  << " failed - returned NULL pointer\n\n";
        return 1;
    }
    else if (opt == 1)
    {
        // Check if retval < 0
        retval = static_cast<int *>(returnvalue);
        if (*retval < 0)
        {
            std::cerr << "\nSUNDIALS_ERROR: " << funcname
                      << " failed with retval = " << *retval << "\n\n";
            return 1;
        }
    }
    else if (opt == 2 && returnvalue == NULL)
    {
        // Check if function returned NULL pointer - no memory allocated
        std::cerr << "\nMEMORY_ERROR: " << funcname
                  << " failed - returned NULL pointer\n\n";
        return 1;
    }

    return 0;
}