/*
 * Methods.h
 *
 *  Created on: Nov 6, 2014
 *      Author: costin
 */

#ifndef METHODS_H_
#define METHODS_H_

#include "Initiators.h"


/*
 * ##################################################################################
 * ## Draw Text
 * ##################################################################################
 */

/*
 * Not sure if this is old or new.. it works. I got it from stackoverflow:
 *
 * http://stackoverflow.com/questions/8847899/opengl-how-to-draw-text-using-only-opengl-methods
 *
 */

void drawString (char *s, float x, float y){
     unsigned int i;
     glRasterPos2f(x, y);

     for (i = 0; i < strlen (s); i++)
          glutBitmapCharacter (GLUT_BITMAP_9_BY_15 , s[i]);
}


/*
 * ##################################################################################
 * ## Tile Management
 * ##################################################################################
 */
bool removeOneBlock(int x, int y) {
	if( x<0 || x>9 || y<0 || y>19) return false; //out of bounds
	int s;
	//animateBlock(x,y);
	for (int t_y = y; t_y < 18; t_y++) {
		for(int i=0; i< s_block; i++) {
			s = (t_y * s_row) + (x * s_block) + i;
			boardcolours[s] = boardcolours[s + s_row];
		}
		board[x][t_y] = board[x][t_y+1];
		board_colors[x][t_y] = board_colors[x][t_y+1];
	}
	board[x][19] = false;
	board_colors[x][19] = black;
	for(int i=0; i < s_block; i++) {
		s = (19 * s_row) + (x * s_block) + i;
		boardcolours[s] = black;
	}


	return true;
}

void setCellColor(int x, int y, vec4 new_c) {
	board_colors[x][y] = new_c;
	GLfloat s = (y * s_row) + (x * s_block);
	for(int i=s; i< s+s_block; i++) {
		boardcolours[i] = new_c;
	}
}

bool cellAlphaDown(int x, int y) {
	if(board_colors[x][y].w > 0.00f) {
		vec4 new_c = board_colors[x][y];
		new_c.w = new_c.w - alphaDown;
		setCellColor(x,y, new_c);
		return true;
	}
	return false;
}


//removes one block and makes everything above it shift down
// block to remove located at x,y on the board
bool markBlockDelete(int x, int y) {
	if( x<0 || x>9 || y<0 || y>19) return false; //out of bounds
	int s = (y * s_row) + (x * s_block);
	board[x][y] = false;
	board_colors[x][y] = black;
	for(int i=s; i < s+s_block; i++) {
		boardcolours[i] = black;
	}
	score += 40;
	if(drop_time >2) {
		drop_time = 8 - score/2000;
	}

	return true;
}

//-------------------------

void checkRemoveQueue() {
	for(vector<vec2>::iterator it = toRemove.begin(); it != toRemove.end();) {
		if(cellAlphaDown(it->x, it->y)){
			it++;
		} else {

			if(debug) { cout<<"\tRemoving:"<<it->x<<" . "<<it->y<<endl;}
			markBlockDelete(it->x, it->y);
			it = toRemove.erase(it);
		}
	}
}

/*
 * ##################################################################################
 * ## CURRENT TILE
 * ##################################################################################
 */



//check if rotation is valid
bool rotateCheck(vec2* nextRotation)
{
	return true;
//-------- not needed anymore--------------
	int t_y;
	int t_x;
	bool ret = true;
	for (int i = 0; i<4;i++)
	{
		t_x = tilepos.x + nextRotation[i].x;
		t_y = tilepos.y + nextRotation[i].y;
		if (t_x < 0 || t_x > 9 || t_y < 0 || t_y > 19 || board[t_x][t_y]) {
			ret = false;
			break;
		}
	}

	return ret;
}


bool shiftColors(bool grey)
{
	if(pause_b)
		return true;

	vec4 newcolours[24*6];

	if(grey) {
		for (int i = 0; i < 4; i++) {
			for (int s = 0; s < 36; s++)
				newcolours[(i*36)+s] = grey2;
		}
	} else {
		for (int i = 0; i < 4; i++) {
			for (int s = 0; s < 36; s++)
				newcolours[(i*36)+s] = tile_color[i];
		}
	}
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[VBOTetriColor]); // Bind the VBO containing current tile vertex colors
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(newcolours), newcolours); // Put the color data in the VBO

	return true;
}

