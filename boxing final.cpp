#include"stdafx.h"
#include<stdio.h>
#include<conio.h>
#include<glut.h>
#include"highgui.h"
#include <cv.h>
#include<BlobResult.h>
#include<stdlib.h>
#include <time.h>
#include<math.h>
#include <string.h>

#ifdef WIN32
#include <windows.h>
#endif
void dispArr3(GLfloat *arr){
		for(int i=0;i<=2;i++)
				printf("%f\n",(float)arr[i]);
		printf("\n");
}
char stringme[22]  ={"Your score:"};
char stringcomp[29]={"Computer:"};
IplImage *score;   
int maxareahand1,minareahand1, maxareahand2,minareahand2,executeinfinteloop,compspeed,inmoveornot,handonedone,handtwodone,timed;
int  jumphandsup1,jumphandsup2,mehit,nohit,comphit,countforcolor,increasingcountforcolor;
float scalingfactor=2700;
IplImage *imgscaling=cvLoadImage("harshit5.jpg");
int width, height;
int winwidth,winheight;
int headcolor,comphandcolor,myhandcolor;

struct Cube{
	GLfloat edge;
	GLfloat Centre[3];
	Cube(){}
	Cube(GLfloat Centre[], GLfloat edge);
	void dispCube();
};
Cube::Cube(GLfloat Centre[],GLfloat edge){
	for(int i=0;i<=2;i++){
		this->Centre[i]=Centre[i];
	}
	this->edge=edge;
}
void Cube::dispCube(){
	glPushMatrix();
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_SPECULAR);
	//glColorMaterial(GL_BACK_RIGHT , GL_DIFFUSE);
	glColor4f(0, .2, 1,1);
	//glColorMaterial(GL_FRONT, GL_SPECULAR);
	//glColor3f(1, 1, .2);
	glTranslatef(Centre[0],Centre[1],Centre[2]);
	glutSolidCube(edge);
	glutPostRedisplay();
	glPopMatrix();	
	glDisable(GL_COLOR_MATERIAL);
}

struct Sphere{
	GLfloat Centre[3];
	GLfloat radius;
	Sphere();
	Sphere(GLfloat Centre[], GLfloat radius);
	void dispSphere();
}; 
Sphere::Sphere ()	{
	Centre[0]=0;
	Centre[1]=0;
	Centre[2]=0;
	radius=0.6;
	
}
Sphere::Sphere (GLfloat Centre[], GLfloat radius){
	for (int i=0;i<3;i++)
		this->Centre[i]=Centre[i];
	this->radius=radius;
	
}
void Sphere::dispSphere(){
		glPushMatrix();
		glEnable(GL_COLOR_MATERIAL);
		glColorMaterial(GL_FRONT_LEFT, GL_AMBIENT);
		//glColor4f(.96, .645,.606,1);
		//glColor4f(0, 1,0,1);

		glColorMaterial(GL_FRONT, GL_AMBIENT);
		if (myhandcolor==1)
			glColor4f(1, .30, 0.0,1);
		if(comphandcolor==1)
			glColor4f(0,1,0,1);
		if (headcolor==1)
			glColor4f(1,1,0,1);
		glTranslatef(Centre[0],Centre[1],Centre[2]);
		glutSolidSphere(radius, 20, 20);
		glutPostRedisplay();
		//glDisable(GL_COLOR_MATERIAL);
		glPopMatrix();
		glDisable(GL_COLOR_MATERIAL);
}

//Cylinder : the structure
struct  Cylinder {
	GLfloat  axisofrotation[3];
	int slices;
	int stacks;
	GLfloat  Centre[3];
	GLfloat radius;
	GLfloat height;
	Cylinder (GLfloat  axisofrotation[],int slices,
		int stacks,GLfloat Centre[], GLfloat radius,GLfloat height);
};

Cylinder ::Cylinder (GLfloat  axisofrotation[], int slices, int stacks,
		 GLfloat Centre[], GLfloat radius,GLfloat height)	{
	
	for (int i=0;i<3;i++){
		this->Centre[i]=Centre[i];
		this->axisofrotation[i]=axisofrotation[i];
	}
	this->radius=radius;
	this->slices=slices;
	this->stacks=stacks;
	this->height=height;
	glPushMatrix();
	glRotatef(90, axisofrotation[0],axisofrotation[1],axisofrotation[2]);
	//glTranslatef((-1*(x-320)*1.2/80),counthand1/3500-4,-1*(y-240)*1.2/60-1.4);
	glTranslatef(Centre[0],Centre[1],Centre[2]);
	GLUquadricObj * cyl;
	cyl=gluNewQuadric();
	gluQuadricDrawStyle(cyl,GLU_FILL);
	gluCylinder(cyl,radius,radius,height,slices,stacks);
	glutPostRedisplay();
	glPopMatrix();
	
};

