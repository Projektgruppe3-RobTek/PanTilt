import processing.serial.*;

Serial port;

final int border = 50;

final int CountBytes = 1;
final int TimeBytes = 3;

void setup(){
  port = new Serial(this, Serial.list()[0], 115200);
  size(400, 500);
  
}

Slider slider1 = new Slider(50, 50, -127, 128, 0, 100, 400, 20);
Slider slider2 = new Slider(250, 50, -127, 128, 0, 100, 400, 20);

void draw(){
  background(0);
  
  if (mousePressed){
    if (slider1.update()){
      port.write(0x00);
      port.write(slider1.getValue());
      port.write(0x01);
      port.write(0x00);
      port.write(0x01);
      port.write(0x00);
    }
    if (slider2.update()){
      port.write(0x10);
      port.write(slider2.getValue());
      port.write(0x11);
      port.write(0x00);
      port.write(0x11);
      port.write(0x00);
    }
  }
  
  slider1.draw();
  slider2.draw();
  
  //print("Slider 1: ");
  //print(slider1.getValue());
  //print("\tSlider 2: ");
  //println(slider2.getValue());
}

void mouseMoved(){
}

class Slider{
  int xPos;
  int yPos;
  
  int min;
  int max;
  
  int val;
  
  int width;
  int height;
  
  int barWidth;
  
  Slider(int _xPos, int _yPos, int _min, int _max, int _val, int _width, int _height, int _barWidth){
    xPos = _xPos;
    yPos = _yPos;
    
    min = _min;
    max = _max;
    
    val = _val;
    
    width = _width;
    height = _height;
    
    barWidth = _barWidth;
  }
  
  void draw(){
    fill(150);
    rect(xPos + width / 2 - barWidth / 2, yPos + barWidth, barWidth, height - 2 * barWidth);
    fill(255,0,0);
    rect(xPos, yPos, width, barWidth);
    fill(0,255,0);
    rect(xPos + width / 4, yPos + height / 2 - barWidth / 2, width / 2, barWidth);
    fill(0,0,255);
    rect(xPos, yPos + height - barWidth, width, barWidth);
    fill(255);
    rect(xPos, yPos + height / 2 - barWidth - val * (height / 2 - barWidth * 4 / 2) / 128, width, barWidth * 2);
  }
  
  boolean update(){
    if (mouseX > xPos && mouseX < xPos + width && mouseY > yPos && mouseY < yPos + height){
      val = - (mouseY - yPos - height / 2) * 128 / (height / 2 - barWidth * 4 / 2);
      if (val > 127) val = 127;
      else if (val < -128) val = -128;
      return true;
    }
    return false;
  }
  
  int getValue(){
    return val;
  }
};
