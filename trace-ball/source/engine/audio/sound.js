Engine.Sound = class
{
  /**
   * Constructs a new object instance of the sound class. It is recommended
   * that engine users do not call this function directly. Instead the Asset
   * Loader system should be used to load in and create sound instances.
   *
   * For more information see file: './source/engine/core/asset-loader.js'
   *
   * @param {Array} bufferData - Raw buffered sound sample data.
   */
  constructor (bufferData)
  {
    this._ = {} // Private members stored in here.
    this._.buffer = bufferData
  }

  /**
   * Plays the loaded sound sample, starting from when the function is called.
   */
  play ()
  {
    // Create a new input and connect it to the context for output.
    let context = Engine.Mixer._.context
    let source = context.createBufferSource()

    source.buffer = this._.buffer
    source.connect(context.destination)
    source.start(0) // Begin playback immediately.
  }
}
