#define ENA 16  // Enable/speed motors Right        GPIO16(D0)
#define ENB 14  // Enable/speed motors Left         GPIO15(D6)
#define IN_1 5  // L298N in1 motors Right           GPIO15(D8)
#define IN_2 4  // L298N in2 motors Right           GPIO13(D7)
#define IN_3 0  // L298N in3 motors Left            GPIO2(D4)
#define IN_4 2  // L298N in4 motors Left            GPIO0(D3)
const unsigned char Passive_buzzer = 12;
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <ESP8266WebServer.h>
SoftwareSerial GPS_SoftSerial(3, 1); /* (Rx, Tx) */
/* Create an object named gps of the class TinyGPSPlus */
TinyGPSPlus gps;
String command;       //String to store app command state.
int speedCar = 1000;  // 400 - 1023.
int speed_Coeff = 4;
volatile float minutes, seconds;
volatile int degree, secs, mins;
const char* ssid = "DARKMONSTER";
const char* pass = "12345678";
ESP8266WebServer server(80);

void setup() {
  //pinMode (Passive_buzzer,OUTPUT) ;
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN_1, OUTPUT);
  pinMode(IN_2, OUTPUT);
  pinMode(IN_3, OUTPUT);
  pinMode(IN_4, OUTPUT);
  Serial.begin(9600);
  GPS_SoftSerial.begin(9600);


  // Connecting WiFi

  // Connect to WiFi
  WiFi.begin(ssid, pass);

  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");

  // Print local IP address
  Serial.print("Local IP address: ");
  Serial.println(WiFi.localIP());

  // Starting Web Server
  server.on("/", HTTP_handleRoot);
  server.onNotFound(HTTP_handleRoot);
  server.begin();
}

void goAhead() {

  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, HIGH);
  analogWrite(ENA, speedCar);

  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, HIGH);
  analogWrite(ENB, speedCar);
}

void goBack() {

  digitalWrite(IN_1, HIGH);
  digitalWrite(IN_2, LOW);
  analogWrite(ENA, speedCar);

  digitalWrite(IN_3, HIGH);
  digitalWrite(IN_4, LOW);
  analogWrite(ENB, speedCar);
}

void goRight() {

  digitalWrite(IN_1, HIGH);
  digitalWrite(IN_2, LOW);
  analogWrite(ENA, speedCar);

  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, HIGH);
  analogWrite(ENB, speedCar);
}

void goLeft() {

  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, HIGH);
  analogWrite(ENA, speedCar);

  digitalWrite(IN_3, HIGH);
  digitalWrite(IN_4, LOW);
  analogWrite(ENB, speedCar);
}

void goAheadRight() {

  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, HIGH);
  analogWrite(ENA, speedCar / speed_Coeff);

  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, HIGH);
  analogWrite(ENB, speedCar);
}

void goAheadLeft() {

  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, HIGH);
  analogWrite(ENA, speedCar);

  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, HIGH);
  analogWrite(ENB, speedCar / speed_Coeff);
}

void goBackRight() {

  digitalWrite(IN_1, HIGH);
  digitalWrite(IN_2, LOW);
  analogWrite(ENA, speedCar / speed_Coeff);

  digitalWrite(IN_3, HIGH);
  digitalWrite(IN_4, LOW);
  analogWrite(ENB, speedCar);
}

void goBackLeft() {

  digitalWrite(IN_1, HIGH);
  digitalWrite(IN_2, LOW);
  analogWrite(ENA, speedCar);

  digitalWrite(IN_3, HIGH);
  digitalWrite(IN_4, LOW);
  analogWrite(ENB, speedCar / speed_Coeff);
}

