__kernel void ERK4(double t, double dt, double* x, uint Nx)
{
    xk[Nx];
	f_ODE(t, x, xk, p);

	// k1
	double k1[Nx];
	for (int i = 0; i < Nx; ++i)
	{
		k1[i] = xk[i];
	}

	// k2
	double x2[Nx];
	for (int i = 0; i < Nx; ++i)
	{
		x2[i] = y[i] + (dt * 0.50)*k1[i];
	}
	
	f_ODE(t + 0.50*dt, x2, k1, p);
	for (int i = 0; i < Nx; ++i)
	{
		xk[i] = xk[i] + 2.00 * k1[i];
	}

	// k3
	for (int i = 0; i < Nx; ++i)
	{
		x2[i] = y[i] + (dt * 0.50)*k1[i];
	}
	f_ODE(t + 0.50*dt, x2, k1, p);

	for (int i = 0; i < Nx; ++i)
	{
		xk[i] = xk[i] + 2.00 * k1[i];
	}

	// k4
	for (int i = 0; i < Nx; ++i)
	{
		x2[i] = y[i] + dt*k1[i];
	}
	f_ODE(t+dt, x2, k1, p);

	for (int i = 0; i < Nx; ++i)
	{
		xk[i] = y[i] + (xk[i] + k1[i]) * dt / 6.00;
	}
}
