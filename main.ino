#include <WiFi.h>

// Replace with your network credentials
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

// Set web server port number to 80
WiFiServer server(80);

// Sensor and LED pins
#define TRIG_LEFT 5    
#define ECHO_LEFT 18   
#define TRIG_RIGHT 15  
#define ECHO_RIGHT 4   
#define IR_SENSOR 19   // IR Sensor pin
#define LED_GREEN 2    // Green LED for correct parking
#define LED_RED 21     // Red LED for wrong parking

// Define parking thresholds (adjust as per slot size)
const float correctParkingThresholdMin = 5.0;  // Minimum width in cm
const float correctParkingThresholdMax = 8.0;  // Maximum width in cm

void setup() {
    Serial.begin(115200);

    pinMode(TRIG_LEFT, OUTPUT);
    pinMode(ECHO_LEFT, INPUT);
    pinMode(TRIG_RIGHT, OUTPUT);
    pinMode(ECHO_RIGHT, INPUT);
    pinMode(IR_SENSOR, INPUT);
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_RED, OUTPUT);

    Serial.println("🚗 Smart Parking System Initialized...");

    // Connect to Wi-Fi network
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi connected.");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    server.begin();
}

// Function to measure distance using ultrasonic sensors
float getDistance(int trigPin, int echoPin) {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    
    long duration = pulseIn(echoPin, HIGH, 30000); // 30ms timeout
    float distance = duration * 0.034 / 2; // Convert to cm
    return distance;
}

String checkParkingStatus() {
    float leftDist = getDistance(TRIG_LEFT, ECHO_LEFT);
    float rightDist = getDistance(TRIG_RIGHT, ECHO_RIGHT);
    float totalWidth = leftDist + rightDist;  
    bool carDetected = digitalRead(IR_SENSOR) == LOW; // IR sensor detects car presence

    Serial.print("Left Distance: "); Serial.print(leftDist); Serial.print(" cm | ");
    Serial.print("Right Distance: "); Serial.print(rightDist); Serial.print(" cm | ");
    Serial.print("Total Width: "); Serial.print(totalWidth); Serial.println(" cm");

    if (carDetected) {
        if (totalWidth >= correctParkingThresholdMin && totalWidth <= correctParkingThresholdMax) {
            Serial.println("✅ Correctly Parked!");
            digitalWrite(LED_GREEN, HIGH);
            digitalWrite(LED_RED, LOW);
            return "Correctly Parked";
        } else {
            Serial.println("❌ Wrongly Parked!");
            digitalWrite(LED_GREEN, LOW);
            digitalWrite(LED_RED, HIGH);
            return "Wrongly Parked";
        }
    } else {
        Serial.println("No car detected.");
        digitalWrite(LED_GREEN, LOW);
        digitalWrite(LED_RED, LOW);
        return "No Car Detected";
    }
}

void loop() {
    WiFiClient client = server.available(); // Listen for incoming clients

    if (client) {
        Serial.println("New Client.");
        String currentLine = "";
        while (client.connected()) {
            if (client.available()) {
                char c = client.read();
                Serial.write(c);
                if (c == '\n') {
                    if (currentLine.length() == 0) {
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-type:text/html");
                        client.println("Connection: close");
                        client.println();
                        
                        // HTML Web Page
                        client.println("<!DOCTYPE html><html>");
                        client.println("<head><meta name='viewport' content='width=device-width, initial-scale=1'>");
                        client.println("<link rel='icon' href='data:,'>");
                        client.println("<style>body { font-family: Helvetica; text-align: center; margin-top: 50px; } h1 { color: #333; }");
                        client.println(".status { font-size: 24px; font-weight: bold; margin-top: 20px; }</style></head>");
                        client.println("<body><h1>Smart Parking System</h1>");
                        
                        // Display parking status
                        String parkingStatus = checkParkingStatus();
                        client.println("<p class='status'>Status: " + parkingStatus + "</p>");
                        
                        client.println("</body></html>");
                        client.println();
                        break;
                    }
                    currentLine = "";
                } else if (c != '\r') {
                    currentLine += c;
                }
            }
        }
        client.stop();
        Serial.println("Client disconnected.");
    }
}
