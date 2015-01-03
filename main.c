#include <stdlib.h>
#include <stdio.h>


#define NUMBIRDS 1
#define MAXHEIGHT 160
#define HARDUNLOCK 30
#define WRAPUNLOCK 15



#include "myLib.h"
#include "text.h"
#include "bird.h"
#include "birdUp.h"
#include "link.h"
#include "failure.h"
#include "win.h"

int splash();
void secret();
int game();
void lose();
void congratulate();

enum {SPLASH, GAME, LOSE, SECRET, CONGRATULATE};

static int easyHighScore = 0;
static int hardHighScore = 0;
static int seed = 0;
static int mirror = 0;
static int secretUnlocked = 0;
static int congratulated = 0;
static int difficulty;
int main()
{

	int state = SPLASH;

	while(1)
	{
		switch(state)
		{
		case SPLASH:
			state = splash();
			break;
		case GAME:
			state = game(difficulty);
			break;
		case LOSE:
			lose();
			state = SPLASH;
			break;
		case SECRET:
			secret();
			state = SPLASH;
			break;
		case CONGRATULATE:
			congratulate();
			state = SPLASH;
		}
	}
}

int splash()
{
	if (easyHighScore >= HARDUNLOCK && hardHighScore >= WRAPUNLOCK && secretUnlocked && !congratulated)
	{
		congratulated = 1;
		return CONGRATULATE;
	}

	//unsigned short *ptr = videoBuffer;
	REG_DISPCTL = MODE3 | BG2_ENABLE;
	char easy[41];
	char hard[41];
	sprintf(easy, "Easy: %d", easyHighScore);
	sprintf(hard, "Hard: %d", hardHighScore);


	fillScreen(WHITE);
	drawString(20, 20, "Shameless Flappy Bird Clone! 2.0", BLACK);
	drawString(150, 80, "Press Start", BLACK);
	drawString(45, 75, "High Scores", BLACK);
	drawString(65, 85, easy, BLACK);
	drawString(80, 85, hard, BLACK);
	if (easyHighScore >= HARDUNLOCK)
	{
		drawString(105, 150, "Hard", BLACK);
	}
	else
	{
		drawString(105, 150, "Hard", GREY);
	}
	drawString(105, 55, "Easy", BLACK);
	drawRect(115, 52, 5, 30, GREEN);
	
	if (hardHighScore >= WRAPUNLOCK)
	{
		drawString(50, 22, "Wrap", BLACK);
		drawString(65, 10, "Boundary", BLACK);
	}
	else
	{
		drawString(50, 22, "Wrap", GREY);
		drawString(65, 10, "Boundary", GREY);
	}
	
	drawString(60, 180, "No Wrap", BLACK);
	drawRect(75, 186, 5, 30, RED);

	if (secretUnlocked)
	{
		drawString(130, 60, "Secret Unlocked: Yes", BLACK);
	}
	else 
	{
		drawString(130, 60, "Secret Unlocked: No", BLACK);
	}
	difficulty = 0;
	mirror = 0;
	while(!KEY_DOWN_NOW(BUTTON_START))
	{
		if (hardHighScore >= WRAPUNLOCK)
		{
			if (KEY_DOWN_NOW(BUTTON_L) && mirror == 0)
			{
			drawRect(75, 186, 5, 30, WHITE);
			drawRect(75, 20, 5, 30, RED);
			mirror = 1;
			}
			while(KEY_DOWN_NOW(BUTTON_L));
			if (KEY_DOWN_NOW(BUTTON_R) && mirror == 1)
			{
				drawRect(75, 20, 5, 30, WHITE);
				drawRect(75, 186, 5, 30, RED);
				mirror = 0;
			}
			while(KEY_DOWN_NOW(BUTTON_R));
		}
		

		if (easyHighScore >= HARDUNLOCK)
		{
			if (KEY_DOWN_NOW(BUTTON_RIGHT) && difficulty == 0)
			{
			drawRect(115, 52, 5, 30, WHITE);
			drawRect(115, 148, 5, 30, GREEN);
			difficulty = 1;
			}
			while(KEY_DOWN_NOW(BUTTON_RIGHT));
			if (KEY_DOWN_NOW(BUTTON_LEFT) && difficulty == 1)
			{
				drawRect(115, 148, 5, 30, WHITE);
				drawRect(115, 52, 5, 30, GREEN);
				difficulty = 0;
			}
			while(KEY_DOWN_NOW(BUTTON_LEFT));
		}
		
		seed++;
	}
	while(KEY_DOWN_NOW(BUTTON_START));
	return GAME;
}

void lose()
{
	fillScreen(RED);
	drawImage3(5, 30, FAILURE_WIDTH, FAILURE_HEIGHT, failure);
	drawString(150, 80, "Press Start", BLACK);
	while(!KEY_DOWN_NOW(BUTTON_START));
	while(KEY_DOWN_NOW(BUTTON_START));
}

