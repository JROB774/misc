extends Node2D

# member variables
var screensize
var padsize
var direction = Vector2(1.0, 0.0)

# constant speed values (in pixels/seccond)
const INITIAL_BALL_SPEED = 80
const PAD_SPEED = 150

# speed of the ball (in pixels/second)
var ballspeed = INITIAL_BALL_SPEED

func _ready():
	screensize = get_viewport_rect().size
	padsize = get_node("paddle_left").get_texture().get_size()
	set_process(true)
	return

func _process(delta):
	var ballpos = get_node("ball").get_pos()
	var rect_left = Rect2(get_node("paddle_left").get_pos() - padsize * 0.5, padsize)
	var rect_right = Rect2(get_node("paddle_right").get_pos() - padsize * 0.5, padsize)
	
	# move the ball in the current direction
	ballpos += direction * ballspeed * delta
	
	# flip when touching the roof or floor
	if ((ballpos.y < 0 && direction.y < 0) || (ballpos.y > screensize.y && direction.y > 0)):
		direction.y = -direction.y
	# flip, change direction and increase speed on pad touch
	if ((rect_left.has_point(ballpos) && direction.x < 0) || (rect_right.has_point(ballpos) && direction.x > 0)):
		direction.x = -direction.x
		direction.y = randf() * 2.0 - 1
		direction = direction.normalized()
		ballspeed *= 1.1
	
	# check for a game over
	if (ballpos.x < 0 || ballpos.x > screensize.x):
		ballpos = screensize * 0.5
		ballspeed = INITIAL_BALL_SPEED
		direction = Vector2(-1, 0)
	
	# update the ball node with the new position value
	get_node("ball").set_pos(ballpos)
	
	# move the left pad based on user input
	var leftpos = get_node("paddle_left").get_pos()
	
	if (leftpos.y > 0 && Input.is_action_pressed("left_move_up")):
		leftpos.y += -PAD_SPEED * delta
	if (leftpos.y < screensize.y && Input.is_action_pressed("left_move_down")):
		leftpos.y += PAD_SPEED * delta
	
	get_node("paddle_left").set_pos(leftpos)
	
	# move the right pad based on user input
	var rightpos = get_node("paddle_right").get_pos()
	
	if (rightpos.y > 0 && Input.is_action_pressed("right_move_up")):
		rightpos.y += -PAD_SPEED * delta
	if (rightpos.y < screensize.y && Input.is_action_pressed("right_move_down")):
		rightpos.y += PAD_SPEED * delta
	
	get_node("paddle_right").set_pos(rightpos)
	
	return