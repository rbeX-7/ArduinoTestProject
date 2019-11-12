
#include <SoftwareSerial.h>

SoftwareSerial mySerial(2,3);  //RX,TX
String ssid ="Wye.net";
String password="PesekNyembul";
String data;
String server = "192.168.0.102"; // www.example.com
String uri = "/test.php";// our example is /esppost.php

bool isSending =false;

void setup() {
  // put your setup code here, to run once:
   mySerial.begin(115200);
   Serial.begin(115200);
   reset();
   connectWifi();
}

void reset() {
  Serial.println("ON RESET");
  mySerial.println("AT+RST");
  delay(5000);
  if(mySerial.find("OK") ) Serial.println("Module Reset");
}

void connectWifi() {
  Serial.println("ON CONNECT");
  String cmd = "AT+CWJAP=\"" +ssid+"\",\"" + password + "\"";
  mySerial.println(cmd);
  delay(5000);
  while(!mySerial.available()){
    Serial.println("waiting response to AP");
    delay(1000);
  }
  //delay(3000);
  if(mySerial.find("OK")) {
    Serial.println("Connected!");
  }else {
    connectWifi();
    Serial.println("Cannot connect to wifi"); 
   }
}

void loop() {
  if(isSending == false)
    SendData_Get();
  // put your main code here, to run repeatedly:
  delay(10000);
}

void SendData_Get()
{
  isSending = true;
  Serial.println("ON SendData use GET");
  int a = random(1,10);
  int b = random(1,10);
  int c = random(1,10);
  mySerial.println("AT+CIPSTART=\"TCP\",\"" + server + "\",80");
  //--
  while(!mySerial.available()){
    Serial.println("Waiting TCP ready...");
    mySerial.println("AT+CIPCLOSE");
    Serial.flush();
    mySerial.flush();
    isSending = false;
    return;
  }
  //--
  if( mySerial.find("OK")) {
    Serial.println("TCP connection ready");
  } 
  //-- 
  data = (String)"a=" + a + "&b=" + b + "&c=" + c;
  String getCmd = "GET " + uri + "?"+ data + " HTTP/1.1\r\n";
  int getLth = getCmd.length();
  String hostCmd = "Host: "+ server +"\r\n";
  int hostLth = hostCmd.length();
  String connCmd = "Connection: keep-alive\r\n";
  int connLth = connCmd.length();
  int SendLth = getLth + hostLth + connLth;
  String cmd = "AT+CIPSEND=" + String(SendLth);
  mySerial.println(cmd);
   //--       
   delay(500);   
  if(mySerial.available())
  {
    Serial.println("Waiting to transmit data..");
  }      
  if (mySerial.find(">"))
  {
    Serial.println("Sending Data....");   
    mySerial.print(getCmd);
    mySerial.print(hostCmd);
    mySerial.print(connCmd);     
    if( mySerial.find("SEND OK")) { 
      Serial.println("Packet sent");      

      if (mySerial.available()>0) {
        String tmpResp = mySerial.readString();
        Serial.println(tmpResp);
      }
      else
      {
        Serial.println("Serial response not available,Packet possibility not delivered");  
      }
    }else
    {
      Serial.println("\"SEND OK\" not Found, No response from Server..");  
    }
  }        
  else
  {
    Serial.println("\">\" char not Found , Failed to send..");  
    }
  mySerial.println("AT+CIPCLOSE");  
  Serial.flush();
  mySerial.flush();
  isSending = false;
}

void SendData_POST()
{
  isSending = true;
  Serial.println("ON SendData");
  int a = random(1,10);
  int b = random(1,10);
  int c = random(1,10);
  mySerial.println("AT+CIPSTART=\"TCP\",\"" + server + "\",80");
  delay(200);
  while(!mySerial.available()){
    Serial.println("Waiting TCP ready...");
    mySerial.println("AT+CIPCLOSE");
    Serial.flush();
    mySerial.flush();
    isSending = false;
    return;
  }
   if( mySerial.find("OK")) {
    Serial.println("TCP connection ready");
  } 
  //delay(1000);
  data = (String)"a=" + a + "&b=" + b + "&c=" + c;
  String postRequest = "POST " + uri + " HTTP/1.0\r\n";
  int reqLength = postRequest.length()+2;
  String postHost = "Host: " + server + "\r\n";
  int hostLength = postHost.length()+2;
  String postAccept = (String)"Accept: *" + "/" + "*\r\n";
  int acceptLength = postAccept.length()+2;
  String postLength = (String)"Content-Length: " + data.length() + "\r\n";
  int postLth = postLength.length()+2;
  String postType = "Content-Type: application/x-www-form-urlencoded\r\n";
  int typeLength = postType.length()+2;
  String postData = "\r\n" + data;  
  int dataLength = postData.length()+2;
  int totLength = (reqLength + hostLength + acceptLength + postLth + typeLength + dataLength);
  
  Serial.println("host : " + server);
  Serial.println("uri : " + uri);
  Serial.println("data : " + data);
  String sendCmd = "AT+CIPSEND=" + (String)totLength + "\r\n";
  Serial.println(sendCmd);
  mySerial.print(sendCmd );
  
  while(!mySerial.available()){
    Serial.println("Waiting for cnnection to server is established...");
  }
//  if(mySerial.available())
//    {
//      String tmpResp = mySerial.readString();
//        Serial.println(tmpResp);
//    }
   if(mySerial.find(">")) {
    Serial.println("Sending Data..."); 
    //mySerial.print(postRequest);
    mySerial.print(postRequest);
    mySerial.print(postHost);
    mySerial.print(postAccept);
    mySerial.print(postLength);
    mySerial.print(postType);
    mySerial.print(postData);
    if( mySerial.find("SEND OK")) { 
      Serial.println("Packet sent");
      while(!mySerial.available()){
        Serial.println("Loading web server response");
      }
      while (mySerial.available()) {
        String tmpResp = mySerial.readString();
        Serial.println(tmpResp);
      }
    }
  }
  else
  {
    Serial.println("No response from server...");
    }
  mySerial.println("AT+CIPCLOSE");
  delay(1000);
  Serial.flush();
  mySerial.flush();
  isSending = false;
}
