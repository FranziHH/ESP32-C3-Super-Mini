/**************************************************************************************
 * Code for Wi-Fi Signal Strength Monitor
 * Written by Sid for Sid’s E Classroom
 * https://www.youtube.com/c/SidsEClassroom
 * All text above must be included in any redistribution.
 * If you find this useful and want to make a donation -> https://paypal.me/sidsclass
 **************************************************************************************/

// WICHTIG: Stellen Sie sicher, dass Sie die Bibliotheken "Adafruit SSD1306"
// und "Adafruit GFX Library" über den Bibliotheksverwalter in der Arduino IDE installiert haben.

#include <Arduino.h>

#include <WiFi.h>
#include <Wire.h>             // Für I2C-Kommunikation
#include <Adafruit_GFX.h>     // Die grundlegende Grafikbibliothek
#include <Adafruit_SSD1306.h> // Der Treiber für das SSD1306 OLED-Display

#if __has_include ("config.h")
  #include "config.h"
#else 
  #warning config.h not found.
#endif

// Definieren Sie die Bildschirmabmessungen für Ihr OLED-Display
#define SCREEN_WIDTH 128 // OLED-Breite in Pixeln
#define SCREEN_HEIGHT 64 // OLED-Höhe in Pixeln

// Definieren Sie den Reset-Pin für das OLED-Display.
// Bei vielen Modulen ist dieser Pin fest verdrahtet oder nicht nötig.
// Wenn Ihr Display keinen dedizierten Reset-Pin hat, setzen Sie ihn auf -1.
#define OLED_RESET -1 // Reset-Pin (oder -1, wenn kein Reset-Pin verwendet wird)

// Erstellen Sie ein Adafruit_SSD1306 Display-Objekt
// Parameter: (Breite, Höhe, I2C-Bus-Referenz, Reset-Pin)
// Der ESP32-C3 hat nur einen I2C-Bus, daher verwenden wir ‚&Wire‘.
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Die folgenden Werte stammen von https://www.intuitibits.com/2016/03/23/dbm-to-percent-conversion/
int signal_dBM[] = {-100, -99, -98, -97, -96, -95, -94, -93, -92, -91, -90, -89, -88, -87, -86, -85, -84, -83, -82, -81, -80, -79, -78, -77, -76, -75, -74, -73, -72, -71, -70, -69, -68, -67, -66, -65, -64, -63, -62, -61, -60, -59, -58, -57, -56, -55, -54, -53, -52, -51, -50, -49, -48, -47, -46, -45, -44, -43, -42, -41, -40, -39, -38, -37, -36, -35, -34, -33, -32, -31, -30, -29, -28, -27, -26, -25, -24, -23, -22, -21, -20, -19, -18, -17, -16, -15, -14, -13, -12, -11, -10, -9, -8, -7, -6, -5, -4, -3, -2, -1};
int signal_percent[] = {0, 0, 0, 0, 0, 0, 4, 6, 8, 11, 13, 15, 17, 19, 21, 23, 26, 28, 30, 32, 34, 35, 37, 39, 41, 43, 45, 46, 48, 50, 52, 53, 55, 56, 58, 59, 61, 62, 64, 65, 67, 68, 69, 71, 72, 73, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 90, 91, 92, 93, 93, 94, 95, 95, 96, 96, 97, 97, 98, 98, 99, 99, 99, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100};
int strength = 0;
int percentage = 0;

void setup()
{
  Serial.begin(115200);
  Serial.println("ESP32C3 WiFi Signal Strength Checker");

  // Initialisieren des Displays
  // Der Parameter SSD1306_SWITCHCAPVCC ist für Displays, die eine interne
  // Ladungspumpe zur Erzeugung der benötigten 3,3V verwenden.
  // 0x3C ist die Standard-I2C-Adresse für die meisten SSD1306-Displays.
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println(F("SSD1306 Initialisierung fehlgeschlagen!"));
    for (;;)
      ; // Endlosschleife, wenn Display nicht gefunden wird
  }

  // Löschen des Puffers und Anzeigen des Adafruit-Logos (optional)
  display.display();
  delay(2000); // Zeigt das Logo für 2 Sekunden
  display.clearDisplay();

  // Setzen der Textgröße und -farbe
  display.setTextSize(1);              // 1 = Standardgröße, 2 = doppelte Größe, etc.
  display.setTextColor(SSD1306_WHITE); // Textfarbe (weiß auf schwarzem Hintergrund)

  // Die Adafruit GFX Library hat keine direkte ‚flipScreenVertically()‘ oder ’setTextAlignment()‘.
  // ’setRotation()‘ kann verwendet werden, um die Ausrichtung zu ändern.
  // 0 = keine Rotation, 1 = 90 Grad, 2 = 180 Grad, 3 = 270 Grad.
  // Eine Rotation von 2 ist oft das Äquivalent zum vertikalen Flip für 128×64 Displays.
  display.setRotation(0); // Passen Sie dies an Ihre Display-Ausrichtung an

  // Set WiFi to Station Mode
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(10);

  // Connect to WiFi
  WiFi.begin(wifissid, wifipass);
  #ifdef WIFI_POWER
    WiFi.setTxPower(WIFI_POWER_8_5dBm);
  #endif
  Serial.print("Connecting to WiFi...");

  // Manuelles Zentrieren des Textes für das Display
  String connectingText = "Connecting to WiFi...";
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(connectingText, 0, 0, &x1, &y1, &w, &h);
  display.setCursor((SCREEN_WIDTH - w) / 2, 15);
  display.print(connectingText);
  display.display();

  // Check if connected to WiFi
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  // Print connection information
  display.clearDisplay();
  String connectedText = "Connected to " + String(wifissid);
  display.getTextBounds(connectedText, 0, 0, &x1, &y1, &w, &h);
  display.setCursor((SCREEN_WIDTH - w) / 2, 15);
  display.print(connectedText);
  display.display();

  Serial.print("\nConnected to: ");
  Serial.println(wifissid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("");
}