struct Player{
	Sphere hand1next;
	Sphere hand2next;
	Sphere hand1;
	Sphere hand2;
	Cube body;
	GLfloat Centre[3];
	GLfloat yspan;
	GLfloat zspan;
	GLint orientation;
	GLint score;
	void dispPlayer();
};

void Player::dispPlayer() {   	
	GLfloat axisofrotation[]={0,0,0};
	GLfloat centrehead[]={ Centre[0], Centre[1], Centre[2]  +  7.0 / 12 * zspan };
	GLfloat centreneck[]={ Centre[0], Centre[1], Centre[2] + 3.5 / 12 * zspan};
	GLfloat centrelegl[]={ Centre[0] - 0.15 * yspan, Centre[1], Centre[2] - zspan * 3.5 /12 };
	GLfloat centrelegr[]={ Centre[0] + 0.15 * zspan, Centre[1], Centre[2] - zspan * 3.5 / 12};
	Sphere comphead(centrehead, 1.5*zspan/12);
	comphead.dispSphere();
	body.dispCube();
}

struct Player;
struct Sphere;
struct Cylinder;
struct Cube;
GLfloat Zero[]={0,0,0};
Player me,comp;
int count1=1,count2=1,decreasestarthand2=0,decreasestarthand1=0;
float tempcomp1[3],tempcomp2[3],q1=1,q2=1;
float x_initpos_hand1,xrandom_hand1,y_initpos_hand1,yrandom_hand1,z_initpos_hand1,zrandom_hand1;			//float value for the random nos generated for hand1
float x_initpos_hand2,xrandom_hand2,y_initpos_hand2,yrandom_hand2,z_initpos_hand2,zrandom_hand2;			//float value for the random nos generated for hand2 
float xpos,ypos,zpos;
float xdir,ydir,zdir;
float counthand1,counthand2,speed=0.2;
CvCapture *hit;	

// the coordinates of our hand1
int x,y,z;				
// the coordinates of our hand2 and the higher and lower values for hsv for trackbar
int X,Y,Z,hh,hl,sh,sl,vh,vl;
IplImage *hi=cvLoadImage("harshit5.jpg");
IplImage *hitemp1=cvCreateImage(cvGetSize(hi),IPL_DEPTH_8U,1);
IplImage *Hi=cvLoadImage("harshit5.jpg");
IplImage *hi2=cvCreateImage(cvGetSize(hi),8,3);
IplImage *out=cvCreateImage(cvGetSize(hi),8,3);
IplImage *hitemp=cvCreateImage(cvGetSize(hi),IPL_DEPTH_8U,1);
//overloaded functions to determine collisions


GLfloat inline getDistance(GLfloat point1 [],GLfloat point2[]){
	return (GLfloat)sqrt( 
			  pow(point1[0]-point2[0],2)
			+ pow(point1[1]-point2[1],2)
			+ pow(point1[2]-point2[2],2)   
		); 
}

boolean isCollission(Sphere &sphere1,Sphere &sphere2){
	if(getDistance(sphere1.Centre,sphere2.Centre)<sphere1.radius+sphere2.radius)
		return true;
	return false;
}

boolean isCollission(Sphere &sphere, Cube &cube){
	int count =0;
	for(int i=0;i<=2;i++) {
		GLfloat temp=cube.Centre[i] - sphere.Centre[i];
		if(temp<0)	temp=-temp; 
		if( temp  <  (cube.edge / 2)  +  sphere.radius   ){
			count++;
		}
	}
	if(count==3){
		return true;
	}
	return false;
}

boolean isCollission(Sphere &hand, Player &cur_player,Player &oppn){
	boolean status=false;
	if( isCollission(hand, oppn.hand1) || isCollission(hand, oppn.hand2) ){
		status=true;
	}
	if( isCollission(hand, oppn.body) ){
		cur_player.score+=10;
		status =true;
	}

	return status;
}

//the fonts function starts here 
typedef enum {
   MODE_BITMAP,
   MODE_STROKE
} mode_type;

static mode_type mode;
static int font_index;

void print_bitmap_string(void* font, char* s)
{
   if (s && strlen(s)) {
      while (*s) {
         glutBitmapCharacter(font, *s);
         s++;
      }
   }
}

void speedincrement (void){
	if (speed<5)
	speed=speed +0.2;
}
void speeddecrement(void)
{if (speed >0.2)
	speed =speed -0.2;
}

void xincrement(void)
{xpos=xpos+speed;
glutPostRedisplay();
}
void yincrement(void)
	{ypos=ypos+speed;
glutPostRedisplay();
}
void zincrement(void)
{zpos=zpos+speed;
glutPostRedisplay();
}

void xdecrement(void)
{xpos=xpos-speed;
glutPostRedisplay();
}
void ydecrement(void){ 
	ypos=ypos-speed;
	glutPostRedisplay();
}
void zdecrement(void){
	zpos=zpos-speed;
	glutPostRedisplay();
}

