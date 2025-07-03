#define BLYNK_TEMPLATE_ID "TMPL36Q7drwKC"
#define BLYNK_TEMPLATE_NAME "Air Quality Monitoring Systen Control"
#define BLYNK_AUTH_TOKEN "0I7j2X0bSHsVq2nBgHgr6EAFLwyaxccd"


#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD setup (change 0x27 to 0x3F if your LCD has a different I2C address)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// WiFi credentials
const char* ssid = "Redmi A4 5G";
const char* password = "123456789";

// Telegram Bot Token and Chat ID
const char* botToken = "7554875965:AAGmE3OGqxH-SvOqqF8F3C_THh3CyxfJ9uI"; // Get this from BotFather
const String chatId = "2073248603"; // Your Telegram user ID

// Create secure client and bot instance
WiFiClientSecure client;
UniversalTelegramBot bot(botToken, client);

// Gas sensor threshold
const int gasThreshold = 200;

// Timing
unsigned long lastSendTime = 0;
const unsigned long sendInterval = 10000; // 10 seconds between alerts

void setup() {
  Serial.begin(115200);

  // LCD init
  lcd.begin(16,2);
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Connecting WiFi");

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi!");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WiFi Connected");
  lcd.setCursor(0, 1);
  lcd.print("Monitoring...");

  // Allow insecure SSL (Telegram cert may not be verified)
  client.setInsecure();

  // Notify via Telegram
  bot.sendMessage(chatId, "ESP8266 is connected to WiFi and monitoring gas levels.", "");
}

void loop() {
  int gasValue = analogRead(A0);
  Serial.println("Gas Sensor Value: " + String(gasValue));

  // Display gas value on LCD
  lcd.setCursor(0, 0);
  lcd.print("Gas: ");
  lcd.print(gasValue);
  lcd.print("    "); // Clear remaining digits

  if (gasValue >= gasThreshold && millis() - lastSendTime > sendInterval) {
    String alertMsg = "🚨 Gas Alert!\nValue: " + String(gasValue) + "\nBe cautious!";
    bot.sendMessage(chatId, alertMsg, "");
    
    lcd.setCursor(0, 1);
    lcd.print("!! GAS ALERT !!  ");
    lastSendTime = millis();
  } else {
    lcd.setCursor(0, 1);
    lcd.print("Status: Safe     ");
  }

  delay(1000); // Wait before next reading
}
