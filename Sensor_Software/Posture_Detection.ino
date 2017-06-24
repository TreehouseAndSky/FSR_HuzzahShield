/*
 * Code to run on the ESP8266 posture detection
 * 
 * Requirements
 * 
 * Setup network (Wifi & MQTT)
 * 
 * Switch ROW AMPLIFIERS
 * 
 *    A   - GPIO13
 *    B   - GPIO12
 *    INH - GPIO14
 *    
 *    B   A   INH   OUT
 *    0   0   0     ROW1
 *    0   1   0     ROW2
 *    1   0   0     ROW3
 *    1   1   0     ROW4
 *    X   X   1     NONE
 *-----------------------------------------    
 *
 * Switch ADC MUX
 * 
 *    A   - GPIO15
 *    B   - GPIO2
 *    INH - GPIO0
 *    
 *    B   A   INH   OUT
 *    0   0   0     COLUMN1
 *    0   1   0     COLUMN2
 *    1   0   0     COLUMN3
 *    1   1   0     COLUMN4
 *    X   X   1     NONE
 *-----------------------------------------
 *    
 * Read ADC value
 * 
 * Analyze/(Running) Average result
 * 
 * Loop all sensor points
 * 
 * Transmit
 * 
 * Sleep
 * 
 * 
 */

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#define DEBUG 1

// Network specific values

const char* ssid = "Telenet_L&K";
const char* password = "Kikikiki01";
const char* mqtt_server = "192.168.0.177";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
int value = 0;
char* outTopic = "dev/test";
char *inTopic = "inTopic";

const int ROW_APin = 13;
const int ROW_BPin = 12;
const int ROW_INHPin = 14;
const int COL_APin = 15;
const int COL_BPin = 2;
const int COL_INHPin = 0;

int matrix[3][3];

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}



/*
 * Callback function for the MQTT library, this function
 * gets called when a message is published in a topic we subscribed to
 */
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is acive low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }

}
/*
 * Wifi connection function
 */
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(outTopic, "hello world");
      // ... and resubscribe
      client.subscribe(inTopic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

/*
 * Switch the row we're measuring on. This drives the row MUX (CD
 */
void switch_row(int row) {
  switch (row) {
    case 0 :
      digitalWrite(ROW_APin, LOW);
      digitalWrite(ROW_BPin, LOW);
      digitalWrite(ROW_INHPin, HIGH);
      break;
      
    case 1 :
      digitalWrite(ROW_APin, LOW);
      digitalWrite(ROW_BPin, LOW);
      digitalWrite(ROW_INHPin, LOW);
      break;
      
    case 2 :
      digitalWrite(ROW_APin, HIGH);
      digitalWrite(ROW_BPin, LOW);
      digitalWrite(ROW_INHPin, LOW);
      break;
      
    case 3 :
      digitalWrite(ROW_APin, LOW);
      digitalWrite(ROW_BPin, HIGH);
      digitalWrite(ROW_INHPin, LOW);
      break;
      
    case 4 :
      digitalWrite(ROW_APin, HIGH);
      digitalWrite(ROW_BPin, HIGH);
      digitalWrite(ROW_INHPin, LOW);
      break;

    default :
      digitalWrite(ROW_APin, LOW);
      digitalWrite(ROW_BPin, LOW);
      digitalWrite(ROW_INHPin, HIGH);
      break;
  }
  
}

void switch_column(int column) {
  switch (column) {
    case 0 :
      digitalWrite(COL_APin, LOW);
      digitalWrite(COL_BPin, LOW);
      digitalWrite(COL_INHPin, HIGH);
      break;
      
    case 1 :
      digitalWrite(COL_APin, LOW);
      digitalWrite(COL_BPin, LOW);
      digitalWrite(COL_INHPin, LOW);
      break;
      
    case 2 :
      digitalWrite(COL_APin, HIGH);
      digitalWrite(COL_BPin, LOW);
      digitalWrite(COL_INHPin, LOW);
      break;
      
    case 3 :
      digitalWrite(COL_APin, LOW);
      digitalWrite(COL_BPin, HIGH);
      digitalWrite(COL_INHPin, LOW);
      break;
      
    case 4 :
      digitalWrite(COL_APin, HIGH);
      digitalWrite(COL_BPin, HIGH);
      digitalWrite(COL_INHPin, LOW);
      break;

    default :
      digitalWrite(COL_APin, LOW);
      digitalWrite(COL_BPin, LOW);
      digitalWrite(COL_INHPin, HIGH);
      break;
  }
  
}

void readPoints() {
  int sensorvalue;
  
  for(int i=1; i<=4; i++) //total delay = 4.(4.(10+4.1)) ms = 64.4ms = 256ms
  {
    switch_row(i);
    for(int j=1; j<=4; j++)
    {
      switch_column(j);
      delay(10); //todo: test settling time;
      sensorvalue = 0;

      //Take an average of 5 analog reads
      for(int k=0; k<5; k++)
      {
        #ifdef DEBUG
        sensorvalue += random(0,1025);
        #else
        sensorvalue += analogRead(A0);
        #endif;
        delay(1);
      };
      
      sensorvalue = sensorvalue/5;
      matrix[i][j] = sensorvalue;
      Serial.print("Element ");
      Serial.print(i);
      Serial.print("x");
      Serial.print(j);
      Serial.print(": ");
      Serial.println(sensorvalue);
    };
  };
  Serial.println("Readout complete");
}

void sendPoints(){
  Serial.println("Sending pressure matrix");
  
  // Prepare a JSON payload string
  String payload = "{";
  payload += "\"pressure\": [";
  for(int i=1; i<=4; i++){
      payload += "{\"row\":";
      payload += String(i);
      payload += ",\"column\":[";
      for(int j=1; j<=4; j++){
        payload += String(matrix[i][j]);
        if(j != 4)
          payload += ",";
      };
      payload += "]}";
      if (i != 4)
        payload += ",";
  };
  payload += "]}";
  
  // Send payload
  char attributes[200];
  payload.toCharArray(attributes, 200);
  client.publish(outTopic, attributes);
  Serial.println(attributes);
}

void setup() {
  pinMode(ROW_APin, OUTPUT);
  pinMode(ROW_BPin, OUTPUT);
  pinMode(ROW_INHPin, OUTPUT);
  pinMode(COL_APin, OUTPUT);
  pinMode(COL_BPin, OUTPUT);
  pinMode(COL_INHPin, OUTPUT);
  switch_row(0);
  switch_column(0);
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  //turn off wifi
  readPoints();

  long now = millis();
  if (now - lastMsg > 5000) {
    //turn on wifi
    lastMsg = now;
    ++value;
    sendPoints();
  }
}
