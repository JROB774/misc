class Enemy
{
  /**
   * Constructs a new object instance of the enemy class.
   *
   * @param {Number} x      - Horizontal position to spawn the enemy at.
   * @param {Number} y      - Vertical position to spawn the enemy at.
   * @param {Object} assets - Asset package containing the needed assets.
   */
  constructor (x, y, assets)
  {
    this.RADIUS       = 16
    this.SPEED        = 400
    this.ACCELERATION = 10
    this.SCALE        = 0.4
    this.GHOSTS       = 4

    this.pos     = new Engine.Vector2(x, y)
    this.lastPos = new Engine.Vector2(x, y)

    this.angle = 0.0

    this.texture = assets.texEntity
    this.anmIdle = assets.anmEnemyIdle
  }

  /**
   * Updates the enemy entity, carrying out its AI/logic. Specifically to chase
   * the passed in player entity and speed up as the game goes onwards.
   *
   * @param {Object} player - Player object to track/chase.
   * @param {Number} dt     - Delta time for the current frame.
   */
  update (player, dt)
  {
    this.lastPos.copy(this.pos)

    // Enemy gets faster as the game goes on.
    this.SPEED += (dt * this.ACCELERATION)

    if (Engine.Collision.circleAndCircle(this.pos.x, this.pos.y,
          this.RADIUS, player.pos.x, player.pos.y, player.RADIUS)) {
      return
    }

    let dir = player.pos.subVector(this.lastPos).normalize()
    this.pos.copy(this.lastPos.addVector(dir.mulScalar(dt * this.SPEED)))
  }

  /**
   * Used during the enemy's draw function to create a visual effect indicating
   * speed. Clones of enemy are drawn behind it with increasing transparency.
   *
   * This function should never be called directly and instead should only be
   * used by the enemy's draw function.
   *
   * @param {Number} sx    - Current horizontal scale to draw at.
   * @param {Number} sy    - Current vertical scale to draw at.
   * @param {Number} count - How many instances of ghosts to draw.
   */
  drawGhostingTrail (sx, sy, count)
  {
    var prev  = new Engine.Vector2().copy(this.lastPos)
    var alpha = 1.0 / (count + 1)

    for (let i = 0; i < count; i++) {
      let x = (this.pos.x + this.lastPos.x) / 2
      let y = (this.pos.y + this.lastPos.y) / 2

      this.texture.setColorRGBA(1, 1, 1, alpha)
      this.texture.drawAnimated(x, y, this.anmIdle, sx, sy, this.angle)

      prev.assign(x, y)
    }

    this.texture.setColorRGBA(1, 1, 1, 1)
  }

  /**
   * Draws the enemy entity to the screen, as well as updates all of the visual
   * elements e.g. angle of the enemy, current animation frame, etc.
   *
   * @param {Number} dt - Delta time for the current frame.
   */
  draw (dt)
  {
    const DOUBLE_PI = Math.PI * 2

    this.anmIdle.update(dt)
    this.angle += 0.08
    if (this.angle > DOUBLE_PI) {
      this.angle -= DOUBLE_PI
    }

    let sx     = this.SCALE
    let sy     = this.SCALE
    let ghosts = this.GHOSTS
    let x      = this.pos.x
    let y      = this.pos.y
    let angle  = this.angle

    this.drawGhostingTrail(sx, sy, ghosts)
    this.texture.drawAnimated(x, y, this.anmIdle, sx, sy, angle)
  }
}