void congratulate()
{
	drawImage3(0, 0, WIN_WIDTH, WIN_HEIGHT, win);
	drawString(145, 80, "Congratulations!", BLACK);
	while(1)
	{

		while(!KEY_DOWN_NOW(BUTTON_START));
		if (KEY_DOWN_NOW(BUTTON_SELECT))
		{
			while(KEY_DOWN_NOW(BUTTON_SELECT));
			return;
		}
		if (KEY_DOWN_NOW(BUTTON_START))
		{
			while(KEY_DOWN_NOW(BUTTON_START));
			return;
		}
	}
	
}

void secret()
{
	secretUnlocked = 1;
	fillScreen(GREEN);
	LINK linkPlayer[1];
	LINK oldLink[1];
	linkPlayer->row = 70;
	linkPlayer->col = 110;
	int count = 0;
	for (int i = 0; i < 155; i += 15)
	{
		for (int j = 0; j < 200; j += 48)
		{
			drawString(i, j, "SECRET!", BLACK);
		}
	}
	
	while (1)
	{

		drawLink(linkPlayer->row, linkPlayer->col, link);
		oldLink->row = linkPlayer->row;
		oldLink->col = linkPlayer->col;
		if (count % 20 == 0)
		{
			if (KEY_DOWN_NOW(BUTTON_LEFT))
			{
				linkPlayer->col -= 1;
			}
			if (KEY_DOWN_NOW(BUTTON_RIGHT))
			{
				linkPlayer->col += 1;
			}
			if (KEY_DOWN_NOW(BUTTON_UP))
			{
				linkPlayer->row -= 1;
			}
			if (KEY_DOWN_NOW(BUTTON_DOWN))
			{
				linkPlayer->row += 1;
			}
			if (linkPlayer->col < 0)
			{
				linkPlayer->col = 0;
			}
			if (linkPlayer->col > 240 - LINK_WIDTH)
			{
				linkPlayer->col = 240 - LINK_WIDTH;
			}
			if (linkPlayer->row > MAXHEIGHT-LINK_HEIGHT+1)
			{
				linkPlayer->row = MAXHEIGHT-LINK_HEIGHT+1;
			}
			if (linkPlayer->row < 0)
			{
				linkPlayer->row = 0;
			}
		}
		if (oldLink-> col != linkPlayer->col || oldLink->row != linkPlayer->row)
		{
			drawRect(oldLink->row, oldLink->col, LINK_HEIGHT, LINK_WIDTH, GREEN);
		}
		count++;

		if (KEY_DOWN_NOW(BUTTON_SELECT))
		{
			while(KEY_DOWN_NOW(BUTTON_SELECT));
			return;
		}
		if (KEY_DOWN_NOW(BUTTON_START))
		{
			while(KEY_DOWN_NOW(BUTTON_START));
			return;
		}
	}
	
}

