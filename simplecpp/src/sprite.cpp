#include <simplecpp>

Sprite::Sprite(Composite* owner) {
  init(owner);
}

Sprite::Sprite(double dx, double dy, Composite* owner) 
  : pose(dx,dy) {
  init(owner);
}
void Sprite::reset(double dx, double dy, Composite* owner){
  pose = Pose(dx,dy);
  init(owner);
}
void Sprite::init(Composite* owner){
  visible = true;
  color = COLOR("black");
  fill = false;
  penIsDown = false;
  if(owner != NULL) owner->addPart(this);
  else addSprite(this);     // Register to canvas
}  

Sprite::~Sprite(){
  removeSprite(this);
}

void Sprite::show(){
  visible = true;
  repaint();
}

void Sprite::hide(){
  visible = false;
  repaint();
}

void Sprite::rotate(double angle, bool repaintP){
  pose = Pose(pose, Pose(angle));  // rotate in its frame of reference
  if(repaintP) repaint();  
}

void Sprite::setScale(double factor, bool repaintP){
  pose.setScale(factor);
  if(repaintP) repaint();
}

void Sprite::scale(double factor, bool repaintP){
  pose = Pose(pose,Pose(factor,true));
  if(repaintP) repaint();
}

void Sprite::moveTo(double x, double y, bool repaintP){
  Position oldorigin = pose.getOrigin();
  move(x-oldorigin.getX(), y-oldorigin.getY(), repaintP);
}

void Sprite::move(double xshift, double yshift, bool repaintP){
  Position oldorigin = pose.getOrigin();
  pose = Pose(Pose(xshift,yshift),pose);  // move in parent's frame

  if(penIsDown){
    Line l(oldorigin.x, oldorigin.y, 
	 oldorigin.x+xshift, oldorigin.y+yshift);
    //l.setColor(COLOR("green"));
    l.imprint(false);
  }
  if(repaintP) repaint();
}

void Sprite::setFill(bool v, bool repaintP){
  fill = v;
  if(repaintP) repaint();
}

void Sprite::setColor(Color c, bool repaintP){
  color = c;
  if(repaintP) repaint();
}


void Sprite::imprint(bool repaintP){
  c_imprint(this);
  if(repaintP) repaint();
}  

void Sprite::forward(double distance){
  Position image  = pose.unitX();
  double xshift = image.getX()*distance;
  double yshift = image.getY()*distance;
  move(xshift,yshift);
}

void Sprite::penDown(bool down){
  penIsDown = down;
}

void Sprite::penUp(bool up){
  penIsDown = !up;
}

void Sprite::face(double x, double y, bool repaintP){
  Position image  = pose.unitX();
  double angle = atan2(image.getY(), image.getX());
  Position origin = getOrigin();
  double target = atan2(y-origin.getY(), x-origin.getX());
  rotate((target - angle));
}
void Sprite::face(Sprite s){
  Position org = s.getOrigin();
  double x = org.getX();
  double y = org.getY();
  face(x,y);
}
