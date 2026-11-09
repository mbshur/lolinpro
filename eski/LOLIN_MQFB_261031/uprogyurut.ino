String upsatdata;
unsigned int upsatno = 0;

int setupSatirNo = 0;
int loopSatirNo = 0;

unsigned long uprogw=0;
bool loopBasladi = false;       // LOOP: etiketinin geçilip geçilmediğini fonksiyonlar arası hafızada tutar

unsigned long uprog_bekleme_pozisyonu = 0; // 🎯 Kaldığımız dosya konumunu tutacak yeni hafıza

// --- 1. UZUN METRAJ İLK KURULUM TARAMASI (SETUP:) ---
// --- 1. UZUN METRAJ İLK KURULUM TARAMASI (SETUP:) ---
void runsetup() {
  Serial.println(F("[uprog] runsetup() basladi..."));
  standartprogram = false; 

  while (udos.available() && upsatno < 100) { 
    unsigned long anlikPozisyon = udos.position();

    upsatdata = udos.readStringUntil('\n');
    upsatdata.trim(); 
    upsatno++;

    Serial.print(F("[uprog-setup] Okunan Satir "));
    Serial.print(upsatno);
    Serial.print(F(": '"));
    Serial.print(upsatdata);
    Serial.println(F("'"));

    if (upsatdata == "") continue; 

    if (upsatdata.indexOf("//") == 0) {
      Serial.println(F("[uprog-setup] YORUM satiri tespit edildi, atlaniyor."));
      continue;
    }

    if (upsatdata.indexOf("SETUP:") != -1) {
      setupSatirNo = upsatno; 
      Serial.print(F("[uprog-setup] SETUP: etiketi pas gecildi. Satir: "));
      Serial.println(setupSatirNo);
      continue; 
    }

    if (upsatdata.indexOf("LOOP:") != -1) {
      loopSatirNo = upsatno; 
      Serial.print(F("[uprog-setup] LOOP: alanina ulasildi. Satir: "));
      Serial.println(loopSatirNo);
      
      udos.seek(anlikPozisyon, SeekSet);
      break; 
    }
    
    Serial.print(F("[uprog-setup] Komut satirislem'e gonderiliyor: "));
    Serial.println(upsatdata);
    satirp = upsatdata;
    satirsayisip = upsatno;
    satirislem();
  }
}