void collisionAndGray() {
	bool curr_col = collisionCheck();
	if( curr_col && !collision ) {
		collision = true;
		shiftColors(collision);
	} else if(!curr_col && collision) {
		collision = false;
		shiftColors(collision);
	}
}

// Rotates the current tile, if there is room
bool rotate()
{
	if(pause_b)
		return true;

	int curr = floor (rotation/4);
	int next = floor ((rotation+1)/4);

	//cout<<"curr is: "<<curr<<" | rotation is: "<<rotation<<" | next is: "<<next<<endl;

	if (next > curr) {
		curr = curr*4;
		if (rotateCheck(allRotationShape[curr])) {
			for (int i = 0; i < 4; i++)
				tile[i] = allRotationShape[curr][i]; // Get the 4 pieces of the new tile
			updatetile();
			rotation = curr;
		}
	} else {
		if (rotateCheck(allRotationShape[rotation+1])) {
			for (int i = 0; i < 4; i++)
				tile[i] = allRotationShape[rotation+1][i]; // Get the 4 pieces of the new tile
			updatetile();
			rotation++;
		}
	}
	collisionAndGray();
	return true;
}



// Checks if the specified row (0 is the bottom 19 the top) is full
// If every cell in the row is occupied, it will clear that cell and everything above it will shift down one row
void checkfullrow( )
{
	int y, x = 0;
	bool full;
	for (y = 0; y <BOARD_HEIGHT; y++) {
		full = true;
		for (x = 0; x < BOARD_WIDTH; x++) {
			if(!board[x][y]) {
				full = false;
			}
		}
		if(full) {
			for (x = 0; x < BOARD_WIDTH; x++) {
				toRemove.push_back(vec2(x,y));
			}
		}
	}
}

// Given (x,y), tries to move the tile x squares to the right and y squares down
// Returns true if the tile was successfully moved, or false if there was some issue
bool movetile(vec2 direction)
{
	if(pause_b)
		return true;
	float t_y;
	float t_x;
	bool ret = true;
	for (int i = 0; i<4;i++)
	{
		t_x = tilepos.x + direction.x + tile[i].x;
		t_y = tilepos.y + direction.y + tile[i].y;
		if (t_x < -1 || t_x > 10 || t_y < 0.0f || t_y > 20) {
			ret = false;
			break;
		}
	}

	if(ret) {
		tilepos.x += direction.x;
		tilepos.y += direction.y;
	}
	return ret;
}

bool movetileDown(vec2 direction)
{
	if(pause_b)
		return true;
	float t_y;
	float t_x;
	bool ret = true;
	for (int i = 0; i<4;i++)
	{
		t_x = tilepos.x + direction.x + tile[i].x;
		t_y = tilepos.y + direction.y + tile[i].y;
		if (t_x < -1 || t_x > 10 || t_y < 0.0f || t_y > 20 || board[(int)floor(t_x)][(int)floor(t_y)]) {
			ret = false;
			break;
		}
	}

	if(ret) {
		tilepos.x += direction.x;
		tilepos.y += direction.y;
	}
	return ret;
}

//-------------------------------------------------------------------------------------------------

/*
 * ##################################################################################
 * ## Robot Arm
 * ##################################################################################
 */




void moveArmY (bool up) {
	using namespace robo_arm;
	if (up && movetile( vec2(0,1) )) {
		startPosition.y++;
		// Incrase the joint angle
		Theta[1] = findAngles(startPosition).x;
		Theta[2] = findAngles(startPosition).y;

	} else if(!up && movetile( vec2(0,-1) )) {
		startPosition.y--;

		Theta[1] = findAngles(startPosition).x;
		Theta[2] = findAngles(startPosition).y;
	}

	collisionAndGray();
	//glutPostRedisplay();

}

void moveArmX (bool left) {

	using namespace robo_arm;
	if (left && movetile( vec2(-1,0) )) {
		startPosition.x--;
		// Incrase the joint angle
		Theta[1] = findAngles(startPosition).x;
		Theta[2] = findAngles(startPosition).y;

	} else if(!left && movetile( vec2(1,0) )) {
		startPosition.x++;

		Theta[1] = findAngles(startPosition).x;
		Theta[2] = findAngles(startPosition).y;
	}

	collisionAndGray();
	//glutPostRedisplay();


}

/* ################################################################################################
 * ##  Gravity related Code
 * ################################################################################################
 */
//for debugging
void testFindShape(vector<vec2> ins) {
	vector<vec2> in = ins;
	while(!in.empty()) {
		vec2 tmp = in.back();
		in.pop_back();
		cout<<" | "<<tmp.x<<", "<<tmp.y;
	}
	cout<<" ## "<<endl;
}


