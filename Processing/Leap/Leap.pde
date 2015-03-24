// Import serial library
import processing.serial.*;
// Initialize serial port
Serial port = new Serial(this, Serial.list()[0], 115200);

// Import Leap Motion labrary
import de.voidplus.leapmotion.*;
// Initialize leap motion
//LeapMotion leap = new LeapMotion(this);
LeapMotion leap = new LeapMotion(this).withGestures();

// Initialize motors
Motor motor0 = new Motor(byte(0x00), 150, 250, 100);

void setup(){
  // Graphic setup
  size(500, 500);
  background(0);
}

void draw(){
  background(0);
  
  // Update motor
  if (mousePressed){
    motor0.update();
    motor0.write();
  }
  // Draw Leap
  
  // Draw motor
  motor0.draw();
  
  // Print details
  println(motor0.getValue());
}

void mousePressed(){
  
}

void keyPressed(){
  
}

void serialEvent(Serial port){
  
}

void leapOnCircleGesture(CircleGesture g, int state){
  
  int     id               = g.getId();
  Finger  finger           = g.getFinger();
  PVector position_center  = g.getCenter();
  float   radius           = g.getRadius();
  float   progress         = g.getProgress();
  long    duration         = g.getDuration();
  float   duration_seconds = g.getDurationInSeconds();
  int     direction        = g.getDirection();
  
  switch(state){
    case 1:  // Start
    
      break;
    case 2: // Update
      switch(direction){
        case 0: // Anticlockwise/Left gesture
          port.write(0x00);
          port.write(0x7f);
          port.write(0x01);
          port.write(0x00);
          
          break;
        case 1: // Clockwise/Right gesture
          port.write(0x00);
          port.write(0x80);
          port.write(0x01);
          port.write(0x00);
          break;
      }
      break;
    case 3: // Stop
        port.write(0x00);
        port.write(0x00);
        port.write(0x01);
        port.write(0x00);
      break;
  }
}
