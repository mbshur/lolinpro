/*


    // Eğer arıza/limit aşımı varsa
    if (anlikFizikselDurum != diNormalDurum[i]) {
      //Serial.println(CHZadi + ":ERR:" + diIsimleri[i] + "=" + String(anlikFizikselDurum));

      //Serial.print(F("🛑 [ACİL STOP] "));
      //Serial.print(diIsimleri[i]);
      //Serial.print(F(" hatti normal durumundan sapti! Canli Değer: "));
      //Serial.println(anlikFizikselDurum);

      // Güvenlik koruması: Tüm step motorları anında durdur
      for (int m = 0; m < toplamStep; m++) {
        digitalWrite(stepPin1[m], LOW);
        digitalWrite(stepPin2[m], LOW);
        stepKalanAdim[m] = 0;  // 🚨 İŞTE TAM BURAYA EKLEYİN! (Arka plan adımlarını da sıfırlar)
      }

      // Tüm dijital çıkışları (Röleleri vb.) emniyet için kapat
      for (int r = 0; r < toplamDo; r++) {
        digitalWrite(doPin[r], LOW);
      }


*/

void CHZkontrol() {
  if (CHZadi == "") return;


  // 🎯 rxdata içinde işlenecek geçerli bir komut var mı?
  if (rxdata == "") return;
  rxdata.trim();

  int EsittirIdx = rxdata.indexOf('=');
  if (EsittirIdx == -1) {
    //Serial.println(CHZadi + ":ERR:PROTOKOL");
    rxdata = "";
    return;
  }

  String arananCihazAdi = rxdata.substring(0, EsittirIdx);
  String parametreler = rxdata.substring(EsittirIdx + 1);
  arananCihazAdi.trim();
  parametreler.trim();

  bool cihazBulundu = false;




  // --- [ST] STEP MOTOR KONTROLÜ (DİNAMİK HIZLI) ---
  for (int i = 0; i < toplamStep; i++) {
    if (stepIsimleri[i] == arananCihazAdi) {
      int pinStep = stepPin1[i];
      int pinDir = stepPin2[i];
      cihazBulundu = true;

      // İlk virgülü bul (Yön ile Adım Sayısı arasındaki)
      int ilkVirgulIdx = parametreler.indexOf(',');
      if (ilkVirgulIdx == -1) {  //Serial.println(CHZadi + ":ERR:PARAMETRE");
        break;
      }

      String yonStr = parametreler.substring(0, ilkVirgulIdx);
      String kalanParametreler = parametreler.substring(ilkVirgulIdx + 1);
      yonStr.trim();
      kalanParametreler.trim();

      // İkinci virgülü bul (Adım Sayısı ile Hız arasındaki)
      int ikinciVirgulIdx = kalanParametreler.indexOf(',');

      String adimStr = "";
      if (ikinciVirgulIdx != -1) {
        // Eğer hız da gönderildiyse (Örn: 01,100,800)
        adimStr = kalanParametreler.substring(0, ikinciVirgulIdx);
        String hizStr = kalanParametreler.substring(ikinciVirgulIdx + 1);
        adimStr.trim();
        hizStr.trim();

        stepAdimGecikmesi[i] = hizStr.toInt();  // Bu motorun mikrosaniye hızını güncelle
      } else {
        // Hız gönderilmediyse eski sistem (Örn: 01,100), varsayılan hız (500us) kalır
        adimStr = kalanParametreler;
        adimStr.trim();
      }

      // Yön pinini ayarla
      if (yonStr == "01") digitalWrite(pinDir, HIGH);
      else digitalWrite(pinDir, LOW);

      // Arka plana değerleri teslim et
      stepKalanAdim[i] = adimStr.toInt();
      stepAktifPin[i] = pinStep;

      //Serial.println("⚙️ [ST] Motor: " + arananCihazAdi + " -> Adim: " + adimStr + " | Hiz: " + String(stepAdimGecikmesi[i]) + "us");
      break;
    }
  }

  // --- [SR] SERVO MOTOR KONTROLÜ (GÜNCELLENDİ) ---
  if (!cihazBulundu) {
    for (int i = 0; i < toplamServo; i++) {
      if (servoIsimleri[i] == arananCihazAdi) {
        int pinNumarasi = servoPin[i];
        cihazBulundu = true;

        int aci = 90;
        // Virgül kontrolü (Örn: SR0:120,30 veya SR0:120)
        int virgulIdx = parametreler.indexOf(',');
        if (virgulIdx != -1) {
          String aciStr = parametreler.substring(0, virgulIdx);
          String hizStr = parametreler.substring(virgulIdx + 1);
          aciStr.trim();
          hizStr.trim();
          aci = aciStr.toInt();
          servoAdimGecikmesi[i] = hizStr.toInt();  // Özel hız atandı
        } else {
          aci = parametreler.toInt();  // Hız belirtilmediyse sadece açı al
        }

        // Eğer bu servo sisteme İLK DEFA komut alıyorsa:
        if (!servoBaglandi[i]) {
          servoNesneleri[i].attach(pinNumarasi);
          servoBaglandi[i] = true;
          servoAnlikAci[i] = aci;        // Başlangıç pozisyonunu gelen ilk değer yap
          servoNesneleri[i].write(aci);  // İlk seferde direkt konuma gitsin
        }

        // Sonraki hareketler için hedefi güncelle (Yumuşak hareket fonksiyonu bunu yürütecek)
        servoHedefAci[i] = aci;

        //Serial.println("🎯 [SR] Servo: " + arananCihazAdi + " -> Hedef Aci: " + String(aci) + " | Hiz Gecikmesi: " + String(servoAdimGecikmesi[i]) + "ms");
        break;
      }
    }
  }

  // --- [PW] PWM ÇIKIŞ KONTROLÜ (DİNAMİK HIZLI) ---
  if (!cihazBulundu) {
    for (int i = 0; i < toplamPwm; i++) {
      if (pwmIsimleri[i] == arananCihazAdi) {
        cihazBulundu = true;

        int hedef = 0;
        int virgulIdx = parametreler.indexOf(',');
        if (virgulIdx != -1) {
          // Eğer hız da gönderildiyse (Örn: RLED:255,20 -> 255 değerine her adımda 20ms bekleyerek git)
          String degerStr = parametreler.substring(0, virgulIdx);
          String hizStr = parametreler.substring(virgulIdx + 1);
          degerStr.trim();
          hizStr.trim();

          hedef = degerStr.toInt();
          pwmAdimGecikmesi[i] = hizStr.toInt();  // Bu PWM çıkışının hızını güncelle
        } else {
          // Hız belirtilmediyse varsayılan hızı (5ms) koru
          hedef = parametreler.toInt();
        }

        pwmHedefDeger[i] = hedef;
        //Serial.println("🎯 [PW] PWM: " + arananCihazAdi + " -> Hedef: " + String(hedef) + " | Adim Gecikmesi: " + String(pwmAdimGecikmesi[i]) + "ms");
        break;
      }
    }
  }

  // --- 🎯 6B. ADIM: [DO] DİJİTAL ÇIKIŞ / RÖLE KONTROL MOTORU ---
  if (!cihazBulundu) {
    //Serial.println(arananCihazAdi);
    for (int i = 0; i < toplamDo; i++) {
      if (doIsimleri[i] == arananCihazAdi) {
        int pinNumarasi = doPin[i];
        int cikisDurumu = parametreler.toInt();  // "1" veya "0"
        cihazBulundu = true;

        if (cikisDurumu == 1) {
          digitalWrite(pinNumarasi, HIGH);  // Röleyi çek/Aç
        } else {
          digitalWrite(pinNumarasi, LOW);  // Röleyi bırak/Kapat
        }

        //Serial.print(F("🎯 [DO] Dijital Cikis: "));
        //Serial.print(arananCihazAdi);
        //Serial.print(F(" -> Durum: "));
        //Serial.println(cikisDurumu == 1 ? F("HIGH (ACIK)") : F("LOW (KAPALI)"));
        break;
      }
    }
  }

  if (!cihazBulundu) {
    Serial.println("aranankomut:" + arananCihazAdi);
    Serial.println("parametreler:" + parametreler);
    if (arananCihazAdi.indexOf("RZ") > -1) {
      if (parametreler.indexOf("?") > -1) {
        raporlanacakcihazismi = parametreler.substring(0, parametreler.indexOf("?"));

        if (parametreler.lastIndexOf("?") + 1 < parametreler.length()) raporzamanlayici = (parametreler.substring(parametreler.indexOf("?") + 1)).toInt();
        else raporzamanlayici = 0;


      } else {
        raporlanacakcihazismi = "";
        raporzamanlayici = parametreler.toInt();
      }


      if (raporlanacakcihazismi != "") {
        for (int i = 0; i < toplamAnalog; i++) {
          if (raporlanacakcihazismi == analogIsimleri[i]) {
            analogRapor[i] = true;
            analogPinEdeg[i] = analogPin[i];
          }
        }
        for (int i = 0; i < toplamDi; i++) {
          if (raporlanacakcihazismi == diIsimleri[i]) {
            diRapor[i] = true;
            diPinEdeg[i] = diPinEdeg[i];
          }
        }
      }
      cihazBulundu = true;
    }
  }



  if (!cihazBulundu) {
    //Serial.println(CHZadi + ":ERR:KAYIP_CIHAZ");
  }
  rxdata = "";
}


