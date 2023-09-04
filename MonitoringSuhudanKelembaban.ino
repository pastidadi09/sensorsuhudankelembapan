#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>

#define WIFI_SSID "Nama WIFI" //ganti nama wifi
#define WIFI_PASSWORD "PASSword WIFI" // ganti password wifi
#define serverUrl "http://xxx-xxx-xxx-xxx/monitoringsuhu/temperature_data.php" // ganti alamat url dengan alamat IP yang ada di wifi

#define DHTPIN 4 // Pin yang terhubung dengan output sensor DHT11
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);

  // Menghubungkan ke jaringan WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  dht.begin();

}

void loop() {
  delay(2000);

  float temperature = dht.readTemperature(); // Membaca suhu dalam Celsius
  float humidity = dht.readHumidity();       // Membaca kelembaban

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C\t");
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  // Membuat data untuk dikirim sebagai POST request
  String postData = "temperature=" + String(temperature) + "&humidity=" + String(humidity);

  // Membuat objek HTTPClient
  HTTPClient http;

  // Mengirim data ke server menggunakan metode POST
  http.begin(serverUrl);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  int httpResponseCode = http.POST(postData);

  if (httpResponseCode > 0) {
    // Serial.print("HTTP Response code: ");
    // Serial.println(httpResponseCode);
    String response = http.getString();
    Serial.println(response);
  } else {
    Serial.print("Error sending POST request. HTTP Response code: ");
    Serial.println(httpResponseCode);
  }

  http.end();
}
