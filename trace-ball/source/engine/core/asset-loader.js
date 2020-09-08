Engine.AssetLoader =
{
  /**
   * Takes an asset package JSON file and proceeds to load all of the assets
   * specified within the package. Once all the assets are successfully loaded
   * the returned promise will trigger its then() condition. The loaded
   * package's assets can then be retrieved later on using the provided name,
   * via the Engine.AssetLoader.getPackage() function.
   *
   * @param  {String} name - Name of the package to store the loaded assets.
   * @param  {String} url  - Location of the package JSON file to load.
   *
   * @return {Object} A promise that triggers once all assets are loaded.
   */
  loadAssetPackage: function (name, url)
  {
    return new Promise(function (resolve, reject)
    {
      Engine.Utility.loadFile(url).then(function (fileData)
      {
        // We need to store these with the actual asset loader so that they
        // persist even after the function ends, when the assets are loaded.
        Engine.AssetLoader._.packages[name] = {}
        Engine.AssetLoader._.packages[name].promises = []

        var assetPackage = Engine.AssetLoader._.packages[name]
        var assetInfo = JSON.parse(fileData).assets

        for (let i = 0; i < assetInfo.length; i++) {
          let promise = Engine.AssetLoader._.loadAsset(name, assetInfo[i])
          assetPackage.promises[i] = promise
        }

        // Only when all assets are loaded does the promise finish.
        Promise.all(assetPackage.promises).then(function ()
        {
          resolve()
        })
      })
    })
  },

  /**
   * Retrieves the assets from a package, previously loaded with the function
   * Engine.AssetLoader.loadAssetPackage().
   *
   * @param  {String} name - Name of the package that was loaded.
   * @return {Object} An object containing all of the loaded assets.
   */
  getPackage: function (name)
  {
    return Engine.AssetLoader._.packages[name]
  },

  /* ------------------------------------------------------------------------ */

  _:
  {
    packages: {}, /** All packages that have been loaded. */

    /**
     * Loads a particular asset from a given asset package, carrying out the
     * appropriate loading and creation process depending on the asset type.
     *
     * @param  {String} name - Name of the package the asset is in.
     * @param  {Object} info - Information about the given asset.
     *
     * @return {Object} A promise triggered once the asset has loaded.
     *
     * @access private
     */
    loadAsset: function (name, info)
    {
      switch (info.type)
      {
        case 'texture':   var load = Engine.AssetLoader._.loadTexture;   break
        case 'font':      var load = Engine.AssetLoader._.loadFont;      break
        case 'animation': var load = Engine.AssetLoader._.loadAnimation; break
        case 'sound':     var load = Engine.AssetLoader._.loadSound;     break
        case 'shader':    var load = Engine.AssetLoader._.loadShader;    break

        default:
          console.error('Attempt to load unknown asset type: ' + info.type)
          return
      }

      return load(name, info)
    },

    /**
     * Carries out the necessary process to load and then create a texture.
     * This function is, and should only be, called by the load asset function
     * if the given asset's type is determined to be of type texture.
     *
     * @param  {String} name - Name of the package the asset is in.
     * @param  {Object} info - Information about the given asset.
     *
     * @return {Object} A promise triggered once the asset has loaded.
     *
     * @access private
     */
    loadTexture: function (name, info)
    {
      var assetPackage = Engine.AssetLoader._.packages[name]

      return new Promise(function (resolve, reject)
      {
        var image = new Image()

        image.src = info.file
        image.onload = function ()
        {
          let texture = new Engine.Texture(image)
          assetPackage[info.name] = texture

          resolve()
        }
      })
    },

    /**
     * Carries out the necessary process to load and then create a font. This
     * function is, and should only be, called by the load asset function if
     * the given asset's type is determined to be of type font.
     *
     * @param  {String} name - Name of the package the asset is in.
     * @param  {Object} info - Information about the given asset.
     *
     * @return {Object} A promise triggered once the asset has loaded.
     *
     * @access private
     */
    loadFont: function (name, info)
    {
      var assetPackage = Engine.AssetLoader._.packages[name]

      // Font loading works slightly differently, due to it being managed by
      // the HTML5 2D canvas context, rather than the WebGL canvas context.
      //
      // Fonts using this system can only be used for drawing if they've been
      // loaded into the webpage's DOM (Document Object Model). So, we load
      // the font file into the DOM by appending an internal CSS stylesheet
      // to the page's head, this stylesheet loads the font ready for drawing.
      //
      // Note, that as a result, fonts do not have to be loaded using the asset
      // loader and instead the engine user can instead put their fonts into
      // their own CSS stylesheet and refer to them that way without issue.

      return new Promise(function (resolve, reject)
      {
        assetPackage[info.name] = info.name

        let style = document.createElement('style')
        style.innerHTML =
        `
        @charset "utf-8";
        @font-face
        {
          font-family: "${info.name}";
          src: url("${info.file}");
        }
        `
        document.head.appendChild(style)

        resolve()
      })
    },

    /**
     * Carries out the necessary process to load and then create an animation.
     * This function is, and should only be, called by the load asset function
     * if the given asset's type is determined to be of type animation.
     *
     * @param  {String} name - Name of the package the asset is in.
     * @param  {Object} info - Information about the given asset.
     *
     * @return {Object} A promise triggered once the asset has loaded.
     *
     * @access private
     */
    loadAnimation: function (name, info)
    {
      var assetPackage = Engine.AssetLoader._.packages[name]

      return new Promise(function (resolve, reject)
      {
        let fileType = 'json'
        Engine.Utility.loadFile(info.file, fileType).then(function (fileData)
        {
          let animation = new Engine.Animation(fileData)
          assetPackage[info.name] = animation

          resolve()
        })
      })
    },

    /**
     * Carries out the necessary process to load and then create a sound. This
     * function is, and should only be, called by the load asset function if
     * the given asset's type is determined to be of type sound.
     *
     * @param  {String} name - Name of the package the asset is in.
     * @param  {Object} info - Information about the given asset.
     *
     * @return {Object} A promise triggered once the asset has loaded.
     *
     * @access private
     */
    loadSound: function (name, info)
    {
      var assetPackage = Engine.AssetLoader._.packages[name]

      return new Promise(function (resolve, reject)
      {
        let fileType = 'arraybuffer'
        Engine.Utility.loadFile(info.file, fileType).then(function (fileData)
        {
          let context = Engine.Mixer._.context
          context.decodeAudioData(fileData).then(function (bufferData)
          {
            let sound = new Engine.Sound(bufferData)
            assetPackage[info.name] = sound

            resolve()
          })
        })
      })
    },

    /**
     * Carries out the necessary process to load and then create a shader. This
     * function is, and should only be, called by the load asset function if
     * the given asset's type is determined to be of type shader.
     *
     * @param  {String} name - Name of the package the asset is in.
     * @param  {Object} info - Information about the given asset.
     *
     * @return {Object} A promise triggered once the asset has loaded.
     *
     * @access private
     */
    loadShader: function (name, info)
    {
      var assetPackage = Engine.AssetLoader._.packages[name]

      return new Promise(function (resolve, reject)
      {
        // Shaders are made up of two files, so we load them both.
        let vertFile = info.file + '.vert'
        let fragFile = info.file + '.frag'

        Promise.all(
        [
          Engine.Utility.loadFile(vertFile),
          Engine.Utility.loadFile(fragFile)

        ]).then(function (fileData)
        {
          let vert = fileData[0]
          let frag = fileData[1]

          let shader = new Engine.Shader(vert, frag)
          assetPackage[info.name] = shader

          resolve()
        })
      })
    },
  },

  /* ------------------------------------------------------------------------ */
}