void servoYumusakHareketYonetimi() {
  unsigned long simdikiZaman = millis();

  for (int i = 0; i < toplamServo; i++) {
    if (!servoBaglandi[i]) continue;

    if (servoAnlikAci[i] != servoHedefAci[i]) {
      if (simdikiZaman - servoSonGuncellemeZamani[i] >= (unsigned long)servoAdimGecikmesi[i]) {
        servoSonGuncellemeZamani[i] = simdikiZaman;

        if (servoAnlikAci[i] < servoHedefAci[i]) {
          servoAnlikAci[i] += 1;
        } else if (servoAnlikAci[i] > servoHedefAci[i]) {
          servoAnlikAci[i] -= 1;
        }

        // --- 🛠️ YENİ KÜTÜPHANE UYUMLAMA SATIRI ---
        // 0-180 derece arası değeri, servonun anlıcağı 750-2250 mikrosaniyeye dönüştürüyoruz
        int mikroSaniye = map(servoAnlikAci[i], 0, 180, 750, 2250);
        servoNesneleri[i].write(mikroSaniye);
        // ----------------------------------------
      }
    }
  }
}

void stepArkaPlanYonetimi() {
  unsigned long simdikiMikroSaniye = micros();

  for (int i = 0; i < toplamStep; i++) {
    if (stepKalanAdim[i] > 0) {
      // Sabit 500 yerine, o motora özel tanımlanan gecikme süresini kontrol et
      if (simdikiMikroSaniye - stepSonAdimZamani[i] >= stepAdimGecikmesi[i]) {
        stepSonAdimZamani[i] = simdikiMikroSaniye;

        bool pinDurumu = digitalRead(stepAktifPin[i]);
        digitalWrite(stepAktifPin[i], !pinDurumu);

        if (pinDurumu == HIGH) {
          stepKalanAdim[i]--;
          if (stepKalanAdim[i] == 0) {
            //Serial.print(F("✅ [ST] "));
            //Serial.print(stepIsimleri[i]);
            //Serial.println(F(" adimlamayi tamamladi."));
          }
        }
      }
    }
  }
}


