/*
CMPT 361 Assignment 2 - FruitTetris -- 3D
BY: Costin Ghiocel
SN: 301027183
 */



/*
 * I did a pretty poor and hasty job on Assignment 1 as i was in a bit of a rush, everything in 1 file with no header
 * is very poor design, i also had very stiff functions, poor naming conventions and a severe lack of modularity.
 *
 * Plan:
 * 	- Statics.h
 * 		* contains all definitions and enums for naming or type defs.
 * 		* renaming important things ensures i update everything properly and don't miss anything
 * 	- Methods.h
 * 		* will contain functions that manipulate map data such as collision detection etc..
 *
 * 	I'm not familiar enough with OpenGL design to go for an Object Oriented approach, it also seems as
 * 	this was designed for C, so it's better to just organize libraries.
 *
 */


#include "Methods.h" //chain of includes -_-






// Starts the game over - empties the board, creates new tiles, resets line counters
void restart()
{

	for (int i = 0; i < BOARD_POINTS; i++)
		boardcolours[i] = black; // Let the empty cells on the board be black

	for(int x=0;x<10;x++) {
		for(int y = 0; y<20; y++) {
			board_colors[x][y] = black;
			board[x][y] = false;
		}
	}

	updateTop();
	toRemove.clear();
	toGravity.clear();
	gravityCheck = true;
	released = false; // keep track if tile was released
	collision = false;
	timer=8;
	score = 0;
	drop_time = 8;
	pause_b = false;
	running = true;
	newtile();

}
//-------------------------------------------------------------------------------------------------------------------


// Draws the game
void display()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//every square = 33points, so.. 1 unit = 33 pts.
	Model = Translate(0, 10/2.0, 0)*
			Scale(1.0/33, 1.0/33, 1.0/33)*
			Translate(-33*10/2.0 - 33, -33*10/2.0 - 33, 0);



	if(!toRemove.empty()) {
		checkRemoveQueue();
		gravityCheck = true;
		while(fixOrphans( findOrphans()));

		// Grid cell vertex colours
		glBindBuffer(GL_ARRAY_BUFFER, vboIDs[VBOBoardColor]);
		glBufferSubData(GL_ARRAY_BUFFER,0, sizeof(boardcolours), boardcolours);
		//unbind
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	} else if(gravityCheck) {

		for(vector<vec2>::iterator it = toGravity.begin(); it != toGravity.end();) {
			gravity(it->x, it->y);
			it = toGravity.erase(it);
		}

		while(fixOrphans( findOrphans()));
		// Grid cell vertex colours
		glBindBuffer(GL_ARRAY_BUFFER, vboIDs[VBOBoardColor]);
		glBufferSubData(GL_ARRAY_BUFFER,0, sizeof(boardcolours), boardcolours);
		//unbind
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	/*
	 * ######################################
	 * # ROBO_ARM
	 * ######################################
	 */

	//pray.................
	glBindVertexArray(robo_arm::vao);
	//---------------------------------------
	// copy pasta magic

    // Accumulate ModelView Matrix as we traverse the tree
	robo_arm::model_view = RotateY(robo_arm::Theta[robo_arm::Base] )*View* Translate(-9, -1, -0.5f);
	robo_arm::base();

	robo_arm::model_view *= ( Translate(0.0, robo_arm::BASE_HEIGHT, 0.0)*
		    RotateZ(robo_arm::Theta[robo_arm::LowerArm]) );
	robo_arm::lower_arm();

	robo_arm::model_view *= ( Translate(0.0, robo_arm::LOWER_ARM_HEIGHT, 0.0) *
    		RotateZ(robo_arm::Theta[robo_arm::UpperArm]) );
    robo_arm::upper_arm();

    //END ROBO ARM ##########################


	mat4 model_view = View*Model;
	glUniformMatrix4fv( MV_loc, 1, GL_TRUE, model_view );

	//---------------------------------------

	glUniform1i(locxsize, xsize); // x and y sizes are passed to the shader program to maintain shape of the vertices on screen
	glUniform1i(locysize, ysize);

	glBindVertexArray(vaoIDs[VAOfloor]); // Bind the VAO representing the floor
	glDrawArrays(GL_TRIANGLES, 0, floor_tile_points);

	glBindVertexArray(vaoIDs[VAOBoard]); // Bind the VAO representing the grid cells
	glDrawArrays(GL_TRIANGLES, 0, BOARD_POINTS); // Draw the board (10*20*2 = 400 triangles)

	glBindVertexArray(vaoIDs[VAOTile]); // Bind the VAO representing the current tile (to be drawn on top of the board)
	glDrawArrays(GL_TRIANGLES, 0, 24*6); // Draw the current tile (8 triangles)

	glBindVertexArray(vaoIDs[VAOGrid]); // Bind the VAO representing the grid lines (to be drawn on top of everything else)
	glDrawArrays(GL_LINES, 0, gridPoints); // Draw the grid lines (21+11 = 32 lines)

	//TEXT

	char s[40];
	sprintf(s, "Time to place block: %4.2f", timer );
	drawString(s, -0.3, 0.9f);
	sprintf(s, "Score: %d", score );
	drawString(s, -0.8f, 0.9f);

	if(pause_b) {
		drawString("Paused", 0, 0);
	}

	glutSwapBuffers();
}