void xdirincrement(void) {
	xdir=xdir+speed;
	glutPostRedisplay();
}
void ydirincrement(void) {
	ydir=ydir+speed;
	glutPostRedisplay();
}
void zdirincrement(void)
{zdir=zdir+speed;
glutPostRedisplay();
}

void xdirdecrement(void)
{xdir=xdir-speed;
glutPostRedisplay();
}
void ydirdecrement(void)
{ydir=ydir-speed;
glutPostRedisplay();
}
void zdirdecrement(void)
{zdir=zdir-speed;
glutPostRedisplay();
}

void nochange (void){
	glutPostRedisplay();
}
void mouse(int button, int state, int x, int y)	{
	switch (button) {
		case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN)
		glutIdleFunc(speedincrement);
		if (state==GLUT_UP)
		glutIdleFunc(nochange);
		break;

		case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN)
		glutIdleFunc(speeddecrement);
		if (state==GLUT_UP)
		glutIdleFunc(nochange);
		break;

		default:
		break;
	}
}	

void coordinates(IplImage *out , int check )	{
	counthand1 =1;counthand2=1;
	int ox,oy,oz,oX,oY,oZ;		//local variables for counting no of pixels in our hands
    ox=oy=oz=oX=oY=oZ=0;
	uchar *temp=(uchar*)out->imageData;

	//coordinates of hands in lappy image
	for (int i=0;i<out->height;i++){
		for (int j=0;j<out->width;j++){
			if (	(temp[i*out->widthStep+j*out->nChannels]==255) &&
				(temp[i*out->widthStep+j*out->nChannels+1]==0) &&
				(temp[i*out->widthStep+j*out->nChannels+2]==0)	)	{
					
				counthand1++;
				ox=ox+j;oy=oy+i;
			}
			if (	(temp[i*out->widthStep+j*out->nChannels+1]==255) &&
				(temp[i*out->widthStep+j*out->nChannels+2]==0)  &&
				(temp[i*out->widthStep+j*out->nChannels]==0)	)	{
					
				counthand2++;
				//printf ("the value of counthadn1 is \t%d\t, and ofcounthand2 is\t%d\n",counthand1, counthand2); 
				oX=oX+j;oY=oY+i;
				
			}
		}
	}
//end of the outer for loop
	x=ox/counthand1;
	y=oy/counthand1;
	X=oX/counthand2;
	Y=oY/counthand2;
	
	
	if(  	(x >  out->width)  ||   (x<=0)   ||						//check statement when blob appears as one .
		(y>out->height)    ||	(y<=0)   ||
		(Y>out->width)     ||   (Y<=0)   ||
		(X>out->width)     ||   (X<=0)    )	{
		
		x=X-40;y=Y;
		X=X+40;Y=Y;
		counthand1=counthand2/2;
		counthand2=counthand2/2;
	}
	if (
		((x >  out->width)  ||   (x<=0)   ||						//check when there is no blob on screen 
		(y>out->height)    ||	(y<=0)   )
		&&
		((Y>out->width)     ||   (Y<=0)   ||
		(X>out->width)     ||   (X<=0)    )
	   )

	{
		x=400;
		X=300;
		y=300;
		Y=300;
		counthand1=counthand2=80;
	}
}
void harshit(int a)	{
	cvSetCaptureProperty(hit,CV_CAP_PROP_POS_FRAMES,a);	
}	
void nowdoasuwish ()
{ 
	inmoveornot=0;
};
void goback()
{
	inmoveornot=1;
};
void keys(unsigned char key , int x, int y){
	switch(key){
		case 115:		//s
		glutIdleFunc(ydecrement);
		break;
	
		
		case 119:		//w
		glutIdleFunc(yincrement);
		break;
	
		case 120:		//x
		glutIdleFunc(zincrement);
		break;

		case 122:		//z
		glutIdleFunc(zdecrement);
		break;

		case 100:		//d
		glutIdleFunc(xincrement);
		break;

		case 97:			//a
		glutIdleFunc(xdecrement);
		break;

		case 99:				//c
		glutIdleFunc(goback);
		break;

		case 102:
		glutIdleFunc(nowdoasuwish);
		break;


//for changing the direction of the camera

		case 106:
		glutIdleFunc(xdirdecrement);
		break;

		case 107:
		glutIdleFunc(ydirdecrement);
		break;

		case 112:
		glutIdleFunc(zdirdecrement);
		break;

		case 108:
		glutIdleFunc(xdirincrement);
		break;

		case 105 :
		glutIdleFunc(ydirincrement);
		break;

		case 111 :
		glutIdleFunc(zdirincrement);
		break;

		case 113:		//q
		glutIdleFunc(nochange);
		break;
		}
}
void inline reshape (int w,int h){
	width=w;
	height=h;
	//my_reshape();
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	glFrustum (-1.0, 1.0, -1.0, 1.0, 1.5, 20.0);
	glMatrixMode (GL_MODELVIEW);
}	
void blobbing( IplImage *hi, char * win1, char * win2, int check )	{
 	cvCvtColor(hi,hi2,CV_BGR2HSV);
	uchar *itemp=(uchar *)(hi2->imageData);
	uchar *itemp1=(uchar *)(hitemp->imageData);
	
	//	 binary conversion 
	for(int i=0;i<hi2->height;i++){
			for(int j=0;j<hi2->width;j++){

					if((itemp[i*hi2->widthStep+j*hi2->nChannels] <hh)	       
					&&
					(itemp[i*hi2->widthStep+j*hi2->nChannels]>hl)
				    &&
					(itemp[i*hi2->widthStep+j*hi2->nChannels+1]<sh)
					&&
					(itemp[i*hi2->widthStep+j*hi2->nChannels+1]>sl)
					
					&& 
					( itemp[i*hi2->widthStep+j*hi2->nChannels+2]<vh)			
					 &&
					( itemp[i*hi2->widthStep+j*hi2->nChannels+2]>vl)		//previous 124
					)
					
					
					{
						itemp1[i*hitemp->widthStep+j]=0;					//dark regions black rest white
						}
					else
						itemp1[i*hitemp->widthStep+j]=255;

		  
			}}
	 
	cvErode( hitemp, hitemp1, NULL, 3);
	cvDilate(hitemp1, hitemp1, NULL, 3);

	hitemp=hitemp1;
	CBlobResult  blob;				
	CBlob *currentBlob=NULL;
	blob=CBlobResult(hitemp1,NULL,255);
	blob.Filter(blob,B_EXCLUDE,CBlobGetArea(),B_LESS,500);
	cvMerge(hitemp1,hitemp1,hitemp1,NULL,out);
 
	CBlob hand1,hand2;																//two blobs,one for each hand
	blob.GetNthBlob( CBlobGetArea(), 0, (hand2));	
	blob.GetNthBlob( CBlobGetArea(), 1, (hand1 ));
	hand1.FillBlob(out,CV_RGB(0,0,255));											//fill the color of blob of hand one with blue
	hand2.FillBlob(out,CV_RGB(0,255,0));											//fill the color of blob of hand two with green 
	coordinates (out,check);														//to find the coordinates of the hands we pass the image onto the function coordinates
	int greater1,greater2,lesser1,lesser2;
	if(x>X)		{
		greater1=x,greater2=y;
		lesser1=X,lesser2=Y;
	}
	else		{
		greater1=X,greater2=Y;
	    lesser1=x,lesser2=y;
	}
 	
	/*cvCircle (  hi, cvPoint(greater1,greater2), 10,
					cvScalar(0,0,255), -1, 8		   );
	cvCircle (  hi, cvPoint(lesser1,lesser2), 10,
					cvScalar(0,255,255), -1, 8		);
					*/
	cvResizeWindow(win2,280,280);
	cvMoveWindow(win2,0,0);
	cvShowImage(win2,out);
	return ; 
}

