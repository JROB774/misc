/*////////////////////////////////////////////////////////////////////////////*/

namespace TCE
{

INLDEF float PerformanceCounterToSeconds (u64 _counter)
{
	return CAST(float, _counter) / CAST(float, performance_frequency);
}

INLDEF void UpdateGame (float _dt)
{
	// Update the timing for all the animations.
	UpdateAnimationTimers(_dt);
	// Call the main script's update function.
	LuaGameUpdate(_dt);
}

INLDEF void RenderGame ()
{
	// Clear the screen for a new frame render.
	RenderClear();
	// Call the main script's render function.
	LuaGameRender();
}

INLDEF void SetupGameTime ()
{
	performance_frequency = SDL_GetPerformanceFrequency();

	last_counter = SDL_GetPerformanceCounter();
	end_counter = 0;

	current_fps = 0.0f;
	fixed_time_step = 1.0f / refresh_rate;
	// Approximate time-step of a frame.
	delta_time = fixed_time_step;
}

INLDEF void UpdateTimeAndFlipBuffers ()
{
	end_counter = SDL_GetPerformanceCounter();
	u64 elapsed_counter = end_counter - last_counter;

	float elapsed_time = PerformanceCounterToSeconds(elapsed_counter);
	float final_elapsed_time = elapsed_time;

	// Sleep until it is time to handle the next frame.
	while (final_elapsed_time < fixed_time_step) {
		u32 sleep = CAST(u32, 1000.0f * (fixed_time_step - final_elapsed_time));
		if (sleep > 0) { SDL_Delay(sleep); }

		elapsed_counter = SDL_GetPerformanceCounter() - last_counter;
		final_elapsed_time = PerformanceCounterToSeconds(elapsed_counter);
	}

	SDL_RenderPresent(renderer);

	// Determine the current FPS which can then be passed to Lua.
	current_fps = CAST(float, performance_frequency) / CAST(float, elapsed_counter);

	last_counter = SDL_GetPerformanceCounter();
	delta_time = fixed_time_step;
}

} // TCE

/*////////////////////////////////////////////////////////////////////////////*/
