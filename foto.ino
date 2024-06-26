#include <WiFi.h>
#include <WebServer.h>
#include <OneWire.h>
#include <DallasTemperature.h>

/* Put your SSID & Password */
const char* ssid = "ESP32";  // Enter SSID here
const char* password = "12345678";  //Enter Password here

/* Put IP Address details */
IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

WebServer server(80);

// azul
int painLED = 4;
// verde
int tissueLED = 5;
// amarillo
int hairLED = 19;

int sensor = 2;
OneWire oneWire(sensor);
DallasTemperature sensors(&oneWire);
float temperatura = 117.0;

void setup() {
  Serial.begin(115200);

  pinMode(painLED, OUTPUT);
  pinMode(tissueLED, OUTPUT);
  pinMode(hairLED, OUTPUT);

  sensors.requestTemperatures();
  temperatura = sensors.getTempCByIndex(0);

  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);
  
  server.on("/", handle_OnConnect);
  server.on("/pain", painTreatment);
  server.on("/tissue", tissueTreatment);
  server.on("/hair", hairTreatment);
  server.on("/cancel", cancelTreatment);

  server.onNotFound(handle_NotFound);
  
  server.begin();
  Serial.println("HTTP server started");
}
void loop() {
  sensors.requestTemperatures();
  temperatura = sensors.getTempCByIndex(0);
  Serial.println(temperatura);
  if(temperatura > 30){
    cancelTreatment();
  }
  server.handleClient();
}

void handle_OnConnect() {
  Serial.println("\nIniciado\n");
  server.send(200, "text/html", SendHTML()); 
}

void painTreatment() {
  Serial.println("\nPain\n");
  analogWrite(painLED, 30);
  server.send(200, "text/html", SendHTML()); 
}

void tissueTreatment() {
  Serial.println("\ntissue\n");
  for(int i = 0; i < 5; i++){
    analogWrite(tissueLED, 255);
    delay(1000);
    analogWrite(tissueLED, 125);
    delay(1000);
  }
  server.send(200, "text/html", SendHTML()); 
}

void hairTreatment() {
  Serial.println("\nhair\n");
  analogWrite(hairLED, 255);
  server.send(200, "text/html", SendHTML()); 
}

