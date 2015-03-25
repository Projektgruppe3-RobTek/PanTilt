class Motor{
  
  Slider slider;
  
  byte id;
  
  int xPos;
  int yPos;
  
  int r;
  
  int pos;
  
  Motor(byte _id, int _xPos, int _yPos, int _r){
    slider = new Slider(_xPos + _r * 2, _yPos - _r, -127, 128, 0, _r / 2, _r * 2, _r / 10);
    
    id = _id;
    
    xPos = _xPos;
    yPos = _yPos;
    
    r = _r;
    
    pos = 0;
  }
  
  void setPos(int _pos){
    pos = _pos;
  }
  
  int getPos(){
    return pos;
  }
  
  void addPos(int _pos){
    pos += _pos;
  }
  
  boolean update(){
    return slider.update();
  }
  
  int getValue(){
    return slider.getValue();
  }
  
  void write(byte speed){
    port.write(0x00);
    port.write(speed);
    port.write(0x01);
    port.write(0x00);
    
    while (port.available() >= 2){
      addPos(byte(port.read()));
      port.read();
    }
  }
  void write(){
    write(byte(slider.getValue()));
  }
  
  void draw(){
    stroke(255);
    fill(0);
    ellipse(xPos, yPos, r * 2, r * 2);
    fill(255);
    ellipse(xPos + r * sin(pos * (TWO_PI / 90.)), yPos + r * cos(pos * (TWO_PI / 90.)), r / 5, r / 5);
    line(xPos, yPos, xPos, yPos);
    stroke(0);
    slider.draw();
  }
};
