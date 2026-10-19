#include <WiFi.h>
#include <WebServer.h>
#include <SPIFFS.h>
445
// Replace with your desired AP credentials
const char* ssid = "ESP32_AP";       // SSID of the ESP32 access point
const char* password = "12345678";   // Password for the access point

// Create a WebServer object on port 80
WebServer server(80);

// Pin where the LED is connected
const int ledPin = 2;  // GPIO 2 (built-in LED on many ESP32 boards)

// HTML content for the web interface
const char* htmlContent = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
  <title>ESP32 Control</title>
  <style>
    body { font-family: Arial, sans-serif; text-align: center; margin-top: 50px; background-color: #f4f4f9; color: #333; }
    h1 { color: #4CAF50; }
    #logo { width: 200px; height: auto; margin-bottom: 20px; }
    button {
      padding: 15px 30px;
      font-size: 18px;
      background-color: #4CAF50;
      color: white;
      border: none;
      cursor: pointer;
      border-radius: 5px;
      margin: 10px;
    }
    button:hover { background-color: #45a049; }
    p { font-size: 20px; font-weight: bold; }
  </style>
</head>
<body>
  <img src="/logo.png" id="logo" />
  <h1>ESP32 Control Interface</h1>
  <p>LED State: <span id="ledState">%STATE%</span></p>
  <button onclick="controlLed('ON')">Turn ON</button>
  <button onclick="controlLed('OFF')">Turn OFF</button>
  <script>
    function controlLed(state) {
      fetch('/' + state.toLowerCase())
        .then(response => response.text())
        .then(data => {
          document.getElementById('ledState').innerText = state;
        });
    }
  </script>
</body>
</html>
)rawliteral";

// Function to replace placeholders in the HTML
String processor(const String& var) {
  if (var == "STATE") {
    return digitalRead(ledPin) ? "ON" : "OFF";
  }
  return String();
}

void setup() {
  // Initialize Serial for debugging
  Serial.begin(115200);

  // Set the LED pin as an output
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);  // Start with the LED off

  // Start the ESP32 as an access point
  WiFi.softAP(ssid, password);

  // Print the IP address of the ESP32
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // Initialize SPIFFS
  if (!SPIFFS.begin()) {
    Serial.println("SPIFFS mount failed");
    return;
  }

  // Define server routes
  server.on("/", HTTP_GET, []() {
    String html = htmlContent;
    html.replace("%STATE%", processor("STATE"));
    server.send(200, "text/html", html);
  });

  // Serve the image
  server.on("/logo.png", HTTP_GET, []() {
    File logo = SPIFFS.open("/logo.png", "r");
    if (logo) {
      server.streamFile(logo, "image/png");
      logo.close();
    } else {
      server.send(404, "text/plain", "File not found");
    }
  });

  server.on("/on", HTTP_GET, []() {
    digitalWrite(ledPin, HIGH);  // Turn on the LED
    server.send(200, "text/plain", "ON");
  });

  server.on("/off", HTTP_GET, []() {
    digitalWrite(ledPin, LOW);  // Turn off the LED
    server.send(200, "text/plain", "OFF");
  });

  // Start the server
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  // Handle client requests
  server.handleClient();
}
