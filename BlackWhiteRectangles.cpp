#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <string>
#include <set>
#include <sstream>
using namespace std;
/* BlackWhiteRectangles.cpp
 * Trevor Sharpe
 * September 24, 2015
 *
 * This class is used to calculate the number of black squares present on a grid upon several rectangles of
 * various patterns and sizes being applied to it.*/

class BlackWhiteRectangles{
	public:
		int blackCount(vector<string> r);
};

//a class used to store the information about the applied rectangles from the input
class Rect{
	public:
		int x1;
		int y1;
		int x2;
		int y2;
		int type;
};

int BlackWhiteRectangles::blackCount(vector<string> r){
	set<int> X; //a set to sort all the x vertices of the rectangles applied to the grid
	set<int> Y; //a set to sort all the y vertices of the rectangles applied to the grid
	set<int>::iterator xit;
	set<int>::iterator yit;
	vector<Rect*> rectList; //used to store pointers to the data for all of the rectangles applied to the grid
	vector<int> pList; //used to store a list of possible patterns for a 2x2 square in the grid
	Rect *nextR; //used to retrieve the relevant information from each subsequent applied rectangle
	/* i is a loop iterator, tmp is a temporary variable used to determine how many squares in a 2x2 square are
	 * black,
	 * pattern is used to bitwise store the color of each digit in a 2x2 square, with the leftmost digit
	 * being the upper left corner of the square, the second from the left digit being the upper right, the third
	 * the bottom left, and the rightmost digit being the bottom right,
	 * lastX and lastY store the lower x and y bound of the sub-rectangle being considered, respectively
	 * evenX and evenY are the upper x and y bound of the largest even height and even width rectangle to fit
	 * within the given sub-rectangle
	 * cflag and rflag store whether or not the height or width, respectively, of the given sub-rectangle are odd
	 * rv is the return value for the function*/
	int i, tmp, pattern, lastX, lastY, evenX, evenY, cflag, rflag, rv;
	istringstream buffer; //used to read in the variables from each string

	//stores each possible pattern that can be applied by a rectangle to a sub-rectangle
	pList.push_back(0b1111); //type 1 - every square black
	pList.push_back(0b1100); //type 2 - every odd row black
	pList.push_back(0b0101); //type 3 - every odd column black
	pList.push_back(0b0110); //type 4 - every odd/odd or even/even row/column black (a checkerboard)
	pList.push_back(0b0011); //type 5 - starting on an even row shifts the list index by 3
	pList.push_back(0b1010); //type 6 - starting on an even column shifts the list index by 3
	pList.push_back(0b1001); //type 7 - starting on an odd/even row/column or column/row shifts the list index by 3

	//reads in each rectangle and stores the data in a member of the Rect class in the storage vector
	for (i=0;i<r.size();i++){
		nextR=new Rect;
		buffer.clear();
		buffer.str(r[i]);
		buffer>>nextR->x1;
		buffer>>nextR->y1;
		buffer>>nextR->x2;
		buffer>>nextR->y2;
		buffer>>nextR->type;
		X.insert(nextR->x1);
		X.insert(nextR->x2);
		Y.insert(nextR->y1);
		Y.insert(nextR->y2);
		rectList.push_back(nextR);
	}

	/*initializes the return value and iterates through every consecutive pair of y values in the set
	 * for every consecutive pair of x values in the set, thus examining every possible sub-rectangle,
	 * where each sub=rectangle has the exact same pattern applied to it*/
	rv=0;
	xit=X.begin();
	if (xit!=X.end())
		xit++;
	lastX=*(X.begin());
	for (;xit!=X.end();xit++){
		yit=Y.begin();
		if (yit!=Y.end())
			yit++;
		lastY=*(Y.begin());
		for (;yit!=Y.end();yit++){

			/*resets the variables for pattern and odd height/width, then checks for an odd height or
			 * width and updates the appropriate variables accordingly*/
			pattern=0;
			cflag=0;
			rflag=0;
			if ((*xit-lastX)%2==1){
				evenX=*xit-1;
				cflag=1;
			}
			else
				evenX=*xit;
			if ((*yit-lastY)%2==1){
				evenY=*yit-1;
				rflag=1;
			}
			else
				evenY=*yit;

			/*iterates through each rectangle applied to the grid, checks to see if it was applied to the
			 * given sub-rectangle, then if it started on an odd row/column and bitwise ors the overall
			 * pattern for the sub-rectangle with the resulting applied pattern*/
			for (i=0;i<rectList.size();i++){
				nextR=rectList[i];
				if (lastX>=nextR->x1 && *xit<=nextR->x2 && lastY>=nextR->y1 && *yit<=nextR->y2){
					if (((lastY-nextR->y1)%2 && nextR->type==2) || ((lastX-nextR->x1)%2 && nextR->type==3) || (((lastX-nextR->x1)%2 != (lastY-nextR->y1)%2) && nextR->type==4))
						pattern |= pList[nextR->type+2];
					else
						pattern |= pList[nextR->type-1];
				}
			}

			/*resets the tmp variable before using it to determine the number of black squares in a 2x2
			 * square and adds total for the sub-rectangle to the return value, ignoring any potential last
			 * row or column if the height or width is odd*/
			tmp=0;
			for (i=0;i<4;i++){
				tmp+=((pattern>>i) & 1);
			}
			rv+=tmp*((evenX-lastX)/2)*((evenY-lastY)/2);

			/*if the height of the sub-rectangle is odd and the first row of the 2x2 square pattern is not blank,
			 * checks for if every or every other square is black and adds the extra row to the return value*/
			if (rflag && ((pattern & 0b11) != 0)){
				if ((pattern & 0b11) != 0b11)
					rv+=((evenX-lastX)/2);
				else
					rv+=evenX-lastX;
			}

			/*if the width of the sub-rectangle is odd and the first column of the 2x2 square pattern is not blank,
			 * checks for if every or every other square is black and adds the extra column to the return value*/
			if (cflag && ((pattern & 0b1010) != 0)){
				if ((pattern & 0b1010) != 0b1010)
					rv+=((evenY-lastY)/2);
				else
					rv+=evenY-lastY;
			}

			/*if only either the height or the width of the sub-rectangle is odd, and the bottom left corner square
			 * of the 2x2 square pattern is black, then adds the extra square to the return value*/
			if (rflag && cflag && ((pattern & 0b0010) == 0b0010))
				rv++;
			lastY=*yit; //updates the lower y bound to that of the next sub-rectangle, if there is one
		}
		lastX=*xit; //updates the lower x bound to that of the next sub-rectangle, if there is one
	}
	return rv;
}
