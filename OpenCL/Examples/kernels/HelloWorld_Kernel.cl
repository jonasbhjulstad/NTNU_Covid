#include <tyche_i.cl>

__kernel void HelloWorld(global uint* seed, global uint* out)
{
	uint id = get_global_id(0);
	tyche_i_state state;
	tyche_i_seed(&state, seed[id]);
	out[id] = tyche_i_uint(state);
}