#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
char map[100][100];
void printMap(int n, int m)
{
	for (int i=0; i<n; i++)
	{
		for (int j=0; j<m; j++)
			printf("%c ", map[i][j]);
		printf("\n");
	}
	sleep(1);
	system("cls");	
}
int main()
{
	int n, m, x ,y;
	scanf("%d%d%d%d", &n, &m, &x, &y);
	for (int i=0; i<n; i++)
		for (int j=0; j<m; j++)
			map[i][j]= 176;
	int xturn=1, yturn=0, xdir=1, ydir=-1, xstep=1, ystep=0;
	while(1)
	{
		if (xturn)
		{
			for (int i=0; i<xstep; i++)
			{
				if (x+xdir>m-1 || x+xdir<0) 
				{
					xturn=0, yturn=1, xdir= (xdir==-1)? 1: -1;
					break;
				}
				else 
				{
					x+=xdir, map[x][y]= 254;
					printMap(n, m);
				}
			}
			xstep++, xturn=0, yturn=1, xdir= (xdir==-1)? 1: -1;
		}
		else
		{
			for (int i=0; i<ystep; i++)
			{
				if (y+ydir>n-1 || y+ydir<0) 
				{
					yturn=0, xturn=1, ydir= (ydir==-1)? 1: -1;
					break;
				}
				else 
				{
					y+=ydir, map[x][y]= 254;
					printMap(n, m);
				}
			}
			ystep++, yturn=0, xturn=1, ydir= (ydir==-1)? 1: -1;
		}
	}
	return 0;
}
