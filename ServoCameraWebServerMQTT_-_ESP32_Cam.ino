#include "esp_camera.h"
#include <WiFi.h>
#include <PubSubClient.h>
#include <ESP32Servo.h>  

// Select camera model
#define CAMERA_MODEL_AI_THINKER

#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

#define RED_LED  33  //RED indicator LED.
#define FLASH_LED 4  //FLASH LED

static const int panPin = 2;
static const int tiltPin = 12;

const char* ssid = "Amaralless AP";
const char* password = "1amaral1";
const char* mqtt_server = "mqtt.internetecoisas.com.br"; //Utilize o broker de sua preferência


WiFiClient espClient;
PubSubClient client(espClient);
Servo panServo;
Servo tiltServo;

void startCameraServer();
void setup_wifi();
void callback(char* topic, byte* payload, unsigned int length);
void reconnect();
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
  digitalWrite(RED_LED, LOW);  //sinaliza que está conectado ao wifi
}



void callback(char* topic, byte* payload, unsigned int length) {
  String inString="";
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  
  for (int i = 0; i < length; i++) {
    //Serial.print((char)payload[i]);
    inString+=(char)payload[i];
  }
  if(String((char*) topic) == "MackMQTT-Emerson"){
    Serial.print(inString);

    if(inString=="ON"){
      client.publish("MackMQTT-Emerson", "Está um pouco escuro aqui, ligando a luz...");
      digitalWrite(FLASH_LED, HIGH);
    }

    if(inString=="OFF"){
      client.publish("MackMQTT-Emerson", "Está claro demais aqui, desligando a luz...");
      digitalWrite(FLASH_LED, LOW);
    }

    if(inString=="DIREITA") {
      client.publish("MackMQTT-Emerson", "Câmerda posicionada para a direita");
      panServo.write(135);
    }

    if(inString=="ESQUERDA") {
      client.publish("MackMQTT-Emerson", "Câmerda posicionada para a esquerda");
      panServo.write(45);
    }

    if(inString=="CIMA") {
      client.publish("MackMQTT-Emerson", "Câmerda posicionada para cima");
      tiltServo.write(45);
    }

    if(inString=="BAIXO") {
      client.publish("MackMQTT-Emerson", "Câmerda posicionada para baixo");
      tiltServo.write(180);
    }    

    if(inString=="CENTRO") {
      client.publish("MackMQTT-Emerson", "Câmerda posicionada para o centro");
      tiltServo.write(135);
      panServo.write(90);
    }    
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP32CamClient-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    const char* willTopic = "MackMQTT-Emerson";
    const char* willMessage = "Web Server desconectado!";
    int willQoS = 1;
    boolean willRetain = true;
    if (client.connect(clientId.c_str(),"72aa269b@228","dc3a7b397429", willTopic, willQoS, willRetain, willMessage, true)){
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("MackMQTT-Emerson", "ESP32 Conectado");
      client.publish("MackMQTT-Emerson", "Camera Ready!");
      
      // ... and resubscribe
      client.subscribe("MackMQTT-Emerson");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();


  pinMode(FLASH_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  digitalWrite(RED_LED, HIGH);
  digitalWrite(FLASH_LED, LOW);

  panServo.attach(panPin);
  tiltServo.attach(tiltPin);
  
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  //init with high specs to pre-allocate larger buffers
  if(psramFound()){
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }


  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  sensor_t * s = esp_camera_sensor_get();
  //initial sensors are flipped vertically and colors are a bit saturated
  if (s->id.PID == OV3660_PID) {
    s->set_vflip(s, 1);//flip it back
    s->set_brightness(s, 1);//up the blightness just a bit
    s->set_saturation(s, -2);//lower the saturation
  }
  //drop down frame size for higher initial frame rate
  s->set_framesize(s, FRAMESIZE_QVGA);


  setup_wifi();
  startCameraServer();

  Serial.print("Camera Ready! Use 'http://");
  Serial.print(WiFi.localIP());
  Serial.println("' to connect");


  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

}

void loop(){
  // put your main code here, to run repeatedly:

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  
  if(Serial.available()){
    if(Serial.read()=='1') digitalWrite(FLASH_LED, HIGH);
    else   digitalWrite(FLASH_LED, LOW);
  }
  delay(1000);  
}
