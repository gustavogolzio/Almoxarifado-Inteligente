#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <dht.h>

const char* ssid = "BAJAMPA TOP1";
const char* pass = "bajampa2018";

HTTPClient http;

const String LOCATION = "http://almoxautomated.herokuapp.com/";

const String UPLOAD = "upload";
const String DOWNLOAD = "download";


dht DHT;

#define pinPIR D1
#define pinTEMP D2
#define pinLED01 D3
#define pinLED02 D5

const String PIR = "\"PIR\":";
const String TEMP = "\"TEMP\":";
const String LED01 = "\"LED01\":";
const String LED02 = "\"LED02\":";

void setup() {
  
pinMode(pinPIR,INPUT);
pinMode(pinLED01,OUTPUT);
pinMode(pinLED02,OUTPUT);

digitalWrite(pinLED01, LOW);

Serial.begin(115200);

Serial.print(DHT_LIB_VERSION);

Serial.print("Wifi conectando à");
Serial.println(ssid);

WiFi.begin(ssid, pass);    //WiFi connection  

Serial.println();
Serial.print("Conectando");

while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.println(" Aguardando conexão");
}

Serial.println();
Serial.println("Wifi conectado com sucesso");
Serial.println("NodeMCU IP Adress: ");
Serial.print(WiFi.localIP());

}



void loop() {

  int pir_value = digitalRead(pinPIR);
  int temp_value = DHT.temperature;
  int ok = DHT.read11(pinTEMP);

  if(ok==DHTLIB_OK){

   if(Serial.available() > 0){
    Serial.println("Serial Disponível");
    String comando = Serial.readString();
    atualizarLeds(comando);
    Serial.print(JSON_LEDS());
    delay(10);
    uploadLEDS();
    }

    atualizarLeds(downloadWEB());
    uploadPIR();     
    delay(10);
    uploadTEMP();
    delay(10);
    uploadLEDS();
  
    if(digitalRead(pinLED01)==1){
      delay(5000);
      digitalWrite(pinLED01,LOW);
      uploadLEDS();
    }
    
    
    Serial.print(JSON_TEMP());
    delay(10);
    Serial.print(JSON_LEDS());
    delay(10);    
    Serial.println(JSON_PIR());
    delay(1000);  
  }else{
    Serial.println("Não entrou no loop");
  }
}

void uploadPIR(){
  http.begin(LOCATION+UPLOAD);       
  http.addHeader("Content-Type", "application/json");      
  http.POST(JSON_PIR());      
  http.end();    
}

void uploadTEMP(){      
  http.begin(LOCATION+UPLOAD);       
  http.addHeader("Content-Type", "application/json");       
  http.POST(JSON_TEMP());       
  http.end();    
}
void uploadLEDS(){      
  http.begin(LOCATION+UPLOAD);       
  http.addHeader("Content-Type", "application/json");       
  http.POST(JSON_LEDS());       
  http.end();    
}


String downloadWEB(){       
  http.begin(LOCATION+DOWNLOAD);       
  http.GET();
  String payload = http.getString();     
  http.end();
  return payload;
}

String JSON_PIR(){
  int pir_value = digitalRead(pinPIR);
 return "{" +
 PIR + String(pir_value) +
 "}";
 }

 String JSON_TEMP(){
 int temp_value = DHT.temperature;
 return "{" +
 TEMP + String(temp_value) +
 "}";                                                                   
 }

String JSON_LEDS(){
  return "{" +
              LED01 + String(digitalRead(pinLED01)) + "," + 
              LED02 + String(digitalRead(pinLED02))  + 
         "}";
}
 
 void atualizarLeds(String comando){
    if(comando.startsWith("{") && comando.endsWith("}")){
      if(comando.indexOf(LED01 +" 1") > 0){
        digitalWrite(pinLED01,HIGH);
        }
       if(comando.indexOf(LED02 + " 1") > 0){
        digitalWrite(pinLED02, HIGH);
        }else if(comando.indexOf(LED02 +" 0") > 0){
          digitalWrite(pinLED02, LOW);
          } 
        }
     }
