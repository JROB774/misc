Engine.Collision =
{
  /**
   * Determines if a 2D point is colliding with a 2D box.
   *
   * @param  {Number}  x  - Horizontal position of the point.
   * @param  {Number}  y  - Vertical position of the point.
   * @param  {Number}  bx - Horizontal position of the box.
   * @param  {Number}  by - Vertical position of the box.
   * @param  {Number}  bw - Width of the box.
   * @param  {Number}  bh - Height of the box.
   *
   * @return {Boolean} True on collision, otherwise false.
   */
  pointAndBox: function (x, y, bx, by, bw, bh)
  {
    return ((x < (bx + bw)) && (x > bx) &&
            (y < (by + bh)) && (y > by))
  },

  /**
   * Determines if two 2D circles are colliding with each other.
   *
   * @param  {Number}  ax - Horizontal position (center) of the first circle.
   * @param  {Number}  ay - Vertical position (center) of the first circle.
   * @param  {Number}  ar - Radius of the first circle.
   * @param  {Number}  bx - Horizontal position (center) of the second circle.
   * @param  {Number}  by - Vertical position (center) of the second circle.
   * @param  {Number}  br - Radius of the second circle.
   *
   * @return {Boolean} True on collision, otherwise false.
   */
  circleAndCircle: function (ax, ay, ar, bx, by, br)
  {
    let deltaXSquared = Math.pow(bx - ax, 2)
    let deltaYSquared = Math.pow(by - ay, 2)

    let radiusSquared = Math.pow(ar + br, 2)

    return ((deltaXSquared + deltaYSquared) < radiusSquared)
  },

  /**
   * Determines if two 2D boxes are colliding with each other.
   *
   * @param  {Number}  ax - Horizontal position of the first box.
   * @param  {Number}  ay - Vertical position of the first box.
   * @param  {Number}  aw - Width of the first box.
   * @param  {Number}  ah - Height of the first box.
   * @param  {Number}  bx - Horizontal position of the second box.
   * @param  {Number}  by - Vertical position of the second box.
   * @param  {Number}  bw - Width of the second box.
   * @param  {Number}  bh - Height of the second box.
   *
   * @return {Boolean} True on collision, otherwise false.
   */
  boxAndBox: function (ax, ay, aw, ah, bx, by, bw, bh)
  {
    return ((ax < (bx + bw)) && ((ax + aw) > bx) &&
            (ay < (by + bh)) && ((ay + ah) > by))
  },
}