//traces out the shape from a full tile
vector<vec2> findShape(int x, int y) {

	stack<vec2> loc_stack;
	loc_stack.push(vec2(x,y));
	vector<vec2> ret;
	int i =0;
	int lx, ly;

	while(!loc_stack.empty()) {

		vec2 t = loc_stack.top();
		loc_stack.pop();
		ret.push_back(t);
		lx = t.x;
		ly = t.y;

		map[lx][ly] = false; 	// remove it from the map
		if( map[lx-1][ly] )		//look left
			loc_stack.push(vec2(lx-1,ly));
		if( map[lx+1][ly] )		//look right
			loc_stack.push(vec2(lx+1,ly));
		if( map[lx][ly-1] )		//look down
			loc_stack.push(vec2(lx,ly-1));
		if( map[lx][ly+1] )		//look up
			loc_stack.push(vec2(lx,ly+1));

		i++;
	}
	return ret;
}

//returns -10 if empty;
vec2 findNonEmpty()
{
	for(int x = 0; x < 10; x++) {
		for(int y=0; y<20; y++) {
			if(map[x][y])
				return vec2(x,y);

		}
	}
	return vec2(-10, -10);;
}


//finds any orphans (floating structures not connected to anything)
vector<vec2> findOrphans ()
{
	for(int xa = 0; xa < 10; xa++)
		for(int ya = 0; ya <20; ya++)
			map[xa][ya] = board[xa][ya];

	vector<vec2> ret;


	while (findNonEmpty().x > -10)
	{
		ret = findShape(findNonEmpty().x, findNonEmpty().y);
		bool grounded = false;
		for(vec2 s : ret)
		{
			if(s.y == 0) {
				grounded = true;
			}
		}
		if(!grounded) {
			return ret;
		}
	}
	vector<vec2> empty;
	empty.clear();
	return empty;
}


//debug helper
void printMap (bool i[10][20]) {
	for (int y = 19; y > -1; y--) {
		for (int x = 0; x <10; x++) {

			if(i[x][y])
				cout<<"|O|";
			else
				cout<<"|_|";
		}
		cout<<endl;
	}
	cout<<endl;
}
//fixer an orphaned block by moving the whole structure down until one of the bottom blocks finds support
bool fixOrphans(vector<vec2> fix) {

	if(fix.empty()) // nothing to work on
		return false;


	bool rebuild[10][20] = {false}; //load the object on an empty map to do some math on it
	for (vec2 s : fix) {
		int x = s.x;
		int y = s.y;
		rebuild[x][y] = true;
	}
	//some locals to keep track of the bottom most tiles and the distance to the next tile underneath
	int bottom[10];
	int distance[10];

	if(debug)
		printMap(rebuild);

	//find the bottom tiles of the object -- moving it down will involved shifting it into empty tiles underneath it
	for (int x = 0; x < 10; x++) {
		bool found = false;
		bottom[x] = -1; //assume there's no tile, if the loop finds one above 0, it will be overwritten.
		for (int y = 0; y<20 && !found; y++) {
			if(rebuild[x][y]) {
				bottom[x] = y;
				found = true;
			}
		}
	}

	if(debug)
		printMap(rebuild);

	int min = 1000; //needs to be initialized big...

	for (int x = 0; x < 10; x++) { //find the distance between our objects tiles and the nearest support underneath it
		bool found = false;
		distance[x] = 0;
		for (int y = bottom[x]-1; y>-1 && !found; y--) {
			if(!board[x][y]) {
				distance[x]++;
			} else {
				found = true;
			}
		}
		if(distance[x] < min && distance[x] > 0) //pick the smallest one which will tell us how much to shift the object
			min = distance[x];
	}

	int tmpss;
	for (int x = 0; x <10; x++) { //shift it down
		if (bottom[x] > 0 ) {
			for(int y = 1; y < min+1; y++)
				tmpss =  bottom[x]-y;
				removeOneBlock(x, tmpss);
		}
	}

	return true;
}


/*===========================================
 * keeps track of top entry in each column:
 *  - assume 0
 *  - scan from the top, first full slot gets stored -> move to next column
 *
 */

void updateTop() {
	for (int x = 0; x < 10; x++) {
		bool found = false;
		board_top[x] = 0; //assume there's no tile, if the loop finds one above 0, it will be overwritten.
		for (int y = 19; y>0 && !found; y--) {
			if(board[x][y]) {
				board_top[x] = y;
				found = true;
			}
		}
	}
}