void cancelTreatment() {
  Serial.println("\ncanceling\n");
  analogWrite(painLED, 0);
  analogWrite(tissueLED, 0);
  analogWrite(hairLED, 0);
  server.send(200, "text/html", SendHTML()); 
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

String SendHTML(){
  String ptr = "<!DOCTYPE html>\n"
"<html lang=\"es\">\n"
"  <head>\n"
"    <meta charset=\"UTF-8\" />\n"
"    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\" />\n"
"    <title>Fotobiomodulación</title>\n"
"    <style>\n"
"      body {\n"
"        font-family: Arial, sans-serif;\n"
"        margin: 0;\n"
"        padding: 0;\n"
"        background: linear-gradient(90deg, #3494e6, #ec6ead);\n"
"        display: flex;\n"
"        flex-direction: column;\n"
"        justify-content: center;\n"
"        align-items: center;\n"
"        height: 100vh;\n"
"      }\n"
"\n"
"      #led-control {\n"
"        width: 90%;\n"
"        max-width: 500px;\n"
"        background-color: rgba(255, 255, 255, 0.5);\n"
"        border-radius: 8px;\n"
"        padding: 20px;\n"
"        box-shadow: 0px 0px 10px rgba(0, 0, 0, 0.1);\n"
"      }\n"
"\n"
"      #info-control {\n"
"        display: block;\n"
"        width: 90%;\n"
"        max-width: 500px;\n"
"        background-color: rgba(255, 255, 255, 0.5);\n"
"        border-radius: 8px;\n"
"        padding: 20px;\n"
"        box-shadow: 0px 0px 10px rgba(0, 0, 0, 0.1);\n"
"        margin-top: 20px;\n"
"        margin-bottom: 20px;\n"
"      }\n"
"\n"
"      h1 {\n"
"        text-align: center;\n"
"        margin-bottom: 50px;\n"
"        color: #333;\n"
"      }\n"
"\n"
"      h2 {\n"
"        text-align: center;\n"
"        margin-bottom: 20px;\n"
"        color: #333;\n"
"      }\n"
"\n"
"      label {\n"
"        display: block;\n"
"        margin-bottom: 10px;\n"
"        color: #666;\n"
"      }\n"
"\n"
"      #intensity {\n"
"        display: block;\n"
"        text-align: center;\n"
"        margin-bottom: 20px;\n"
"        color: #666;\n"
"      }\n"
"\n"
"      #controls {\n"
"        display: flex;\n"
"        justify-content: center;\n"
"        align-items: center;\n"
"      }\n"
"\n"
"      button {\n"
"        padding: 10px 20px;\n"
"        font-size: 16px;\n"
"        margin: 0 10px;\n"
"        background-color: #007bff;\n"
"        color: #fff;\n"
"        border: none;\n"
"        border-radius: 4px;\n"
"        cursor: pointer;\n"
"        transition: background-color 0.3s ease;\n"
"        margin-bottom: 50px;\n"
"      }\n"
"\n"
"      button:hover {\n"
"        background-color: #ec6ead;\n"
"      }\n"
"\n"
"      #timer-container {\n"
"        text-align: center;\n"
"        margin-top: 20px;\n"
"        color: #666;\n"
"        font-size: 24px;\n"
"        background-color: rgba(255, 255, 255, 0.3);\n"
"        padding: 15px;\n"
"        border-radius: 6px;\n"
"        box-shadow: 0px 0px 10px rgba(0, 0, 0, 0.1);\n"
"        width: 200px;\n"
"        display: none;\n"
"        margin: 0 auto;\n"
"        margin-bottom: 40px;\n"
"      }\n"
"\n"
"      #message {\n"
"        text-align: center;\n"
"        margin-top: 20px;\n"
"        color: #666;\n"
"        font-size: 24px;\n"
"        display: none;\n"
"      }\n"
"\n"
"      #cancel {\n"
"        background-color: #dc3545;\n"
"        display: none;\n"
"        margin-top: 20px;\n"
"        align-self: center;\n"
"      }\n"
"      #paragraph {\n"
"        text-align: center;\n"
"        margin-bottom: 20px;\n"
"        color: #7b7b7b;\n"
"      }\n"
"\n"
"      #info-pain-relief,\n"
"      #info-tissue-treatment,\n"
"      #info-hair-growth {\n"
"        display: none; /* Ocultar al inicio */\n"
"      }\n"
"\n"
"      #temperatura {\n"
"        display: flex;\n"
"        align-items: center;\n"
"        justify-content: center;\n"
"        font-size: 24px;\n"
"        margin: 20px auto;\n"
"      }\n"
"\n"
"      #valor-temperatura {\n"
"        font-weight: bold;\n"
"        color: #000; /* Color por defecto */\n"
"      }\n"
"\n"
"      #valor-temperatura.calor {\n"
"        color: #f00; /* Rojo */\n"
"      }\n"
"\n"
"      #valor-temperatura.frio {\n"
"        color: #00f; /* Azul */\n"
"      }\n"
"\n"
"      /* Estilos para pantallas más pequeñas */\n"
"      @media only screen and (max-width: 600px) {\n"
"        #led-control {\n"
"          width: 100%;\n"
"          border-radius: 0;\n"
"          box-shadow: none;\n"
"        }\n"
"      }\n"
"    </style>\n"
"  </head>\n"
"  <body>\n"
"    <h1>Sistemas Embebidos - Fotobiomodulación</h1>\n"
"    <div id=\"led-control\">\n"
"      <h2>Bienvenido</h2>\n"
"      <h3 id=\"paragraph\">\n"
"        Selecciona cualquiera de las opciones para comenzar con la sesión\n"
"      </h3>\n"
"      <div id=\"controls\">\n"
"        <button id=\"btn1\" onclick=\"confirmStart(10, 'info-pain-relief')\">\n"
"          Alivio del dolor en los músculos\n"
"        </button>\n"
"        <button id=\"btn2\" onclick=\"confirmStart(20, 'info-tissue-treatment')\">\n"
"          Tratamiento para los tejidos\n"
"        </button>\n"
"        <button id=\"btn3\" onclick=\"confirmStart(30, 'info-hair-growth')\">\n"
"          Crecimiento del cabello\n"
"        </button>\n"
"      </div>\n"
"      <div id=\"timer-container\">\n"
"        <div><strong>Tiempo restante de la sesión</strong></div>\n"
"        <div id=\"timer\"></div>\n"
"      </div>\n"
"      <button\n"
"        id=\"cancel\"\n"
"        onclick=\"cancelTimer()\"\n"
"        style=\"\n"
"          background-color: #dc3545;\n"
"          display: none;\n"
"          margin: 0 auto;\n"
"          align-self: center;\n"
"        \"\n"
"      >\n"
"        Terminar con la sesión\n"
"      </button>\n"
"      <div id=\"message\">La sesión ha terminado</div>\n"
"    </div>\n"
"    <div id=\"info-control\">\n"
"      <div id=\"temperatura\">\n"
"        <span id=\"valor-temperatura\"></span>\n"
"      </div>\n"
"    </div>\n"
"\n"
"    <div id=\"info-control\">\n"
"      <div style=\"text-align: center\"></div>\n"
"      <div id=\"info-pain-relief\">\n"
"        <h2>Alivio del dolor en los músculos</h2>\n"
"        <p>La fotobiomodulación puede ayudarte a:</p>\n"
"        <ul>\n"
"          <li>Reducir la inflamación y el dolor de forma natural.</li>\n"
"          <li>\n"
"            Acelerar la recuperación muscular, permitiéndote volver a tu rutina\n"
"            más rápido.\n"
"          </li>\n"
"          <li>\n"
"            Mejorar la flexibilidad y el rango de movimiento, previniendo\n"
"            futuras lesiones.\n"
"          </li>\n"
"          <li>\n"
"            Es un tratamiento seguro y no invasivo, sin efectos secundarios\n"
"            conocidos.\n"
"          </li>\n"
"        </ul>\n"
"        <h3>Tiempo recomendado:</h3>\n"
"        <p>6 minutos despues del ejercicio.</p>\n"
"        <h3>Posibles resultados:</h3>\n"
"        <ul>\n"
"          <li>Disminución significativa del dolor e inflamación.</li>\n"
"          <li>Recuperación muscular más rápida y eficiente.</li>\n"
"          <li>Mayor flexibilidad y rango de movimiento.</li>\n"
"          <li>Mejora en el rendimiento deportivo.</li>\n"
"        </ul>\n"
"      </div>\n"
"\n"
"      <div id=\"info-tissue-treatment\">\n"
"        <h2>Tratamiento para los tejidos</h2>\n"
"        <p>La fotobiomodulación puede ayudarte a:</p>\n"
"        <ul>\n"
"          <li>\n"
"            Estimular la producción de colágeno y elastina, mejorando la\n"
"            elasticidad de la piel.\n"
"          </li>\n"
"          <li>\n"
"            Mejorar la cicatrización de heridas, reduciendo el tiempo de\n"
"            recuperación.\n"
"          </li>\n"
"          <li>\n"
"            Reducir la inflamación y el edema, aliviando el dolor y la\n"
"            hinchazón.\n"
"          </li>\n"
"          <li>\n"
"            Es un tratamiento efectivo para diversas condiciones, como úlceras,\n"
"            quemaduras y lesiones deportivas.\n"
"          </li>\n"
"        </ul>\n"
"        <h3>Tiempo recomendado:</h3>\n"
"        <p>3-4 sesiones por semana, de 20 a 30 minutos por sesión.</p>\n"
"        <h3>Posibles resultados:</h3>\n"
"        <ul>\n"
"          <li>Piel más firme y tersa, con menos arrugas y líneas finas.</li>\n"
"          <li>Cicatrices menos visibles y más planas.</li>\n"
"          <li>Reducción de la inflamación y el edema.</li>\n"
"          <li>Mayor rapidez en la cicatrización de heridas.</li>\n"
"        </ul>\n"
"      </div>\n"
"\n"
"      <div id=\"info-hair-growth\">\n"
"        <h2>Crecimiento del cabello</h2>\n"
"        <p>La fotobiomodulación puede ayudarte a:</p>\n"
"        <ul>\n"
"          <li>\n"
"            Estimular el crecimiento del cabello, aumentando la densidad y el\n"
"            volumen.\n"
"          </li>\n"
"          <li>\n"
"            Fortalecer los folículos pilosos, previniendo la caída del cabello.\n"
"          </li>\n"
"          <li>\n"
"            Mejorar la calidad del cabello, haciéndolo más fuerte y brillante.\n"
"          </li>\n"
"          <li>\n"
"            Es un tratamiento seguro y efectivo para combatir la alopecia\n"
"            androgénica y otras causas de pérdida de cabello.\n"
"          </li>\n"
"        </ul>\n"
"        <h3>Tiempo recomendado:</h3>\n"
"        <p>Dos sesiones a la semana (5 minutos por sesion), por 6 meses.</p>\n"
"        <h3>Posibles resultados:</h3>\n"
"        <ul>\n"
"          <li>Crecimiento visible del cabello en 3 a 6 meses.</li>\n"
"          <li>Cabello más fuerte, grueso y brillante.</li>\n"
"          <li>Reducción de la caída del cabello.</li>\n"
"          <li>Mayor densidad y volumen capilar.</li>\n"
"        </ul>\n"
"      </div>\n"
"    </div>\n"
"\n"
"    <script>\n"
"      let countdown;\n"
"\n"
"      function confirmStart(seconds, infoIdToShow) {\n"
"        if (confirm(\"¿Estás seguro de que quieres comenzar con esta sesión?\")) {\n"
"          startTimer(seconds);\n"
"          showInfo(infoIdToShow);\n"
"          startTreatment(infoIdToShow);\n"
"        }\n"
"      }\n"
"\n"
"      function startTreatment(infoIdToShow) {\n"
"        if(infoIdToShow === 'info-pain-relief') {\n"
"          fetch('http://192.168.1.1/pain');\n"
"        } else if(infoIdToShow === 'info-tissue-treatment') {\n"
"            fetch('http://192.168.1.1/tissue');\n"
"        } else if(infoIdToShow === 'info-hair-growth') {\n"
"            fetch('http://192.168.1.1/hair');\n"
"        }\n"
"      }\n"
"\n"
"      function startTimer(seconds) {\n"
"        clearInterval(countdown);\n"
"        const timerDisplay = document.getElementById(\"timer\");\n"
"        const messageDisplay = document.getElementById(\"message\");\n"
"        const cancelBtn = document.getElementById(\"cancel\");\n"
"        const timerContainer = document.getElementById(\"timer-container\");\n"
"        const now = Date.now();\n"
"        const then = now + seconds * 1000;\n"
"\n"
"        displayTimeLeft(seconds);\n"
"\n"
"        countdown = setInterval(() => {\n"
"          const secondsLeft = Math.round((then - Date.now()) / 1000);\n"
"\n"
"          if (secondsLeft < 0) {\n"
"            clearInterval(countdown);\n"
"            messageDisplay.style.display = \"block\";\n"
"            cancelBtn.style.display = \"none\";\n"
"            fetch('http://192.168.1.1/cancel');\n"
"            return;\n"
"          }\n"
"\n"
"          displayTimeLeft(secondsLeft);\n"
"        }, 1000);\n"
"\n"
"        timerContainer.style.display = \"block\";\n"
"        cancelBtn.style.display = \"inline-block\";\n"
"      }\n"
"\n"
"      function displayTimeLeft(seconds) {\n"
"        const minutes = Math.floor(seconds / 60);\n"
"        const remainderSeconds = seconds % 60;\n"
"        const display = `${minutes}:${\n"
"          remainderSeconds < 10 ? \"0\" : \"\"\n"
"        }${remainderSeconds}`;\n"
"        document.getElementById(\"timer\").textContent = display;\n"
"      }\n"
"\n"
"      function cancelTimer(infoIdToShow) {\n"
"        clearInterval(countdown);\n"
"        document.getElementById(\"timer\").textContent = \"\";\n"
"        document.getElementById(\"message\").style.display = \"none\";\n"
"        document.getElementById(\"timer-container\").style.display = \"none\";\n"
"        document.getElementById(\"cancel\").style.display = \"none\";\n"
"        showInfo(infoIdToShow);\n"
"\n"
"        fetch('http://192.168.1.1/cancel');\n"
"      }\n"
"\n"
"      function showInfo(infoId) {\n"
"        const infoElements = document.querySelectorAll(\"#info-control > div\");\n"
"        infoElements.forEach((info) => {\n"
"          if (info.id === infoId) {\n"
"            info.style.display = \"block\"; // Mostrar información específica\n"
"          } else {\n"
"            info.style.display = \"none\";\n"
"          }\n"
"        });\n"
"\n"
"        // Si no hay un infoId válido, mostrar el primer div que contiene \"Información\"\n"
"        if (!infoId) {\n"
"          infoElements[0].style.display = \"block\";\n"
"        }\n"
"      }\n"
"\n"
"      function mostrarTemperatura() {\n"
"        const temperaturaActual =\n"
//"          Math.floor(Math.random() * (35 - 15 + 1)) + 15;\n"
+ String(temperatura) + ";\n"
"        const elementoTemperatura =\n"
"          document.getElementById(\"valor-temperatura\");\n"
"        elementoTemperatura.textContent = `${temperaturaActual}°C`;\n"
"\n"
"        if (temperaturaActual >= 25) {\n"
"          elementoTemperatura.classList.add(\"calor\");\n"
"        } else {\n"
"          elementoTemperatura.classList.remove(\"calor\");\n"
"          elementoTemperatura.classList.add(\"frio\");\n"
"        }\n"
"      }\n"
"\n"
"      mostrarTemperatura();\n"
"    </script>\n"
"  </body>\n"
"</html>\n"
"";
  return ptr;
}