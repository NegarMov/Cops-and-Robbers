#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
const long int N=100, M=100;
long int map[N][M], n, m, k, policen, policenk, randpx, randpy, robberx, robbery, police[M*N][3], station[M*N], stat=1, policeMoves, robberMoves, bobView[5][5], alert;
void setMap()
{
	printf("-Enter n, m and the number of station houses:\n");
    scanf("%d%d%d", &n, &m, &k);
	for (int i=0; i<n; i++)
		for (int j=0; j<m; j++)
			map[i][j]= 176;
	printf("-Enter the number of cops each station holds:\n");
    for (int i=0; i<k; i++) {
    	scanf("%d", &policenk);
    	for (int j=0; j<policenk; j++) {
			do
				randpx= rand()%n, randpy= rand()%m;
			while (map[randpx][randpy]!=176);
			map[randpx][randpy]= i+49, police[policen+j][0]= randpx, police[policen+j][1]= randpy, police[policen+j][2]= i;
		}
		policen+= policenk;
	}
	do
		robberx= rand()%n, robbery= rand()%m;
	while (map[robberx][robbery]!=176);
	map[robberx][robbery]= 'T';
	for (int i=1; i<=policenk; i++) station[i]=0;
}
void normalMove(int x, int y, int who)
{
	int randmovex, randmovey;
	if (who==-1) {	
		do
			randmovex= (rand()%3)-1, randmovey= (rand()%3)-1;
		while(y+randmovey>m-1 || y+randmovey<0 || x+randmovex>n-1 || x+randmovex<0);
		robberMoves= (randmovey==0 && randmovex==0)? robberMoves: robberMoves+1, map[robberx][robbery]= 176, robberx+= randmovex, robbery+= randmovey;
		if (map[robberx][robbery]!=176){
			map[robberx][robbery]= 254, stat=0;
			return;
		}
		map[robberx][robbery]= 'T';
	}
	else {
		do
			randmovex= (rand()%3)-1, randmovey= (rand()%3)-1;
		while(y+randmovey>m-1 || y+randmovey<0 || x+randmovex>n-1 || x+randmovex<0 || (map[x+randmovex][y+randmovey]!=176 && map[x+randmovex][y+randmovey]!='T'));
		map[x+randmovex][y+randmovey]= map[x][y], map[x][y]= 176;
		police[who][0]+= randmovex, police[who][1]+= randmovey, policeMoves= (randmovey==0 && randmovex==0)? policeMoves: policeMoves+1;
		if (police[who][0]==robberx && police[who][1]==robbery){
			map[x][y]= 176, map[police[who][0]][police[who][1]]= 254, stat=0;
			return;
		}
	}
}
void isBobSeen()
{
	for (int i=1; i<=k; i++) station[i]=0;
	alert=0;
	for (int i=robberx-2; i<=robberx+2; i++)
		for (int j=robbery-2; j<=robbery+2; j++)
			if (i>=0 && i<n && j>=0 && j<m && map[i][j]!=176 && map[i][j]!='T') 
				station[map[i][j]-48]= 1, alert=1;
}
void arrestMove(int x, int y, int who)
{
	if (y==robbery) {
		int sgn= (robberx- x)/ fabs(robberx- x);
		if (map[x+sgn][y]=='T' || map[x+sgn][y]== 176) map[x+sgn][y]= map[x][y], map[x][y]= 176, police[who][0]+= sgn, policeMoves++;
	}
	else if (x==robberx) {
		int sgn= (robbery- y)/ fabs(robbery- y);
		if (map[x][y+sgn]=='T' || map[x][y+sgn]== 176) map[x][y+sgn]= map[x][y], map[x][y]= 176, police[who][1]+= sgn, policeMoves++;
	}
	else {
		int sgnx= (robberx- x)/ fabs(robberx- x), sgny= (robbery- y)/ fabs(robbery- y);
		if (map[x+sgnx][y+sgny]=='T' || map[x+sgnx][y+sgny]==176) map[x+sgnx][y+sgny]= map[x][y], map[x][y]= 176, police[who][1]+= sgny, police[who][0]+= sgnx, policeMoves++;
		else if (map[x+sgnx][y]=='T' || map[x+sgnx][y]==176) map[x+sgnx][y]= map[x][y], map[x][y]= 176, police[who][0]+= sgnx, policeMoves++;
		else if (map[x][y+sgny]=='T' || map[x][y+sgny]==176) map[x][y+sgny]= map[x][y], map[x][y]= 176, police[who][1]+= sgny, policeMoves++;
	}
	if (police[who][0]==robberx && police[who][1]==robbery) {
		map[x][y]= 176, map[police[who][0]][police[who][1]]= 254, stat=0;
		return;
	}
}
int callSaul(int x, int y)
{
	int res=0;
	if (bobView[x][y]==254) return 100;
	for (int i=x-1; i<=x+1; i++)
		for (int j=y-1; j<=y+1; j++)
			if (i>=0 && i<5 && j<5 && j>=0 && bobView[i][j]!=176 && bobView[i][j]!=254) res+=10;
	for (int i=x-2; i<=x+2; i++)
		for (int j=y-2; j<=y+2; j+=4)
			if (i>=0 && i<5 && j<5 && j>=0 && bobView[i][j]!=176 && bobView[i][j]!=254) res++;
	for (int i=x-2; i<=x+2; i+=4)
		for (int j=y-1; j<=y+1; j++)
			if (i>=0 && i<5 && j<5 && j>=0 && bobView[i][j]!=176 && bobView[i][j]!=254) res++;
	return res;
}
void escapeMove()
{
	for (int i=robberx-2; i<=robberx+2; i++)
		for (int j=robbery-2; j<=robbery+2; j++)
			bobView[i-robberx+2][j-robbery+2]= (i<0 || i>n-1 || j<0 || j>m-1)? 254: (i==robberx && j==robbery)? 176: map[i][j];
	int min=100, indexx, indexy;
	for (int i=-1; i<=1; i++)
		for (int j=-1; j<=1; j++)
			if (callSaul(2+i, 2+j) < min && (map[i+robberx][j+robbery]==176 || map[i+robberx][j+robbery]=='T')) min= callSaul(2+i, 2+j), indexx=i, indexy=j;
	robberMoves= (indexy==0 && indexx==0)? robberMoves: robberMoves+1, map[robberx][robbery]= 176, robberx+= indexx, robbery+= indexy, map[robberx][robbery]= 'T';
}
void printMap(int n, int m)
{
	static int t=1;
	for (int i=0; i<n; i++) {
		for (int j=0; j<m; j++)
			if (map[i][j]==254) printf("\033[1;33m%c\033[m ", map[i][j]);
			else if (map[i][j]!='T' && map[i][j]!=176) printf("\033[1;36m%c\033[m ", map[i][j]);
			else if (map[i][j]!=176) printf("\033[1;35m%c\033[m ", map[i][j]);
			else printf("%c ", map[i][j]);
		printf("\n");
	}
	printf("\033[0;32m<time= %d>\033[m\n", t++);
	sleep(1.5);
	if (stat==1) system("cls");
}
int main()
{
	srand(time(0));
	setMap();
	isBobSeen();
	system("cls");
    printMap(n, m);
    do {	
    	isBobSeen();
		if (alert) escapeMove(); 
		else normalMove(robberx, robbery, -1);
	    for (int i=0; i<policen; i++) {
	    	isBobSeen();
	    	if (stat==0) goto end;
	    	else if (station[police[i][2]+1]==0) normalMove(police[i][0], police[i][1], i);
			else arrestMove(police[i][0], police[i][1], i);
		}
		end: ;
		printMap(n, m);
	} while(stat);
	printf("\n\033[1;33m**BUSTED!**\033[m\n\n\033[1;36m-Number of police moves: %ld\n\033[1;35m-Number of robber moves: %ld\033[m\n", policeMoves, robberMoves);
	return 0;
}
