#include <iostream>
#include <vector>
#include <math.h>
#include <simplecpp>

using namespace std;

struct V2{ //V2 is a function which represents a vector in 2-D
	double x,y;
V2(){};
V2(double a,double b){
	x=a;y=b;
	}
double mod();
//{//this function gives the magnitude of a V2 vector
	//	return sqrt(x*x+y*y);}
	V2 operator+(V2 a){
		return V2(x+a.x,y+a.y);}
		};

		double V2::mod(){
			return sqrt(x*x+y*y);
		}
struct V4{//V4 is a function which stores the position and velocity of each of 3 balls
	V2 pos,vel;//pos is for position.vel is for velocity
V4(V2 a,V2 b){//this is a constructor to define V4 structs
	pos=a;vel=b;}
};

bool collide(V4 a,V4 b){
	//this function is used to know whether a collision is going to happen in next step
//by knowing their velocities and increasing position by velocity
//it takes its arguments as two V4 structs
	double vx=a.vel.x-b.vel.x,vy=a.vel.y-b.vel.y,x1=a.pos.x+vx,y1=a.pos.y+vy;
	return (pow(b.pos.x-x1,2)+pow(b.pos.y-y1,2)<60*60);
}

bool compgoal(V4 &b){//returns true if comp scores a goal
	return ((b.pos.y>660)&&((b.pos.x>200)||(b.pos.x<400)));
}

bool playergoal(V4 &p){//returns true if player scores a goal
	return ((p.pos.y<40)&&((p.pos.x>200)||(p.pos.x<400)));
}


void movetowards(V4 &comp,V4 ball,double v){/*a is comp b is ball.
this function moves com towards puck by adjusting pos and vel of comp
this should be called only if ball is in comp's arena
it takes 3 arguments ,v is speed of comp which is always fixed but the velocities in x&y direction are
changing so that it always points towards ball*/
	double phi;/*this is the angle comp's velocity would make with x axis after this loop
	phi is taken care that it is positive always in the 4 cases
	the 4 cases below are based on whether ball with respect to comp lies in which of the 4 quadrants*/
	if((comp.pos.x<ball.pos.x)&&(comp.pos.y>=ball.pos.y)){
		phi=atan((-comp.pos.y+ball.pos.y)/(comp.pos.x-ball.pos.x));
		comp.vel.x=v*cos(phi);
		comp.vel.y=-v*sin(phi);
	}
	else if((comp.pos.x>ball.pos.x)&&(comp.pos.y>=ball.pos.y)){
		phi=atan((comp.pos.y-ball.pos.y)/(comp.pos.x-ball.pos.x));
		comp.vel.x=-v*cos(phi);
		comp.vel.y=-v*sin(phi);
	}
	else if((comp.pos.x<ball.pos.x)&&(comp.pos.y<ball.pos.y)){
		phi=atan((comp.pos.y-ball.pos.y)/(comp.pos.x-ball.pos.x));
		comp.vel.x=v*cos(phi);
		comp.vel.y=v*sin(phi);
	}
	else if((comp.pos.x>ball.pos.x)&&(comp.pos.y<ball.pos.y)){
		phi=atan((-comp.pos.y+ball.pos.y)/(comp.pos.x-ball.pos.x));
		comp.vel.x=-v*cos(phi);
		comp.vel.y=v*sin(phi);
	}
	else{//this case is when comp and ball have same x coordinate
		comp.vel.x=0;
		if(comp.pos.y<ball.pos.y){
			comp.vel.y=v;
		}
		else{
			comp.vel.y=-v;
		}
	}

}

bool compcoll(V2 a,V4 b){/*It takes as arguments a struct V2 position of disc1 and V4
information of disc2 and checks  whether movement of disc2 in the next instant causes a collision or not.*/
	b.pos.x+=b.vel.x;
	b.pos.y+=b.vel.y;
	return (pow(a.x-b.pos.x,2)+pow(a.y-b.pos.y,2)<60*60);
	}

void movex(V4 &comp,V2 b,double v){/*This function will be called only if puck is not in computers arena.
 It positions the computer disc in its arena according to a set of commands. It moves the computer
 in x direction depending on the x direction velocity of the puck and moves in the y direction such that it gets closer to its goalpost.*/
	if((b.x)>v){comp.vel.x=v;}
	else if((b.x)<-v){comp.vel.x=-v;}
	else {comp.vel.x=b.x;}
	if(comp.pos.y>102)comp.vel.y=-2;
	else if(comp.pos.y<98)comp.vel.y=2;
	else comp.vel.y=0;
}

	void collcalc(V4 a,V4&ball){/*This function is executed when the puck collides with the computer or player.
	 It calculates the x and y components of the puck after the collision.*/
	double phi,alpha,vel;
phi=180+arctan2(ball.vel.y-a.vel.y,ball.vel.x-a.vel.x);
alpha=180+arctan2(ball.pos.y-a.pos.y,ball.pos.x-a.pos.x);
vel=sqrt(pow(ball.vel.y-a.vel.y,2)+pow(ball.vel.x-a.vel.x,2));
ball.vel.x=(vel*cosine(2*alpha-phi)+a.vel.x);
ball.vel.y=(vel*sine(2*alpha-phi)+a.vel.y);
}

