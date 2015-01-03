#include "myLib.h"
#include "bird.h"
#include "birdUp.h"
#include "link.h"
#include "win.h"

unsigned short *videoBuffer = (unsigned short *)0x6000000;

void setPixel(int row, int col, unsigned short color)
{
	videoBuffer[OFFSET(row, col, 240)] = color;
}


void drawRect(int row, int col, int height, int width, volatile unsigned short color)
{
	int r;
	for(r=0; r<height; r++)
	{
		DMA[3].src = &color;
		DMA[3].dst = &videoBuffer[OFFSET(row+r, col, 240)];
		DMA[3].cnt = width | DMA_SOURCE_FIXED | DMA_ON;
	}
}

void waitForVblank()
{
	while(SCANLINECOUNTER >160);
	while(SCANLINECOUNTER < 160);
}

void fillScreen(volatile unsigned short color)
{
	DMA[3].src = &color;
	DMA[3].dst = videoBuffer;
	DMA[3].cnt = (160*240) | DMA_SOURCE_FIXED | DMA_ON;
}


void drawBird(int row, int col, const u16 *image)
{
	//drawImage3(row, col, PIDGEY_WIDTH, PIDGEY_HEIGHT, pidgey);
	drawImage3(row, col, BIRD_WIDTH, BIRD_HEIGHT, image);
}

void drawBirdUp(int row, int col, const u16 *image)
{
	//drawImage3(row, col, PIDGEY_WIDTH, PIDGEY_HEIGHT, pidgey);
	drawImage3(row, col, BIRDUP_WIDTH, BIRDUP_HEIGHT, image);
}

void drawLink(int row, int col, const u16 *image)
{
	drawImage3(row, col, LINK_WIDTH, LINK_HEIGHT, image);
}

void drawImage3(int r, int c, int width, int height, const u16 *image)
{
	int i;
	for(i = 0; i < height; i++)
	{
		DMA[3].src = image + (i * width);
		DMA[3].dst = &videoBuffer[OFFSET(r + i, c, 240)];
		DMA[3].cnt = width | DMA_ON;
	}
}

void drawPipe(PIPE *pipe, u16 color)
{
	int r;
	for(r=0; r < pipe->height; r++)
	{
		DMA[3].src = &color;
		DMA[3].dst = &videoBuffer[OFFSET(pipe->row + r, pipe->col, 240)];
		DMA[3].cnt = pipe->width | DMA_SOURCE_FIXED | DMA_ON;
	}
	//drawHollowRect(pipe->row, pipe->col, pipe->width, pipe->height, color);
}

int passedPipe(BIRD *bird, PIPE *pipe)
{
	if (bird->col > pipe->width + pipe->col)
	{
		return 1;
	}
	return 0;
}

int intersects(BIRD *bird, PIPE *pipe)
{

	if (bird->col + bird->size < pipe-> col || bird->col > pipe->col + pipe->width)
	{
		return 0;
	}
	if (bird->row + bird->size < pipe->row || bird->row > pipe->row + pipe->height)
	{
		return 0;
	}
	return 1;

}

void drawHollowRect(int row, int col, int width, int height, u16 color)
{
	int i = 0;
	for (i = 0; i <= width; i++)
	{
		setPixel(row, col + i, color);
		setPixel(row + height, col + i, color);
	}
	for (i = 0; i < height; i++)
	{ 
		setPixel(row + i, col, color);
		setPixel(row + i, col + width, color);
	}
}


	