// --- 2. UZUN METRAJ GERÇEK ZAMANLI ÇALIŞTIRMA TARAMASI (LOOP:) ---
// --- 2. UZUN METRAJ GERÇEK ZAMANLI ÇALIŞTIRMA TARAMASI (LOOP:) ---
void urunloop() {

  // 🎯 1. ASENKRON BEKLEME FREMİ (Zaman dolmadıysa dosyaya dokunma!)
  if (uprogw > millis()) {
    return; 
  }

  // Eğer bir alt senaryo tetiklendiyse, ana dosyayı okumayı durdur ve alt dosyayı koştur
  if (altDosyaCalisiyor) {
    altDosyaYurut();
    return; 
  }

  if (!udos) {
    udos = LittleFS.open("/uprogram.txt", "r");
    loopSatirNo = 0; 
    loopBasladi = false;
    uprog_bekleme_pozisyonu = 0;
  }
  if (!udos) {
    Serial.println(F("[uprog-loop] HATA: Dosya acilamadi!"));
    return;
  }

  // 🎯 2. GERÇEKTEN DOSYA BİTTİĞİNDE BAŞA SARMA KONTROLÜ
  if (!udos.available()) {
    udos.seek(0, SeekSet); 
    loopSatirNo = 0;
    loopBasladi = false; 
    uprog_bekleme_pozisyonu = 0; // Sıfırla
    //Serial.println(F("[uprog-loop] 🔄 Dosya sonuna ulasildi, LOOP basina sarildi."));
    return;
  }

  // 🎯 3. SİHİRLİ HAFIZA: BEKLEME BİTTİYSE KALDIĞIMIZ SATIRA IŞINLAN
  if (uprog_bekleme_pozisyonu > 0) {
    udos.seek(uprog_bekleme_pozisyonu, SeekSet);
    uprog_bekleme_pozisyonu = 0; // Kilidi aç ki akış normal devam etsin
    Serial.println(F("[uprog-loop] 🎯 Gecikme bitti, alt satirdaki komuttan devam ediliyor..."));
  }

  standartprogram = false; 
  ptm = "";                
  ifsonuc = true;          

  while (udos.available()) {

    String satir = udos.readStringUntil('\n');
    satir.trim();
    loopSatirNo++;

    if (satir == "") continue; 

    if (satir.indexOf("//") == 0) continue;

    // LOOP: ETİKETİNİ ARAMA KONTROLÜ
    if (!loopBasladi) {
      if (satir.indexOf("LOOP:") != -1) {
        loopBasladi = true; 
        //Serial.println(F("[uprog-loop] LOOP: alani basariyla gecildi."));
      }
      continue; 
    }
    
    if (satir.indexOf("SETUP:") != -1) continue;

    // 🎯 4. KUSURSUZ ASENKRON BEKLEME MOTORU (W: YAKALAYICI)
    if (satir.indexOf("W:") > -1) {
      String sureStr = satir.substring(satir.indexOf("W:") + 2);
      if (sureStr.endsWith(";")) sureStr = sureStr.substring(0, sureStr.length() - 1);
      sureStr.trim();
      
      uprogw = millis() + sureStr.toInt(); // Hedef zamanı kur
      
      // 🚨 İŞTE EN KRİTİK DOKUNUŞ:
      // readStringUntil satırı okuduğu için dosya imleci şu an tam olarak
      // bir alt satırın (TX>=PR2;) BAŞINDA bekliyor. Bu konumu hafızaya kaydediyoruz!
      uprog_bekleme_pozisyonu = udos.position(); 
      
      Serial.print(F("[uprog-wait] ⏱️ Gecikme tetiklendi: "));
      Serial.print(sureStr);
      Serial.println(F(" ms boyunca dosya okuma donduruldu..."));
      
      return; // Çık ve zamanın dolmasını bekle
    }

    satirp = satir; 
    satirsayisip = loopSatirNo; 
    ifsonuc = false;

    satirislem(); 

    // EĞER ALT DOSYA BAŞLADIYSA ANA DOSYA DÖNGÜSÜNÜ KIR:
    if (altDosyaCalisiyor) {
      return; 
    }

    // KUSURSUZ PARANTEZ ATLAMA MOTORU
    if (ifsonuc == false && satir.indexOf("{") != -1) {
      int p_sayac = 1; 
      
      if (satir.indexOf("}") != -1 && satir.indexOf("}") > satir.indexOf("{")) {
        p_sayac--;
      }

      while (udos.available() && p_sayac > 0) {
        String atlanansatir = udos.readStringUntil('\n');
        atlanansatir.trim();
        loopSatirNo++;

        if (atlanansatir.indexOf("{") != -1) p_sayac++;
        if (atlanansatir.indexOf("}") != -1) p_sayac--;

        yield(); 
      }
      ifsonuc = true; 
    }
    yield(); 
  }
}

// --- 3. ANA KONTROL FONKSİYONU ---
void uzunmetraj() {
  if (stg == 0) {
    stg = 1;
    if (udos) udos.close(); 
    if (LittleFS.exists("/uprogram.txt")) {
      udos = LittleFS.open("/uprogram.txt", "r");
      if (udos) Serial.println(F("[LittleFS] Uzun metraj program dosyasi acildi."));
    }
  }

  if (stg == 1) {
    runsetup();
    stg = 2; 
    Serial.println(F("[uprog-ana] SETUP bitti, stg=2 yapildi. Dosya urunloop için hazirlaniyor..."));
  }

  if (stg == 2) {
    urunloop(); 
  }
}