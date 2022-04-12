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

#include <cvode/cvode.h> // access CVODE fcts., consts.
#include <sunlinsol/sunlinsol_spgmr.h> // access the SPGMR SUNLinearSolver
#include <nvector/nvector_serial.h>
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


const realtype R0 = .4;
const realtype S0 = 1000000.;
const realtype I0 = 100.;
const realtype N_pop = S0 + I0 + R0;
// User data sstructure contains model and discretization parameters
struct UserData
{
    realtype alpha = .7;
    realtype beta = (R0 * alpha)/N_pop;
    uint32_t NX = 3;
    uint32_t NEQ = 1;
};

// Functions Called by the Solver
static int f(realtype t, N_Vector u, N_Vector udot, void *user_data);

static void PrintSIR(realtype t, sunrealtype* p_x, int nst);

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

    // Create user data and set queue
    UserData data;

    // Create a SYCL vector
    N_Vector u = N_VNew_Serial(data.NEQ*data.NX, sunctx);
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

    PrintSIR(0, udata, 0);

    for (int iout = 0; iout < NOUT; iout++)
    {
        // Advance in time
        retval = CVode(cvode_mem, tout, u, &t, CV_NORMAL);
        if (check_retval(&retval, "CVode", 1))
            break;

        // Output status
        retval = CVodeGetNumSteps(cvode_mem, &nst);
        if (check_retval(&retval, "CVodeGetNumSteps", 1))
            break;

        sunrealtype* p_u = N_VGetArrayPointer(u);

        PrintSIR(t, p_u, nst);

        // Update output time
        tout += DTOUT;
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

    realtype *p_y = N_VGetArrayPointer(y);
    realtype *p_ydot = N_VGetArrayPointer(ydot);
    uint32_t NX = data->NX;
    uint32_t NEQ = data->NEQ;
    realtype S, I, R;

    for (int eq_idx= 0; eq_idx< NEQ; eq_idx++)
    {
        sunindextype idx = 3 * eq_idx;
        realtype S = p_y[idx];
        realtype I = p_y[idx + 1];
        realtype R = p_y[idx + 2];

        p_ydot[idx] = -beta * S * I;
        p_ydot[idx + 1] = beta * S * I - alpha * I;
        p_ydot[idx + 2] = alpha * I;
    }
return 0;
}

// Print current value
static void PrintSIR(realtype t, sunrealtype* p_x, int nst)
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