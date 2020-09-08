var Camera =
{
  /**
   * Updates the virual game camera, specifically to carry out any current
   * screen shake operations that are taking place.
   *
   * @param {Number} dt - Delta time for the current frame.
   */
  update: function (dt)
  {
    if (Camera._.timer <= 0.0) { return }

    let min = new Engine.Vector2().copy(Camera._.shakeMin)
    let max = new Engine.Vector2().copy(Camera._.shakeMax)

    let x = Engine.Utility.randomRangeInteger(min.x, max.x)
    let y = Engine.Utility.randomRangeInteger(min.y, max.y)

    Engine.Canvas.setDrawOffset(x, y)

    Camera._.timer -= dt
    if (Camera._.timer <= 0.0) {
      Engine.Canvas.setDrawOffset(0, 0)
    }
  },

  /**
   * Sets up a camera shake to be carried out for the specified time.
   *
   * @param {Number} time - How long the shake should occur, in seconds.
   * @param {Number} minX - Minimum amount of horizontal shake to occur.
   * @param {Number} minY - Minimum amount of vertical shake to occur.
   * @param {Number} maxX - Maximum amount of horizontal shake to occur.
   * @param {Number} maxY - Maximum amount of vertical shake to occur.
   */
  shake: function (time, minX, minY, maxX, maxY)
  {
    Camera._.shakeMin = new Engine.Vector2(minX, minY)
    Camera._.shakeMax = new Engine.Vector2(maxX, maxY)

    Camera._.timer = time
  },

  /* ------------------------------------------------------------------------ */

  _:
  {
    shakeMin: null, /* 2D vector containing the minimum shake amount.   */
    shakeMax: null, /* 2D vector containing the maximum shake amount.   */

    timer:     0.0, /* How long the current shake has left, in seconds. */
  },

  /* ------------------------------------------------------------------------ */
}