void collision(V4 &a,V4 &b,Circle &f,bool z){/*This function is executed only when we know that in next loop/instant a collision
is going to happen between disc1 and disc2. If disc1 is puck and disc2 is computer then the computer is moved close to the puck
*  using a repeat loop by breaking the event into 10 steps. Else If disc1 is  player and disc2 is ball then the puck is moved
*  close to the player using a repeat loop by breaking the event into 10 steps. At the end the collcalc function is called.
*  In case the collision is between computer and puck then the velocity of computer is reversed at the end.*/
	V2 rel(b.vel.x-a.vel.x,b.vel.y-a.vel.y);
	V2 c=b.pos;
	repeat(10){
				c.x+=rel.x/10;c.y+=rel.y/10;
		if((pow((c.x-a.pos.x),2)+pow((c.y-a.pos.y),2))<=60*60)break;
		}
		b.pos.x=c.x-rel.x/10;
		b.pos.y=c.y-rel.y/10;
		f.moveTo(b.pos.x,b.pos.y);
		if(z){collcalc(b,a);b.vel.y=-b.vel.y;b.vel.x=-b.vel.x;}
		else collcalc(a,b);
		}

void wall(V4 &a,bool c){/*This function sees to it that computer and puck do not go out of their allowed regions.
It checks this by checking whether there is a collision/crossing with the boundaries in the next instant.
*  If there is a collision/crossing in the next instant then the function reverses the corresponding component of velocity.*/
	V2 abc=a.pos+a.vel;
	if(abc.x<80||abc.x>520){
		if(a.vel.x<0){
			a.pos.y=a.pos.y+(a.vel.y/a.vel.x)*(80-a.pos.x);
			a.pos.x=80;
			a.vel.x=-a.vel.x;
			}
			else{
				a.pos.y=a.pos.y+(a.vel.y/a.vel.x)*(520-a.pos.x);
				a.pos.x=520;
				a.vel.x=-a.vel.x;
				}}
				if(c){
				if((abc.y<70||abc.y>630)&&((abc.x<200)||(abc.x>400))){
					if(a.vel.y<0){
						a.pos.x=a.pos.x+(a.vel.x/a.vel.y)*(70-a.pos.y);
						a.pos.y=70;
						a.vel.y=-a.vel.y;
					}
					else{
						a.pos.x=a.pos.x+(a.vel.x/a.vel.y)*(630-a.pos.y);
						a.pos.y=630;
						a.vel.y=-a.vel.y;
					}}}
					else {
						if(abc.y<70||abc.y>320){
					if(a.vel.y<0){
						a.pos.x=a.pos.x+(a.vel.x/a.vel.y)*(70-a.pos.y);
						a.pos.y=70;
						a.vel.y=-a.vel.y;
					}
					else{
						a.pos.x=a.pos.x+(a.vel.x/a.vel.y)*(320-a.pos.y);
						a.pos.y=320;
						a.vel.y=-a.vel.y;
						}}}}

