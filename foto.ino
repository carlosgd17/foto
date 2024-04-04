#include <WiFi.h>
#include <WebServer.h>

/* Put your SSID & Password */
const char* ssid = "ESP32";  // Enter SSID here
const char* password = "12345678";  //Enter Password here

/* Put IP Address details */
IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

WebServer server(80);

uint8_t LED1pin = 4;
bool LED1status = LOW;

uint8_t LED2pin = 5;
bool LED2status = LOW;

int intensidad = 0;

void setup() {
  Serial.begin(115200);
  pinMode(LED1pin, OUTPUT);
  pinMode(LED2pin, OUTPUT);

  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);
  
  server.on("/", handle_OnConnect);
  
  server.on("/detener", detenerLeds);
  server.on("/iniciar", iniciarLeds);
  //server.on("/intensidad/", setIntensidad);
  server.on("/intensidad", HTTP_GET, []() {
  if (server.hasArg("value")) {
    String valueStr = server.arg("value");
    int value = valueStr.toInt();
    // Here you can add code to set the LED intensity using the 'value' variable
    Serial.print("Value: ");
    Serial.println(value);
  }
  server.send(200, "text/plain", "OK");
});

  server.onNotFound(handle_NotFound);
  
  server.begin();
  Serial.println("HTTP server started");
}
void loop() {
  server.handleClient();
  if(LED1status)
  {digitalWrite(LED1pin, HIGH);}
  else
  {digitalWrite(LED1pin, LOW);}
  
  if(LED2status)
  {digitalWrite(LED2pin, HIGH);}
  else
  {digitalWrite(LED2pin, LOW);}
}

void handle_OnConnect() {
  LED1status = LOW;
  LED2status = LOW;
  Serial.println("GPIO4 Status: OFF | GPIO5 Status: OFF");
  server.send(200, "text/html", SendHTML(LED1status,LED2status)); 
}

void detenerLeds() {
  LED2status = LOW;
  Serial.println("Apagando");
  Serial.println("GPIO5 Status: OFF");
  server.send(200, "text/html", SendHTML(LED1status,false)); 
}

void iniciarLeds() {
  LED2status = LOW;
  Serial.println("Iniciando");
  Serial.println("GPIO5 Status: OFF");
  server.send(200, "text/html", SendHTML(LED1status,false)); 
}