int game(int difficulty)
{
	REG_DISPCTL = MODE3 | BG2_ENABLE;
	BIRD objs[NUMBIRDS];
	BIRD oldObjs[NUMBIRDS];
	BIRD *cur;
	int NUMPIPES = 3;
	PIPE pipes[NUMPIPES];
	PIPE oldPipes[NUMPIPES];
	PIPE *curPipe;
	PIPE *curOldPipe;
	char buffer[41];
	
	
	
	int size = 7;
	int oldsize = size;
	int i, j;
	int score = 0;

	fillScreen(CYAN);


	srand(seed);
	for(i=0; i<NUMBIRDS; i++)
	{
		cur = objs + i;
		cur->row = 80;
		cur->col = 20 + 10 * i;
		cur->rd = -2;
		cur->cd = 0;
		cur->size = size;
		cur->color = RED;
		oldObjs[i] = objs[i];
	}


	//Delays initial creation of pipes
	int pipeSpawnIndex = 0;
	//Delays update of bird trajectory
	int count = 0;
	//Used to disallow holding up to constantly gain altitude
	int toggle1 = 0;
	//int toggle2 = 0;
	//Used to not update the pipes every single time
	int pipeDelay = 0;
	//Dictates the speed of the pipes
	int pipeSpeed = 4 * difficulty + 4;
	// Game Loop
	while(1)
	{

		if(KEY_DOWN_NOW(BUTTON_SELECT))
		{
			return LOSE;
		}

		if (pipeSpawnIndex < NUMPIPES)
		{
			curPipe = pipes + pipeSpawnIndex;
			if (pipeSpawnIndex == 0 || (curPipe - 1)->col < 180)
			{
				curPipe->width = 5;
				curPipe->col = 240 - curPipe->width;
				curPipe->height = rand() % 80 + 20;
				curPipe->row = rand() % (160 - curPipe->height);
				curPipe->color = RED;
				curPipe->passed = 0;
				oldPipes[pipeSpawnIndex] = pipes[pipeSpawnIndex];
				pipeSpawnIndex++;
			}
		}
		
		for(i=0; i<NUMBIRDS; i++)
		{	
			cur = objs + i;
			cur->row = cur->row + cur->rd;
			cur->col += cur->cd;

			for (j = 0; j < pipeSpawnIndex; j++)
			{
				curPipe = pipes + j;
				if (intersects(cur, curPipe))
				{
					if (difficulty == 0)
					{
						if (score > easyHighScore)
						{
							easyHighScore = score;
						}
					}
					else 
					{
						if (score > hardHighScore)
						{
							hardHighScore = score;
						}
					}
					
					return LOSE;
				} else if(curPipe->passed == 0 && passedPipe(cur, curPipe))
				{
					curPipe->passed = 1;
					score++;
				}
			}

			if (cur->col < 0)
			{
				cur->col = 0;
			}

			if(cur->row<0)
			{
				
				if (mirror)
				{
					cur->row = MAXHEIGHT-size+1;
				}
				else 
				{
					cur->row = 0;
					cur->rd = 0;
				}
				
				
			}
			if(cur->row>MAXHEIGHT-size+1)
			{
				if (mirror)
				{
					cur->row = 0;
				}
				else
				{
					if (difficulty == 0)
					{
						if (score > easyHighScore)
						{
							easyHighScore = score;
						}
					}
					else 
					{
						if (score > hardHighScore)
						{
							hardHighScore = score;
						}
					}
					return LOSE;


					/* bouncing physics
					cur->rd = -(cur->rd - 1);
					if (cur->rd == 0)
					{
						cur->rd = -2;
					}
					*/
				}
				
				
				count = 0;
			}
			if (cur-> rd < 0) {

				count++;
				if (count == 15)
				{
					(cur->rd)++;
					count = 0;
				} 
				
			} else if (cur->rd == 0)
			{
				(cur->rd)++;
			}
			else if (cur-> rd >= 0)
			{
				count++;
				if (count == 15)
				{
					(cur->rd)++;
					count = 0;
				} 
			}

			if (cur->col + cur->size > 240)
			{
				if (difficulty == 0)
				{
					if (score > easyHighScore)
					{
						easyHighScore = score;
					}
				}
				else 
				{
					if (score > hardHighScore)
					{
						hardHighScore = score;
					}
				}
				return SECRET;
			}

			if(KEY_DOWN_NOW(BUTTON_LEFT))
			{
				cur->col -= 1;
			}
			if(KEY_DOWN_NOW(BUTTON_RIGHT))
			{
				cur->col += 1;
			}

			if(KEY_DOWN_NOW(BUTTON_A) && !toggle1)
			{
				cur->row -= 3;
				cur->rd = -1;
				count = 0;
				toggle1 = 1;
			}
			else if (KEY_DOWN_NOW(BUTTON_A))
			{

			}
			else 
			{
				toggle1 = 0;
			}
			
			
		}

		pipeDelay++;
		for (i = 0; i < NUMPIPES; i++)
		{
			curPipe = pipes + i;
			if (pipeDelay % 3 == 0)
			{
				curPipe->col -= pipeSpeed;	
			}
			if(curPipe->col < 0)
			{

				
				curPipe->width = 5;
				curPipe->col = 240 - curPipe->width;
				curPipe->height = rand() % 60 + 20;
				curPipe->row = rand() % (160 - curPipe->height);
				curPipe->color = RED;
				curPipe->passed = 0;
				
			}
		}

		sprintf(buffer, "Score: %d", score);
		
		drawRect(150, 170, 10, 72, CYAN); 
		drawString(150, 170, buffer, BLACK);
		for(i=0; i<NUMBIRDS; i++)
		{

			drawRect(oldObjs[i].row, oldObjs[i].col, BIRDUP_WIDTH, BIRDUP_HEIGHT, CYAN);
		}
		for(i=0; i<NUMBIRDS; i++)
		{
			cur = objs + i;
			if (cur->rd <= 0)
			{
				drawBird(cur->row, cur->col, bird);
			} else 
			{
				drawBirdUp(cur->row, cur->col, birdUp);

			}
			//drawRect(cur->row, cur->col, size, size, BLUE);

			oldObjs[i] = objs[i];
		} // for
		waitForVblank();
		for(i=0; i<pipeSpawnIndex; i++)
		{
			curPipe = pipes + i;
			curOldPipe = oldPipes + i;
			if (curOldPipe->col != curPipe->col)
			{
				drawRect(curOldPipe->row, curOldPipe->col, curOldPipe->height, curOldPipe->width, CYAN);
			}
			
		}

		for(i=0; i<pipeSpawnIndex; i++)
		{
			curPipe = pipes + i;
			curOldPipe = oldPipes + i;
			if (curOldPipe->col != curPipe->col)
			{
				drawPipe(curPipe, curPipe->color);
				oldPipes[i] = pipes[i];
			}
		}
		oldsize = size;
	}
}