void stadium(){//This function creates the stadium and imprints it. It has all the graphics of the game in it. Now we can play the game.
	double verleft[8][2]={{200,35},{45,35},{45,665},{200,665},{200,660},{50,660},{50,40},{200,40}};
    double verright[8][2]={{400,40},{400,35},{555,35},{555,665},{400,665},{400,660},{550,660},{550,40}};
    double vermid[4][2]={{50,40},{550,40},{550,660},{50,660}};
    double semicircle1[183][2];
    for(int i=0;i<181;i++){
		semicircle1[i][0]=300+100*cosine(i);
		semicircle1[i][1]=40+100*sine(i);
		}
		semicircle1[181][0]=200;
		semicircle1[181][1]=35;
		semicircle1[182][0]=400;
		semicircle1[182][1]=35;
	double semicircle2[183][2];
    for(int i=0;i<181;i++){
		semicircle2[i][0]=300+100*cosine(i);
		semicircle2[i][1]=660-100*sine(i);
		}
		semicircle2[181][0]=200;
		semicircle2[181][1]=665;
		semicircle2[182][0]=400;
		semicircle2[182][1]=665;

	  Polygon p1(0,0,verleft,8);
    p1.setColor(COLOR("brown"));
    p1.setFill();
    p1.imprint();
    Polygon p2(0,0,verright,8);
    p2.setColor(COLOR("brown"));
    p2.setFill();
    p2.imprint();
    Polygon p3(0,0,vermid,4);
    p3.setColor(COLOR("lightyellow"));
    p3.setFill();
    p3.imprint();
    Line(50,350,550,350).imprint();
    Polygon p4(0,0,semicircle1,183);
    p4.setColor(COLOR("lightcyan"));
    p4.setFill();
    p4.imprint();
    Polygon p5(0,0,semicircle2,183);
    p5.setColor(COLOR("lightcyan"));
    p5.setFill();
    p5.imprint();
    Polygon p6(0,0,semicircle1,183),p7(0,0,semicircle2,183);
    p6.setColor(COLOR("darkgreen"));
    p7.setColor(COLOR("darkgreen"));
    p6.imprint();
    p7.imprint();
    Rectangle r(580,300,20,20),r1(580,400,20,20),r2(600,250,75,30),r3(300,15,200,30);
    r2.setColor(COLOR("cyan"));
	r.setColor(COLOR("red"));
	r1.setColor(COLOR("blue"));
	r3.setColor(COLOR("cyan"));
	r.setFill();
	r1.setFill();
	r.imprint();
	r1.imprint();
	r2.setFill();
	r2.imprint();
	r3.setFill();
	r3.imprint();
	Line(200,35,400,35).imprint();
	Line(200,665,400,665).imprint();
	Text(600,250,"Goals").imprint();
	Text(300,15,"GLOW HOCKEY").imprint();
	Circle c(300,350,90);
	c.setColor(COLOR("darkgreen"));
	c.imprint();
}

	double levels(){//This funnctions opens a window which asks the level of the game.
		double box1[4][2]={{200,160},{400,160},{400,120},{200,120}};
		double box2[4][2]={{200,260},{400,260},{400,300},{200,300}};
		double box3[4][2]={{200,400},{400,400},{400,440},{200,440}};
		double box4[4][2]={{200,580},{400,580},{400,540},{200,540}};
		Polygon p1(0,0,box1,4);p1.imprint();
		Text t1(300,140,"VERY EASY");t1.imprint();
		Polygon p2(0,0,box2,4);p2.imprint();
		Text t2(300,280,"EASY");t2.imprint();
		Polygon p3(0,0,box3,4);p3.imprint();
		Text t3(300,420,"MEDIUM");t3.imprint();
		Polygon p4(0,0,box4,4);p4.imprint();
		Text t4(300,560,"HARD");t4.imprint();
		double v;//It represents the velocity which is going to be returns to the main function
		while(1){
		int n=getClick();
		V2 m;
		m.x=n/65536;
		m.y=n%65536;
		//the below piece of code checks which button the player clicks on.
		if((m.x>=200)&&(m.x<=400)&&(m.y<=160)&&(m.y>=120)){
			 v=1;
			 break;
		 }
		if((m.x>=200)&&(m.x<=400)&&(m.y<=300)&&(m.y>=260)) {
			v=2;
			break;
		}
		if((m.x>=200)&&(m.x<=400)&&(m.y<=440)&&(m.y>=400)) {
			v=3;
			break;
		}
		if((m.x>=200)&&(m.x<=400)&&(m.y<=580)&&(m.y>=540)) {
			v=4;
			break;
		}}

		return v;
	}