void init(void)	{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 10.0 };
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	//GLfloat ambientLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	if (nohit)			//if none are hit then keep the background color black . 
		{glClearColor(1,1,1,0);
		}
	if (mehit)													//if i am hit turn the background color to red
	 {glClearColor (1.0, 0.0, 0.0, 0.0);
	 }
	if (comphit )												//if the comp is hit turn the background color to cyan 
		{glClearColor(0,1,1,0);
	    }
	//printf("crazy things are going around");
	
	glShadeModel (GL_SMOOTH);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	//glLightModelfv(GL_LIGHT_MODEL_AMBIENT,ambientLight);
	
	GLfloat light_position4[] =  { -5.0, -9.0, 20.0, 100.0 };
	GLfloat light_position1[] = {13.0, 1.0, 20.0, 100.0 };
	GLfloat light_position2[] = { -5.0,  1.0, 20.0, 100.0 };
	GLfloat light_position3[] = { 13.0, -9, 20.0, 100.0 };
	GLfloat light_position[] = { 100.0, 20, 20.0, 100.0 };
	

	/*glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	
	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT1);

	glLightfv(GL_LIGHT2, GL_POSITION, light_position2);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT2);

	glLightfv(GL_LIGHT3, GL_POSITION, light_position3);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT3);
	*/
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
}								
																		