void loop()
{
  // While checking for signal strength, check if ESP got disconnected
  if (WiFi.status() != WL_CONNECTED)
  {
    display.clearDisplay();
    String connectionLostText = "Connection lost";
    int16_t x1, y1;
    uint16_t w, h;
    display.getTextBounds(connectionLostText, 0, 0, &x1, &y1, &w, &h);
    display.setCursor((SCREEN_WIDTH - w) / 2, 15);
    display.print(connectionLostText);
    display.display();
    delay(1000);
  }
  // Get signal strength if ESP is connected to WiFi
  else if (WiFi.status() == WL_CONNECTED)
  {
    display.clearDisplay();
    Serial.println(" ");
    Serial.print("WiFi Signal Strength - ");
    // Print the received signal strength in dBm
    Serial.print("RSSI: ");
    strength = WiFi.RSSI();
    Serial.print(strength);
    Serial.print("dBm");
    Serial.print(" ");
    // Get the signal percentage value
    for (int x = 0; x < 100; x = x + 1)
    {
      if (signal_dBM[x] == strength)
      {
        // Print the received signal strength in percentage
        Serial.print("Percentage: ");
        percentage = signal_percent[x];
        Serial.print(percentage);
        Serial.println("%");
        break;
      }
    }
    drawProgressBar();
    display.display();
    delay(1000);
  }
}

// Using a progress bar for visual strength indication
void drawProgressBar()
{
  // Adafruit GFX hat keine direkte drawProgressBar Funktion.
  // Wir zeichnen einen Balken manuell.
  // drawRect(x, y, width, height, color) für den Rahmen
  // fillRect(x, y, width, height, color) für den gefüllten Teil

  int barX = 10;
  int barY = 32;
  int barWidth = 100;
  int barHeight = 10;

  // Zeichne den Rahmen des Fortschrittsbalkens
  display.drawRect(barX, barY, barWidth, barHeight, SSD1306_WHITE);

  // Berechne die Breite des gefüllten Teils basierend auf dem Prozentsatz
  int filledWidth = (barWidth - 2) * percentage / 100; // -2 für den inneren Bereich, um den Rahmen zu berücksichtigen
  display.fillRect(barX + 1, barY + 1, filledWidth, barHeight - 2, SSD1306_WHITE);

  // Text für Signalstärke und Prozent
  String titleText = "WiFi Signal Strength";
  String strengthPercentText = String(strength) + "dBm" + " | " + String(percentage) + "%";
  String statusText = "";

  if (percentage == 0)
  {
    statusText = "No Signal";
  }
  else if (percentage == 100)
  {
    statusText = "Maximum Signal";
  }

  int16_t x1, y1;
  uint16_t w, h;

  // Zentriere den Titel
  display.getTextBounds(titleText, 0, 0, &x1, &y1, &w, &h);
  display.setCursor((SCREEN_WIDTH - w) / 2, 0);
  display.print(titleText);

  // Zentriere die dBm/Prozent-Anzeige
  display.getTextBounds(strengthPercentText, 0, 0, &x1, &y1, &w, &h);
  display.setCursor((SCREEN_WIDTH - w) / 2, 15);
  display.print(strengthPercentText);

  // Zentriere den Status-Text
  if (statusText != "")
  {
    display.getTextBounds(statusText, 0, 0, &x1, &y1, &w, &h);
    display.setCursor((SCREEN_WIDTH - w) / 2, 45);
    display.print(statusText);
  }
}