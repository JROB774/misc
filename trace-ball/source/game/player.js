class Player
{
  /**
   * Constructs a new object instance of the player class.
   *
   * @param {Number} x      - Horizontal position to spawn the player at.
   * @param {Number} y      - Vertical position to spawn the player at.
   * @param {Object} assets - Asset package containing the needed assets.
   */
  constructor (x, y, assets)
  {
    this.IMMUNE_TIME     =  1.25
    this.RADIUS          =  16
    this.TEXT_FLASH_TIME =  0.15
    this.MIN_SHAKE       = -15
    this.MAX_SHAKE       =  15
    this.SHAKE_TIME      =  0.3
    this.GHOSTS          =  4
    this.RIGIDNESS       =  100
    this.SCALE           =  0.4

    this.pos     = new Engine.Vector2(x, y)
    this.lastPos = new Engine.Vector2(x, y)

    this.angle    = 0
    this.distance = 0
    this.lives    = 3
    this.state    = 'idle'

    this.texture = assets.texEntity
    this.sndHit  = assets.sndPlayerHit
    this.anmIdle = assets.anmPlayerIdle
    this.anmHit  = assets.anmPlayerHit
    this.anmDead = assets.anmPlayerDead

    this.immune      = false
    this.immuneTimer = 0
    this.immuneFlash = false

    this.textFlashTimer = 0

    this.scoreTime = 0.0
  }

  /**
   * Handles transitioning the player into the hit state and setting up the
   * temporary immunity when the player is hit by the enemy entity.
   *
   * This function should never be called directly and instead should only be
   * used by the player's update function.
   */
  takeDamage ()
  {
    let min  = this.MIN_SHAKE
    let max  = this.MAX_SHAKE
    let time = this.SHAKE_TIME

    Camera.shake(time, min, min, max, max)

    this.sndHit.play()
    this.anmHit.reset()

    this.state          = 'hit'
    this.immune         = true
    this.immuneTimer    = this.IMMUNE_TIME
    this.textFlashTimer = this.TEXT_FLASH_TIME

    this.lives--
    if (this.lives <= 0) {
      this.state = 'dead'
    }
  }

  /**
   * Extra update logic to be executed if the player is currently immune. This
   * ticks down the immunity timer after being hit and applies the extra
   * effects that are displayed as a result of being hit by the enemy entity.
   *
   * This function should never be called directly and instead should only be
   * used by the player's update function.
   */
  updateImmune (dt)
  {
    let time    = this.immuneTimer / 300
    let dblTime = time * 2
    let immune  = this.immuneTimer * 30

    let pulse = [ 0, 0 ]
    pulse[0]  = Engine.Utility.sinInRange(immune, -dblTime, dblTime)
    pulse[1]  = Engine.Utility.sinInRange(immune, -time, time)

    Engine.Effect.setCurrentUniform('pulse', pulse)

    this.immuneFlash  = !this.immuneFlash
    this.immuneTimer -= dt
    if (this.immuneTimer <= 0) {
      Engine.Effect.setCurrentUniform('pulse', [ 0, 0 ])

      this.immune      = false
      this.immuneFlash = false
    }

    this.anmHit.update(dt)
  }

  /**
   * Updates the player entity, carrying out its logic. Specifically to follow
   * the mouse and to check for any collisions with the passed in enemy entity.
   *
   * @param {Object} enemy - Enemy object to check collisions with.
   * @param {Number} dt    - Delta time for the current frame.
   */
  update (enemy, dt)
  {
    this.distance = 0.0

    if (this.state === 'dead') {
      this.anmDead.update(dt)
    } else {
      if (this.state === 'hit') {
        if (this.anmHit.isDone()) {
          this.state = 'idle'
        }
      }

      let cw = Engine.Canvas.getWidth()
      let ch = Engine.Canvas.getHeight()

      this.lastPos.copy(this.pos)
      this.pos = Engine.Event.getCursorPosition()

      this.pos.x = Engine.Utility.clamp(this.pos.x, 0, cw)
      this.pos.y = Engine.Utility.clamp(this.pos.y, 0, ch)

      if (!this.lastPos.equalTo(this.pos)) {
        let dir       = this.pos.subVector(this.lastPos).normalize()
        this.distance = this.pos.subVector(this.lastPos).absolute().length()
        this.angle    = Math.atan2(dir.y, dir.x)
      }

      if (this.immune) {
        this.updateImmune(dt)
      } else {
        let x = this.pos.x
        let y = this.pos.y

        if (Engine.Collision.circleAndCircle(x, y, this.RADIUS,
              enemy.pos.x, enemy.pos.y, enemy.RADIUS)) {
          this.takeDamage()
        }
      }

      this.scoreTime += dt
    }
  }

  /**
   * Draws the various user interface elements relating to the player. This
   * includes the current number of lives and the amount of time survived.
   *
   * This function should never be called directly and instead should only be
   * used by the player's draw function.
   */
  drawUIElements (dt)
  {
    let timeText  = 'Time: '  + this.scoreTime.toFixed(2)
    let livesText = 'Lives: ' + this.lives

    if (this.textFlashTimer <= 0) {
      Engine.Text.setColorRGBA(0.1, 0.1, 0.1, 1.0)
    } else {
      Engine.Text.setColorRGBA(1.0, 1.0, 1.0, 1.0)
      this.textFlashTimer -= dt
    }

    let text = timeText + '\n' + livesText
    Engine.Text.drawFill(8, 8, text)
  }

  /**
   * Used during the player's draw function to create a visual effect to show
   * speed. Clones of player are drawn behind it with increasing transparency.
   *
   * This function should never be called directly and instead should only be
   * used by the player's draw function.
   *
   * @param {Object} anim  - Current animation to draw the ghost with.
   * @param {Number} sx    - Current horizontal scale to draw at.
   * @param {Number} sy    - Current vertical scale to draw at.
   * @param {Number} count - How many instances of ghosts to draw.
   */
  drawGhostingTrail (anim, sx, sy, count)
  {
    var prev  = new Engine.Vector2().copy(this.lastPos)
    var alpha = 1.0 / (count + 1)

    for (let i = 0; i < count; i++) {
      let x = (this.pos.x + this.lastPos.x) / 2
      let y = (this.pos.y + this.lastPos.y) / 2

      this.texture.setColorRGBA(1, 1, 1, alpha)
      this.texture.drawAnimated(x, y, anim, sx, sy, this.angle)

      prev.assign(x, y)
    }

    this.texture.setColorRGBA(1, 1, 1, 1)
  }

  /**
   * Draws the player entity to the screen and the associated UI elements.
   *
   * @param {Number} dt - Delta time for the current frame.
   */
  draw (dt)
  {
    if (!this.anmDead.isDone()) {
      this.drawUIElements(dt)

      // We don't want to draw the player in either of these conditions.
      if (this.state !== 'dead' && this.immuneFlash) { return }

      switch (this.state) {
        case 'idle': var anim = this.anmIdle; break
        case 'hit':  var anim = this.anmHit;  break
        case 'dead': var anim = this.anmDead; break
      }

      let sxFactor =  this.distance / this.RIGIDNESS
      let syFactor = (this.distance / this.RIGIDNESS) / 15

      var sx     = this.SCALE + Engine.Utility.clamp(sxFactor, 0, 1.50)
      var sy     = this.SCALE - Engine.Utility.clamp(syFactor, 0, 0.10)
      var ghosts = this.GHOSTS
      var x      = this.pos.x
      var y      = this.pos.y
      var angle  = this.angle

      if (this.state !== 'dead') {
        this.drawGhostingTrail(anim, sx, sy, ghosts)
      }
      this.texture.drawAnimated(x, y, anim, sx, sy, angle)
    }
  }
}
