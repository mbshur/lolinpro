
String upsatdata;
unsigned int upsatno = 0;

int setupSatirNo = 0;
int loopSatirNo = 0;

// --- 1. UZUN METRAJ İLK KURULUM TARAMASI (SETUP:) ---
// --- 1. UZUN METRAJ İLK KURULUM TARAMASI (SETUP:) ---
void runsetup() {
  Serial.println(F("[uprog] runsetup() basladi..."));
  standartprogram = false; 

  while (udos.available() && upsatno < 100) { // <-- NOT: Satır sayısı 100'ü geçiyorsa bu sınır engeller!
    unsigned long anlikPozisyon = udos.position();

    upsatdata = udos.readStringUntil('\n');
    upsatdata.trim(); 
    upsatno++;

    // 1. Loglamayı en başa alalım ki kodun gerçekten o satırı okuyup okumadığını görelim:
    Serial.print(F("[uprog-setup] Okunan Satir "));
    Serial.print(upsatno);
    Serial.print(F(": '"));
    Serial.print(upsatdata);
    Serial.println(F("'"));

    // Boş satırları atla
    if (upsatdata == "") continue; 

    // 2. YORUM SATIRI KONTROLÜ
    if (upsatdata.indexOf("//") == 0) {
      Serial.println(F("[uprog-setup] YORUM satiri tespit edildi, atlaniyor."));
      continue;
    }

    // 3. SETUP: Etiketini Pas Geç
    if (upsatdata.indexOf("SETUP:") != -1) {
      setupSatirNo = upsatno; 
      Serial.print(F("[uprog-setup] SETUP: etiketi pas gecildi. Satir: "));
      Serial.println(setupSatirNo);
      continue; 
    }

    // 4. LOOP: Etiketini Bulduğunda Dur ve Konumlan
    if (upsatdata.indexOf("LOOP:") != -1) {
      loopSatirNo = upsatno; 
      Serial.print(F("[uprog-setup] LOOP: alanina ulasildi. Satir: "));
      Serial.println(loopSatirNo);
      
      udos.seek(anlikPozisyon, SeekSet);
      break; 
    }
    
    // 5. Geri kalan her şeyi satirislem'e gönder
    Serial.print(F("[uprog-setup] Komut satirislem'e gonderiliyor: "));
    Serial.println(upsatdata);
    satirp = upsatdata;
    satirsayisip = upsatno;
    satirislem();
  }
}

// --- 2. UZUN METRAJ GERÇEK ZAMANLI ÇALIŞTIRMA TARAMASI (LOOP:) ---
void urunloop() {

  // Eğer bir alt senaryo tetiklendiyse, ana dosyayı okumayı durdur ve alt dosyayı koştur
  if (altDosyaCalisiyor) {
    altDosyaYurut();
    return; // Alt dosya bitene kadar urunloop'un kalan kısmına (ana dosyaya) geçme!
  }


  if (!udos) {
    udos = LittleFS.open("/uprogram.txt", "r");
    loopSatirNo = 0; 
  }
  if (!udos) {
    Serial.println(F("[uprog-loop] HATA: Dosya acilamadi!"));
    return;
  }

  if (stg == 2 && loopSatirNo > 0) {
    udos.seek(0, SeekSet); 
    loopSatirNo = 0;
  }

  bool loopBasladi = false;
  standartprogram = false; 
  ptm = "";                
  ifsonuc = true;          

  while (udos.available()) {
    String satir = udos.readStringUntil('\n');
    satir.trim();
    loopSatirNo++;

    if (satir == "") continue; 

    // 🎯 YORUM SATIRI KONTROLÜ: Satır LOOP içinde de "//" ile başlıyorsa es geç
    if (satir.indexOf("//") == 0) continue;

    // LOOP: etiketini görene kadar hiçbir şey yapma, o satırı da atla
    if (!loopBasladi) {
      if (satir.indexOf("LOOP:") != -1) {
        loopBasladi = true; 
        //Serial.print(F("[uprog-loop] LOOP: etiketi pas gecildi, altindan devam ediliyor. Satir: "));
        //Serial.println(loopSatirNo);
      }
      continue; 
    }
    
    // LOOP içindeyken yanlışlıkla tekrar SETUP: etiketine rastlanırsa atla
    if (satir.indexOf("SETUP:") != -1) continue;

        satirp = satir; 
    satirsayisip = loopSatirNo; 

    ifsonuc = false;



    satirislem(); // Burada alt dosya tetiklendi ve altDosyaCalisiyor = true oldu

    // 🎯 EĞER ALT DOSYA BAŞLADIYSA ANA DOSYA DÖNGÜSÜNÜ KIR:
    if (altDosyaCalisiyor) {
      return; // urunloop'tan hemen çık, bir sonraki çağrıda altDosyaYurut() çalışsın!
    }

    

    Serial.println("satirp " + satirp);

    Serial.println("PROGNO: " + PROGNOX);
    Serial.println("ifsonuc: " + String(ifsonuc)); 
    //if(ifsonuc) Serial.println("true");
    //else Serial.println("false");

    // 🎯 KUSURSUZ PARANTEZ ATLAMA MOTORU:
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