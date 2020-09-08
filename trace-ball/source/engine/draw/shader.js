Engine.Shader = class
{
  /**
   * Sets the current shader to use for drawing operations. This function
   * mainly used by the engine within the internals of its various drawing
   * operations (shapes, textures, etc.).
   *
   * @param {Object} shader - Shader to use for drawing.
   *
   * @static
   */
  static setCurrent (shader)
  {
    Engine.Canvas._.webgl.useProgram(shader._.program)
  }

  /**
   * Unsets the current shader from being used for drawing. This function is
   * mainly used by the engine within the internals of its various drawing
   * operations (shapes, textures, etc.).
   *
   * @static
   */
  static unsetCurrent ()
  {
    Engine.Canvas._.webgl.useProgram(null)
  }

  /**
   * Constructs a new object instance of the shader class. It is recommended
   * that engine users do not call this function directly. Instead the Asset
   * Loader system should be used to load in and create shader instances.
   *
   * For more information see file: './source/engine/core/asset-loader.js'
   *
   * @param {String} vertSource - Vertex shader GLSL source code.
   * @param {String} fragSource - Fragment shader GLSL source code.
   */
  constructor (vertSource, fragSource)
  {
    var webgl = Engine.Canvas._.webgl

    // Internal function used by the shader constructor to create the vertex
    // and fragment shaders using WebGL, this saves on writing repeated code.
    let createShader = function (source, type)
    {
      let shader = webgl.createShader(type)

      webgl.shaderSource(shader, source)
      webgl.compileShader(shader)

      let status = webgl.getShaderParameter(shader, webgl.COMPILE_STATUS)
      if (status) { return shader } // No issues with compilation.

      let infoLog = webgl.getShaderInfoLog(shader)
      console.error('Failed to compile shader: ' + infoLog)

      return null
    }

    let vert = createShader(vertSource, webgl.VERTEX_SHADER)
    let frag = createShader(fragSource, webgl.FRAGMENT_SHADER)

    if (!vert || !frag) { return null }

    var program = webgl.createProgram()

    webgl.attachShader(program, vert)
    webgl.attachShader(program, frag)

    webgl.linkProgram(program)

    if (!webgl.getProgramParameter(program, webgl.LINK_STATUS)) {
      let infoLog = webgl.getProgramInfoLog(program)
      console.error('Failed to link shader: ' + infoLog)
    }

    // It is considered polite to free up resources after use.
    webgl.detachShader(program, frag)
    webgl.detachShader(program, vert)

    webgl.deleteShader(vert)
    webgl.deleteShader(frag)

    // We cache information about the shader's attributes and uniforms which we
    // can use later to provide an easy interface for modifying their values.
    //
    // These will be used to make drawing with shaders and VBOs a much easier
    // experience and to allow for the engine user to make use of a custom
    // interface for modifying uniform values from within the JavaScript code.

    let activeAttributes = webgl.ACTIVE_ATTRIBUTES
    let activeUniforms = webgl.ACTIVE_UNIFORMS

    let attributeCount = webgl.getProgramParameter(program, activeAttributes)
    let uniformCount = webgl.getProgramParameter(program, activeUniforms)

    let attributes = {}
    let uniforms = {}

    for (let i = 0; i < attributeCount; i++) {
      let info = webgl.getActiveAttrib(program, i)
      let loc = webgl.getAttribLocation(program, info.name)
      attributes[info.name] = {
        location: loc
      }
    }
    for (let i = 0; i < uniformCount; i++) {
      let info = webgl.getActiveUniform(program, i)
      let loc = webgl.getUniformLocation(program, info.name)
      uniforms[info.name] = {
        type: info.type,
        location: loc
      }
    }

    this._ = {} // Private members stored in here.
    this._.program    = program
    this._.attributes = attributes
    this._.uniforms   = uniforms
  }

  /**
   * Enables and specified the structure for a specific attribute to be used by
   * the currently loaded shader, via Engine.Shader.setCurrent(), for drawing.
   * This function is usually called by internal engine functions before a draw
   * operation is carried out; however, an engine user can also make use of it.
   *
   * @param {String}  name       - Name of the attribute to be enabled.
   * @param {Number}  size       - Number of elements in the attribute type.
   * @param {Boolean} normalized - Whether data is normalized or not.
   * @param {Number}  stride     - Elements separating each attribute.
   * @param {Number}  offset     - Element offset from start of buffer.
   */
  enableAttribute (name, size, normalized, stride, offset)
  {
    // We can only enable attributes that actually exist.
    if (!(name in this._.attributes)) { return }
    let attribute = this._.attributes[name]

    let webgl = Engine.Canvas._.webgl

    // These are in elements but they need to be in bytes.
    stride *= Float32Array.BYTES_PER_ELEMENT
    offset *= Float32Array.BYTES_PER_ELEMENT

    let loc = attribute.location
    let type = webgl.FLOAT

    webgl.enableVertexAttribArray(loc)
    webgl.vertexAttribPointer(loc, size, type, normalized, stride, offset)
  }

  /**
   * Sets the value of a uniform for the shader that is currently loaded, via
   * Engine.Shader.setCurrent(). This allows for data to be passed into a given
   * shader for use when carrying out GPU vertex and fragment draw calculations.
   * This function is usually called by internal engine functions before a draw
   * operation is carried out; however, an engine user can also make use of it.
   *
   * Note that the type of the argument 'value' varies depending on what type
   * the uniform is within the shader. It will either be a number or array.
   *
   * @param {String} name  - Name of the uniform to be set.
   * @param {Mixed}  value - Value of the uniform (either array or number).
   */
  setUniform (name, value)
  {
    // We can only set uniforms that actually exist.
    if (!(name in this._.uniforms)) { return }
    let uniform = this._.uniforms[name]

    var webgl = Engine.Canvas._.webgl

    var loc  = uniform.location
    var type = uniform.type

    switch (type) {
      case webgl.FLOAT:      webgl.uniform1f       (loc,           value); break
      case webgl.FLOAT_VEC2: webgl.uniform2f       (loc,        ...value); break
      case webgl.FLOAT_VEC3: webgl.uniform3f       (loc,        ...value); break
      case webgl.FLOAT_VEC4: webgl.uniform4f       (loc,        ...value); break
      case webgl.FLOAT_MAT3: webgl.uniformMatrix3fv(loc, false,    value); break
      case webgl.SAMPLER_2D: webgl.uniform1i       (loc,           value); break

      default:
        console.error('Attempt to set unknown uniform type: ' + type)
        return
    }
  }
}
