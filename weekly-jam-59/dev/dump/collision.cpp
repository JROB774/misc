	// Handle collisions with solid tiles.
	for (u16 i = 0; i < room.size; ++i) {
		// Store the current tile and offset it's height to be smaller.
		collider_t tile = room.tile[i];
		tile.h -= TILE_OFFSET;
		// Check if there was even a collision between the two.
		if (Collision(&player.collider, &tile)) {
			// If there was calculate the x and y interpolation (how deep in).
			i32 xInterp = INT_MAX, yInterp = INT_MAX;
			if (player.collider.x < tile.x) { xInterp = -((player.collider.x + player.collider.w) - tile.x); }
			else { xInterp = ((tile.x + tile.w) - player.collider.x); }
			if (player.collider.y < tile.y) { yInterp = -((player.collider.y + player.collider.h) - tile.y); }
			else { yInterp = ((tile.y + tile.h) - player.collider.y); }

			// Find out which is shortest and move the player back by that amount and zero off the velocity.
			if (abs(xInterp) > abs(yInterp)) { player.y += yInterp, player.collider.y += yInterp, player.velocity.y = 0.0; }
			else if (abs(xInterp) < abs(yInterp)) { player.x += xInterp, player.collider.x += xInterp, player.velocity.x = 0.0; }
		}
	}