void display()	{
	Hi=cvQueryFrame (hit);	
	hi=Hi;
	//cvFlip(Hi,hi,1);
	/*cvCreateTrackbar(     "huehigher",	"blobbed lappy", &hh,180,harshit);
	cvCreateTrackbar(     "huelower",	"blobbed lappy", &hl,180,harshit);
	cvCreateTrackbar(     "saturationhigher",	"blobbed lappy", &sh,255,harshit);
	cvCreateTrackbar(     "saturationlower",	"blobbed lappy", &sl,255,harshit);
	cvCreateTrackbar(     "valuehiger",	"blobbed lappy",&vh,255,harshit);
	cvCreateTrackbar(     "valuelower",	"blobbed lappy",&vl,255,harshit);
	*/
	blobbing(hi,"binary lappy","blobbed lappy",0);	
	glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	float greaterx,greatery, greaterz , lesserx, lessery, lesserz;
	if (x<X)	{
		greaterx=x;
		greatery=y;
		greaterz=counthand1;
		lesserx=X;
		lessery=Y;
		lesserz=counthand2;
	}
	else	{
		greaterx=X;
		greatery=Y;
		greaterz=counthand2;
		lesserx=x;
		lessery=y;
		lesserz=counthand1;
	}
	
	GLfloat y1=-4 + (-1*(greaterx-320)*1.2/80);
	GLfloat y2= -4+(-1*(lesserx-320)*1.2/80);
	GLfloat sum=y1+y2,diff=y1-y2;

	GLfloat Centre1[]= {6-1*greaterz/scalingfactor,y1 , -1*(greatery-240)*1.2/60 };
	GLfloat Centre2[]= {6-1* lesserz/scalingfactor, y2, -1*(lessery-240)*1.2/60 };
	
	if (y1>-2)
		Centre1[1]=-2;
	if (y1<-6)
		Centre1[1]=-6;
	if(y2>-2)
		Centre2[1]=-2;
	if(y2<-6)
		Centre2[1]=-6;
	me.hand1next= Sphere( Centre1, .5);
	me.hand2next= Sphere( Centre2, .5);
	


	// Moving the computer's hands...


	//to put the comp suddenly backwards 
	if (inmoveornot||inmoveornot) 
	{//inmoveornot=1;
	if (q1>x_initpos_hand1)
	 {	q1=q1-compspeed;
	
	 }
	else
	{	q1=x_initpos_hand1;
		
		handonedone=0;
		if (jumphandsup1==1)
		{
			jumphandsup1=0;
			tempcomp1[2]=tempcomp1[2]+.15;
		}
		else 
		{
			jumphandsup1=1;
			tempcomp1[2]=tempcomp1[2]-.15;
		}
	}
	comp.hand1next=Sphere(tempcomp1,0.5);
	tempcomp1[0]=q1;
	tempcomp1[1]=((q1-x_initpos_hand1)*1.0)/(xrandom_hand1-x_initpos_hand1)*(yrandom_hand1-y_initpos_hand1)+y_initpos_hand1;
	tempcomp1[2]=((q1-x_initpos_hand1)*1.0)/(xrandom_hand1-x_initpos_hand1)*(zrandom_hand1-z_initpos_hand1)+z_initpos_hand1;


	if (q2>x_initpos_hand2)
	 {	q2=q2-compspeed;
	 }
	else
	{	q2=x_initpos_hand2;
		handtwodone=0;
		if (jumphandsup2==1)
		{
			jumphandsup2=0;
			tempcomp2[2]=tempcomp2[2]+.15;
		}
		else 
		{
			jumphandsup2=1;
			tempcomp2[2]=tempcomp2[2]-.15;
		}
	}
	comp.hand2next=Sphere(tempcomp2,0.5);
	tempcomp2[0]=q2;
	tempcomp2[1]=((q2-x_initpos_hand2)*1.0)/(xrandom_hand2-x_initpos_hand2)*(yrandom_hand2-y_initpos_hand2)+y_initpos_hand2;
	tempcomp2[2]=((q2-x_initpos_hand2)*1.0)/(xrandom_hand2-x_initpos_hand2)*(zrandom_hand2-z_initpos_hand2)+z_initpos_hand2;
	if (handonedone==0 && handtwodone==0)
		{   
			inmoveornot=0;
			handonedone=handtwodone=1;
		}
	
	

	}
	else
	{
	if (count1==1)  {	 
		srand((unsigned)time (NULL));
		xrandom_hand1 =rand()%2+5;
		yrandom_hand1 =rand()%1-3.5;
		zrandom_hand1 =rand()%1;
		count1 =0;
	}
	
	tempcomp1[0]=q1;
	tempcomp1[1]=((q1-x_initpos_hand1)*1.0)/(xrandom_hand1-x_initpos_hand1)*(yrandom_hand1-y_initpos_hand1)+y_initpos_hand1;
	tempcomp1[2]=((q1-x_initpos_hand1)*1.0)/(xrandom_hand1-x_initpos_hand1)*(zrandom_hand1-z_initpos_hand1)+z_initpos_hand1;
			
	if (q1>=abs(xrandom_hand1-x_initpos_hand1)||decreasestarthand1) {						
		q1=q1-compspeed;
		decreasestarthand1=1;
	}
	else {
		q1=compspeed+q1; 
	};
					
	if (q1<=0) {
		q1=0;
		decreasestarthand1=0;
		count1 =1;
	}
	
	comp.hand1next=Sphere(tempcomp1,0.5);
	



	if (count2==1) {
		 srand((unsigned)(time (NULL)*13));
		 xrandom_hand2 =-1*rand()%2-5;
		 yrandom_hand2 =rand()%1-4.5;
		 zrandom_hand2 =rand()%1;
		  count2 =0;
	}
	tempcomp2[0]=q2;
	tempcomp2[1]=((q2-x_initpos_hand2)*1.0)/(xrandom_hand2-x_initpos_hand2)*(yrandom_hand2-y_initpos_hand2)+y_initpos_hand2;
	tempcomp2[2]=((q2-x_initpos_hand2)*1.0)/(xrandom_hand2-x_initpos_hand2)*(zrandom_hand2-z_initpos_hand2)+z_initpos_hand2;
			
	if (q2>=abs(xrandom_hand2-x_initpos_hand2)||decreasestarthand2) {
		q2=q2-compspeed;
		 decreasestarthand2=1;
	}
	else {
		q2=q2+compspeed; 
	};
					
	if (q2<=0) {
		q2=0;
		decreasestarthand2=0;
		count2 =1;
	}
	
	comp.hand2next=Sphere(tempcomp2,0.5);
	}

	if (countforcolor>1)				//the color of screen should remain red or cyan until 6 iterations
	{
	countforcolor=0;
	increasingcountforcolor=0;
	mehit=0;
	comphit=0;
	nohit=1;
	}

	if (increasingcountforcolor==1)		//increases the no. of iterations
	{countforcolor++;
	}

	//.....in the below else parts the colors are decide in the event of collision and 
	//......initialising of increasingcountforcolor is done

	//Determines collission of the hands with hands+bodies...
	if(isCollission(me.hand1next,me,comp) == false )
    {
		me.hand1=me.hand1next;
	}
	else {
	mehit=0;
	 nohit=0;
	 comphit=1;
	 increasingcountforcolor=1;
	 //printf("yeah the comp has been hit\n");
	}
	
	if(isCollission(me.hand2next,me,comp) == false )
	{
		me.hand2=me.hand2next;	
	}
	else
	{mehit=0;
	 nohit=0;
	 comphit=1;
	 increasingcountforcolor=1;
	 //printf("yeah the comp has been hit\n");
	}
	
	if(isCollission(comp.hand1next,comp,me) == false ) 
	{
		comp.hand1=comp.hand1next;
	}
	else
	{mehit=1;
	 nohit=0;
	 comphit=0;
	 increasingcountforcolor=1;
	 //printf("you have been hit\n");
	}
	if(isCollission( comp.hand2next,comp,me) == false ) 
	{
		comp.hand2=comp.hand2next;
	}
	else
	{mehit=1;
	nohit=0;
	comphit=0;
	increasingcountforcolor=1;
	//printf("sorry,sorry , sorry you have been hit\n");
	}

	init();
 	//displaying the hands and bodies...
	//me.dispPlayer();  dont intend to display me's body
	headcolor=1;comphandcolor=0;myhandcolor=0;
	comp.dispPlayer();

	headcolor=0;comphandcolor=0;myhandcolor=1;
	me.hand1.dispSphere();
	me.hand2.dispSphere();

	headcolor=0;comphandcolor=1;myhandcolor=0;
	comp.hand1.dispSphere();
	comp.hand2.dispSphere();
	glLoadIdentity ();													
	gluLookAt (xpos, ypos, zpos, xdir, ydir, zdir, 0.0, 0,1);
	glutSwapBuffers();	
	glFlush();
	
	for(int i=0;i<score->height;i++){
		for(int j=0;j<score->width;j++){
			for(int k=0;k<score->nChannels;k++){
				score->imageData[i * score->widthStep +j  * score->nChannels +k]=0;	
			}
		}
	}
	sprintf(&stringme[11],"%d",me.score);
	sprintf(&stringcomp[9],"%d",comp.score);
	CvFont font;
	cvInitFont(&font,CV_FONT_HERSHEY_SIMPLEX,1,1);
	cvPutText(score,stringme,cvPoint(50,50),&font,cvScalar(255,0,0));
	cvPutText(score,stringcomp, cvPoint(370,50),&font,cvScalar(0,0,255));
	
	cvShowImage("Score",score);
	if (me.score>=1000)	{
		char stringmeend[22]="YOU WIN";							//deciding who wins
		for(int i=0;i<score->height;i++){
		for(int j=0;j<score->width;j++){
			for(int k=0;k<score->nChannels;k++){
				score->imageData[i * score->widthStep +j  * score->nChannels +k]=0;	
				}
			}
		}
		cvPutText(score,stringmeend,cvPoint(50,50),&font,cvScalar(255,0,0));
		cvShowImage("Score",score);
		cvWaitKey();
		exit (0);
	}
	if (comp.score>=1000) {
		char stringcompend[29]="YOU LOOSE.";					//decides who loose
		for(int i=0;i<score->height;i++){
		for(int j=0;j<score->width;j++){
		for(int k=0;k<score->nChannels;k++){
				score->imageData[i * score->widthStep +j  * score->nChannels +k]=0;	
				}
			}
		}	
		cvPutText(score,stringcompend, cvPoint(370,50),&font,cvScalar(0,0,255));
		cvWaitKey();
		exit (0);
		}
	
}																																				

