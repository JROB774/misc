Engine.Utility =
{
  /**
   * Loads a file of the desired type asynchronously. Once the loading process
   * is complete the returned Promise triggers its then() function. This
   * function makes use of the JavaScript Promise API to handle async control /
   * flow of the program, see the following for more information:
   *
   * <developer.mozilla.org/Web/JavaScript/Guide/Using_promises>
   *
   * The optional 'responseType' parameter must be a one of the valid response
   * type specifier strings. A list of these string values can be found here:
   *
   * <developer.mozilla.org/Web/API/XMLHttpRequest/responseType>
   *
   * @param  {String} url          - URL/Name of the file to load.
   * @param  {String} responseType - Type of the data to be returned.
   *
   * @return {Object} A Promise object that can be used to handle flow.
   */
  loadFile: function (url, responseType = "text")
  {
    return new Promise(function (resolve, reject)
    {
      var httpRequest = new XMLHttpRequest()

      httpRequest.responseType = responseType
      httpRequest.open('GET', url, true) // True for asynchronous loading.
      httpRequest.onload = function ()
      {
        // A status of 200 means that the HTTP request was successful.
        resolve((httpRequest.status === 200) ? httpRequest.response : null)
      }

      httpRequest.send()
    })
  },

  /**
   * Performs a standard sine operation using time, but it ensures that the
   * resulting value will remain within the specified range of values.
   *
   * @param  {Number} time - Time value to use in the sine operation.
   * @param  {Number} min  - Minimum value of the sine operation (inclusive).
   * @param  {Number} max  - Maximum value of the sine operation (inclusive).
   *
   * @return {Number} A sine wave value within the specified range.
   */
  sinInRange: function (time, min, max)
  {
    let halfRange = (max - min) / 2
    return (min + halfRange + (Math.sin(time) * halfRange))
  },

  /**
   * Clamps a value within the specified range of numbers.
   *
   * @param  {Number} value - Value to clamp in the range.
   * @param  {Number} min   - Minimum value of the number (inclusive).
   * @param  {Number} max   - Maximum value of the number (inclusive).
   *
   * @return {Number} Value clamped within the range.
   */
  clamp: function (value, min, max)
  {
    if (value < min) { return min }
    else if (value > max) { return max }

    return value
  },

  /**
   * Returns a random integer within the specified range of values.
   *
   * @param  {Number} min - Minimum random value to generate (inclusive).
   * @param  {Number} max - Maximum random value to generate (inclusive).
   *
   * @return {Number} A random integer within the range.
   */
  randomRangeInteger: function (min, max)
  {
    min = Math.ceil(min)
    max = Math.floor(max)

    // THis is a standardized method of generating ranged integers.
    return (Math.floor(Math.random() * ((max + 1) - min)) + min)
  },

  /**
   * Returns a random number within the specified range of values.
   *
   * @param  {Number} min - Minimum random value to generate (inclusive).
   * @param  {Number} max - Maximum random value to generate (inclusive).
   *
   * @return {Number} A random number within the range.
   */
  randomRangeNumber: function (min, max)
  {
    // This is a standardized method of generating ranged numbers.
    return ((Math.random() * ((max + 1) - min)) + min)
  },
}