void stopRobot() {

  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, LOW);
  analogWrite(ENA, speedCar);

  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, LOW);
  analogWrite(ENB, speedCar);
}
void buzzer() {
  tone(Passive_buzzer, 523);  //DO note 523 Hz
  delay(50);
  tone(Passive_buzzer, 587);  //RE note ...
  delay(50);
  tone(Passive_buzzer, 659);  //MI note ...
  delay(50);
  tone(Passive_buzzer, 783);  //FA note ...
  delay(50);
  tone(Passive_buzzer, 880);  //SOL note ...
  delay(50);
  tone(Passive_buzzer, 987);  //LA note ...
  delay(50);
  tone(Passive_buzzer, 1046);  // SI note ...
  delay(50);
  noTone(Passive_buzzer);
}


void loop() {
  server.handleClient();
  gps1();
  command = server.arg("State");
  if (command == "F") goAhead();
  else if (command == "B") goBack();
  else if (command == "L") goRight();
  else if (command == "R") goLeft();
  else if (command == "I") goAheadRight();
  else if (command == "G") goAheadLeft();
  else if (command == "J") goBackRight();
  else if (command == "V") buzzer();
  else if (command == "H") goBackLeft();
  else if (command == "0") speedCar = 100;
  else if (command == "1") speedCar = 200;
  else if (command == "2") speedCar = 300;
  else if (command == "3") speedCar = 400;
  else if (command == "4") speedCar = 500;
  else if (command == "5") speedCar = 600;
  else if (command == "6") speedCar = 700;
  else if (command == "7") speedCar = 800;
  else if (command == "8") speedCar = 900;
  else if (command == "9") speedCar = 1000;
  else if (command == "S") stopRobot();
}

void HTTP_handleRoot(void) {

  if (server.hasArg("State")) {
    Serial.println(server.arg("State"));
  }
  server.send(200, "text/html", getGpsLocationString());
  delay(1);
}

void gps1() {
  getGpsLocationString();
}

String getGpsLocationString() {
    while (Serial.available()) {
      gps.encode(Serial.read());  // Parse GPS data continuously
    }

    if (gps.location.isValid()) {
      String latitude = String(gps.location.lat(), 6);  // Format latitude with 6 decimal places
      String longitude = String(gps.location.lng(), 6);
      String satellites = String(gps.satellites.value());
      String hdop = String(gps.hdop.hdop(), 1);
      String age = String(gps.location.age(), 2);
      String altitude = String(gps.altitude.meters());
      String deg = String(gps.course.deg());
      String speed = String(gps.speed.kmph(), 2);
      Serial.println(latitude);
      Serial.println(longitude);
      return ValueToJSON(latitude, longitude, satellites, hdop, age, altitude, deg, speed);  // Combine latitude and longitude into a string
    } else {
      return "No GPS fix";  // Return a message if no valid GPS fix is available
    }
  }
//latitude11.203484,longitude 77.344700,satellites 20,hdop 0.9,age 4,altitude 320.80000,deg 14.000,speed 0.2963
String ValueToJSON(String lat, String log, String sat, String hdop, String age, String alt, String deg, String speed) {
  // Access the values from the array
  float humidity = 97;//TandH[0];
  float temperature = 87;//TandH[1];
  float fahrenheit =78;//TandH[2];
  int dis = 0;//filtervalue();
  String wiinfo = "hggh";
    String r = "{\"latitude\": " + String(lat) + ", ";
  r += "\"longitude\": " + String(log) + ", ";
  r += "\"satellites\": " + String(sat) + ", ";
  r += "\"hdop\": " + String(hdop) + ", ";
  r += "\"age\": " + String(age) + ", ";
  r += "\"altitude\": " + String(alt) + ", ";
  r += "\"deg\": " + String(deg) + ", ";
  r += "\"speed\": " + String(speed) + ",";
  r += "\"humidity\": " + String(humidity) + ",";
  r += "\"temperature\": " + String(temperature) + ",";
  r += "\"fahrenheit\": " + String(fahrenheit) + ",";
  r += "\"wifi\": " + wiinfo + ",";
   r += "\"distance\": " + String(dis) + "}";
  return r;
}
