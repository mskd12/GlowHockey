#include <iostream>
#include <vector>
#include <math.h>
using namespace std;

struct V2{//V2 is a function which represents a vector in 2-D 
	double x;double y;
V2(){};
V2(double a,double b){
	x=a;y=b;
	}
double mod(){//this function gives the magnitude of a V2 vector
		return sqrt(x*x+y*y);
	}
	V2 operator+(V2 a){
		return V2(x+a.x,y+a.y);}
		};
		
struct V4{//V4 is a function which stores the position and velocity of each of 3 balls 
	V2 pos;V2 vel;//pos is for position.vel is for velocity
V4(V2 a,V2 b){//this is a constructor to define V4 structs 
	pos=a;vel=b;}
};

bool collide(V4 a,V4 b){//this function is used to know whether a collision is going to happen in next step
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

void movetowards(V4 &comp,V4 ball,double v){
//this should be called only if ball is in comp's arena
//this function moves comp towards puck
//it takes 3 arguments ,v is speed of comp which is always fixed but the velocities in x&y direction are 
//changing so that it always points towards ball
	double phi;//this is the angle comp's velocity would make with x axis after this loop
	//phi is taken care that it is positive always in the 4 cases
	//the 4 cases below are based on whether ball with respect to comp lies in which of the 4 quadrants
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

bool compcoll(V2 a,V4 b){//this function is when one of the 2 circles representing the 2 arguments
//has already been moved on the screen.this function checks by increasing the position and checking whether  
	b.pos.x+=b.vel.x;
	b.pos.y+=b.vel.y;
	return (pow(a.x-b.pos.x,2)+pow(a.y-b.pos.y,2)<60*60);
	}
	
void movex(V4 &comp,V2 b,double v){//this function will be called only if ball is not in comps arena
//a is comp b is velocity of puck c is comps ball
	if((b.x)>v){comp.vel.x=v;}
	else if((b.x)<-v){comp.vel.x=-v;}
	else {comp.vel.x=b.x;}
	if(comp.pos.y>102)comp.vel.y=-2;
	else if(comp.pos.y<98)comp.vel.y=2;
	else comp.vel.y=0; 
}

	void collcalc(V4 a,V4&ball){
	double phi,alpha,vel;
phi=180+arctan2(ball.vel.y-a.vel.y,ball.vel.x-a.vel.x);
alpha=180+arctan2(ball.pos.y-a.pos.y,ball.pos.x-a.pos.x);
vel=sqrt(pow(ball.vel.y-a.vel.y,2)+pow(ball.vel.x-a.vel.x,2));
ball.vel.x=(vel*cosine(2*alpha-phi)+a.vel.x);
ball.vel.y=(vel*sine(2*alpha-phi)+a.vel.y);
}

void collision(V4 &a,V4 &b,Circle &f,bool z){//a is ball b is comp ,player
//this function should be used only when we know that in next while loop a collision is going to happen
//this function changes the position of puck near to com or user and also changes velocity of puck  
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
		
void wall(V4 &a,bool c){
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
	
void stadium(){
	double verleft[8][2]={{200,35},{45,35},{45,665},{200,665},{200,660},{50,660},{50,40},{200,40}};
double verright[8][2]={{400,40},{400,35},{555,35},{555,665},{400,665},{400,660},{550,660},{550,40}};
double vermid[4][2]={{50,40},{550,40},{550,660},{50,660}};
Polygon p1(0,0,verleft,8);
p1.setColor(COLOR("orange"));
p1.setFill();
p1.imprint();
Polygon p2(0,0,verright,8);
p2.setColor(COLOR("orange"));
p2.setFill();
p2.imprint();
Polygon p3(0,0,vermid,4);
p3.setColor(COLOR("yellow"));
p3.setFill();
p3.imprint();
Line(50,350,550,350).imprint();
}

int main(){
initCanvas("Glow Hockey",600,700);
stadium();
Circle com(300,80,30),user(300,620,30),puck(300,350,30);
	XEvent event;
	double x1=300,y1=620;
	V2 a(300,80),b(300,620),c(300,350),d(0,0);
	V4 comp(a,d),player(b,d),ball(c,d);
	user.setColor(COLOR("blue"));
     com.setColor(COLOR("red")); puck.setColor(COLOR("green"));
     user.setFill(1);com.setFill(1);puck.setFill(1);
	bool y=false,z=false;
	int i=0,j=0;
	int t=0,u=0;
	Rectangle r(575,300,30,30),r1(575,400,30,30);
	r.imprint();
	r1.imprint();
	Text g(575,300,i),p(575,400,u);
	 while(1){
		 		 nextEvent(&event);
      if(mouseDragEvent(&event)||keyPressEvent(&event))
      {
		  if((ball.pos.y<=350)&&(!z))
		  {
		//if(!(collide(comp,ball)))
		movetowards(comp,ball,1);
        if(collide(ball,comp))
         {
			collision(ball,comp,com,1);
        z=true;
        }}
else if((ball.pos.y<=350)&&(z))
{
	if(t==0) {
		comp.vel.x=-comp.vel.x;
		comp.vel.y=-comp.vel.y;
	}
	t++;
	wall(comp,0);
	if(compcoll(ball.pos,comp))collision(ball,comp,com,1);
	if(t==50)
	{
		t=0;
		z=false;
	}
}
else 
{
	z=false;
	movex(comp,ball.vel,1);
}

		  if(sqrt(pow(event.xmotion.x-x1,2)+pow(event.xmotion.y-y1,2))<=60){
     	  player.vel.x=event.xmotion.x-x1;
	      player.vel.y=event.xmotion.y-y1;
	      }

if(collide(player,ball)) {
	collision(player,ball,puck,0);
	y=true;}
	
	if(ball.vel.mod()>6){
			  ball.vel.x=ball.vel.x/2;
			  ball.vel.y/=2;
}
		 if(!y){
		  V2 c;
	      c=player.pos+player.vel;
	      if((c.x>80)&&(c.x<520)&&(c.y>380)&&(c.y<630)){
			  if(!collide(ball,player)){
			  player.pos=c;
		  user.moveTo(player.pos.x,player.pos.y);
	      x1=player.pos.x;
	      y1=player.pos.y;
	      }
	    }}
	    else {
			j++;
			if(j==2) 
			{
				j=0;
				y=false;
		}}
	
			wall(ball,1);
				
			  
			  if((comp.pos.x+comp.vel.x>=80)&&(comp.pos.x+comp.vel.x<=520)&&(comp.pos.y+comp.vel.y>=70)&&(comp.pos.y+comp.vel.y<=320)){
			  if(!collide(comp,ball)){ 
				  comp.pos=comp.pos+comp.vel;
			   beginFrame();
			  com.moveTo(comp.pos.x,comp.pos.y);
			  endFrame();
			  }}
	if(!compcoll(player.pos,ball)&&!compcoll(comp.pos,ball)){
			ball.pos=ball.pos+ball.vel;	 
			beginFrame();    
			  puck.moveTo(ball.pos.x,ball.pos.y);
			  endFrame();
		  }
		  	if(compgoal(ball)){
	    i++;
	    g.reset(575,300,i);
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
	    p.reset(575,400,u);
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
	if(i==7){
	   cout<<"Computer wins"<<endl;
	   Text w(300,350,"Computer wins");
	   w.imprint();
   }
   if(u==7){
	   cout<<"You win"<<endl;
	   Text w(300,350,"You win");
	   w.imprint();
   }
	}}}