/* Gravity
 *
 * My design decision is to let the game play as tetris and thus have gaps under block assuming the shape holds everything on top.
 * My As soon as a color match-remove was done, we assume the supports for the column crash down to the next nearest support below.
 * this makes the game more interesting than breaking block shape and letting everything fill gaps, thus removing the tetris side of things..
 *
 * USAGE	:
 * 		pass in the coordinates of the block to check under, the block being passes is ASSUMED to be filled
 * Returns	:
 * 		true : if changes were made
 * 		false: nothing done
 */

bool gravity (int x, int y)
{
	bool done = false;
	int ym;
	for(ym = y-1; ym > -1 && !done; ym--) {
		if(board[x][ym]) { // stop, a block is here
			done=true;
		} else {
			removeOneBlock(x,ym);
		}
	}

	updateTop();
	return !(ym == y-1);
}


bool removePattern(int x, int y, int dir)
{
	bool next = true;
	int tx = x;
	int ty = y;
	vec4 tmp_c = board_colors[tx][ty];
	bool grav = false;
	vec2 gravY;
	while(next) {
		toRemove.push_back(vec2(tx,ty));
		if(dir==0) 	{ toGravity.push_back(vec2(tx,ty)); tx++; next = (board_colors[tx][ty]==tmp_c); tmp_c = board_colors[tx][ty];}
		else		{ grav = true; ty++; next = (board_colors[tx][ty]==tmp_c); tmp_c = board_colors[tx][ty]; gravY.x = tx; gravY.y = ty;} //don't need to look at the next one up as removeBlock will bring it all down back to the same spot.

	}
	if(grav)
		toGravity.push_back(gravY);
	return true;
}

bool find4() //pattern matching,  O 4*( X * Y) ~> O( max(x,y) ^2) as a safe upper bound, improvement possible but hard to code..
{

	for (int y=0; y<20; y++)
	{
		for(int x=0; x<10; x++)
		{
			for(int dir=0; dir<2; dir++)
			{
				int lx = x;
				int ly = y;
				bool match = true;
				for(int i=0; i<3 && match; i++)
				{

					if(dir == 0 && lx+1 < 10){ //look right
						if(board_colors[lx][y].zero() || board_colors[lx+1][y].zero() || !(board_colors[lx][y] == board_colors[lx+1][y])) { match = false;}
						lx++;
					} else if(dir == 1 && ly+1 < 20){ // look up
						if(board_colors[x][ly].zero() || board_colors[x][ly+1].zero() || !(board_colors[x][ly] == board_colors[x][ly+1])) { match = false;}
						ly++;
					} else { //out of bounds
						match = false;
					}
				}
				if (match) {
					if(debug) { cout<<"Remove at (x,y) "<<x+1<<" , "<<y+1<<" direction: "<<dir<<endl;}
					return removePattern(x,y,dir);
				}
			}
		}
	}
	return false; //nothing found if it got here
}

//-------------------------------------------------------------------------------------------------------------------
//TOOLS:
string boolPrint(bool i) {
	if(i) return "true";
	return "false";
}
//-------------------------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------------------

// Places the current tile - update the board vertex colour VBO and the array maintaining occupied cells
void settile()
{
	int s, t_x, t_y = 0;

	for (int i = 0; i<4;i++) // for every block
	{
		t_x = tilepos.x + tile[i].x; // find location on the board
		t_y = tilepos.y + tile[i].y;
		s = (t_y * s_row) + (t_x * s_block); //find block starting position in the vertex array
		board[t_x][t_y] = true; //set occupied
		board_colors[t_x][t_y] = tile_color[i]; //save color info
		for(int j = s; j< (s+s_block); j++) {   // cycle through vertices for the block on the board
			boardcolours[j] = tile_color[i];
		}

	}

	checkfullrow();

	bool tmp;
	tmp = find4();
	if(tmp)
		tmp=find4();
	if(debug) {cout<<"Pattern match: "<< boolPrint(tmp) <<endl;}

	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[VBOBoardColor]);
	glBufferData(GL_ARRAY_BUFFER, BOARD_POINTS*sizeof(vec4), boardcolours, GL_DYNAMIC_DRAW);
	//initGrid();
	updateTop();
	newtile();
	collisionAndGray();

}

//-------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------

#endif /* METHODS_H_ */