void setIntensidad() {
  LED2status = LOW;
  Serial.println("Intensidad");
  Serial.println("GPIO5 Status: OFF");
  server.send(200, "text/html", SendHTML(LED1status,false)); 
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

String SendHTML(uint8_t led1stat,uint8_t led2stat){
  String ptr = "<!DOCTYPE html>\n"
"<html lang=\"es\">\n"
"<head>\n"
"    <meta charset=\"UTF-8\">\n"
"    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
"    <title>Control de LEDs</title>\n"
"    <style>\n"
"        body {\n"
"            font-family: Arial, sans-serif;\n"
"            margin: 0;\n"
"            padding: 0;\n"
"            background: linear-gradient(90deg, #3494e6, #ec6ead);\n"
"            display: flex;\n"
"            flex-direction: column;\n"
"            justify-content: center;\n"
"            align-items: center;\n"
"            height: 100vh;\n"
"        }\n"
"        #led-control {\n"
"            width: 90%;\n"
"            max-width: 500px;\n"
"            background-color: rgba(255, 255, 255, 0.5);\n"
"            border-radius: 8px;\n"
"            padding: 20px;\n"
"            box-shadow: 0px 0px 10px rgba(0, 0, 0, 0.1);\n"
"        }\n"
"        h1 {\n"
"            text-align: center;\n"
"            margin-bottom: 20px;\n"
"            color: #333;\n"
"        }\n"
"        h2 {\n"
"            text-align: center;\n"
"            margin-bottom: 20px;\n"
"            color: #333;\n"
"        }\n"
"        label {\n"
"            display: block;\n"
"            margin-bottom: 10px;\n"
"            color: #666;\n"
"        }\n"
"        #slider {\n"
"            width: calc(100% - 20px);\n"
"            margin: 0 auto 20px auto;\n"
"            display: block;\n"
"        }\n"
"        #intensity {\n"
"            display: block;\n"
"            text-align: center;\n"
"            margin-bottom: 20px;\n"
"            color: #666;\n"
"        }\n"
"        #controls {\n"
"            display: flex;\n"
"            justify-content: center;\n"
"            align-items: center;\n"
"        }\n"
"        button {\n"
"            padding: 10px 20px;\n"
"            font-size: 16px;\n"
"            margin: 0 10px;\n"
"            background-color: #007bff;\n"
"            color: #fff;\n"
"            border: none;\n"
"            border-radius: 4px;\n"
"            cursor: pointer;\n"
"            transition: background-color 0.3s ease;\n"
"        }\n"
"        button:hover {\n"
"            background-color: #ec6ead;\n"
"        }\n"
"\n"
"        /* Estilos para pantallas más pequeñas */\n"
"        @media only screen and (max-width: 600px) {\n"
"            #led-control {\n"
"                width: 100%;\n"
"                border-radius: 0;\n"
"                box-shadow: none;\n"
"            }\n"
"        }\n"
"    </style>\n"
"</head>\n"
"<body>\n"
"    <h1>LEDS PROJECT</h1>\n"
"    <div id=\"led-control\">\n"
"        <h2>Control de Intensidad de LEDs</h2>\n"
"        <label for=\"slider\">Intensidad:</label>\n"
"        <input type=\"range\" min=\"0\" max=\"255\" value=\"0\" id=\"slider\">\n"
"        <span id=\"intensity\">0</span>\n"
"        <div id=\"controls\">\n"
"            <button id=\"start\">Iniciar</button>\n"
"            <button id=\"stop\">Detener</button>\n"
"        </div>\n"
"    </div>\n"
"\n"
"    <div style=\"text-align: center; margin-top: 20px; color: #333;\" id=\"led-control\">\n"
"        <p style=\"text-align: left; color: #333;\">Integrantes del equipo:</p>\n"
"        <ul>\n"
"            <li>Samuel Flores García</li>\n"
"            <li>Carlos García Días</li>\n"
"            <li>Jesús Alberto Sánchez Mendieta</li>\n"
"            <li>Daisy Montserrat Sánchez Hernández</li>\n"
"        </ul>\n"
"    </div>\n"
"\n"
"    <script>\n"
"        const slider = document.getElementById(\"slider\");\n"
"        const intensityDisplay = document.getElementById(\"intensity\");\n"
"        const startButton = document.getElementById(\"start\");\n"
"        const stopButton = document.getElementById(\"stop\");\n"
"\n"
"        slider.addEventListener(\"input\", function() {\n"
"            intensityDisplay.textContent = this.value;\n"
"            // Aquí se puede enviar la intensidad al ESP32 para controlar los LEDs\n"
"            // Por ejemplo, con fetch() para enviar la intensidad a una URL en el ESP32\n"
"            fetch('http://192.168.1.1/intensidad?value=' + this.value);\n"
"        });\n"
"\n"
"        startButton.addEventListener(\"click\", function() {\n"
"            // Aquí se puede enviar una señal al ESP32 para iniciar la acción de los LEDs\n"
"            // Por ejemplo, con fetch() para enviar una solicitud de inicio a una URL en el ESP32\n"
"            fetch('http://192.168.1.1/iniciar');\n"
"        });\n"
"\n"
"        stopButton.addEventListener(\"click\", function() {\n"
"            // Aquí se puede enviar una señal al ESP32 para detener la acción de los LEDs\n"
"            // Por ejemplo, con fetch() para enviar una solicitud de detener a una URL en el ESP32\n"
"            fetch('http://192.168.1.1/detener');\n"
"        });\n"
"    </script>\n"
"</body>\n"
"</html>";
  return ptr;
}