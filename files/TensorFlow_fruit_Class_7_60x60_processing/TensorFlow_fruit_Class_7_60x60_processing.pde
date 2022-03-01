
import processing.serial.*;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;

// fpr UI
import controlP5.*;
ControlP5 cp5;

Serial myPort;

// must match resolution used in the sketch
final int cameraWidth = 60;
final int cameraHeight = 60;
final int cameraBytesPerPixel = 2;
final int bytesPerFrame = cameraWidth * cameraHeight * cameraBytesPerPixel;

PImage myImage;
int sice =6;

//variables para los botones
boolean ingray = false;
boolean tf = false;

//para leler datos seriales  de arduino 
String datas;
float[] datai;
//boolean log = true;
float apple;
float orange;

PFont f;
boolean isOpen;

PImage i_apple;
PImage i_orange;

void setup()
{
  size(500, 750);
  background(51);
  smooth();
  i_apple = loadImage("apple.png");
  i_orange = loadImage("orange.png");
  cp5 = new ControlP5(this);

  String portName = Serial.list()[0];
  // if you know the serial port name
  myPort = new Serial(this, portName , 115200);                    // Windows

  myPort.clear();
  // wait for full frame of bytes
  

  myImage = createImage(cameraWidth, cameraHeight, RGB);
    
  // create some controllers
  cp5.addButton("button")
     .setValue(10)
     .setPosition(80,360)
     .setSize(360,120)
     //.setColorBackground(color(0, 50, 100))
     //.setColorLabel(color(255))
     .setColorActive(color(0,150,250))
     .setId(1); 
     
  PFont pfont = createFont("COOPBL.TTF",20,true); // use true/false for smooth/no-smooth
  ControlFont font = new ControlFont(pfont,241);
    // (3)
  // change the font and content of the captionlabels 
  // for both buttons create earlier.
  cp5.getController("button")
     .getCaptionLabel()
     .setFont(font)
     .toUpperCase(false)
     .setSize(24)
     ;
   
  f = createFont("COOPBL.TTF", 24);
  textFont(f);

}

void draw()
{
  background(51);
  noStroke();
  image(myImage, 0, 0);
    
  for (int h = 0; h < cameraWidth; h++) {
    for (int w = 0; w < cameraHeight; w++) {
    //img.pixels[i] = color(0, 90, 102, i % img.width * 2); 
      color c = get(h, w);
      //println(c);
      fill(c);
      //fill(153);
      square((h*sice)+80, w*sice, sice);
    }
  }
  
  if(orange>0 || apple > 0){
    fill(51);
    rect(150, 630, 220, 80);
    if(orange>apple){
      image(i_orange, 200, 500);
      fill(255);
      text("Orange: "+ nf(orange,0,1) +" %", 180, 650);
      fill(200);
      text("Apple : "+ nf(apple,0,1) +" %", 180, 680);
    }else{
      image(i_apple, 200, 500);
      fill(200);
      text("Orange: "+ nf(orange,0,1) +" %", 180, 650);
      fill(255);
      text("Apple : "+ nf(apple,0,1) +" %", 180, 680);
    }
  }
}


public void button() {
  //println("a button event. "+theValue);
  isOpen = !isOpen;
  cp5.getController("button").setCaptionLabel((isOpen==true) ? "Getting a picture":"Inference");
  if(!isOpen){
    cp5.getController("button").setColorBackground(color(0, 50, 100));
    myPort.buffer(bytesPerFrame);  
    ingray = true;
    myPort.clear();
    myPort.write('c');
  }else{
    cp5.getController("button").setColorBackground(color(0, 60, 100));
    myPort.bufferUntil('\n');
    tf = true;
    myPort.clear();
    myPort.write('i');
  }
}

//public void TENSOR_FLOW() {
//  println("Doing an inference ");
//  // wait for full frame of bytes
//  //myPort.buffer(bytesPerFrame); 
//  myPort.bufferUntil('\n');
//  tf = true;
//  myPort.clear();
//  myPort.write('i');  
//}

//public void TAKE_PICTURE() {
//  println("Taking a picture ");
//  myPort.buffer(bytesPerFrame); 
//  // wait for full frame of bytes
//  //myPort.buffer(bytesPerFrame);  
//  ingray = true;
//  myPort.clear();
//  myPort.write('c');  
//}

void serialEvent(Serial myPort) {
  
  Serial puerto = myPort;
  if (ingray){
    imagen(puerto);
  } 
  if (tf){
    TensorFlow(puerto);
  }
}

void TensorFlow(Serial myPort){
  datas = trim(myPort.readStringUntil('\n'));
  if (datas != null) {
    //if ( log ) println(datas); 
    fill(255);
    datai = float( split(datas, ",") ); 
    if ( datai.length >= 2  ) { //&& log
       for ( int i=0; i < datai.length; i++ ){
         if(i==0){
           print("Naranja : " + datai[i]+"%   "); 
           orange =  datai[i];
         }else{
           print("Manzana : " + datai[i]+"%");
           apple = datai[i];          
         }
       }
       println();
    } //else println("shortline");
  }
  tf = false;
  
}
void imagen(Serial myPort){
  byte[] frameBuffer = new byte[bytesPerFrame];

  // read the saw bytes in
  myPort.readBytes(frameBuffer);

  // create image to set byte values
  PImage img = createImage(cameraWidth, cameraHeight, RGB);

  // access raw bytes via byte buffer
  ByteBuffer bb = ByteBuffer.wrap(frameBuffer);
  bb.order(ByteOrder.BIG_ENDIAN);

  int i = 0;

  img.loadPixels();
  while (bb.hasRemaining()) {
    // read 16-bit pixel
    short p = bb.getShort();

    // convert RGB565 to RGB 24-bit
    int r = ((p >> 11) & 0x1f) << 3; // 0x1f = b0001 1111
    int g = ((p >> 5) & 0x3f) << 2;  // 0x3f = b0011 1111
    int b = ((p >> 0) & 0x1f) << 3;  // 0x1f = b0001 1111
    //int a = ((p >> 8) & 0xFF);  // solo para YUV 0xFF = b1111 1111

    // set pixel color
    img.pixels[i++] = color(r, g, b);
    //img.pixels[i++] = color(a);
  }
  img.updatePixels();

  // assign image for next draw
  myImage = img;
  ingray = false;
  //myPort.clear();
}
