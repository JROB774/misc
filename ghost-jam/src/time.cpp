namespace internal
{
    float target_seconds;

    u64 frequency;
    u64 end_counter;
    u64 last_counter;
    u64 elapsed_counter;
}

FILDEF void setup_delta_timer ()
{
    internal::target_seconds = 1.0f / REFRESH_RATE;
    internal::frequency = SDL_GetPerformanceFrequency();

    internal::end_counter = 0;
    internal::last_counter = SDL_GetPerformanceCounter();
}

FILDEF void cap_current_frame ()
{
    internal::end_counter = SDL_GetPerformanceCounter();
    internal::elapsed_counter = internal::end_counter - internal::last_counter;

    float elapsed_seconds = CAST(float, internal::elapsed_counter) / CAST(float, internal::frequency);
    float elapsed_seconds_final = elapsed_seconds;

    if (elapsed_seconds_final < internal::target_seconds) {
        while (elapsed_seconds_final < internal::target_seconds) {
            u32 sleep = CAST(u32, 1000.0f * (internal::target_seconds - elapsed_seconds_final));
            if (sleep > 0) {
                SDL_Delay(sleep);
            }
            internal::elapsed_counter = SDL_GetPerformanceCounter() - internal::last_counter;
            elapsed_seconds_final = CAST(float, internal::elapsed_counter) / CAST(float, internal::frequency);
        }
    }
}

FILDEF void update_delta_time ()
{
    dt = CAST(float, internal::elapsed_counter) / CAST(float, internal::frequency);
    internal::last_counter = SDL_GetPerformanceCounter();
}
