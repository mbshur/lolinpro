#include <SoftwareSerial.h>



SoftwareSerial unoSerial;

void rxtxsetup() {
rxtxvar=true;  
const int8_t ESP_RX_PIN = D6; // Uno'nun TX'inden (direnç korumalı) gelen hat
const int8_t ESP_TX_PIN = D7; // Uno'nun RX'ine giden hat
const uint32_t BAUD_RATE = 57600;

  // Software Serial pin yapılandırması ve 19200 baud başlatılması
  // (RX, TX) parametreleri sırasıyla verilir
// Yazılımsal portu sabit pinlerimiz ve 19200 baud ile başlatıyoruz
  unoSerial.begin(BAUD_RATE, SWSERIAL_8N1, ESP_RX_PIN, ESP_TX_PIN, false);
  
  Serial.println(F("Sistem Hazır. D6(RX) ve D7(TX) üzerinden 19200 baud ile dinleniyor..."));
}

void rxtxloop() {
  // Uno'dan veri gelip gelmediğini kontrol et
  if (unoSerial.available()) {
    String gelenVeri = unoSerial.readStringUntil('\n');
    gelenVeri.trim();
    
    rxtampon += gelenVeri;

    // Gelen veriyi (D1|OUT|DIG|...) ekrana bas
    Serial.printf_P(PSTR("Gelen Paket: %s\n"), gelenVeri.c_str());
    
    // Ayrıştırma ve işleme mantığınızı buraya ekleyebilirsiniz
  }
  }

void txsend(String txveri)
{
   unoSerial.println(txveri);
}