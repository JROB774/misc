/*******************************************************************************
 * GAME
 * Handles the core looping systems for the game (input, update, render).
 *
*******************************************************************************/

#ifndef __TCE_GAME_H__ /*/////////////////////////////////////////////////////*/
#define __TCE_GAME_H__

namespace TCE
{

// This needs a default value in case the variable tweak value cannot be loaded!
GLOBAL float refresh_rate = 60.0f;

GLOBAL u64 performance_frequency;

GLOBAL u64 last_counter;
GLOBAL u64 end_counter;

GLOBAL float current_fps;
GLOBAL float fixed_time_step;
GLOBAL float delta_time;

INLDEF float PerformanceCounterToSeconds (u64 _counter);

INLDEF void UpdateGame (float _dt);
INLDEF void RenderGame ();

INLDEF void SetupGameTime ();
INLDEF void UpdateTimeAndFlipBuffers ();

} // TCE

#endif /* __TCE_GAME_H__ /////////////////////////////////////////////////////*/
