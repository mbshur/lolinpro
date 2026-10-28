void CHZtanimla(String icerikMetni) {
  // Örnek girdi: "ST=M1:2,3|SR=SR1:4|SR=SR2:5|PW=RLED:6|AN=POT1:A0"
  toplamStep = 0; toplamServo = 0; toplamPwm = 0; toplamAnalog = 0;

  int baslangic = 0;
  while (baslangic < icerikMetni.length()) {
    int sonrakiBoru = icerikMetni.indexOf('|', baslangic);
    if (sonrakiBoru == -1) sonrakiBoru = icerikMetni.length();

    String parca = icerikMetni.substring(baslangic, sonrakiBoru);
    parca.trim();
    baslangic = sonrakiBoru + 1;

    if (parca == "") continue;

    int esitIdx = parca.indexOf('=');
    int ikiNoktaIdx = parca.indexOf(':');

    if (esitIdx > -1 && ikiNoktaIdx > esitIdx) {
      String modulTipi = parca.substring(0, esitIdx);       
      String modulAdi = parca.substring(esitIdx + 1, ikiNoktaIdx); 
      String pinlerStr = parca.substring(ikiNoktaIdx + 1);    
      modulTipi.trim(); modulAdi.trim(); pinlerStr.trim();

      // 🎯 1. STEP MOTOR AYRIŞTIRMA (ST)
      if (modulTipi == "ST" && toplamStep < MAX_MODUL) {
        int virgulIdx = pinlerStr.indexOf(',');
        if (virgulIdx > -1) {
          stepIsimleri[toplamStep] = modulAdi;
          stepPin1[toplamStep] = pinlerStr.substring(0, virgulIdx).toInt(); 
          stepPin2[toplamStep] = pinlerStr.substring(virgulIdx + 1).toInt();
          
          pinMode(stepPin1[toplamStep], OUTPUT);
          pinMode(stepPin2[toplamStep], OUTPUT);
          
          Serial.println("⚙️ [Tanim] Step Motor: " + modulAdi + " -> Pin: " + String(stepPin1[toplamStep]) + "," + String(stepPin2[toplamStep]));
          toplamStep++;
        }
      }
      // 🎯 2. SERVO MOTOR AYRIŞTIRMA (SR)
      else if (modulTipi == "SR" && toplamServo < MAX_MODUL) {
        servoIsimleri[toplamServo] = modulAdi;
        servoPin[toplamServo] = pinlerStr.toInt();
        Serial.println("⚙️ [Tanim] Servo Motor: " + modulAdi + " -> Pin: " + String(servoPin[toplamServo]));
        toplamServo++;
      }
      // 🎯 3. PWM ÇIKIŞ AYRIŞTIRMA (PW)
      else if (modulTipi == "PW" && toplamPwm < MAX_MODUL) {
        pwmIsimleri[toplamPwm] = modulAdi;
        pwmPin[toplamPwm] = pinlerStr.toInt();
        pinMode(pwmPin[toplamPwm], OUTPUT);
        
        Serial.println("⚙️ [Tanim] PWM Cikis: " + modulAdi + " -> Pin: " + String(pwmPin[toplamPwm]));
        toplamPwm++;
      }
      // 🎯 4. ANALOG GİRİŞ AYRIŞTIRMA (AN)
      else if (modulTipi == "AN" && toplamAnalog < MAX_MODUL) {
        analogIsimleri[toplamAnalog] = modulAdi; // Örn: "POT1"
        
        // Gelen pin metnini temizleyelim (Örn: "A0" veya "0")
        pinlerStr.toUpperCase();
        
        if (pinlerStr.indexOf('A') > -1) {
          // "A0" içindeki 'A' harfini silip sadece rakamı alıyoruz (Örn: "0")
          int aNo = pinlerStr.substring(pinlerStr.indexOf('A') + 1).toInt();
          analogPin[toplamAnalog] = A0 + aNo; // Arduino'da A0 + 0 = A0, A0 + 1 = A1 olur
        } else {
          // Eğer doğrudan sadece rakam yazıldıysa (Örn: "0" -> A0 anlamına gelir)
          int aNo = pinlerStr.toInt();
          analogPin[toplamAnalog] = A0 + aNo;
        }
        
        // Pini fiziksel olarak giriş (INPUT) moduna alıyoruz
        pinMode(analogPin[toplamAnalog], INPUT);
        
        Serial.println("⚙️ [Tanim] Analog Giris: " + modulAdi + " -> Pin MCU: " + String(analogPin[toplamAnalog]));
        toplamAnalog++;
      }
            // 🎯 5. DİJİTAL GİRİŞ AYRIŞTIRMA (DI) -> Örn: DI=SW1:7,0
      else if (modulTipi == "DI" && toplamDi < MAX_MODUL) {
        diIsimleri[toplamDi] = modulAdi; // "SW1"
        
        int virgulIdx = pinlerStr.indexOf(',');
        if (virgulIdx > -1) {
          diPin[toplamDi] = pinlerStr.substring(0, virgulIdx).toInt(); // 7
          diNormalDurum[toplamDi] = pinlerStr.substring(virgulIdx + 1).toInt(); // 0
        } else {
          diPin[toplamDi] = pinlerStr.toInt();
          diNormalDurum[toplamDi] = 0; // Eğer belirtilmediyse varsayılan normal durum 0 (LOW)
        }
        
        pinMode(diPin[toplamDi], INPUT); // Fiziksel pini giriş yap
        Serial.println("⚙️ [Tanim] Dijital Giris: " + modulAdi + " -> Pin: " + String(diPin[toplamDi]) + " (Normal: " + String(diNormalDurum[toplamDi]) + ")");
        toplamDi++;
      }
      
      // 🎯 6. DİJİTAL ÇIKIŞ AYRIŞTIRMA (DO) -> Örn: DO=ROLE:8
      else if (modulTipi == "DO" && toplamDo < MAX_MODUL) {
        doIsimleri[toplamDo] = modulAdi; // "ROLE"
        doPin[toplamDo] = pinlerStr.toInt(); // 8
        
        pinMode(doPin[toplamDo], OUTPUT); // Fiziksel pini çıkış yap
        digitalWrite(doPin[toplamDo], LOW); // Başlangıçta güvenli mod (Kapat)
        Serial.println("⚙️ [Tanim] Dijital Cikis: " + modulAdi + " -> Pin: " + String(doPin[toplamDo]));
        toplamDo++;
      }
    }
  }
}