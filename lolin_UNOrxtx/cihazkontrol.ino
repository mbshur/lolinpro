Servo servoNesneleri[MAX_MODUL];
bool servoBaglandi[MAX_MODUL] = {false};
// --- 🚨 ACİL DURDURMA PİN TANIMLAMALARI ---


void CHZkontrol() {
  if (CHZadi == "") return;

  // 🚨 2. SENARYO (Gelişmiş): Canlı Dijital Giriş (DI) ve Akıllı Arıza Bildirimi
  // Hafızaya alınmış tüm limit anahtarlarını/sensörleri tek tek yokla
  for (int i = 0; i < toplamDi; i++) {
    int anlikFizikselDurum = digitalRead(diPin[i]);
    
    // Eğer pindeki canlı durum, dosyanın belirttiği "Normal Durum" değerine eşit değilse (Arıza var!)
    if (anlikFizikselDurum != diNormalDurum[i]) {
      // 🚀 ESP'ye akıllı hata paketi fırlatıyoruz! Format: Cihaz:ERR:AnahtarAdi=HatalıDurum
      Serial.println(CHZadi + ":ERR:" + diIsimleri[i] + "=" + String(anlikFizikselDurum));
      
      Serial.print(F("🛑 [ACİL STOP] "));
      Serial.print(diIsimleri[i]);
      Serial.print(F(" hatti normal durumundan sapti! Canli Değer: "));
      Serial.println(anlikFizikselDurum);

      // Güvenlik koruması: Tüm step motorları anında durdur
      for (int m = 0; m < toplamStep; m++) {
        digitalWrite(stepPin1[m], LOW);
        digitalWrite(stepPin2[m], LOW);
      }
      
      // Tüm dijital çıkışları (Röleleri vb.) emniyet için kapat
      for (int r = 0; r < toplamDo; r++) {
        digitalWrite(doPin[r], LOW);
      }

      rxdata = ""; // Yeni komut işlemeyi engelle
      return;
    }
  }

  // 🎯 rxdata içinde işlenecek geçerli bir komut var mı?
  if (rxdata == "") return;
  rxdata.trim();

  int ikiNoktaIdx = rxdata.indexOf(':');
  if (ikiNoktaIdx == -1) {
    Serial.println(CHZadi + ":ERR:PROTOKOL");
    rxdata = ""; 
    return;
  }

  String arananCihazAdi = rxdata.substring(0, ikiNoktaIdx);      
  String parametreler = rxdata.substring(ikiNoktaIdx + 1);  
  arananCihazAdi.trim(); parametreler.trim();

  bool cihazBulundu = false;

  // --- [ST] STEP MOTOR KONTROLÜ (Mevcut kodunuz aynen kalıyor) ---
  for (int i = 0; i < toplamStep; i++) {
    if (stepIsimleri[i] == arananCihazAdi) {
      int pinStep = stepPin1[i]; int pinDir = stepPin2[i]; cihazBulundu = true;
      int virgulIdx = parametreler.indexOf(',');
      if (virgulIdx == -1) { Serial.println(CHZadi + ":ERR:PARAMETRE"); break; }

      String yonStr = parametreler.substring(0, virgulIdx);
      String adimStr = parametreler.substring(virgulIdx + 1);
      yonStr.trim(); adimStr.trim();

      if (yonStr == "01") digitalWrite(pinDir, HIGH); else digitalWrite(pinDir, LOW);
      int adimSayisi = adimStr.toInt();

      for(int x = 0; x < adimSayisi; x++) {
        // 🚨 Hareket esnasında da dinamik DI kontrolü yapalım ki motor mekanizmayı kırmasın
        for (int k = 0; k < toplamDi; k++) {
          if (digitalRead(diPin[k]) != diNormalDurum[k]) {
            Serial.println(CHZadi + ":ERR:" + diIsimleri[k] + "=" + String(digitalRead(diPin[k])));
            digitalWrite(pinStep, LOW);
            rxdata = "";
            return;
          }
        }
        digitalWrite(pinStep, HIGH); delayMicroseconds(500);
        digitalWrite(pinStep, LOW);  delayMicroseconds(500);
      }
      Serial.print(F("✅ [ST] ")); Serial.print(arananCihazAdi); Serial.println(F(" adimlamayi tamamladi."));
      break; 
    }
  }

  // --- [SR] SERVO MOTOR KONTROLÜ (Mevcut kodunuz...) ---
  if (!cihazBulundu) {
    for (int i = 0; i < toplamServo; i++) {
      if (servoIsimleri[i] == arananCihazAdi) {
        int pinNumarasi = servoPin[i]; int aci = parametreler.toInt(); cihazBulundu = true;
        if (!servoBaglandi[i]) { servoNesneleri[i].attach(pinNumarasi); servoBaglandi[i] = true; }
        servoNesneleri[i].write(aci);
        Serial.println("🎯 [SR] Servo: " + arananCihazAdi + " -> Aci: " + String(aci));
        break;
      }
    }
  }

  // --- [PW] PWM ÇIKIŞ KONTROLÜ (Mevcut kodunuz...) ---
  if (!cihazBulundu) {
    for (int i = 0; i < toplamPwm; i++) {
      if (pwmIsimleri[i] == arananCihazAdi) {
        int pinNumarasi = pwmPin[i]; int pwmDegeri = parametreler.toInt(); cihazBulundu = true;
        analogWrite(pinNumarasi, pwmDegeri); 
        Serial.println("🎯 [PW] PWM Cikisi: " + arananCihazAdi + " -> Deger: " + String(pwmDegeri));
        break;
      }
    }
  }

  // --- 🎯 6B. ADIM: [DO] DİJİTAL ÇIKIŞ / RÖLE KONTROL MOTORU ---
  if (!cihazBulundu) {
    for (int i = 0; i < toplamDo; i++) {
      if (doIsimleri[i] == arananCihazAdi) {
        int pinNumarasi = doPin[i];
        int cikisDurumu = parametreler.toInt(); // "1" veya "0"
        cihazBulundu = true;

        if (cikisDurumu == 1) {
          digitalWrite(pinNumarasi, HIGH); // Röleyi çek/Aç
        } else {
          digitalWrite(pinNumarasi, LOW);  // Röleyi bırak/Kapat
        }
        
        Serial.print(F("🎯 [DO] Dijital Cikis: "));
        Serial.print(arananCihazAdi);
        Serial.print(F(" -> Durum: "));
        Serial.println(cikisDurumu == 1 ? F("HIGH (ACIK)") : F("LOW (KAPALI)"));
        break;
      }
    }
  }

  if (!cihazBulundu) {
    Serial.println(CHZadi + ":ERR:KAYIP_CIHAZ");
  }

  rxdata = ""; 
}