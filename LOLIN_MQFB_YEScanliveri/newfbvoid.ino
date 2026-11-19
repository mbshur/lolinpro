

// 1. NOKTA ATIŞI VERİ GÜNCELLEME (PUT) FONKSİYONU
void firebasePutData(const char* url, String stringVeri) {
  fbisleniyor = true;
  
  // Fonksiyon içindeki eski client satırını sildik, global fbClient'ı yapılandırıyoruz
  fbClient.setInsecure(); 
  fbClient.setBufferSizes(512, 512); 

  HTTPClient http;
  http.setTimeout(4000); // Zaman aşımını biraz daha düşürüp kuyruğu rahatlattık
  
  if (http.begin(fbClient, url)) { // fbClient kullanılıyor
    http.addHeader("Content-Type", "application/json");
    String payload = "\"" + stringVeri + "\"";
    int httpResponseCode = http.PUT(payload); 
    
    if (httpResponseCode > 0) {
      Serial.printf("Yazma Basarili! Kod: %d\n", httpResponseCode);
    } else {
      Serial.printf("Yazma Hatasi: %s\n", http.errorToString(httpResponseCode).c_str());
    }
    http.end();
  }
  fbisleniyor = false;
}

String firebaseGetData(const char* url) {
  fbisleniyor = true;
  
  fbClient.setInsecure();
  fbClient.setBufferSizes(512, 512);

  HTTPClient http;
  http.setTimeout(4000);
  
  String sonuc = "";
  if (http.begin(fbClient, url)) { // fbClient kullanılıyor
    int httpResponseCode = http.GET(); 
    if (httpResponseCode == 200) {
      sonuc = http.getString();
      sonuc.trim();
      if(sonuc.startsWith("\"") && sonuc.endsWith("\"")) {
        sonuc = sonuc.substring(1, sonuc.length() - 1);
      }
    } else {
      Serial.printf("Okuma Hatasi: %s\n", http.errorToString(httpResponseCode).c_str());
    }
    http.end();
  }
  fbisleniyor = false;
  return sonuc; 
}


/*
void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    

    
    Serial.println("\n>>> Dinamik Adrese Pin Durumları Güncelleniyor...");
    String dat = "D1:0,D2:1,D3:0,D6:0,D7:1,";
    
    // .c_str() kullanarak String nesnesini const char* tipine dönüştürüyoruz
    firebasePutData(tamUrl.c_str(), dat); 

    delay(3000);

    Serial.println("\n>>> Dinamik Adresten Paylaşım Bilgisi Okunuyor...");
    String payYolu = "kev1/pays/MUTFAKpay";
    String tamPayUrl = anaUrl + payYolu + ".json";
    
    String mutfakAyarlari = firebaseGetData(tamPayUrl.c_str());
    Serial.print("Gelen Metin: ");
    Serial.println(mutfakAyarlari);
  }
  
  delay(20000);
}
*/