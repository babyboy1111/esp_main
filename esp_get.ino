#ifdef ESP32
  #include <WiFi.h>
  #include <HTTPClient.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESP8266HTTPClient.h>
  #include <WiFiClient.h>
#endif
#include <Wire.h>
#include "ESP32_MailClient.h"
// REPLACE WITH YOUR NETWORK CREDENTIALS

#define emailSenderAccount    "ghuyniot@gmail.com"    
#define emailSenderPassword   "weekndcold1"
#define emailRecipient        "nguyenkhaikh321@gmail.com"
#define smtpServer            "smtp.gmail.com"
#define smtpServerPort        465
#define emailSubject          "NodeWifi32 Test"
#define RXD2 16
#define TXD2 17
#define LED 2
// The Email Sending data object contains config and data to send
SMTPData smtpData;
// Callback function to get the Email sending status

//Access point credentials
const char* ssid = "Sinh Vien Home";
const char* password = "06117370";
const char* host = "http://dighuyniot.tech";
String get_host = "http://dighuyniot.tech";
unsigned long startMillis;  //some global variables available anywhere in the program
unsigned long currentMillis;
const unsigned long period = 3000000; 
int check=0;
char buff[10];
char str1='1';
char str0='0';
char str3='3';
char str2='2';
char str5='5';
char str4='4';
char str7='7';
char str6='6';
volatile byte indx;
const char* serverName = "http://dighuyniot.tech/demo/postdemo1.php";
String apiKeyValue = "tPmAT5Ab3j7F9";

WiFiServer server(80);  // open port 80 for server connection
void sendCallback(SendStatus info);
  void setup() {
    // Note the format for setting a serial port is as follows: Serial2.begin(baud-rate, protocol, RX pin, TX pin);
    Serial.begin(115200);
    //Serial1.begin(9600, SERIAL_8N1, RXD2, TXD2);
    Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);
    pinMode(LED,OUTPUT);
  WiFi.begin(ssid, password);
    //starting the server
    server.begin();
    startMillis = millis();
  }
