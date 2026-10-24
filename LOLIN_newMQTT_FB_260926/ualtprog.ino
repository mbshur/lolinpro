// 🎯 DERLEYİCİYE BİLDİRİM: Bu pointer projenin ana dosyasında zaten var, onu kullan diyoruz
extern String* aktifTmrDeger; // int* yerine String* yapıldı

// --- 4. ALT MODÜL DOSYASINI SATIR SATIR İŞLEYEN YENİ MOTOR ---
void altDosyaYurut() {
  // 🎯 1. EN ÜST ANLIK KESME KONTROLÜ (Bekleme uykusundayken DUR tetiklenirse)
  if (altProgramIptalBayragi == true || !altDosyaCalisiyor) {
    ualtprogdur(); 
    return;
  }

  // 🎯 2. ASENKRON BEKLEME KİLİDİ (W:5000 komutu için)
  if (ualt_bekliyor) {
    if (millis() < ualt_bekleme_sure) {
      yield();
      return; // Süre dolana kadar fonksiyonu burada kes, YENİ SATIRA GEÇME!
    } else {
      ualt_bekliyor = false; // Süre doldu, kilidi aç
    }
  }

  // Sizin orijinal zamanlayıcı pointer kilidiniz
  if (aktifTmrDeger != nullptr && (*aktifTmrDeger).toInt() > 0) { 
    yield();
    return; 
  }

  // Güvenlik Kontrolü: Alt dosya açık değilse çık
  if (!ualt_dosya) {
    altDosyaCalisiyor = false;
    Serial.println(F("[uprog-alt] HATA: Alt dosya hafizada bulunamadi!"));
    return;
  }

  // 3. Alt dosyada okunacak satır var mı?
  if (ualt_dosya.available()) {
    String altSatir = ualt_dosya.readStringUntil('\n');
    altSatir.trim();

    // Boş satırları ve yorum satırlarını doğrudan pas geç
    if (altSatir == "" || altSatir.indexOf("//") == 0) {
      yield();
      return; 
    }

    // Etiket satırlarını (Örn: E1:) doğrudan pas geç
    if (altSatir.endsWith(":")) {
      yield();
      return; 
    }

    // 🎯 4. BEKLEME KOMUTUNU YAKALAMA (Örn: W:5000;)
    if (altSatir.indexOf("W:") > -1) {
      String sureStr = altSatir.substring(altSatir.indexOf("W:") + 2);
      if (sureStr.endsWith(";")) sureStr = sureStr.substring(0, sureStr.length() - 1);
      
      ualt_bekleme_sure = millis() + sureStr.toInt(); 
      ualt_bekliyor = true;                          
      yield();
      return; 
    }

    // 🎯 5. GOTO / ETIKET (DÖNGÜ) MOTORU (Örn: GOTO:BASLA;)
    if (altSatir.indexOf("GOTO:") == 0) {
      String hedefEtiket = altSatir.substring(5);
      if (hedefEtiket.endsWith(";")) hedefEtiket = hedefEtiket.substring(0, hedefEtiket.length() - 1);
      hedefEtiket.trim();

      ualt_dosya.seek(0, SeekSet); 
      while (ualt_dosya.available()) {
        String arananSatir = ualt_dosya.readStringUntil('\n');
        arananSatir.trim();
        
        if (arananSatir == (hedefEtiket + ":")) {
          Serial.printf("[uprog-goto] '%s' etiketine basariyla atlandi.\n", hedefEtiket.c_str());
          yield();
          return; 
        }
      }
      Serial.printf("[uprog-goto] HATA: '%s' etiketi bulunamadi!\n", hedefEtiket.c_str());
    }

    // Normal satır işleme hazırlığı
    satirp = altSatir; 
    satirsayisip = 999; 
    ifsonuc = false;

    // Sizin mevcut satirislem motorunuz satırı yorumluyor (Uno'ya basıyor)
    satirislem();

    // 🎯 6. SATIR İŞLENDİKTEN HEMEN SONRAKİ ANLIK FREN (Sızıntıyı Önleyen Sihirli Kısım)
    if (altProgramIptalBayragi == true || !altDosyaCalisiyor) {
      Serial.println(F("[uprog-alt] ⚡ FREN: satirislem() sonrasi kesme fark edildi, sonraki satira gecilmiyor."));
      yield();
      return; 
    }

    // 🎯 7. GERİ KAZANILAN KUSURSUZ PARANTEZ ATLAMA MOTORUNUZ (Orijinal Kodunuz):
    if (ifsonuc == false && altSatir.indexOf("{") != -1) {
      int p_sayac = 1;
      
      if (altSatir.indexOf("}") != -1 && altSatir.indexOf("}") > altSatir.indexOf("{")) {
        p_sayac--;
      }

      while (ualt_dosya.available() && p_sayac > 0) {
        String atlanansatir = ualt_dosya.readStringUntil('\n');
        atlanansatir.trim();

        if (atlanansatir.indexOf("{") != -1) p_sayac++;
        if (atlanansatir.indexOf("}") != -1) p_sayac--;

        yield(); 
      }
      ifsonuc = true; 
    }
  } 
  else {
    // Alt dosyadaki tüm satırlar bittiğinde kapat ve temizle
    ualt_dosya.close();
    altDosyaCalisiyor = false;
    aktifTmrDeger = nullptr; 
    ualt_bekliyor = false;
    altProgramIptalBayragi = false;
    aktifAltDosyaAdi = "";
  }
  
  yield(); 
}



void ualtprogdur() {
  if (ualt_dosya) ualt_dosya.close(); 
  altDosyaCalisiyor = false;          
  ualt_bekliyor = false;              
  aktifTmrDeger = nullptr;            
  aktifAltDosyaAdi = "";
  
  altProgramIptalBayragi = false; // 🎯 Sihirli dokunuş: İptal bayrağını bir sonraki sefer için indirdik!

  Serial.println(F("[uprog-alt] 🛑 KESME: ualtprogdur() cagrildi, alt program imha edildi."));
  yield();
}