//-------------------------------------------------------------------------------------------------------------------

// Reshape callback will simply change xsize and ysize variables, which are passed to the vertex shader
// to keep the game the same from stretching if the window is stretched
void reshape(GLsizei w, GLsizei h)
{
	xsize = w;
	ysize = h;
	glViewport(0, 0, w, h);

	Projection = Perspective(50, 1.0*xsize/ysize, 1, 100);
	glUniformMatrix4fv( Proj_loc, 1, GL_TRUE, Projection );


}

//-------------------------------------------------------------------------------------------------------------------

// Handle arrow key keypresses
void special(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		if(glutGetModifiers() == GLUT_ACTIVE_CTRL)
			View *= RotateY(-5);


		break;
	case GLUT_KEY_RIGHT:
		if(glutGetModifiers() == GLUT_ACTIVE_CTRL)
			View *= RotateY(5);


		break;
	case GLUT_KEY_UP:

		rotate();

		break;
	case GLUT_KEY_DOWN:
		/*
		if(movetile( vec2(0,-1) ) ) {
			updatetile();
		} else {
			settile();
		}
		*/
		break;
	}
	glutPostRedisplay();
}

//-------------------------------------------------------------------------------------------------------------------
// Handles standard keypresses
void keyboard(unsigned char key, int x, int y)
{
	//if(debug) {cout<<"Key is.. "<<key<<endl;}
	switch(key) 
	{
	case 033: // Both escape key and 'q' cause the game to exit
		exit(EXIT_SUCCESS);
		break;
	case 'q':
		exit (0);
		break;
	case ' ':
		if (!released && !collision && !pause_b) {
			released = true;
		}
		break;
	case 'p': //mostly for debugging (suspends movement)
		if(pause_b)
			pause_b=false;
		else
			pause_b=true;
		break;
	case 'r': // 'r' key restarts the game
		restart();
		break;
	case 't':
		//testFindShape(findShape(0,0));
		break;
	case 'w':
		if(!released && !pause_b)
			moveArmY(true);
		break;
	case 's':
		if(!released && !pause_b)
			moveArmY(false);
		break;
	case 'a':
		if(!released && !pause_b)
			moveArmX(true);
		break;
	case 'd':
		if(!released && !pause_b)
			moveArmX(false);
		break;
	}

	updatetile();
	glutPostRedisplay();
}

//-------------------------------------------------------------------------------------------------------------------

void move_down (int i) {

	if(!pause_b) {
		if(!released)
			timer -= 0.05;
		if(released) {
			if(movetileDown( vec2(0,-0.5) ) ) {
				updatetile();
			} else {
				released = false;
				settile();
			}
		}else if(timer < 0.0f) {
			if (!released && !collision && !pause_b) {
				released = true;
			} else {
				newtile();
				collisionAndGray();
			}
			timer = drop_time;
		}


	}
	glutTimerFunc(TIME_PER_MOVE, move_down, 0);

}
void idle(void)
{
	glutPostRedisplay();
}

//-------------------------------------------------------------------------------------------------------------------

int main(int argc, char **argv)
{
	//--------------------------------------
	srand (time(NULL)); //initiate random seed
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_MULTISAMPLE | GLUT_DEPTH | GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(xsize, ysize);
	glutInitWindowPosition(680, 178); // Center the game window (well, on a 1920x1080 display)
	glutCreateWindow("Fruit Tetris");
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable( GL_BLEND );

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearDepth(1.0);
	//system("canberra-gtk-play -f tetris.ogg&");
	glewInit();
	init();


	// start thread
	// Callback functions
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutSpecialFunc(special);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);
	glutTimerFunc(TIME_PER_MOVE, move_down, 0);
	glutMainLoop(); // Start main loop

	return 0;
}
