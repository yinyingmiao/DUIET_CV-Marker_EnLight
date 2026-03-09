//*********************************************
// Example Code: ArUCo Fiducial Marker Detection in OpenCV Python and then send to Processing via OSC
// Rong-Hao Liang: r.liang@tue.nl

// V1 code completed on December 19th, 2023, for individual CV marker Project DUIET.
// Integrated by Yinying Miao for the CV Marker detection, tracking, and processing.
// Supported by Olaf Adan with suggestions and small sample codes.

// V2 integration made on Januarary 14th, 2024, by Yinying Miao for Group Project DUIET.
// Reference for processing port write https://processing.org/reference/libraries/serial/Serial_write_.html
// Reference for sending an array of values: https://gist.github.com/atduskgreg/1349176
// Processing creating table: https://processing.org/reference/saveTable_.html
// Improved version with time delays for sending the data.

// V3 integration made on January 23rd, 2024, by Yinying Miao for Group Project DUIET.


//*********************************************

import org.ejml.simple.SimpleMatrix;
import oscP5.*;
import netP5.*;
import processing.net.*;

//Serial data
import processing.serial.*;
Serial port;

TagManager tm;
OscP5 oscP5;
boolean serialDebug = true;

int[] cornersID = {1, 3, 2, 0};
int[][] bundlesIDs = {{57}, {49}, {55}, {27}};
PVector[][] bundlesOffsets = { {new PVector(0, 0, 0)}, {new PVector(0, 0, 0)}, {new PVector(0, 0, 0)}, {new PVector(0, 0, 0)}};

int camWidth = 1280; //checked 21-01-2024
int camHeight = 720;

float touchThreshold = 0.025; //unit: m

float paperWidthOnScreen = 490; //unit: mm // not sure about this line - ASK OLAF
float markerWidth = 20; //unit: mm

//ASK OLAF - not sure about these two following lines: 
float calibgridWidth = 199; //unit:mm
float calibgridHeight = 197; //unit:mm

PImage calibImg;

ArrayList<DataObject> DOlist = new ArrayList<DataObject>();

ArrayList<Tag> activeTagList = new ArrayList<Tag>();

int tagsAmountSec2; //section 2 amount of tags
float temp_yloc;
float hum_yloc;
float noi_rxloc;
int tagsAmountSec4;

//Saving the data into a .csv file:
Table table;

//Updating the data Frequency:
int delayTime = 10000; // the delay between 2 times sending the data from Processing to Arduino
long updateTime;

void setup() {

  size(1280, 720);
  oscP5 = new OscP5(this, 9000);
  initTagManager();
  
  //added these two lines of code (21-01-2024):
  calibImg = loadImage("ArUco_Grid20.png");
  imageOffset.set((width - calibImg.width)/2 , (height - calibImg.height)/2);

  String[] portList = Serial.list();

  if (portList.length > 0) {

    // Use the last available port in the list (modify this logic if needed)
    for (int i = 0; i < Serial.list().length; i++) println("[", i, "]:", Serial.list()[i]);

    String portName = portList[3];

    // Print the selected port to the console for verification
    println("Using serial port:", portName);

    port = new Serial(this, portName, 9600);
    port.bufferUntil('\n'); // Arduino ends each data packet with a carriage return
    port.clear();
  }

  table = new Table ();
  table.addColumn ("id");
  table.addColumn ("Temperature");
  table.addColumn ("Humidity");
  table.addColumn ("Light Intensity");
  table.addColumn ("Noise");
  table.addColumn ("Air Quality");

  saveTable (table, "/Users/michelle_myy__/Downloads/DUIET_Lab2/Processing/data/DUIET_final.csv");
}

void draw() {
  tm.update();
  background(200);
  tm.displayRaw();
  showInfo("Unit: cm", 0, height);
  
  while(port.available() > 0){
    String inBuffer = port.readString();
    if (inBuffer != null){
      println("data here ");
      println(inBuffer);
    }
  }

  //int tagsAmount = 0;

  //add this to your 'void draw' -> aka main loop
  for (Tag t : tm.tags) {                                     // look into the tags seen by TagManager
    if (t.active) {                                     // if a tag is active then
      //println("tag with id " + t.id +  " is active");      // print the id of this tag

      if (t.id >= 27 & t.id <= 46) {        //do something with these tags only
        tagsAmountSec2 ++;
      }

      if (t.id == 52) { //temperature bar
        //print ("Tag 52 - temperature bar locates on the x-axis at: ");
        //println (t.tx);
        temp_yloc = t.tx * 1000;
      }

      if (t.id == 53) { //humidity bar
        //print ("Tag 53 - humidity bar locates on the x-axis at: ");
        //println (t.tx);
        hum_yloc = t.tx * 1000;
      }

      if (t.id == 81) { // IRIS turning for noise
        //print ("Tag 81 - noise level is at the angle of: ");
        //println (t.rx);
        noi_rxloc = t.rx;
      }

      if (t.id >= 60 & t.id <= 64) { // air quality ball
        tagsAmountSec4 ++;
      }  
    }
  }

  //print ("the number of tags being detected in section 02: ");
  //println (tagsAmountSec2);
  //print ("the number of tags in section 04: ");
  //println (tagsAmountSec4);

  processdata();
  
  if (tagsAmountSec2 > 0) {

    tagsAmountSec2 = 0;
  }

  if (tagsAmountSec4 > 0) {

    tagsAmountSec4 = 0;
  }



  delay(1000);
}


void processdata() {
  
    if (millis() > updateTime + delayTime) {

    println("time to save data");

    //process the data to port going to Arduino:
    String dataString = tagsAmountSec2 + "," + temp_yloc + "," + hum_yloc + "," + noi_rxloc + "," + tagsAmountSec4 + "\n";
    port.write(dataString);

    println ("check mark #1, data sent!");


    TableRow newRow = table.addRow();
    newRow.setInt ("id", table.getRowCount() - 1);
    newRow.setFloat ("Temperature", temp_yloc);
    newRow.setFloat ("Humidity", hum_yloc);
    newRow.setInt ("Light Intensity", tagsAmountSec2);
    newRow.setFloat ("Noise", noi_rxloc);
    newRow.setInt ("Air Quality", tagsAmountSec4);
    saveTable (table, "/Users/michelle_myy__/Downloads/DUIET_Lab2/Processing/data/DUIET_final.csv");

    println ("check mark #2, data saved!");


    updateTime = millis();
  }
}  

void drawCalibImage() {
  pushStyle();
  imageMode(CENTER);
  image(calibImg, width/2, height/2, (float)calibImg.width*tag2screenRatio, (float)calibImg.height*tag2screenRatio);
  popStyle();
}

void drawCanvas() {
  pushStyle();
  noStroke();
  fill(255);
  rectMode(CENTER);
  rect(width/2, height/2, (float)calibImg.width*tag2screenRatio, (float)calibImg.height*tag2screenRatio);
  popStyle();
}

void showInfo(String s, int x, int y) {
  pushStyle();
  fill(52);
  textAlign(LEFT, BOTTOM);
  textSize(48);
  text(s, x, y);
  popStyle();
}
