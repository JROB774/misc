Engine.Animation = class
{
  /**
   * Constructs a new object instance of the animation class. It is recommended
   * that engine users do not call this function directly. Instead the Asset
   * Loader system should be used to load in and create animation instances.
   *
   * For more information see file: './source/engine/core/asset-loader.js'
   *
   * @param {Object} animationData - Information about the specific animation.
   */
  constructor (animationData)
  {
    this._ = {} // Private members stored in here.
    this._.frameWidth  = animationData.frameSize.width
    this._.frameHeight = animationData.frameSize.height
    this._.startX      = animationData.startOffset.x
    this._.startY      = animationData.startOffset.y
    this._.frames      = animationData.frames
    this._.looped      = animationData.looped

    // There are used for timing the playback.
    this._.paused = false
    this._.timer = 0.0
  }

  /**
   * Updates the animation's internal clock by the specified elapsed time, this
   * clock controls what frame of the animation is going to be played on draw.
   * The elapsed parameter (in most cases) should be set to the current frame's
   * delta time, in order to keep animations running smoothly and in real-time.
   *
   * Paused animations will not be affected by this function call at all.
   *
   * @param {Number} elapsed - Time in seconds to advance the animation by.
   */
  update (elapsed)
  {
    // No need to update the playback timer.
    if (this._.paused) { return }

    // It is okay for us to keep adding time even after surpassing the total
    // time/length of the animation as the getCurrentFrame() function will
    // simply return the last frame if we overflow the amount of time needed.
    this._.timer += elapsed

    // However, looped animations should go back to the start.
    if (this._.looped) {
      var length = this.getLength()
      if (length <= 0) { return } // Avoid infinite loops!
      while (this._.timer >= length) {
        this._.timer -= length
      }
    }
  }

  /**
   * Toggles whether the animation is paused or playing.
   *
   * @param {Boolean} pause - True to pause, false to play.
   */
  pause (pause)
  {
    this._.paused = pause
  }

  /**
   * Starts the animation back at the beginning (first frame). This is useful
   * for non-looping animations in order to play them again after they finish.
   */
  reset ()
  {
    this._.timer = 0.0
  }

  /**
   * Retrieves the current frame of the animation for drawing. An animation
   * that loops will continue to provide new frames, whereas a non-looping
   * animation will continuously return the last frame once it is finished.
   * This function is called by the Engine.Texture.drawAnimated() function
   * internally, for the animation that is passed as a parameter.
   *
   * @return {Object} Current frame/clip of the animation to play.
   */
  getCurrentFrame ()
  {
    // Empty animations have no frames to return.
    if (this._.frames.length === 0) { return null }

    // Go through the frames until we find the frame that corresponds to the
    // current value of the animation's internal timer/clock, then stop.
    var accumulatedTime = 0
    var currentFrame = 0

    for (let i = 0; i < this._.frames.length; i++) {
      accumulatedTime += this._.frames[i]
      if (accumulatedTime >= this._.timer) {
        break
      }
      currentFrame++
    }

    if (currentFrame >= this._.frames.length) {
      currentFrame = this._.frames.length - 1
    }

    let frame = {}

    // Animations frames are stored left-to-right consecutively.
    frame.x = this._.startX + (currentFrame * this._.frameWidth)
    frame.y = this._.startY
    frame.w = this._.frameWidth
    frame.h = this._.frameHeight

    return frame
  }

  /**
   * Retrieves the total length of the animation in seconds.
   *
   * @return {Number} Animation length in seconds.
   */
  getLength ()
  {
    return this._.frames.reduce((a, b) => a + b, 0)
  }

  /**
   * Retrieves the width of the animation's individual frames.
   * All frames in an animation will have the same width.
   *
   * @return {Number} Width of a single frame.
   */
  getFrameWidth ()
  {
    return this._.frameWidth
  }

  /**
   * Retrieves the height of the animation's individual frames.
   * All frames in an animation will have the same height.
   *
   * @return {Number} Height of a single frame.
   */
  getFrameHeight ()
  {
    return this._.frameHeight
  }

  /**
   * Retrieves whether the animation is a looped animation or not.
   *
   * @return {Boolean} True if looped, otherwise false.
   */
  isLooped ()
  {
    return this._.looped
  }

  /**
   * Retrieves whether the animation is current paused or not
   *
   * @return {Boolean} True if paused, otherwise false.
   */
  isPaused ()
  {
    return this._.paused
  }

  /**
   * Retrieves whether the animation has finished playing or not. If it is a
   * looped animation then this function will always return a value of false.
   *
   * @return {Boolean} True if finished, otherwise false.
   */
  isDone ()
  {
    return ((this._.looped) ? false : (this._.timer >= this.getLength()))
  }
}
