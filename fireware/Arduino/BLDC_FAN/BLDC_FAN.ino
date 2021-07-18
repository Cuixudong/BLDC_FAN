/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-web-server-slider-pwm/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*********/

// Import required libraries
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

// Replace with your network credentials
const char* wifi_ssid = "@PHICOMM_94";
const char* wifi_pswd = "";

const int output = 2;

String sliderValue1 = "0";
String sliderValue2 = "0";
String sliderValue3 = "0";

// setting PWM properties
const int freq = 5000;
const int ledChannel = 0;
const int resolution = 8;

const char* PARAM_INPUT = "value";
const char* PARAM_ID = "id";
const char* PARAM_RELAY = "relay";
const char* PARAM_STATE = "state";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>无刷小风扇控制台</title>
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    h2 {font-size: 2.3rem;color: #7263ff;}
    p {font-size: 1.6rem;}
    body {max-width: 400px; margin:0px auto; padding-bottom: 25px;}
    .slider { -webkit-appearance: none; margin: 14px; width: 360px; height: 25px; background: #867991;
      outline: none; -webkit-transition: .2s; transition: opacity .2s;}
    .slider::-webkit-slider-thumb {-webkit-appearance: none; appearance: none; width: 35px; height: 35px; background: #993269; cursor: pointer;}
    .slider::-moz-range-thumb { width: 35px; height: 35px; background: #003249; cursor: pointer; } 
    .button {background-color: #4CAF50; border: none; color: white; padding: 10px 10px;font-size: 1.5rem;}
    .button2 {background-color: #555555; border: none; color: white; padding: 10px 10px;font-size: 1.5rem;}
    .switch {position: relative; display: inline-block; width: 120px; height: 68px} 
    .switch input {display: none}
    .slider1 {position: absolute; top: 0; left: 0; right: 0; bottom: 0; background-color: #ccc; border-radius: 34px}
    .slider1:before {position: absolute; content: ""; height: 52px; width: 52px; left: 8px; bottom: 8px; background-color: #fff; -webkit-transition: .4s; transition: .4s; border-radius: 68px}
    input:checked+.slider1 {background-color: #2196F3}
    input:checked+.slider1:before {-webkit-transform: translateX(52px); -ms-transform: translateX(52px); transform: translateX(52px)}
  </style>
</head>
<body>
  <h2>无刷小风扇控制台</h2>
  <p><span>速度：</span><span id="textSliderValue1">%SLIDERVALUE%</span></p>
  <p><input type="range" onchange="updateSliderPWM1(this)" id="pwmSlider1" min="0" max="10" value="%SLIDERVALUE1%" step="1" class="slider"></p>
  <p><span>角度：</span><span id="textSliderValue2">%SLIDERVALUE%</span></p>
  <p><input type="range" onchange="updateSliderPWM2(this)" id="pwmSlider2" min="0" max="4000" value="%SLIDERVALUE2%" step="1" class="slider"></p>
  <p><span>幅度：</span><span id="textSliderValue3">%SLIDERVALUE%</span></p>
  <p><input type="range" onchange="updateSliderPWM3(this)" id="pwmSlider3" min="0" max="4000" value="%SLIDERVALUE3%" step="1" class="slider"></p>
  <p><a><button onclick="updateButton(this)" id="1" class="button">打开风扇</button></a>
  <a><button onclick="updateButton(this)" id="2" class="button2">关闭风扇</button></a></p>
<script>
function updateSliderPWM1(element) {
  var sliderValue1 = document.getElementById("pwmSlider1").value;
  document.getElementById("textSliderValue1").innerHTML = sliderValue1;
  console.log(sliderValue1);
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/slider1?value="+sliderValue1, true);
  xhr.send();
}
function updateSliderPWM2(element) {
  var sliderValue2 = document.getElementById("pwmSlider2").value;
  document.getElementById("textSliderValue2").innerHTML = sliderValue2;
  console.log(sliderValue2);
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/slider2?value="+sliderValue2, true);
  xhr.send();
}
function updateSliderPWM3(element) {
  var sliderValue3 = document.getElementById("pwmSlider3").value;
  document.getElementById("textSliderValue3").innerHTML = sliderValue3;
  console.log(sliderValue3);
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/slider3?value="+sliderValue3, true);
  xhr.send();
}
function updateButton(element) {
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/bldc?value="+element.id+"&state=1", true);
  xhr.send();
}
function toggleCheckbox(element) {
  var xhr = new XMLHttpRequest();
  if(element.checked){ xhr.open("GET", "/update?relay="+element.id+"&state=1", true); }
  else { xhr.open("GET", "/update?relay="+element.id+"&state=0", true); }
  xhr.send();
}
</script>
</body>
</html>
)rawliteral";

// Replaces placeholder with button section in your web page
String processor(const String& var){
  //Serial.println(var);
  if (var == "SLIDERVALUE1"){
    return sliderValue1;
  }
  else if (var == "SLIDERVALUE2"){
    return sliderValue2;
  }
  else if (var == "SLIDERVALUE3"){
    return sliderValue3;
  }
  return String();
}

void SendFOCCmd(int motorid,int code,int cmd)
{
  byte data_mem[4];
  int CRC = 0;
  data_mem[0] = (motorid & 0x07) << 5 | (code & 0x1F);
  data_mem[1] = 1;
  data_mem[2] = cmd & 0xFF;
  CRC = (data_mem[0] + data_mem[1] + data_mem[2])&0xFF;
  CRC += (data_mem[0] + data_mem[1] + data_mem[2]) >> 8;
  data_mem[3] = CRC;
  Serial.write(data_mem,sizeof(data_mem));
}

void SendFOCData(int motorid,int code,int reg,int dat)
{
  byte data_mem[6];
  int CRC = 0;
  data_mem[0] = (motorid & 0x07) << 5 | (code & 0x1F);
  data_mem[1] = 3;
  data_mem[2] = reg & 0xFF;
  data_mem[3] = dat & 0xFF;
  data_mem[4] = (dat >> 8) & 0xFF;
  CRC = (data_mem[0] + data_mem[1] + data_mem[2] + data_mem[3] + data_mem[4])&0xFF;
  CRC += (data_mem[0] + data_mem[1] + data_mem[2] + data_mem[3] + data_mem[4]) >> 8;
  data_mem[5] = CRC;
  Serial.write(data_mem,sizeof(data_mem));
}

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);
  
  // configure LED PWM functionalitites
  ledcSetup(ledChannel, freq, resolution);
  
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(output, ledChannel);
  
  ledcWrite(ledChannel, sliderValue1.toInt());

  // Connect to Wi-Fi
  WiFi.begin(wifi_ssid, wifi_pswd);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print('.');
  }

  // Print ESP Local IP Address
  Serial.printf("ip:%s\n",WiFi.localIP().toString().c_str());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  // Send a GET request to <ESP_IP>/slider?value=<inputMessage>
  server.on("/slider1", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    int num;
    // GET input1 value on <ESP_IP>/slider?value=<inputMessage>
    if (request->hasParam(PARAM_INPUT)) {
      inputMessage = request->getParam(PARAM_INPUT)->value();
      sliderValue1 = inputMessage;
      num = inputMessage.toInt();
      SendFOCData(0x01,0x01,131,num);
    }
    request->send(200, "text/plain", "OK");
  });
  server.on("/slider2", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    int num;
    // GET input1 value on <ESP_IP>/slider?value=<inputMessage>
    if (request->hasParam(PARAM_INPUT)) {
      inputMessage = request->getParam(PARAM_INPUT)->value();
      sliderValue2 = inputMessage;
      num = inputMessage.toInt();
      SendFOCData(0x01,0x01,132,num);
    }
    else {
      inputMessage = "No message sent";
    }
    request->send(200, "text/plain", "OK");
  });
  server.on("/slider3", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    int num;
    // GET input1 value on <ESP_IP>/slider?value=<inputMessage>
    if (request->hasParam(PARAM_INPUT)) {
      inputMessage = request->getParam(PARAM_INPUT)->value();
      sliderValue3 = inputMessage;
      num = inputMessage.toInt();
      SendFOCData(0x01,0x01,133,num);
    }
    else {
      inputMessage = "No message sent";
    }
    request->send(200, "text/plain", "OK");
  });

  server.on("/update", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String id,state;
    // GET input1 value on <ESP_IP>/slider?value=<inputMessage>
    if (request->hasParam(PARAM_RELAY)) {
      id = request->getParam(PARAM_RELAY)->value();
      state = request->getParam(PARAM_STATE)->value();
    }
    Serial.println("update:"+id+",state:"+state);
    request->send(200, "text/plain", "OK");
  });

  server.on("/bldc", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    int num;
    // GET input1 value on <ESP_IP>/slider?value=<inputMessage>
    if (request->hasParam(PARAM_INPUT)) {
      inputMessage = request->getParam(PARAM_INPUT)->value();
      num = inputMessage.toInt();
      if(num == 1)
      {
        //打开闭环控制
        SendFOCCmd(0x01,0x03,0x11);
      }
      else if(num == 2)
      {
        //关闭闭环控制
        SendFOCCmd(0x01,0x03,0x02);
      }
    }
    request->send(200, "text/plain", "OK");
  });
  
  // Start server
  server.begin();
}
  
void loop() {
  
}
