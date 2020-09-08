Engine.Text =
{
  /**
   * Draws the specified text at a given position as an outline. To draw text
   * as a filled in shape instead see the function Engine.Text.drawFill().
   *
   * @param {Number} x    - Horizontal position to draw the text.
   * @param {Number} y    - Vertical position to draw the text.
   * @Param {String} text - Actual string to draw with the font.
   */
  drawStroke: function (x, y, text)
  {
    var canvas = Engine.Canvas._
    var lines = text.split('\n')

    x += canvas.drawOffset.x
    y += canvas.drawOffset.y

    // The built-in text drawing API does not handle newlines so we have to
    // manually handle them by splitting the text into lines and offsetting.
    for (let i = 0; i < lines.length; i++) {
      let yOff = Engine.Text._.size * i
      let line = lines[i]

      canvas.text.strokeText(line, x, y + yOff)
    }
  },

  /**
   * Draws the specified text at a given position as a filled shape. To draw
   * text as a shape outline instead see the function Engine.Text.drawStroke().
   *
   * @param {Number} x    - Horizontal position to draw the text.
   * @param {Number} y    - Vertical position to draw the text.
   * @Param {String} text - Actual string to draw with the font.
   */
  drawFill: function (x, y, text)
  {
    var canvas = Engine.Canvas._
    var lines = text.split('\n')

    x += canvas.drawOffset.x
    y += canvas.drawOffset.y

    // The built-in text drawing API does not handle newlines so we have to
    // manually handle them by splitting the text into lines and offsetting.
    for (let i = 0; i < lines.length; i++) {
      let yOff = Engine.Text._.size * i
      let line = lines[i]

      canvas.text.fillText(line, x, y + yOff)
    }
  },

  /**
   * Sets a new font-face to be used for all subsequent draw operations until
   * the next time this function is called with a different font-face argument.
   *
   * Valid fonts that can are fonts that meet the following requirements:
   *  - The font is loaded with the engine's asset loader.
   *  - The font is installed on the end-user's machine.
   *  - The font is loaded into the webpage DOM with CSS.

   * @param {String} font - Name of the font-face to set for drawing.
   */
  setFont: function (font)
  {
    // The canvas 2D context API wants the font and size specified as a single
    // string formatted like so: '30px sans-serif', etc. Whereas we store these
    // two details separately, so we need to merge them before we can apply it.

    let size = Engine.Text._.size
    let format = `${size}px ${font}`

    Engine.Canvas._.text.font = format
    Engine.Text._.font = font
  },

  /**
   * Sets a new size (in pixels) to be used for all subsequent draw operations
   * until the next time this function is called with a different size value.
   *
   * @param {Number} size - New size to start drawing text at.
   */
  setSize: function (size)
  {
    // The canvas 2D context API wants the font and size specified as a single
    // string formatted like so: '30px sans-serif', etc. Whereas we store these
    // two details separately, so we need to merge them before we can apply it.

    let font = Engine.Text._.font
    let format = `${size}px ${font}`

    Engine.Canvas._.text.font = format
    Engine.Text._.size = size
  },

  /**
   * Sets the alignment to draw text at relative to the horizontal position.
   *
   * The value passed to this function must be a valid alignment string:
   * <developer.mozilla.org/Web/API/CanvasRenderingContext2D/textAlign>
   *
   * @param {String} alignment - A valid alignment string specifier.
   */
  setAlignment: function (alignment)
  {
    Engine.Canvas._.text.textAlign = alignment
    Engine.Text._.alignment = alignment
  },

  /**
   * Sets the color to draw text for all subsequent text draw calls until the
   * next time that the text draw color function is called with a new value.
   *
   * @param {Number} r - Red component of the draw color.
   * @param {Number} g - Green component of the draw color.
   * @param {Number} b - Blue component of the draw color.
   * @param {Number} a - Alpha component of the draw color.
   */
  setColorRGBA: function (r, g, b, a)
  {
    let color = new Engine.Color(r, g, b, a)
    Engine.Text.setColor(color)
  },

  /**
   * Sets the color to draw text for all subsequent text draw calls until the
   * next time that the text draw color function is called with a new value.
   *
   * @param {Object} color - New draw color value.
   */
  setColor: function (color)
  {
    // The 2D context RGB value for drawing is measured in 0-255 whereas our
    // color values are 0-1, so we need to convert them to the right values.
    let r = color.r * 255
    let g = color.g * 255
    let b = color.b * 255

    let finalColor = `rgb(${r},${g},${b})`

    Engine.Canvas._.text.strokeStyle = finalColor
    Engine.Canvas._.text.fillStyle   = finalColor
    Engine.Canvas._.text.globalAlpha = color.a

    // We make a copy to avoid passing by reference.
    Engine.Text._.color.copy(color)
  },

  /**
   * Sets the color to draw text shadows for all subsequent text draw calls
   * until the next time that the shadow color function sets a new color value.
   *
   * @param {Number} r - Red component of the shadow color.
   * @param {Number} g - Green component of the shadow color.
   * @param {Number} b - Blue component of the shadow color.
   * @param {Number} a - Alpha component of the shadow color.
   */
  setShadowColorRGBA: function (r, g, b, a)
  {
    let color = new Engine.Color(r, g, b, a)
    Engine.Text.setShadowColor(color)
  },

  /**
   * Sets the color to draw text shadows for all subsequent text draw calls
   * until the next time that the shadow color function sets a new color value.
   *
   * @param {Object} color - New shadow color value.
   */
  setShadowColor: function (color)
  {
    // The 2D context RGB value for drawing is measured in 0-255 whereas our
    // color values are 0-1, so we need to convert them to the right values.
    let r = color.r * 255
    let g = color.g * 255
    let b = color.b * 255

    let finalColor = `rgba(${r},${g},${b},${color.a})`
    Engine.Canvas._.text.shadowColor = finalColor

    // We make a copy to avoid passing by reference.
    Engine.Text._.shadowColor.copy(color)
  },

  /**
   * Sets how blurry the text's shadow should using an arbitrary measurement.
   * This is applied for all subsequent draw calls until the value is changed.
   *
   * @param {Number} blur - Level of blur to be applied to text shadows.
   */
  setShadowBlur: function (blur)
  {
    Engine.Canvas._.text.shadowBlur = blur
    Engine.Text._.shadowBlur = blur
  },

  /**
   * Sets how much the text's shadow should be offset from the original draw
   * point of the text (accepts both positive and negative offset values).
   *
   * @param {Number} x - Horizontal offset of the text shadow.
   * @param {Number} y - Vertical offset of the text shadow.
   */
  setShadowOffset: function (x, y)
  {
    Engine.Canvas._.text.shadowOffsetX = x
    Engine.Canvas._.text.shadowOffsetY = y

    Engine.Text._.shadowOffsetX = x
    Engine.Text._.shadowOffsetY = y
  },

  /**
   * Calculates how much horizontal and vertical space will be taken up by a
   * text string that is being drawn using all of the currently selected text
   * drawing options. The resulting bounds are returned to the function caller.
   *
   * @param  {String} text - Text string to determine the bounds of.
   * @return {Object} Bounding box width and height of the given text.
   */
  getBounds: function (text)
  {
    var bounds = { width: 0, height: 0 }
    var lines = text.split('\n')

    bounds.height = lines.length * Engine.Text._.size

    // Determine which line is longest and use that as the bounding width.
    for (let i = 0; i < lines.length; i++) {
      var width = Engine.Canvas._.text.measureText(lines[i]).width
      if (bounds.width < width) {
        bounds.width = width
      }
    }

    return bounds
  },

  /* ------------------------------------------------------------------------ */

  _:
  {
    font:          null, /** Currently selected font-face to draw with.  */
    size:          null, /** Size to draw the font-face at, in pixels.   */
    alignment:     null, /** How to align the text to the X position.    */
    color:         null, /** Currently selected color to draw text with. */

    shadowColor:   null, /** Currently selected color to draw shadows.   */
    shadowBlur:    null, /** How blurry the cast text shadows should be. */
    shadowOffsetX: null, /** Horizontal offset to apply to text shadow.  */
    shadowOffsetY: null, /** Vertical offset to apply to text shadow.    */

    /**
     * Initializes the internals of the text system so that it is ready for
     * use. This function is called on start-up by the engine, so it does not
     * need to be called anywhere else in the code.
     *
     * @access private
     */
    initialize: function ()
    {
      Engine.Text._.font          = 'sans-serif'
      Engine.Text._.size          = 32
      Engine.Text._.alignment     = 'start' // Default language alignment.
      Engine.Text._.color         = new Engine.Color() // Default is white.

      // By default, we don't want to draw any text shadow.
      Engine.Text._.shadowColor   = new Engine.Color(0, 0, 0, 0)
      Engine.Text._.shadowBlur    = 0
      Engine.Text._.shadowOffsetX = 0
      Engine.Text._.shadoeOffsetY = 0
    },

    /**
     * On a canvas resize, for some reason, the settings applied to the text
     * context are reset. This function is called to restore the currently set
     * text drawing options back to their original state after a new resize.
     *
     * This is also the reason why we cache copies of the text context's values
     * rather than just solely setting them with the 2D canvas text context.
     *
     * @access private
     */
    restore: function ()
    {
      // So that we can draw text from the top position instead.
      Engine.Canvas._.text.textBaseline = 'top'

      Engine.Text.setFont        (Engine.Text._.font)
      Engine.Text.setSize        (Engine.Text._.size)
      Engine.Text.setAlignment   (Engine.Text._.alignment)
      Engine.Text.setColor       (Engine.Text._.color)

      let shadowX = Engine.Text._.shadowOffsetX
      let shadowY = Engine.Text._.shadowOffsetY

      Engine.Text.setShadowColor (Engine.Text._.shadowColor)
      Engine.Text.setShadowBlur  (Engine.Text._.shadowBlur)
      Engine.Text.setShadowOffset(shadowX, shadowY)
    }
  },

  /* ------------------------------------------------------------------------ */
}
