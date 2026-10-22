#define MAX_TIMERS 11

// NOT: Tüm dosyalar .ino olduğu için 'extern' satırları tamamen kaldırıldı!
// Ana sekmedeki udos, satirp, satirsayisip, standartprogram, milis100, tmris ve tmrdeg değişkenlerini doğrudan kullanıyoruz.

String progmsg = ""; 
uint8_t stg = 0;
String upsatdata;
unsigned int upsatno = 0;

// Uzun metraj programı timer dizileri
String utmris[MAX_TIMERS];
String utmrdeg[MAX_TIMERS];

// --- 1. SETUP AŞAMASINDA DOSYADAN TİMER TANIMA ---
void utimertanitimi() {
  int esittirIndeks = upsatdata.indexOf("=");
  int noktalivirgulIndeks = upsatdata.indexOf(";");

  if (esittirIndeks > 4 && noktalivirgulIndeks > esittirIndeks) {
    String tmrAdi = upsatdata.substring(4, esittirIndeks);
    tmrAdi.trim();
    
    String tmrDeger = upsatdata.substring(esittirIndeks + 1, noktalivirgulIndeks);
    tmrDeger.trim();

    for (int ii = 0; ii < MAX_TIMERS; ii++) {
      if (utmris[ii] == tmrAdi) {
        utmrdeg[ii] = tmrDeger; 
        break;
      }
      if (utmris[ii] == "" || utmris[ii] == "null") {
        utmris[ii] = tmrAdi;
        utmrdeg[ii] = tmrDeger; 
        break;
      }
    }
  } else {
    progmsg += "UZUN PROGRAM TMR hatası. TMR>TIMER1=10; şeklinde olmalı. satır no: " + String(upsatno) + " komut: " + upsatdata + "\n";
    Serial.print(progmsg);
  }
}

// --- 2. BAĞIMSIZ ZAMAN EKSİLTME MOTORU ---
void timerGeriSay() {
  if (millis() >= milis100) {
    // 1. STANDART PROGRAM TİMER'LARI
    for (int ii = 0; ii < MAX_TIMERS; ii++) {
      if (tmris[ii].length() > 0) {
        float standartVal = tmrdeg[ii].toFloat();
        if (standartVal > 0) {
          tmrdeg[ii] = String(standartVal - 1);
        }
      }
    }

    // 2. UZUN METRAJ PROGRAM TİMER'LARI
    for (int ii = 0; ii < MAX_TIMERS; ii++) {
      if (utmris[ii].length() > 0) {
        float uzunmetrajVal = utmrdeg[ii].toFloat();
        if (uzunmetrajVal > 0) {
          utmrdeg[ii] = String(uzunmetrajVal - 1);
        }
      }
    }
    milis100 = millis() + 100;
  }
}

// --- 3. UZUN METRAJ İLK KURULUM TARAMASI (SETUP:) ---
void runsetup() {
  while (udos.available() && upsatno < 100) {
    upsatdata = udos.readStringUntil('\n');
    upsatdata.trim(); 
    upsatno++;

    if (upsatdata == "") continue;

    if (upsatdata.indexOf("SETUP:") != -1) {
      if (udos.available()) {
        upsatdata = udos.readStringUntil('\n');
        upsatdata.trim();
        upsatno++;
      }
    }

    if (upsatdata.indexOf("LOOP:") != -1) {
      Serial.println(F("[LittleFS] LOOP: Alanına ulaşıldı. Kurulum tamamlandı."));
      break; 
    }
    
    if (upsatdata.indexOf("TMR>") != -1) {
      utimertanitimi();
    }
  }
}

// --- 4. UZUN METRAJ GERÇEK ZAMANLI ÇALIŞTIRMA TARAMASI (LOOP:) ---
void urunloop() {
  if (!udos) {
    udos = LittleFS.open("/uprogram.txt", "r");
  }
  if (!udos) {
    Serial.println(F("[LittleFS] LOOP aşamasında dosya açılamadı!"));
    return;
  }

  udos.seek(0, SeekSet); 
  unsigned int loopSatirNo = 0;
  bool loopBasladi = false;

  standartprogram = false; // Uzun metraj çalıştığını sisteme bildir

  while (udos.available()) {
    String satir = udos.readStringUntil('\n');
    satir.trim();
    loopSatirNo++;

    if (!loopBasladi) {
      if (satir.indexOf("LOOP:") != -1) {
        loopBasladi = true; 
      }
      continue; 
    }

    if (satir == "") continue;

    // --- GOTOIF PARANTEZ ATLAMA MANTIĞI ENTEGRASYONU ---
    // Eğer ana sistemde ifsonuc == false olmuşsa ve bu satırda bir if bloğu başladıysa
    // ya da bir önceki adımdan ifsonuc false kalmışsa ve süslü parantez içindeysek:
    satirp = satir; 
    satirsayisip = loopSatirNo;

    // Önce satırı normal şekilde işletiyoruz (Burada if kontrolü yapılır ve ifsonuc belirlenir)
    satirislem(); 

    // 🎯 Eğer çalıştırılan satır bir IF satırıysa ve ŞART SAĞLANMADIYSA ({ parantez içine girmememiz gerekiyorsa):
    // Not: goif() fonksiyonunuzun yapısına göre ifsonuc'u nerede false yaptığınıza bağlı olarak
    // buradaki kontrolü senkronize ediyoruz.
    if (ifsonuc == false && (satir.indexOf("{") > -1 || susluparantezsayisi > 0)) {
      
      // Eğer satırda zaten bir { açıldıysa sayacı 1 yap, yoksa kodunuz goif içinde zaten sayıyordur.
      // Sizin kendi süslü parantez sayacınız (susluparantezsayisi) 0 olana kadar dosyadan satırları okuyup ATLIYORUZ:
      int p_sayac = (satir.indexOf("{") > -1) ? 1 : 0;
      
      while (udos.available() && p_sayac > 0) {
        String atlanansatir = udos.readStringUntil('\n');
        atlanansatir.trim();
        loopSatirNo++;
        
        // Atlanan satırda süslü parantez takibi yapıyoruz (+ ve -)
        if (atlanansatir.indexOf("{") > -1) p_sayac++;
        if (atlanansatir.indexOf("}") > -1) p_sayac--;
        
        yield(); // Kilitlenmeyi önle
      }
      
      // Parantez bloku bittiği için ifsonuc'u tekrar nötrlüyoruz ki sonraki kodlar çalışabilsin
      ifsonuc = true; 
      continue; // Bu turu bitir, sonraki satıra geç
    }
    
    yield(); 
  }
}

// --- 5. ANA KONTROL FONKSİYONU ---
void uzunmetraj() {
  if (stg == 0) {
    stg = 1;
    if (udos) udos.close(); 

    if (LittleFS.exists("/uprogram.txt")) {
      udos = LittleFS.open("/uprogram.txt", "r");
      if (udos) {
        Serial.println(F("[LittleFS] Uzun metraj program dosyası açıldı."));
      } else {
        return;
      }
    } else {
      return;
    }
  }

  if (stg == 1) {
    runsetup();
    stg = 2; 
  }

  if (stg == 2) {
    urunloop(); 
  }
}