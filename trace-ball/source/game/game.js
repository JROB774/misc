// We use strict mode to enforce stronger syntactic rules. Helping to ensure
// that code is written correctly and more secure than standard JavaScript.
//
// For info on the benefits see: <https://www.w3schools.com/js/js_strict.asp>
'use strict'

// Because all of the games's source code files are merged into a single file
// (with this file's code being at the top) we only need to declare 'use
// strict' in this file and it will be applied to the rest of the game code.

class Game
{
  /**
   * Prepares the game for execution. Setting up the font and creating the
   * necessary entities needed for the game to run correctly.
   *
   * This function is called automatically by the game engine on start-up and
   * does not need to be called anywhere else in the codebase.
   */
  initialize ()
  {
    Engine.Text.setFont('Multicolore')

    Engine.Text.setColorRGBA(0.1, 0.1, 0.1, 1.0)
    Engine.Text.setShadowColorRGBA(0, 0, 0, 0.7)

    Engine.Text.setShadowBlur(4)

    Engine.Canvas.setSize(640, 480)
    Engine.Canvas.showCursor(false)

    this.player = new Player(  0,   0, this.assets)
    this.enemy  = new Enemy (640, 480, this.assets)
  }

  /**
   * Updates all of the game's internal logic and the entities in the game.
   *
   * This function is called automatically by the game engine at the specified
   * refresh rate (defaults to 60 FPS) and does not need to be called anywhere
   * else in the codebase.
   *
   * @param {Number} dt - Delta time for the current frame.
   */
  update (dt)
  {
    this.player.update(this.enemy, dt)
    this.enemy.update(this.player, dt)

    Camera.update(dt)
  }

  /**
   * Draws all of the game's content, such as the UI, entities, world, etc.
   *
   * This function is called automatically by the game engine at the specified
   * refresh rate (defaults to 60 FPS) and does not need to be called anywhere
   * else in the codebase.
   *
   * @param {Number} dt - Delta time for the current frame.
   */
  draw (dt)
  {
    Engine.Effect.setCurrent(this.assets.chromatic)
    Engine.Canvas.clearRGBA(0.9, 0.9, 0.9, 1.0)

    let halfWidth  = Engine.Canvas.getWidth()  / 2
    let halfHeight = Engine.Canvas.getHeight() / 2

    this.assets.texBack.draw(halfWidth, halfHeight)

    this.enemy.draw(dt)
    this.player.draw(dt)
  }
}