void givedepth(IplImage *localimagergb)
{	IplImage*localimage=cvCreateImage(cvGetSize(localimagergb),IPL_DEPTH_8U,3);
	cvCvtColor(localimagergb,localimage,CV_BGR2HSV);
	IplImage *blobbedscaling=cvCreateImage(cvGetSize(localimagergb),IPL_DEPTH_8U,3);
	uchar *itemp=(uchar *)(localimage->imageData);
    IplImage *binaryscaling=cvCreateImage(cvGetSize(localimagergb),IPL_DEPTH_8U,1);
	uchar *itemp1=(uchar *)(binaryscaling ->imageData);
	for(int i=0;i<hi2->height;i++){
			for(int j=0;j<hi2->width;j++){	

					if((itemp[i*localimage->widthStep+j*localimage->nChannels] <hh)	       
					&&
					(itemp[i*localimage->widthStep+j*localimage->nChannels]>hl)
				    &&
					(itemp[i*localimage->widthStep+j*localimage->nChannels+1]<sh)
					&&
					(itemp[i*localimage->widthStep+j*localimage->nChannels+1]>sl)
					&& 
					( itemp[i*localimage->widthStep+j*localimage->nChannels+2]<vh)			
					 &&
					( itemp[i*localimage->widthStep+j*localimage->nChannels+2]>vl)		//previous 124
					)	{
						itemp1[i*binaryscaling->widthStep+j]=0;					//dark regions black rest white
						}
					else
						itemp1[i*binaryscaling->widthStep+j]=255;
			}}
    cvErode( binaryscaling, binaryscaling, NULL, 4);
	cvDilate(binaryscaling, binaryscaling, NULL, 4);
	CBlobResult  blob;				
	CBlob *currentBlob=NULL;
	blob=CBlobResult(binaryscaling,NULL,255);
	blob.Filter(blob,B_EXCLUDE,CBlobGetArea(),B_LESS,500);
	cvMerge(binaryscaling,binaryscaling,binaryscaling,NULL,blobbedscaling);
	CBlob hand1,hand2;																//two blobs,one for each hand
	blob.GetNthBlob( CBlobGetArea(), 0, (hand2));	
	blob.GetNthBlob( CBlobGetArea(), 1, (hand1 ));
	hand1.FillBlob(blobbedscaling,CV_RGB(0,0,255));											//fill the color of blob of hand one with blue
	hand2.FillBlob(blobbedscaling,CV_RGB(0,255,0));											//fill the color of blob of hand two with green 
	coordinates (blobbedscaling,0);
}

