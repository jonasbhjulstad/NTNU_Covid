/* ---------------------------------------------------------------------------
 * Programmer(s): David J. Gardner @ LLNL
 * ---------------------------------------------------------------------------
 * SUNDIALS Copyright Start
 * Copyright (c) 2002-2022, Lawrence Livermore National Security
 * and Southern Methodist University.
 * All rights reserved.
 *
 * See the top-level LICENSE and NOTICE files for details.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * SUNDIALS Copyright End
 * ---------------------------------------------------------------------------
 * Example problem:
 *
 * The following is a simple example problem with a banded Jacobian, with the
 * program for its solution by CVODE. The problem is the semi-discrete form of
 * the advection-diffusion equation in 2-D:
 *
 *   u_t = u_xx + u_yy + 0.5 u_x
 *
 * on the rectangle 0 <= x <= 2, 0 <= y <= 1, and the time interval 0 <= t <= 1.
 * Homogeneous Dirichlet boundary conditions are posed, and the initial
 * condition is
 *
 *   u(x,y,0) = x (2-x) y (1-y) exp(5xy).
 *
 * The PDE is discretized on a uniform MX+2 by MY+2 grid with central
 * differencing, and with boundary values eliminated, leaving an ODE system of
 * size NEQ = MX*MY.
 *
 * This program solves the problem with the BDF method, Newton iteration with
 * the GMRES linear solver, and a user-supplied Jacobian-vector product routine.
 * It uses scalar relative and absolute tolerances.
 *
 * Output is printed at t = .1, .2, ..., 1. Run statistics (optional outputs)
 * are printed at the end.
 * ---------------------------------------------------------------------------*/

#include <cstdlib>
#include <cmath>
#include <iostream>

#include <cvode/cvode.h>          // access CVODE fcts., consts.
#include <nvector/nvector_sycl.h> // access the SYCL NVector
#include <sundials/sundials_sycl_policies.hpp>
#include <sunlinsol/sunlinsol_spgmr.h> // access the SPGMR SUNLinearSolver

#include <Eigen/Dense>

// Real Constants

#define ATOL RCONST(1.0e-5) // scalar absolute tolerance
#define T0 RCONST(0.0)      // initial time
#define T1 RCONST(0.1)      // first output time
#define DTOUT RCONST(0.1)   // output time increment
#define NOUT 10             // number of output times

#define ZERO RCONST(0.0)
#define HALF RCONST(0.5)
#define ONE RCONST(1.0)
#define TWO RCONST(2.0)
#define FIVE RCONST(5.0)

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

const realtype R0 = .7;
const realtype S0 = 10000.;
const realtype I0 = 100.;
// User data sstructure contains model and discretization parameters
struct UserData
{
    realtype alpha = .7;
    realtype beta = R0 * alpha;
    uint32_t NX = 3;
    uint32_t NEQ = 1;
    sycl::queue *queue;
};

// Functions Called by the Solver
static int f(realtype t, N_Vector u, N_Vector udot, void *user_data);

static void PrintOutput(realtype t, long int nst);
static void PrintFinalStats(void *cvode_mem);

// Private function to check function return values
static int check_retval(void *returnvalue, const char *funcname, int opt);

/* ---------------------------------------------------------------------------
 * Main Program
 * ---------------------------------------------------------------------------*/

int main(int argc, char **argv)
{
    // SUNDIALS simulation context
    sundials::Context sunctx;

    // return flag value
    int retval;

    // Create an in-order GPU queue
    sycl::cpu_selector selector;
    sycl::queue myQueue(selector,
                        sycl::property_list{sycl::property::queue::in_order{}});

    sycl::device dev = myQueue.get_device();

    int size = dev.get_info<sycl::info::device::max_work_group_size>();

    std::cout << "size: " << size << std::endl;

    std::cout << "Running on "
              << (dev.get_info<sycl::info::device::name>())
              << std::endl;

    // Create user data and set queue
    UserData data;
    data.queue = &myQueue;
    sundials::sycl::ThreadDirectExecPolicy policy(size / 2);

    // Create a SYCL vector
    N_Vector u = N_VNew_Sycl(data.NX * data.NEQ, &myQueue, sunctx);
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
        udata[data.NX * eq_i + 2] = R0;
    }

    // Copy data to device
    N_VCopyToDevice_Sycl(u);

    // Create CVODE and specify the Backward Differentiation Formula
    void *cvode_mem = CVodeCreate(CV_BDF, sunctx);
    if (check_retval((void *)cvode_mem, "CVodeCreate", 0))
        return 1;

    // Specify the right hand side function in f(t,u), initial condition (t0, u0)
    retval = CVodeInit(cvode_mem, f, T0, u);
    if (check_retval(&retval, "CVodeInit", 1))
        return 1;

    // Specify the scalar relative tolerance and scalar absolute tolerance
    realtype reltol = ZERO;
    realtype abstol = ATOL;
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


    realtype tout = T1; // output time
    realtype t;         // CVODE return time
    long int nst;       // number of time steps

    for (int iout = 0; iout < NOUT; iout++)
    {
        // Advance in time
        std::cout << "Vector created" << std::endl;

        retval = CVode(cvode_mem, tout, u, &t, CV_NORMAL);
        if (check_retval(&retval, "CVode", 1))
            break;

        // Output status
        retval = CVodeGetNumSteps(cvode_mem, &nst);
        if (check_retval(&retval, "CVodeGetNumSteps", 1))
            break;

        PrintOutput(t, nst);

        // Update output time
        tout += DTOUT;
    }

    PrintFinalStats(cvode_mem); // Print some final statistics

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

    realtype *p_y = N_VGetArrayPointer(y);
    realtype *p_ydot = N_VGetArrayPointer(ydot);
    uint32_t NX = data->NX;
    uint32_t NEQ = data->NEQ;
    realtype S, I, R;

    data->queue->submit([&](sycl::handler &h)
                        { h.parallel_for(sycl::range{NX, NEQ}, [=](sycl::id<2> idx)

                                         {
    sunindextype eq_idx = idx[0] + 3*idx[1];
    realtype S = p_y[eq_idx];
    realtype I = p_y[eq_idx + 1];
    realtype R = p_y[eq_idx + 2];
                           
    p_ydot[eq_idx] = -beta*S*I;
    p_ydot[eq_idx + 1] = beta*S*I - alpha*I;
    p_ydot[eq_idx + 2] = alpha*I; }); });
    return 0;
}



