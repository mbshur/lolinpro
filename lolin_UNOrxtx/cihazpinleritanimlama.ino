void CHZtanimla(String icerikMetni) {
  // 🛡️ RAM Temizliği ve Sayaç Sıfırlamaları
  toplamStep = 0; toplamServo = 0; toplamPwm = 0; toplamAnalog = 0;
  toplamDi = 0; toplamDo = 0; 

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
          
          // 🧠 F() Makrosu ile RAM koruması sağlandı
          Serial.print(F("⚙️ [Tanim] Step Motor: ")); Serial.print(modulAdi);
          Serial.print(F(" -> Pin: ")); Serial.print(stepPin1[toplamStep]);
          Serial.print(F(",")); Serial.println(stepPin2[toplamStep]);
          toplamStep++;
        }
      }
      // 🎯 2. SERVO MOTOR AYRIŞTIRMA (SR)
      else if (modulTipi == "SR" && toplamServo < MAX_MODUL) {
        servoIsimleri[toplamServo] = modulAdi;
        servoPin[toplamServo] = pinlerStr.toInt();
        
        servoBaglandi[toplamServo] = false;
        servoHedefAci[toplamServo] = 90;
        servoAnlikAci[toplamServo] = 90;
        servoAdimGecikmesi[toplamServo] = 15; 

        Serial.print(F("⚙️ [Tanim] Servo Motor: ")); Serial.print(modulAdi);
        Serial.print(F(" -> Pin: ")); Serial.println(servoPin[toplamServo]);
        toplamServo++;
      }
      // 🎯 3. PWM ÇIKIŞ AYRIŞTIRMA (PW)
      else if (modulTipi == "PW" && toplamPwm < MAX_MODUL) {
        pwmIsimleri[toplamPwm] = modulAdi;
        pwmPin[toplamPwm] = pinlerStr.toInt();
        pinMode(pwmPin[toplamPwm], OUTPUT);
        
        Serial.print(F("⚙️ [Tanim] PWM Cikis: ")); Serial.print(modulAdi);
        Serial.print(F(" -> Pin: ")); Serial.println(pwmPin[toplamPwm]);
        toplamPwm++;
      }
      // 🎯 4. ANALOG GİRİŞ AYRIŞTIRMA (AN)
      else if (modulTipi == "AN" && toplamAnalog < MAX_MODUL) {
        analogIsimleri[toplamAnalog] = modulAdi; 
        pinlerStr.toUpperCase();
        
        if (pinlerStr.indexOf('A') > -1) {
          int aNo = pinlerStr.substring(pinlerStr.indexOf('A') + 1).toInt();
          analogPin[toplamAnalog] = A0 + aNo; 
        } else {
          int aNo = pinlerStr.toInt();
          analogPin[toplamAnalog] = A0 + aNo;
        }
        
        pinMode(analogPin[toplamAnalog], INPUT);
        
        Serial.print(F("⚙️ [Tanim] Analog Giris: ")); Serial.print(modulAdi);
        Serial.print(F(" -> Pin MCU: ")); Serial.println(analogPin[toplamAnalog]);
        toplamAnalog++;
      }
      // 🎯 5. DİJİTAL GİRİŞ AYRIŞTIRMA (DI)
      else if (modulTipi == "DI" && toplamDi < MAX_MODUL) {
        diIsimleri[toplamDi] = modulAdi; 
        
        int virgulIdx = pinlerStr.indexOf(',');
        if (virgulIdx > -1) {
          diPin[toplamDi] = pinlerStr.substring(0, virgulIdx).toInt(); 
          diNormalDurum[toplamDi] = pinlerStr.substring(virgulIdx + 1).toInt(); 
        } else {
          diPin[toplamDi] = pinlerStr.toInt();
          diNormalDurum[toplamDi] = 0; 
        }
        
        pinMode(diPin[toplamDi], INPUT); 
        Serial.print(F("⚙️ [Tanim] Dijital Giris: ")); Serial.print(modulAdi);
        Serial.print(F(" -> Pin: ")); Serial.print(diPin[toplamDi]);
        Serial.print(F(" (Normal: ")); Serial.print(diNormalDurum[toplamDi]); Serial.println(F(")"));
        toplamDi++;
      }
      // 🎯 6. DİJİTAL ÇIKIŞ AYRIŞTIRMA (DO)
      else if (modulTipi == "DO" && toplamDo < MAX_MODUL) {
        doIsimleri[toplamDo] = modulAdi; 
        doPin[toplamDo] = pinlerStr.toInt(); 
        
        pinMode(doPin[toplamDo], OUTPUT); 
        digitalWrite(doPin[toplamDo], LOW); 
        
        Serial.print(F("⚙️ [Tanim] Dijital Cikis: ~")); Serial.print(doIsimleri[toplamDo]);
        Serial.print(F("~ -> Pin: ")); Serial.println(doPin[toplamDo]);
        toplamDo++;
      }
    }
  }
}