void todecidescalingfactor() 
{	CvCapture *hiscaling=cvCreateCameraCapture(0);
	for (int w=0;w<400000000;w++);
	int getoutoflooptime=0;
	
	score=cvCreateImage(cvSize(600,120),IPL_DEPTH_8U,3);

	cvNamedWindow("Score",CV_WINDOW_AUTOSIZE);				//to create the window displaying our original instructions to the player for scalingfactor....
	cvMoveWindow("Score",300,650);
	for(int i=0;i<score->height;i++){
		for(int j=0;j<score->width;j++){
			for(int k=0;k<score->nChannels;k++){
				score->imageData[i * score->widthStep +j  * score->nChannels +k]=0;	
				}
			}
		}
	CvFont font;
	cvInitFont(&font,CV_FONT_HERSHEY_SIMPLEX,.4,.5);
	char stringmeend[]="The comp will wait for 8 sec so that you can set ur hands in fowrward position";
	cvPutText(score,stringmeend, cvPoint(0,40),&font,cvScalar(255,255,0));
	cvShowImage("Score",score);

	cvWaitKey(2000);
	
	while(1)
	{	if (getoutoflooptime>65)							//approx. equivalent to 8 seconds 
			break;
		cvWaitKey(10);
		imgscaling=cvQueryFrame(hiscaling);
		cvMoveWindow("harshits",280,140);
		cvShowImage("harshits",imgscaling);
		if (!imgscaling)
			break;
		getoutoflooptime++;
		
	}
	givedepth(imgscaling);
	getoutoflooptime=0;
	float forward=counthand1+counthand2;
	

	cvNamedWindow("Score",CV_WINDOW_AUTOSIZE);				//to create the window displaying our original instructions to the player for scalingfactor....
	cvMoveWindow("Score",300,650);
	for(int i=0;i<score->height;i++){
		for(int j=0;j<score->width;j++){
			for(int k=0;k<score->nChannels;k++){
				score->imageData[i * score->widthStep +j  * score->nChannels +k]=0;	
				}
			}
		}
	cvInitFont(&font,CV_FONT_HERSHEY_SIMPLEX,.4,.5);
	char stringmeend2[]="The comp will wait for 8 sec so that you can set ur hands in backward position";
	cvPutText(score,stringmeend2, cvPoint(0,40),&font,cvScalar(0,255,255));
	cvShowImage("Score",score);
	
	while(1)
	{	if (getoutoflooptime>65)
			break;
		cvWaitKey(10);
		imgscaling=cvQueryFrame(hiscaling);
		cvMoveWindow("harshits",280,140);
		cvShowImage("harshits",imgscaling);
		getoutoflooptime++;
	}
	givedepth(imgscaling);
	cvWaitKey(2000);
	cvDestroyWindow("harshits");
	float backward=counthand1+counthand2;
	printf("the forward is %d\n ",forward);
	printf("the backward is %d\n ",backward);
	scalingfactor=(forward-backward)/(2*8);
	printf("The scaling facator is =%f\n\n\n\n",scalingfactor);
}

