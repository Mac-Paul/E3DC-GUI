/*
gcc -g -o RscpGui  RscpGui.c
./RscpGui
*/

#include <linux/input.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include "touch.h"
#include "touch.c"
#include "framebuffer.c"
#include <signal.h>
#include <time.h>
#include <stdlib.h>

#define X 0
#define Y 1
#define W 2
#define H 3


char TAG_PVI[10];
char TAG_Bat[10];
char TAG_Home[10];
char TAG_Grid[10];
char TAG_SOC[10];
char TAG_Time[20];
char TAG_Date[20];

int SOCx;
int SOCw;


void createButton(int x, int y, int w, int h, char *text, int backgroundColor, int foregroundColor)
{
	int size = sizeof(text);
	char *p = text;
	int length = 0;
	while(*(p+length))
      		length++;
	if((length*8)> (w-2)){
		printf("####error,button too small for text####\n");
		exit(1);
	}
	drawSquare(x-2,y-2,w+4,h+4,backgroundColor);
	drawSquare(x,y,w,h,foregroundColor);
	put_string(x+((w-(length*8))/2), y+((h-8)/2),text,4);
}

void  INThandler(int sig)
{

        signal(sig, SIG_IGN);
	closeFramebuffer();
        exit(0);
}

int main()
{
	signal(SIGINT, INThandler);

	int  xres,yres,x;

	int screenXmax, screenXmin;
	int screenYmax, screenYmin;

	float scaleXvalue, scaleYvalue;

	int rawX, rawY, rawPressure, scaledX, scaledY;

	char VAR [20];
	char OUT [100];

	if (openTouchScreen() == 1)
		perror("error opening touch screen");

	getTouchScreenDetails(&screenXmin,&screenXmax,&screenYmin,&screenYmax);

	framebufferInitialize(&xres,&yres);

	scaleXvalue = ((float)screenXmax-screenXmin) / xres;
	printf ("X Scale Factor = %f\n", scaleXvalue);
	scaleYvalue = ((float)screenYmax-screenYmin) / yres;
	printf ("Y Scale Factor = %f\n", scaleYvalue);

	drawSquare(3,3,314,234,BLACK);
	int buttonCordsLTGREY[4] = {80,60,160,120};
	int buttonCordsDate[4] = {15,215,130,20};
	createButton(buttonCordsDate[X],buttonCordsDate[Y],buttonCordsDate[W],buttonCordsDate[H],"Date",WHITE,GREY);
	int buttonCordsTime[4] = {175,215,130,20};
	createButton(buttonCordsTime[X],buttonCordsTime[Y],buttonCordsTime[W],buttonCordsTime[H],"Time",WHITE,GREY);
	int buttonCordsPVI[4] = {15,45,60,60};
	createButton(buttonCordsPVI[X],buttonCordsPVI[Y],buttonCordsPVI[W],buttonCordsPVI[H],"",WHITE,GREY);
	int buttonCordsGrid[4] = {245,45,60,60};
	createButton(buttonCordsGrid[X],buttonCordsGrid[Y],buttonCordsGrid[W],buttonCordsGrid[H],"",WHITE,GREY);
	int buttonCordsSOC[4] = {60,8,200,20};
	createButton(buttonCordsSOC[X],buttonCordsSOC[Y],buttonCordsSOC[W],buttonCordsSOC[H],"SOC",RED,BLACK);
	int buttonCordsHome[4] = {245,125,60,60};
	createButton(buttonCordsHome[X],buttonCordsHome[Y],buttonCordsHome[W],buttonCordsHome[H],"",WHITE,GREY);
	int buttonCordsBat[4] = {15,125,60,60};
	createButton(buttonCordsBat[X],buttonCordsBat[Y],buttonCordsBat[W],buttonCordsBat[H],"",WHITE,GREY);
	drawSquare(148,43,24,124,WHITE);
	snprintf (OUT, (size_t)100, "AUTARKY");
	put_string(129,182,OUT,4);

	while(1){
		sleep(1);
		FILE *fp;
		fp = fopen("/mnt/RAMDisk/RscpGui.txt", "r");
		if(fp == NULL) {
			printf("Datei konnte NICHT geoeffnet werden.\n");
		}
		else {
			fgets(TAG_Date,20,fp);
			strtok(TAG_Date, "\n");
			fgets(TAG_Time,20,fp);
			strtok(TAG_Time, "\n");
			fgets(TAG_PVI,20,fp);
			strtok(TAG_PVI, "\n");
			fgets(TAG_Bat,20,fp);
			strtok(TAG_Bat, "\n");
			fgets(TAG_Home,20,fp);
			strtok(TAG_Home, "\n");
			fgets(TAG_Grid,20,fp);
			strtok(TAG_Grid, "\n");
			fgets(TAG_SOC,20,fp);
			strtok(TAG_SOC, "\n");
		printf("%s;%s;%s;%s;%s;%s;%s\n", TAG_Date, TAG_Time, TAG_PVI, TAG_Bat, TAG_Home, TAG_Grid, TAG_SOC);
		fclose(fp);
	 }
	 snprintf (OUT, (size_t)100, "%s", TAG_PVI);
	 createButton(buttonCordsPVI[X],buttonCordsPVI[Y],buttonCordsPVI[W],buttonCordsPVI[H],OUT,WHITE,GREY);
	 put_string(32,54,"PVI",4);
	 drawSquare(80,65,60,20,BLACK);
	 put_string(85,71,"> > >",4);
	 int TAG_Gridint = atoi(TAG_Grid);
	 if(TAG_Gridint < 0){
		 TAG_Gridint = TAG_Gridint * -1;
		 snprintf (OUT, (size_t)100, "%i", TAG_Gridint);
		 createButton(buttonCordsGrid[X],buttonCordsGrid[Y],buttonCordsGrid[W],buttonCordsGrid[H],OUT,WHITE,GREY);
		 put_string(260,54,"GRID",4);
		 drawSquare(180,65,60,20,BLACK);
		 put_string(190,71,"> > >",4);
	 }
	 else{
		 snprintf (OUT, (size_t)100, "%s", TAG_Grid);
		 createButton(buttonCordsGrid[X],buttonCordsGrid[Y],buttonCordsGrid[W],buttonCordsGrid[H],OUT,WHITE,GREY);
		 put_string(260,54,"GRID",4);
		 drawSquare(180,65,60,20,BLACK);
		 put_string(190,71,"< < <",4);
	 }
	 snprintf (OUT, (size_t)100, "%s", TAG_Home);
	 createButton(buttonCordsHome[X],buttonCordsHome[Y],buttonCordsHome[W],buttonCordsHome[H],OUT,WHITE,GREY);
	 put_string(260,134,"HOME",4);
	 drawSquare(180,145,60,20,BLACK);
	 put_string(190,151,"> > >",4);
	 int TAG_Batint = atoi(TAG_Bat);
	 if(TAG_Batint < 0){
	 	TAG_Batint = TAG_Batint * -1;
	 	snprintf (OUT, (size_t)100, "%i", TAG_Batint);
	 	createButton(buttonCordsBat[X],buttonCordsBat[Y],buttonCordsBat[W],buttonCordsBat[H],OUT,WHITE,GREY);
	 	put_string(32,134,"BAT",4);
		drawSquare(80,145,60,20,BLACK);
	 	put_string(85,151,"> > >",4);
 		}
 		else{
	 snprintf (OUT, (size_t)100, "%s", TAG_Bat);
	 createButton(buttonCordsBat[X],buttonCordsBat[Y],buttonCordsBat[W],buttonCordsBat[H],OUT,WHITE,GREY);
	 put_string(32,134,"BAT",4);
	 drawSquare(80,145,60,20,BLACK);
	 put_string(85,151,"< < <",4);
 	}
	 snprintf (OUT, (size_t)100, "%s", TAG_Time);
	 createButton(buttonCordsTime[X],buttonCordsTime[Y],buttonCordsTime[W],buttonCordsTime[H],OUT,WHITE,GREY);
	 snprintf (OUT, (size_t)100, "%s", TAG_Date);
	 createButton(buttonCordsDate[X],buttonCordsDate[Y],buttonCordsDate[W],buttonCordsDate[H],OUT,WHITE,GREY);
	 snprintf (OUT, (size_t)100, "%s %%", TAG_SOC);
	 int TAG_SOCint = atoi(TAG_SOC);
	 SOCx = 2 * TAG_SOCint;
	 drawSquare(58,6,204,24,WHITE);
	 drawSquare((60+SOCx),8,(200-SOCx),20,BLACK);
	 drawSquare(60,8,SOCx,20,LIGHT_BLUE);
	 put_string(150,13,OUT,4);
	 int TAG_Homeint = atoi(TAG_Home);
	 int Autarky;
	 TAG_Gridint = atoi(TAG_Grid);
	 if(TAG_Gridint < 0){
	 		Autarky = 100	;
 		}
		else{
			Autarky = (TAG_Homeint - TAG_Gridint)* 100 / TAG_Homeint;
		}
	 int Autarkyx = 1.2 * Autarky;
	 drawSquare(148,43,24,124,WHITE);
	 drawSquare(150,(45+120-Autarkyx),20,Autarkyx,GREEN);
	 drawSquare(122,170,70,30,BLACK);
	 snprintf (OUT, (size_t)100, "%i %%", Autarky);
	 put_string(143,172,OUT,4);
	 snprintf (OUT, (size_t)100, "AUTARKY");
	 put_string(129,182,OUT,4);
	}
}