// Print current value
static void PrintOutput(realtype t, long int nst)
{
    std::cout << "At t = " << t
              << "   nst = " << nst << std::endl;
    return;
}

// Get and print some final statistics
static void PrintFinalStats(void *cvode_mem)
{
    long lenrw, leniw;
    long lenrwLS, leniwLS;
    long int nst, nfe, nsetups, nni, ncfn, netf;
    long int nli, npe, nps, ncfl, nfeLS;
    int retval;

    retval = CVodeGetWorkSpace(cvode_mem, &lenrw, &leniw);
    check_retval(&retval, "CVodeGetWorkSpace", 1);
    retval = CVodeGetNumSteps(cvode_mem, &nst);
    check_retval(&retval, "CVodeGetNumSteps", 1);
    retval = CVodeGetNumRhsEvals(cvode_mem, &nfe);
    check_retval(&retval, "CVodeGetNumRhsEvals", 1);
    retval = CVodeGetNumLinSolvSetups(cvode_mem, &nsetups);
    check_retval(&retval, "CVodeGetNumLinSolvSetups", 1);
    retval = CVodeGetNumErrTestFails(cvode_mem, &netf);
    check_retval(&retval, "CVodeGetNumErrTestFails", 1);
    retval = CVodeGetNumNonlinSolvIters(cvode_mem, &nni);
    check_retval(&retval, "CVodeGetNumNonlinSolvIters", 1);
    retval = CVodeGetNumNonlinSolvConvFails(cvode_mem, &ncfn);
    check_retval(&retval, "CVodeGetNumNonlinSolvConvFails", 1);

    retval = CVodeGetLinWorkSpace(cvode_mem, &lenrwLS, &leniwLS);
    check_retval(&retval, "CVodeGetLinWorkSpace", 1);
    retval = CVodeGetNumLinIters(cvode_mem, &nli);
    check_retval(&retval, "CVodeGetNumLinIters", 1);
    retval = CVodeGetNumPrecEvals(cvode_mem, &npe);
    check_retval(&retval, "CVodeGetNumPrecEvals", 1);
    retval = CVodeGetNumPrecSolves(cvode_mem, &nps);
    check_retval(&retval, "CVodeGetNumPrecSolves", 1);
    retval = CVodeGetNumLinConvFails(cvode_mem, &ncfl);
    check_retval(&retval, "CVodeGetNumLinConvFails", 1);
    retval = CVodeGetNumLinRhsEvals(cvode_mem, &nfeLS);
    check_retval(&retval, "CVodeGetNumLinRhsEvals", 1);

    std::cout << "\nFinal Statistics.. \n\n";
    std::cout << "lenrw   = " << lenrw << "     leniw   = " << leniw << "\n";
    std::cout << "lenrwLS = " << lenrwLS << "     leniwLS = " << leniwLS << "\n";
    std::cout << "nst     = " << nst << "\n";
    std::cout << "nfe     = " << nfe << "     nfeLS   = " << nfeLS << "\n";
    std::cout << "nni     = " << nni << "     nli     = " << nli << "\n";
    std::cout << "nsetups = " << nsetups << "     netf    = " << netf << "\n";
    std::cout << "npe     = " << npe << "     nps     = " << nps << "\n";
    std::cout << "ncfn    = " << ncfn << "     ncfl    = " << ncfl << "\n\n";

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