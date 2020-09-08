/*******************************************************************************
 * @file An implementation of the classic retro game 'Snake' within JavaScript.
 * @license {@link https://unlicense.org/ The Unlicense (Public Domain)}
 * @author Joshua Robertson
 *
 ******************************************************************************/

const START_POS = 3;
const TILE_SIZE = 10;
const START_SPEED = 10;
const MAX_SPEED = 30;

var canvas;
var speed;
var snake;
var food;

function centerCanvas ()
{
  // centers the canvas in the window
  var x = (windowWidth - width) / 2;
  var y = (windowHeight - height) / 2;

  canvas.position(x, y);
}

function spawnFood ()
{
  while (true) {
    // generates a new random food position
    var x = floor(random(width / TILE_SIZE)) * TILE_SIZE;
    var y = floor(random(height / TILE_SIZE)) * TILE_SIZE;

    // makes sure it is not inside the snake or its tail
    if (x === snake.x && y === snake.y) { continue; }
    else {
      var intail = false;
      for (var i = 0; i < snake.tail.length; ++i) {
        if (x === snake.tail[i].x && y === snake.tail[i].y) {
          intail = true;
        }
      }
      if (intail) { continue; }
    }

    // spawns the food once valid
    food = createVector(x, y);
    break;
  }
}

function Snake ()
{
  // gives the snake its default values
  this.x = START_POS * TILE_SIZE;
  this.y = START_POS * TILE_SIZE;
  this.hdir = 1;
  this.vdir = 0;
  this.dead = false;
  this.paused = false;
  this.score = 0;
  this.tail = [];

  this.input = function ()
  {
    // only let the player control the snake whilst alive
    if (!this.dead) {
      switch (keyCode) {
        // sets the snakes new direction
        case (LEFT_ARROW): if (this.hdir !== 1) { this.hdir = -1, this.vdir = 0; } break;
        case (UP_ARROW):  if (this.vdir !== 1) { this.hdir = 0, this.vdir = -1; } break;
        case (RIGHT_ARROW): if (this.hdir !== -1) { this.hdir = 1, this.vdir = 0; } break;
        case (DOWN_ARROW): if (this.vdir !== -1) { this.hdir = 0, this.vdir = 1; } break;

        // toggles pausing on and off
        case (ESCAPE): { this.paused = !this.paused; break; }
      }
    }
    // lets the user restart on the game over screen
    else { if (key == 'r' || key == 'R') { reset(); } }
  }

  this.update = function ()
  {
    // updates each of the tail pieces starting from the end to the head
    for (var i = this.tail.length - 1; i >= 0; --i) {
      if (i === 0) { this.tail[i].x = this.x, this.tail[i].y = this.y; }
      else { this.tail[i].x = this.tail[i-1].x, this.tail[i].y = this.tail[i-1].y; }
    }

    // moves a whole tile based on the current direction
    if (this.hdir === -1) { this.x -= TILE_SIZE; }
    else if (this.hdir === 1) { this.x += TILE_SIZE; }
    if (this.vdir === -1) { this.y -= TILE_SIZE; }
    else if (this.vdir === 1) { this.y += TILE_SIZE; }

    // if out of bounds or within a tail segment then the snake dies
    if (this.x < 0 || this.x >= width || this.y < 0 || this.y >= height) { this.dead = true; }
    for (var i = 0; i < this.tail.length; ++i) { if (this.tail[i].x === this.x && this.tail[i].y === this.y) { this.dead = true; break; } }

    // if in contact with a food piece...
    if (this.x === food.x && this.y === food.y) {
      // the snake increases and gains scroe
      this.score += 1;
      this.tail.push(createVector(this.x, this.y));
      // new food is spawned
      spawnFood();
      // the speed/difficulty increases
      if (speed < MAX_SPEED) {
        speed += 0.1;
        frameRate(speed);
      }
    }
  }

  this.render = function ()
  {
    // draws the snake as a series of rects
    fill(0xFF);
    noStroke();
    for (var i = 0; i < this.tail.length; ++i) { rect(this.tail[i].x, this.tail[i].y, TILE_SIZE, TILE_SIZE); }
    rect(this.x, this.y, TILE_SIZE, TILE_SIZE);
  }
}

function reset ()
{
  // resets the snake's starting values
  snake.x = START_POS * TILE_SIZE;
  snake.y = START_POS * TILE_SIZE;
  snake.hdir = 1;
  snake.vdir = 0;
  snake.dead = false;
  snake.paused = false;
  snake.score = 0;
  snake.tail = [];

  // resets the speed/difficulty
  speed = START_SPEED;
  frameRate(speed);

  // spawns a new piece of food
  spawnFood();
}

function setup ()
{
  // creates and center the canvas
  canvas = createCanvas(320, 240);
  centerCanvas();
  // sets the text alignment
  textAlign(CENTER, CENTER);

  // sets the speed/difficulty
  speed = START_SPEED;
  frameRate(speed);

  // creates all the entities
  snake = new Snake();
  spawnFood();
}

function windowResized ()
{
  // centers the canvas if the browser is resized
  centerCanvas();
}

function keyPressed ()
{
  // handles snakes input when a key is pressed
  snake.input();
}

function draw ()
{
  // fills in the background
  background(0x2F);

  // the active game screen
  if (!snake.dead) {
    if (!snake.paused) {
      snake.update();
      snake.render();

      fill(0xF0, 0xF0, 0x3F);
      noStroke();
      rect(food.x, food.y, TILE_SIZE, TILE_SIZE);
    }
    // the pause screen
    else {
      fill(0xFF);
      textSize(32);
      text("PAUSED", width / 2, height / 2);
    }
  }
  // the game over screen
  else {
    fill(0xFF);
    textSize(32);
    text("GAME OVER!", width / 2, height / 2);
    textSize(16);
    text("Score: " + snake.score.toString(), width / 2, (height / 2) + 32);
  }
}