boolean checkArrays(char arrayA[],char arrayB, long numItems) {

    boolean same = true;
    long i = 0;
    while(i<numItems ) { 
      same = arrayA[i] == arrayB;
      i++;
      if(same){return same;}
    }
    return same;
  }
  void post_btn(String value,String device){
  //Check WiFi connection status
    WiFiClient client;
    HTTPClient http;
    
    // Your Domain name with URL path or IP address with path
    http.begin(client, serverName);
    
    // Specify content-type header
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    
    // Prepare your HTTP POST request data
    String httpRequestData = "api_key=" + apiKeyValue + "&value1=" + value
                           + "&value2=" + device + "";
    Serial.print("httpRequestData: ");
    Serial.println(httpRequestData);
    
    // You can comment the httpRequestData variable above
    // then, use the httpRequestData variable below (for testing purposes without the BME280 sensor)
    //String httpRequestData = "api_key=tPmAT5Ab3j7F9&value1=24.75&value2=49.54&value3=1005.14";

    // Send HTTP POST request
    int httpResponseCode = http.POST(httpRequestData);
     
    // If you need an HTTP request with a content type: text/plain
    //http.addHeader("Content-Type", "text/plain");
    //int httpResponseCode = http.POST("Hello, World!");
    
    // If you need an HTTP request with a content type: application/json, use the following:
    //http.addHeader("Content-Type", "application/json");
    //int httpResponseCode = http.POST("{\"value1\":\"19\",\"value2\":\"67\",\"value3\":\"78\"}");
    
    if (httpResponseCode>0) {
      String respone=http.getString();
      Serial.println(respone);
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();
  
 //Send an HTTP POST request every 30 seconds
}
void get_send_mail(String value, String noidung)
  {
    
    WiFiClient client = server.available();
 
        HTTPClient http;
        String url = get_host+"/demo/mail/get_status.php?value="+value;
        
        http.begin(url);
        Serial.println(url);
        //GET method
        int httpCode = http.GET();
        String payload = http.getString();
        Serial.println("sssssssss");
        Serial.println(payload);
        payload.trim();
        int result=atoi(payload.c_str());
        
        if(value=="value1" && result>=100 && check==0)
          {
           send_mail(noidung);
           check=1;
          }
        else if(value=="value2" && result<=100 && check==0)
          {
           send_mail(noidung);
           check=1;
          }
        else if (value=="value3" && result>=100 && check==0)
          {
           send_mail(noidung);
           check=1;
          }
       
        http.end();
       
  
  }
  void send_mail(String noidung)
  {
    // Set the SMTP Server Email host, port, account and password
  smtpData.setLogin(smtpServer, smtpServerPort, emailSenderAccount, emailSenderPassword);
  // Set the sender name and Email
  smtpData.setSender("SmartGarden", emailSenderAccount);
  // Set Email priority or importance High, Normal, Low or 1 to 5 (1 is highest)
  smtpData.setPriority("High");
  // Set the subject
  smtpData.setSubject(emailSubject);
  // Set the message with HTML format
  smtpData.setMessage("<div style=\"color:#2f4468;\"><h1>"+noidung+"</p></div>", true);
  // Set the email message in text format (raw)
  //smtpData.setMessage("Hello World! - Sent from NodeWifi32 board", false);
  // Add recipients, you can add more than one recipient
  smtpData.addRecipient(emailRecipient);
  //smtpData.addRecipient("YOUR_OTHER_RECIPIENT_EMAIL_ADDRESS@EXAMPLE.com");
  smtpData.setSendCallback(sendCallback);
  //Start sending Email, can be set callback function to track the status
  if (!MailClient.sendMail(smtpData))
    Serial.println("Error sending Email, " + MailClient.smtpErrorReason());
  //Clear all data from Email object to free memory
  smtpData.empty();
  }
  // Callback function to get the Email sending status
void sendCallback(SendStatus msg) {
  // Print the current status
  Serial.println(msg.info());
  // Do something when complete
  if (msg.success()) {
    Serial.println("----------------");
  }
}
  void loop()
{
  for(int i=0;i<10;i++){
  if(Serial2.available()>0){
    char c=Serial2.read();
    if(indx<sizeof buff){
      buff[indx++]=c;
      Serial.println(c);
      if(c=='x'){
        Serial.println(buff);
      if(checkArrays(buff,str1,10))
      {
         post_btn("onl1","light1");
      }
      else if(checkArrays(buff,str0,10))
      {
         post_btn("offl1","light1");
      }
       if(checkArrays(buff,str3,10))
      {
         post_btn("onl2","light2");
      }
      else if(checkArrays(buff,str2,10))
      {
         post_btn("offl2","light2");
      }
       if(checkArrays(buff,str5,10))
      {
         post_btn("onp1","pump1");
      }
      else if(checkArrays(buff,str4,10))
      {
         post_btn("offp1","pump1");
      }
      if(checkArrays(buff,str7,10))
      {
         post_btn("onp2","pump2");
      }
     else if(checkArrays(buff,str6,10))
      {
        post_btn("offp2","pump2");    
      }
        indx=0;
      }
    }else{indx=0;}
   }
  }
  get_device_status("light1");
  get_device_status("light2");
  get_device_status("pump1");
  get_device_status("pump2");
   currentMillis = millis();
   if(currentMillis-startMillis>= period){
    check=0;
    startMillis = currentMillis;
   }
   get_send_mail("value1","Nhiet Do Qua Cao");
   get_send_mail("value2","Do Am Qua Cao");
  
}
 
 
void get_device_status(String device_name)
  {
    
    WiFiClient client = server.available();
 
        HTTPClient http;
        String url = get_host+"/demo/control/get_status.php?device_name="+device_name;
        
        http.begin(url);
        Serial.println(url);
        //GET method
        int httpCode = http.GET();
        String payload = http.getString();
        Serial.println("sssssssss");
        Serial.println(payload);
        payload.trim();
        if(payload=="onl1")
        {
          Serial2.println("1");
          Serial.println(Serial.read());    
        }
        else if (payload=="offl1")
        {
          Serial2.println("2");    
        }
        else if (payload=="onl2")
        {
          Serial2.println("3");    
        }
        else if (payload=="offl2")
        {
          Serial2.println("4");    
        }
        else if (payload== "onp1")
        {
          Serial2.println("5");    
        }
        else if (payload== "offp1")
        {
          Serial2.println("6");    
        }
        else if (payload== "onp2")
        {
          Serial2.println("7");    
        }
        else if (payload=="offp2")
        {
          Serial2.println("8");    
        }
   
          http.end();
  
  }