void tobecalledbymain(int argc, char** argv,int executeinfiniteloop)	{
	score=cvCreateImage(cvSize(600,120),IPL_DEPTH_8U,3);
	cvNamedWindow("Score",CV_WINDOW_AUTOSIZE);
	cvMoveWindow("Score",300,650);
	//initialising the position of the camera
	xpos=12;								//12,-5,0
	ypos=-5;
	zpos=0;
	
	//initialising the direction of the camera 
	xdir=-1;								//-1,-3,5,0
	ydir=-3.5;
	zdir=0;

	//	TRACKBARS
	/*
	hh=cvGetTrackbarPos("huehigher","blobbed lappy");
	hl=cvGetTrackbarPos("huelower","blobbed lappy");
 	sh=cvGetTrackbarPos("saturationhigher","blobbed lappy");
 	sl=cvGetTrackbarPos("saturationlower","blobbed lappy");
	vh=cvGetTrackbarPos("valuehigher","blobbed lappy");
 	vl=cvGetTrackbarPos("valuelower","blobbed lappy");
	*/
	//	Creating camera capture
	hit=cvCreateCameraCapture(0);
	cvWaitKey(5000);
	if(hit==NULL) {
		printf("problem initialising camera feed....");
		exit(0);
	}
	
	//Delay Loop
	for (int h=0;h<300000000;h++);

	//starting the openGL interface 
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB|GLUT_DEPTH);
	glEnable(GL_DEPTH_TEST);
	glutInitWindowSize (600, 600);
	glutInitWindowPosition (300, 50);

	glutCreateWindow (argv[0]);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	init ();

	glutMouseFunc(mouse);
	glutKeyboardFunc(keys);
	glutMainLoop();
	
//Releasing memory used by the program	
	cvReleaseCapture(&hit);
	cvWaitKey();
}

int main(int argc, char ** argv){
	// here the initialising part of various variables is done that are to be further used in the program
	hh=135,hl=108,sh=255,sl=40,vh=255,vl=35;		//initializing the trackbarparameters.
	compspeed=2;									//to control the speed of movement of computer's hand
	inmoveornot=0;									//to check wherther to move in or not
	handonedone=handtwodone=1;						//to pass the control when both hands are back to original place to ...
	timed=0;										//to see for how long the screen should remain colored
	jumphandsup1=jumphandsup2=0;					//to make hands jump up and down
	mehit=comphit=0,nohit=1;						//to see which color the screen should be of ..
	countforcolor=0;								//similar function as timed
	increasingcountforcolor=0;
	myhandcolor=0;									//The following are to decide what should be the color of the spheres
	comphandcolor=0;
	headcolor=0;
	
	me.Centre[0]=9;
	me.Centre[1]=-4;
	me.Centre[2]=0;
	
	comp.Centre[0]=-2;
	comp.Centre[1]=-4;
	comp.Centre[2]=0;

	x_initpos_hand1=comp.Centre[0]+2.2;
	y_initpos_hand1=comp.Centre[1]+1;
	z_initpos_hand1=comp.Centre[2];
	
	x_initpos_hand2=comp.Centre[0]+2.2;
	y_initpos_hand2=comp.Centre[1]-1;
	z_initpos_hand2=comp.Centre[2];
	
	//the randomized coordinates initialised to zero...
	xrandom_hand1=yrandom_hand1=zrandom_hand1=0;
	xrandom_hand2=yrandom_hand2=zrandom_hand2=0;
		
	//help the isCollission function to decide what constitutes 
	//a collission with the opponent...
	me.orientation=1;
	comp.orientation=-1;
	
	me.yspan=comp.yspan=4;						//previous were 2 and 5 respectively
	me.zspan=comp.zspan=8;
	
	me.body.edge=me.yspan;
	comp.body.edge=comp.yspan;
	
	for(int i=0;i<=2;i++){
		me.body.Centre[i]=me.Centre[i];
		comp.body.Centre[i]=comp.Centre[i];
	}
	me.body.Centre[2]+=me.zspan/12;
	comp.body.Centre[2]+=comp.zspan/12;
	todecidescalingfactor();
	tobecalledbymain(argc, argv, 1 );
}
