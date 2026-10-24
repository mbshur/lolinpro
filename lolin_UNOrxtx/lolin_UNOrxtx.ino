//donanımsal seri portu (Hardware Serial yani 
// D0/RX ve D1/TX pinlerini) kullanıyoruz,
#define CIHAZ_ADI "UNO1" // İkinci denemede "UNO2" yapacaksınız
const int ledPin = 13;   // Temsili animasyon pini (Dahili led)

void setup() {
  // Donanımsal seri port 57600 baud hızında başlatılıyor (Halkadaki tek hız)
  Serial.begin(19200);   
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // Donanımsal porta veri geldi mi?
  if (Serial.available() > 0) {
    // Satır sonu karakterine (\n) kadar oku
    String gelenVeri = Serial.readStringUntil('\n');
    gelenVeri.trim(); 

    if (gelenVeri.length() > 0) {
      String arananEtiket = String(CIHAZ_ADI) + ":";
      
      // Filtre: Gelen komut tam olarak bu cihaz adına mı ait?
      if (gelenVeri.indexOf(arananEtiket) == 0) {
        String animasyonKomutu = gelenVeri.substring(arananEtiket.length());

        // Gelen animasyon komutuna göre işlem yap
        if (animasyonKomutu == "ANI1") {
          digitalWrite(ledPin, HIGH); // Animasyon tetiklendi (Led yandı)
          delay(1000);                // 1 saniye bekle
          digitalWrite(ledPin, LOW);
        } 
        else if (animasyonKomutu == "ANI2") {
          // Gerekirse diğer animasyonlar...
        }
      }

      // 🔁 Donanımsal halka (Token Ring) aktarımı:
      // Tek bir Uno ile test ederken kablo bağlı olmasa da bu satır çalışabilir,
      // hattı uç uca eklediğinizde veriyi bir sonraki Uno'ya veya ESP RX'e iletir.
      Serial.println(gelenVeri); 
    }
  }
}