void pwmYumusakGecisYonetimi() {
  unsigned long simdikiZaman = millis();

  for (int i = 0; i < toplamPwm; i++) {
    // Eğer anlık parlaklık hedeften farklıysa ve KENDİ adım süresi dolduysa değişimi yap
    if ((int)pwmAnlikDeger[i] != pwmHedefDeger[i]) {
      if (simdikiZaman - pwmSonGuncellemeZamani[i] >= (unsigned long)pwmAdimGecikmesi[i]) {
        pwmSonGuncellemeZamani[i] = simdikiZaman;

        if (pwmAnlikDeger[i] < pwmHedefDeger[i]) {
          pwmAnlikDeger[i] += 1;  // Daha pürüzsüz geçiş için her adımda 1 birim artır
          if (pwmAnlikDeger[i] > pwmHedefDeger[i]) pwmAnlikDeger[i] = pwmHedefDeger[i];
        } else if (pwmAnlikDeger[i] > pwmHedefDeger[i]) {
          pwmAnlikDeger[i] -= 1;  // Her adımda 1 birim azalt
          if (pwmAnlikDeger[i] < pwmHedefDeger[i]) pwmAnlikDeger[i] = pwmHedefDeger[i];
        }

        analogWrite(pwmPin[i], (int)pwmAnlikDeger[i]);
      }
    }
  }
}