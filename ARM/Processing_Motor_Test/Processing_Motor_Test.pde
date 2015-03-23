import processing.serial.*;

Serial port;

final int border = 50;

void setup(){
  port = new Serial(this, "/dev/ttyACM0", 115200);
  size(200, 500);
  
}

int speed = 127;

void draw(){
  background(0);
  
  drawSlider();
  
  println(127 - speed);
  
  port.write(00);
  port.write(127 - speed);
}

void drawSlider(){
  fill(255);
  rect(width / 2 - width / 20, border, width / 10, height - 2 * border);
  
  rect(border, border, width - 2 * border, width / 10);
  rect(border / 2, height / 2 - width / 20, width - 2 * border + border, width / 10);
  rect(border, height - border - width / 10, width - 2 * border, width / 10);
  
  rect(border, speed * (height - border * 2 - width / 10 * 2 - (width - 2 * border) / 2) / 256 + border + width / 10, width - 2 * border, (width - 2 * border) / 2);
}

void mouseMoved(){
  speed = (mouseY - border - width / 10 - (width - 2 * border) / 2 / 2) * 256 / (height - border * 2 - width / 10 * 2 - (width - 2 * border) / 2);
  if (speed > 255) speed = 255;
  else if (speed < 0) speed = 0;
}

void keyPressed(){
  if (key == CODED){
    if (keyCode == UP){
    println("Key");
      speed -= 8;
    }
    else if (keyCode == DOWN){
      if (speed == 0) speed = 7;
      else speed += 8;
    }
    if (speed > 255) speed = 255;
    else if (speed < 0) speed = 0;
  }
}
