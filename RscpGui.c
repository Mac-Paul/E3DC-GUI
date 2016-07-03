/*
gcc -g -o RscpGui  RscpGui.c
sudo ./RscpGui
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

void createButton(int x, int y, int w, int h, char *text, int backgroundColor, int foregroundColor)
{
	int size = sizeof(text);
	char *p = text;
	int length = 0;
	while(*(p+length))
      		length++;
	if((length*8)> (w-2)){
		printf("####error,button too small for text####\n");
		//exit(1);
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

	char TAG_Time[20];
	char TAG_Date[20];
	char TAG_PVI[10];
	char TAG_Bat[10];
	char TAG_Home[10];
	char TAG_Grid[10];
	char TAG_SOC[10];
	char TAG_BatState[10];
	char TAG_Autarky[20];
	char TAG_SelfCon[20];
	char TAG_SerialNr[20];
	char TAG_UnixTime[20];
	char Additional[10];
	char TAG_ADD[10];
	char Wallbox[10];
	char TAG_WbAll[10];
	char TAG_WbSolar[10];
	char TAG_PVIState[10];
	char TAG_PMState[10];

	int SOCx;
	int SOCw;
	char OUT [100];
	int change = 0;
	int changeStop = 0;
	int start = 0;

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
	int buttonCordsPVI[4] = {15,45,60,40};
	createButton(buttonCordsPVI[X],buttonCordsPVI[Y],buttonCordsPVI[W],buttonCordsPVI[H],"PVI",WHITE,GREY);
	int buttonCordsGrid[4] = {245,45,60,40};
	createButton(buttonCordsGrid[X],buttonCordsGrid[Y],buttonCordsGrid[W],buttonCordsGrid[H],"GRID",WHITE,GREY);
	int buttonCordsSOC[4] = {60,8,200,20};
	createButton(buttonCordsSOC[X],buttonCordsSOC[Y],buttonCordsSOC[W],buttonCordsSOC[H],"SOC",WHITE,BLACK);
	int buttonCordsHome[4] = {245,145,60,40};
	createButton(buttonCordsHome[X],buttonCordsHome[Y],buttonCordsHome[W],buttonCordsHome[H],"HOME",WHITE,GREY);
	int buttonCordsBat[4] = {15,145,60,40};
	createButton(buttonCordsBat[X],buttonCordsBat[Y],buttonCordsBat[W],buttonCordsBat[H],"BAT",WHITE,GREY);
	int buttonCordsADD[4] = {15,95,60,40};
	createButton(buttonCordsADD[X],buttonCordsADD[Y],buttonCordsADD[W],buttonCordsADD[H],"ADD",WHITE,GREY);
	int buttonCordsWB[4] = {245,95,60,40};
	createButton(buttonCordsWB[X],buttonCordsWB[Y],buttonCordsWB[W],buttonCordsWB[H],"WB All",WHITE,GREY);
	sleep(8)

	while(1){
//Bildwechsel
		if(changeStop == 0){
			if(change == 1){
				change = 0;
			}
			else{
				change = 1;
			}
		}
//Daten aus RAMDisk Datei einlesen
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
			fgets(TAG_BatState,20,fp);
			strtok(TAG_BatState, "\n");
			fgets(TAG_Autarky,20,fp);
			strtok(TAG_Autarky, "\n");
			fgets(TAG_SelfCon,20,fp);
			strtok(TAG_SelfCon, "\n");
			fgets(TAG_SerialNr,20,fp);
			strtok(TAG_SerialNr, "\n");
			fgets(TAG_UnixTime,20,fp);
			strtok(TAG_UnixTime, "\n");
			fgets(Additional,20,fp);
			strtok(Additional, "\n");
			fgets(TAG_ADD,20,fp);
			strtok(TAG_ADD, "\n");
			fgets(Wallbox,20,fp);
			strtok(Wallbox, "\n");
			fgets(TAG_WbAll,20,fp);
			strtok(TAG_WbAll, "\n");
			fgets(TAG_WbSolar,20,fp);
			strtok(TAG_WbSolar, "\n");
			fgets(TAG_PVIState,20,fp);
			strtok(TAG_PVIState, "\n");
			fgets(TAG_PMState,20,fp);
			strtok(TAG_PMState, "\n");
		printf("%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s\n", TAG_Date, TAG_Time, TAG_PVI, TAG_Bat, TAG_Home, TAG_Grid, TAG_SOC, TAG_BatState, TAG_Autarky, TAG_SelfCon, TAG_SerialNr, TAG_UnixTime, TAG_ADD, TAG_WbAll, TAG_WbSolar, TAG_PVIState, TAG_PMState);
		fclose(fp);
	 }
//Time
	int AktuallTime = time(NULL);
	int TAG_UnixTimeint = atoi(TAG_UnixTime);
	int DiffTime = AktuallTime - TAG_UnixTimeint;
	snprintf (OUT, (size_t)100, "%s", TAG_Time);
	if(DiffTime > 180){
		createButton(buttonCordsTime[X],buttonCordsTime[Y],buttonCordsTime[W],buttonCordsTime[H],OUT,RED,GREY);
		printf("Time False %i; ", DiffTime);
		changeStop = 1;
	}
	else{
		createButton(buttonCordsTime[X],buttonCordsTime[Y],buttonCordsTime[W],buttonCordsTime[H],OUT,GREEN,GREY);
		printf("Time OK %i; ", DiffTime);
		changeStop = 0;
	}
//Date
	snprintf (OUT, (size_t)100, "%s", TAG_Date);
	createButton(buttonCordsDate[X],buttonCordsDate[Y],buttonCordsDate[W],buttonCordsDate[H],OUT,WHITE,GREY);
//Programm-Start
	 if(start == 0){
		 snprintf (OUT, (size_t)100, "%s", TAG_SerialNr);
		 put_string(95,100,OUT,4);
		 start = 1;
		 sleep (5);
		 drawSquare(90,95,140,20,BLACK);
	 }
//PVI
	 snprintf (OUT, (size_t)100, "%sW", TAG_PVI);
	 int TAG_PVIint = atoi(TAG_PVI);
	 int TAG_PVIStateint = atoi(TAG_PVIState);
	 if(TAG_PVIStateint >= 1){
	 	if(TAG_PVIint > 50){
		 	createButton(buttonCordsPVI[X],buttonCordsPVI[Y],buttonCordsPVI[W],buttonCordsPVI[H],OUT,YELLOW,GREY);
		 	printf("PVI-ON %s; ", OUT);
	 	}
	 	else{
		 	createButton(buttonCordsPVI[X],buttonCordsPVI[Y],buttonCordsPVI[W],buttonCordsPVI[H],OUT,WHITE,GREY);
		 	printf("PVI-OFF %s; ", OUT);
	 	}
	 	put_string(32,50,"PVI",4);
	 	drawSquare(80,65,60,20,BLACK);
	 	if(TAG_PVIint > 0){
		 	if(change == 1){
		 		put_string(85,71,"> > >",4);
				printf("> > >; ");
		 	}
		 	else{
				put_string(85,71," > > ",4);
				printf(" > > ; ");
	   	}
	 	}
 	}
	 else{
		 createButton(buttonCordsPVI[X],buttonCordsPVI[Y],buttonCordsPVI[W],buttonCordsPVI[H],"DOWN",RED,GREY);
		 printf("PVI-DOWN; ");
		 put_string(32,50,"PVI",4);
	 }
//Grid
	 int TAG_Gridint = atoi(TAG_Grid);
	 int TAG_PMStateint = atoi(TAG_PMState);
	 if(TAG_PMStateint >= 1){
	 	if(TAG_Gridint < 0){
		 	TAG_Gridint = TAG_Gridint * -1;
		 	snprintf (OUT, (size_t)100, "%iW", TAG_Gridint);
		 	createButton(buttonCordsGrid[X],buttonCordsGrid[Y],buttonCordsGrid[W],buttonCordsGrid[H],OUT,CYAN,GREY);
		 	put_string(260,50,"GRID",4);
		 	drawSquare(180,65,60,20,BLACK);
		 	if (TAG_Gridint > 15){
		 		if(change == 1){
			 		put_string(190,71,"> > >",4);
		 		}
 	 	 		else{
			 		put_string(190,71," > > ",4);
		 		}
			}
			else{
				createButton(buttonCordsGrid[X],buttonCordsGrid[Y],buttonCordsGrid[W],buttonCordsGrid[H],OUT,LIGHT_GREEN,GREY);
				put_string(260,50,"GRID",4);
			}
	 		printf("Grid-IN %s; ", OUT);
	 	}
	 	else{
		 	snprintf (OUT, (size_t)100, "%sW", TAG_Grid);
		 	createButton(buttonCordsGrid[X],buttonCordsGrid[Y],buttonCordsGrid[W],buttonCordsGrid[H],OUT,BLUE,GREY);
		 	put_string(260,50,"GRID",4);
		 	drawSquare(180,65,60,20,BLACK);
		 	if (TAG_Gridint > 5){
		 		if(change == 1){
			 		put_string(190,71,"< < <",4);
		 		}
 	 	 		else{
			 		put_string(190,71," < < ",4);
		 		}
		 		printf("Grid-OUT %s; ", OUT);
	 		}
			else{
				createButton(buttonCordsGrid[X],buttonCordsGrid[Y],buttonCordsGrid[W],buttonCordsGrid[H],OUT,LIGHT_GREEN,GREY);
				put_string(260,50,"GRID",4);
			}
		}
	}
	else{
		createButton(buttonCordsGrid[X],buttonCordsGrid[Y],buttonCordsGrid[W],buttonCordsGrid[H],"DOWN",RED,GREY);
		put_string(260,50,"GRID",4);
		printf("Grid-DOWN; ");
	}
//Home
	 snprintf (OUT, (size_t)100, "%sW", TAG_Home);
	 createButton(buttonCordsHome[X],buttonCordsHome[Y],buttonCordsHome[W],buttonCordsHome[H],OUT,WHITE,GREY);
	 put_string(260,150,"HOME",4);
	 drawSquare(180,150,60,20,BLACK);
	 if(change == 1){
		 put_string(190,151,"> > >",4);
	 }
	 else{
		 put_string(190,151," > > ",4);
	 }
	 printf("Home %s; ", OUT);
//Battery
	 int TAG_Batint = atoi(TAG_Bat);
	 if(TAG_Batint < 0){
	 	TAG_Batint = TAG_Batint * -1;
	 	snprintf (OUT, (size_t)100, "%iW", TAG_Batint);
	 	createButton(buttonCordsBat[X],buttonCordsBat[Y],buttonCordsBat[W],buttonCordsBat[H],OUT,WHITE,GREY);
	 	put_string(32,150,"BAT",4);
		drawSquare(80,150,60,20,BLACK);
		if(change == 1){
			put_string(85,151,"> > >",4);
		}
 	 else{
		 put_string(85,151," > > ",4);
	 }
		printf("Bat OUT %s; ", OUT);
		}
 		else{
	 		snprintf (OUT, (size_t)100, "%sW", TAG_Bat);
	 		createButton(buttonCordsBat[X],buttonCordsBat[Y],buttonCordsBat[W],buttonCordsBat[H],OUT,WHITE,GREY);
	 		put_string(32,150,"BAT",4);
	 		drawSquare(80,150,60,20,BLACK);
			if(TAG_Batint > 0){
				if(change == 1){
					put_string(85,151,"< < <",4);
				}
		 	 else{
				 put_string(85,151," < < ",4);
			 }
				printf("Bat IN %s; ", OUT);
			}
			else{
				printf("Bat OFF; ");
			}
 		}
//Additional
		int Additionalint = atoi(Additional);
		drawSquare(80,110,60,20,BLACK);
		if(Additionalint == 1){
			snprintf (OUT, (size_t)100, "%sW", TAG_ADD);
			createButton(buttonCordsADD[X],buttonCordsADD[Y],buttonCordsADD[W],buttonCordsADD[H],OUT,WHITE,GREY);
 	 		put_string(32,100,"ADD",4);
 	 		int TAG_ADDint = atoi(TAG_ADD);
 	 		if(TAG_ADDint > 0){
				if(change == 1){
					put_string(85,111,"> > >",4);
				}
		 	 else{
				 put_string(85,111," > > ",4);
			 }
				printf("ADD %s; ", OUT);
 	 		}
			else{
				printf("Add OFF; ");
			}
		}
		else{
			createButton(buttonCordsADD[X],buttonCordsADD[Y],buttonCordsADD[W],buttonCordsADD[H],"",BLACK,BLACK);
		}
//Wallbox
		int Wallboxint = atoi(Wallbox);
		drawSquare(180,95,60,40,BLACK);
		if(Wallboxint == 1){
	 		snprintf (OUT, (size_t)100, "%sW", TAG_WbAll);
			int TAG_WbAllint = atoi(TAG_WbAll);
			int TAG_WbSolarint = atoi(TAG_WbSolar);
			int diffWB = TAG_WbAllint - TAG_WbSolarint;
			if(diffWB > 50){
				createButton(buttonCordsWB[X],buttonCordsWB[Y],buttonCordsWB[W],buttonCordsWB[H],OUT,BLUE,GREY);
				printf("WBAll %s; ", OUT);
			}
			else{
				createButton(buttonCordsWB[X],buttonCordsWB[Y],buttonCordsWB[W],buttonCordsWB[H],OUT,YELLOW,GREY);
				snprintf (OUT, (size_t)100, "%s W", TAG_WbSolar);
				printf("WBSolar %s; ", OUT);
			}
			put_string(250,100,"WB All",4);
			if(TAG_WbAllint > 0){
				if(change == 1){
					put_string(190,111,"> > >",4);
				}
		 	 else{
				 put_string(190,111," > > ",4);
			 }
			 	put_string(190,101,"SOLAR",4);
				snprintf (OUT, (size_t)100, "%sW", TAG_WbSolar);
				put_string(190,121,OUT,4);
 	 		}
			else{
				snprintf (OUT, (size_t)100, "%sW", TAG_WbAll);
				createButton(buttonCordsWB[X],buttonCordsWB[Y],buttonCordsWB[W],buttonCordsWB[H],OUT,WHITE,GREY);
				put_string(250,100,"WB All",4);
			}
		}
		else{
			createButton(buttonCordsWB[X],buttonCordsWB[Y],buttonCordsWB[W],buttonCordsWB[H],"",BLACK,BLACK);
		}
//Time
	 int AktuallTime = time(NULL);
	 int TAG_UnixTimeint = atoi(TAG_UnixTime);
	 int DiffTime = AktuallTime - TAG_UnixTimeint;
	 snprintf (OUT, (size_t)100, "%s", TAG_Time);
	 if(DiffTime > 888680){
		 createButton(buttonCordsTime[X],buttonCordsTime[Y],buttonCordsTime[W],buttonCordsTime[H],OUT,RED,GREY);
		 printf("Time False %i; ", DiffTime);
		 changeStop = 1;
	 }
	 else{
		 createButton(buttonCordsTime[X],buttonCordsTime[Y],buttonCordsTime[W],buttonCordsTime[H],OUT,GREEN,GREY);
		 printf("Time OK %i; ", DiffTime);
		 changeStop = 0;
	 }
//Date
	 snprintf (OUT, (size_t)100, "%s", TAG_Date);
	 createButton(buttonCordsDate[X],buttonCordsDate[Y],buttonCordsDate[W],buttonCordsDate[H],OUT,WHITE,GREY);
	 snprintf (OUT, (size_t)100, "%s %%", TAG_SOC);
//SOC
	snprintf (OUT, (size_t)100, "%s %%", TAG_SOC);
	int TAG_BatStateint = atoi(TAG_BatState);
	if(TAG_BatStateint >= 1){
	 	int TAG_SOCint = atoi(TAG_SOC);
	 	SOCx = 2 * TAG_SOCint;
	 	drawSquare(58,6,204,24,WHITE);
	 	drawSquare((60+SOCx),8,(200-SOCx),20,BLACK);
	 	drawSquare(60,8,SOCx,20,LIGHT_BLUE);
	 	put_string(150,13,OUT,4);
		printf("SOC %s;\n", OUT);
	 }
	 else{
		 createButton(buttonCordsSOC[X],buttonCordsSOC[Y],buttonCordsSOC[W],buttonCordsSOC[H],"Breaker OFF",RED,BLACK);
		 printf("Breaker OFF;\n");
	 }
//Autarky
	 int TAG_Autarkyint = atoi(TAG_Autarky);
	 //int Autarkyx = 1.2 * TAG_Autarkyint;
	 drawSquare(160,63,19,104,WHITE);
	 drawSquare(162,(65+100- TAG_Autarkyint),15,TAG_Autarkyint,GREEN);
	 drawSquare(134,170,70,25,BLACK);
	 snprintf (OUT, (size_t)100, "%s %%", TAG_Autarky);
	 put_string(165,172,OUT,4);
	 snprintf (OUT, (size_t)100, "AUTARKY");
	 put_string(159,182,OUT,4);
//SelfConsumption
	 int TAG_SelfConint = atoi(TAG_SelfCon);
	 drawSquare(143,63,19,104,WHITE);
	 drawSquare(145,(65+100- TAG_SelfConint),15,TAG_SelfConint,LTGREY);
	 drawSquare(120,39,65,23,BLACK);
	 snprintf (OUT, (size_t)100, "%s %%", TAG_SelfCon);
	 put_string(128,51,OUT,4);
	 snprintf (OUT, (size_t)100, "SelfCon.");
	 put_string(104,43,OUT,4);
//Abfrageintervall
	 sleep(1);
	}
}