int main(){
initCanvas("Glow Hockey",600,700);
double v=levels();
v=10;
closeCanvas();// To close the canvas which was opened to find out the level
int maxgoals;
cout<<"No. of goals required to end the game:"<<endl;// asks the maximum goals to win the game
cin>>maxgoals;
initCanvas("Glow Hockey",650,700);// Creates the canvas on which we play the game
stadium();
Circle com(300,80,30),user(300,620,30),puck(300,350,30);//These are the discs. com is the computer. user is the player. puck is the ball/puck.

	XEvent event;
	double x1=300,y1=620;//x1 and y1 are used to store the previous position of the player(user).
	V2 a(300,80),b(300,620),c(300,350),d(0,0);//a,b,c represent the initial positions and d represents initial velocity pf the three discs
	V4 comp(a,d),player(b,d),ball(c,d);// info of the computer,player and puck respectivelys
	user.setColor(COLOR("blue"));
     com.setColor(COLOR("red")); puck.setColor(COLOR("green"));
     user.setFill(1);com.setFill(1);puck.setFill(1);

	bool y=false,z=false;//z is used as a bool condition to stop comp chasing the ball for some loops immediately after collision
	int i=0,j=0;

	int t=0,u=0;
	Text g(600,300,i),p(600,400,u);//scores of the game
	 while(1){
		 		 nextEvent(&event);
      if(mouseDragEvent(&event)||keyPressEvent(&event)){//only when a button is pressed on the keyboard or the mouse is being dragged
      //continuosly the program enters the loop otherwise the game is paused
           /*From here we are trying to adjust the movements of computer
           First we are checking whether the puck comes into the arena of the computer.Use of z will be explained below*/
		  if((ball.pos.y<=380)&&(!z))
		  {
		movetowards(comp,ball,v);//it adjusts the velocity of comp
        if(collide(ball,comp))//checks for collision between the ball and comp
         {
			collision(ball,comp,com,1);//after this they are in collision and the new velocities of comp and puck are determined
        z=true;// Next z is made true so that the comp doesnt chase the puck for a certain number of loops
        }}
else if((ball.pos.y<=380)&&(z))//
{
	if(t==0) {//we are reversing the velocity of comp after collision. t=0 is used for not changing the velocity for the next 49 loop events
		comp.vel.x=-comp.vel.x;
		comp.vel.y=-comp.vel.y;
	}
	t++;
	wall(comp,0);// This checks that the comp doesnt leave its arena
	if(collide(ball,comp))collision(ball,comp,com,1);//This checks the collsion between puck and comp
	if(t==50)// in the 50th loop it sets t to 0 and z to false and the normal process continues
	{
		t=0;
		z=false;
	}
}
else //THis condition is when puck is not in the arena of the computer
{
	z=false;// z is false because sometimes the puck might come out of computers arena before the 50 loops get over and hence we
	//need to set z to false for normal process to continue
	movex(comp,ball.vel,v/2);
}

		  if(sqrt(pow(event.xmotion.x-x1,2)+pow(event.xmotion.y-y1,2))<=30){//Checks whether the player clicks inside ball and sets the velocity
     	  player.vel.x=(event.xmotion.x-x1);
	      player.vel.y=(event.xmotion.y-y1);
	      }

if(collide(player,ball)) {//checks whether there is a collision between player and ball
	collision(player,ball,puck,0);// Here we get updated balls velocity
	y=true;// y behaves similarly to z
	}

	if(ball.vel.mod()>6){
			  ball.vel.x=ball.vel.x/2;
			  ball.vel.y/=2;
}
		 if(!y){
		  V2 c;
	      c=player.pos+player.vel;
	      if((c.x>80)&&(c.x<520)&&(c.y>380)&&(c.y<630)){// To see that the player doesnt exceed the boundaries
			  if(!collide(ball,player)){
			  player.pos=c;
		  user.moveTo(player.pos.x,player.pos.y);
	      x1=player.pos.x;
	      y1=player.pos.y;
	      }
	    }}
	    else {// by the below code we are not allowing the player to move for two while loops
			j++;
			if(j==2)
			{
				j=0;
				y=false;
		}}

			wall(ball,1);//this function checks that the ball doesnot exceed the boundary


			  if((comp.pos.x+comp.vel.x>=80)&&(comp.pos.x+comp.vel.x<=520)&&(comp.pos.y+comp.vel.y>=70)&&(comp.pos.y+comp.vel.y<=320)){
				  //this function checks that the comp doesnot exceed the boundary
			  if(!collide(comp,ball)){ //if there is no collision in the next instant comp moves
				  comp.pos=comp.pos+comp.vel;
			   beginFrame();
			  com.moveTo(comp.pos.x,comp.pos.y);
			  endFrame();
			  }}
	if(!compcoll(player.pos,ball)&&!compcoll(comp.pos,ball)){
		//this is for checking that puck doesnt colllide with anything
			ball.pos=ball.pos+ball.vel;
			beginFrame();
			  puck.moveTo(ball.pos.x,ball.pos.y);
			  endFrame();
		  }
		  	if(compgoal(ball)){//This is for checking the goals
	    i++;
	    g.reset(600,300,i);
		wait(2);
		comp.pos.x=300;
		comp.pos.y=80;
		ball.pos.x=300;
		ball.pos.y=440;
		player.pos.x=300;
		player.pos.y=600;
		comp.vel.x=0;
		comp.vel.y=0;
		ball.vel.x=0;
		ball.vel.y=0;
		com.reset(300,80,30);
		puck.reset(300,440,30);
		user.reset(300,600,30);
	}
	if(playergoal(ball)){
		u++;
	    p.reset(600,400,u);
		wait(2);
		comp.pos.x=300;
		comp.pos.y=80;
		ball.pos.x=300;
		ball.pos.y=260;
		player.pos.x=300;
		player.pos.y=620;
		comp.vel.x=0;
		comp.vel.y=0;
		ball.vel.x=0;
		ball.vel.y=0;
		com.reset(300,80,30);
		puck.reset(300,260,30);
		user.reset(300,620,30);
	}
	if(i==maxgoals){
	   cout<<"Computer wins"<<endl;
	   Text w(300,350,"Computer wins");
	   w.imprint();
	   break;
   }
   if(u==maxgoals){
	   cout<<"You win"<<endl;
	   Text w(300,350,"You win");
	   w.imprint();
	   break;
   }
	}}
	getClick();
	}
