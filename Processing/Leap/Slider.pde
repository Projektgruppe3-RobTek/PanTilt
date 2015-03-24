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
