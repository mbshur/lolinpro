

void rxtxsetup(String rxtxdatas) {
  Serial.print("RXTXDATAS: ");
  Serial.println(rxtxdatas);
  if (rxtxdatas.indexOf("RXTX=") > -1) rxtxvar = true;
  else return;
  rxtxdatas = rxtxdatas.substring(rxtxdatas.indexOf("RXTX=") + 5);
  int pos = rxtxdatas.indexOf('\n');
  if (pos > -1) rxtxdatas = rxtxdatas.substring(0, pos);

  Serial.print("RXTXDATAS: ");
  Serial.println(rxtxdatas);

  int ESP_RX_PIN = D0;    // Uno'nun TX'inden (direnç korumalı) gelen hat
  int ESP_TX_PIN = D2;    // Uno'nun RX'ine giden hat
  int BAUD_RATE = 19200;  // Haberleşme frekansı

  String rxpinStr = "";
  String txpinStr = "";
  String baudRStr = "";

  if (rxtxdatas.indexOf(":") > -1 && rxtxdatas.indexOf(",") > -1) {
    rxpinStr = rxtxdatas.substring(0, rxtxdatas.indexOf(":"));
    txpinStr = rxtxdatas.substring(rxtxdatas.indexOf(":") + 1, rxtxdatas.indexOf(","));
    baudRStr = rxtxdatas.substring(rxtxdatas.indexOf(",") + 1, rxtxdatas.indexOf(";"));
    ESP_RX_PIN = getGPIOPin(rxpinStr);
    ESP_TX_PIN = getGPIOPin(txpinStr);
    BAUD_RATE = baudRStr.toInt();
  }

  Serial.printf("RXSTR=[%s]\n", rxpinStr.c_str());
  Serial.printf("TXSTR=[%s]\n", txpinStr.c_str());

  Serial.printf("RXPIN=%d\n", ESP_RX_PIN);
  Serial.printf("TXPIN=%d\n", ESP_TX_PIN);

  // Software Serial pin yapılandırması ve 19200 baud başlatılması
  // (RX, TX) parametreleri sırasıyla verilir
  // Yazılımsal portu sabit pinlerimiz ve 19200 baud ile başlatıyoruz
  unoSerial.begin(BAUD_RATE, SWSERIAL_8N1, ESP_RX_PIN, ESP_TX_PIN, false);

  Serial.printf_P(PSTR("Sistem Hazır. %s(RX) ve %s(TX) üzerinden %s dinleniyor..."), rxpinStr.c_str(), txpinStr.c_str(), baudRStr.c_str());
}

void rxtxloop() {
  if (unoSerial.available()) {
    String gelenHat = unoSerial.readStringUntil('\n');

    gelenHat.trim();
    Serial.print(F("<unodangelen> "));
    Serial.println(gelenHat);




    // 🎯 2. ADIM: AKILLI ARIZA / ACİL STOP TEŞHİS MOTORU (:ERR:)
    if (gelenHat.indexOf(":ERR=") > -1) {
      // Gelen paketi parçala. Örn: "UNO1:ERR:SW1=1;"
      // Önce sonundaki ";" işaretini temizleyelim
      if (gelenHat.endsWith(";")) gelenHat = gelenHat.substring(0, gelenHat.length() - 1);

      int ilkIkiNokta = gelenHat.indexOf(':');
      int ikinciIkiNokta = gelenHat.indexOf(":ERR=");

      if (ilkIkiNokta > 0 && ikinciIkiNokta > ilkIkiNokta) {
        String cihaz = gelenHat.substring(0, ilkIkiNokta);            // Örn: "UNO1"
        String arizaDetayi = gelenHat.substring(ikinciIkiNokta + 5);  // Örn: "SW1=1"
        cihaz.trim();
        cihaz.toUpperCase();
        arizaDetayi.trim();

        // 🚨 SİSTEMİ ANINDA ACİL DURDURMAYA ALIYORUZ!
        Serial.println(F("\n=================================================="));
        Serial.printf("🚨 [ACİL STOP SİSTEMİ] %s Cihazından Hata Sinyali Alındı!\n", cihaz.c_str());
        Serial.printf("⚠️  Tetiklenen Anahtar/Arıza Detayı: %s\n", arizaDetayi.c_str());

        // Bu cihaza gönderilen en son emri de sistem teşhisi için ekrana basıyoruz
        for (int s = 0; s < 10; s++) {
          if (slaves[s] == cihaz) {
            Serial.printf("--> Gönderilen Son Emir: %s\n", slavestx[s].c_str());
            slavesrx[s] = gelenHat;  // Durumu hafızaya da yedekle (Web arayüzü için)


            break;
          }
        }
        Serial.println(F("==================================================\n"));

        // Sihirli durdurma fonksiyonumuzu çağırarak uzun metrajı imha ediyoruz
        ualtprogdur();
        return;  // İşlem bitti, fonksiyonu sonlandır
      }
    }

    // 3. Adım: gelen kodları çözüyoruz.
    if (gelenHat.indexOf("E!>") > -1 || gelenHat.indexOf("<" + esphostname + "!>") > -1) {

      if (gelenHat.indexOf("<" + esphostname + "!>") > -1) gelenHat.replace("<" + esphostname + "!>", "E!>");
      String cihaz = gelenHat.substring(gelenHat.indexOf("!>") + 2, gelenHat.indexOf("?"));
      //Serial.print(" chz: ");
      //Serial.print(cihaz);
      String durum = gelenHat.substring(gelenHat.indexOf("=") + 1, gelenHat.length());
      String rxgelenpinismi = gelenHat.substring(gelenHat.indexOf("?") + 1, gelenHat.indexOf("="));
      //Serial.print(" rxgelenpinismi: ");
      //Serial.print(rxgelenpinismi);
      String rxgelenpindeger = gelenHat.substring(gelenHat.indexOf("=") + 1, gelenHat.length());
      //Serial.print(" rxgelenpindeger: ");
      //Serial.println(rxgelenpindeger);
      if (rxgelenpindeger.indexOf(";") > -1) rxgelenpindeger.replace(";", "");
      cihaz.trim();
      cihaz.toUpperCase();

      rxgelenpinismi.trim();
      rxgelenpindeger.trim();
      //Serial.print("rxgelenpindeger:");
      //Serial.println(rxgelenpindeger);

        String rXdegiskenAdi = cihaz + rxgelenpinismi;
        //Serial.print("rXdegiskenAdi:");
        //Serial.println(rXdegiskenAdi);
        for (int i=11;i<31;i++){
          if (degis[i] == rXdegiskenAdi) {
        //Serial.print("degis[i]:");
        //Serial.println(degis[i]);
        //Serial.println(i);
            degdeg[i] = rxgelenpindeger;
            break;
          }
          if (degis[i] == "") {

           // Serial.print("değişken adı bulunamadı:<");
           // Serial.print(rXdegiskenAdi);Serial.println(">");
           // Serial.println(i);
            break;
          }
        }

    }
  }
}

void txsend(String txveri) {

  String txv = txveri;
  for (int tv = 0; tv < 10; tv++) {
    if (txv.indexOf(slaves[tv] + ":") > -1 && slaves[tv].length() > 0) {
      int slaveveribasi = txv.indexOf(slaves[tv] + ":") + slaves[tv].length() + 1;
      String gidenverikesi = txv.substring(slaveveribasi, txv.length());
      int benimgidenveriSonum = gidenverikesi.indexOf(";");
      //slavestx[tv] = gidenverikesi.substring(0, benimgidenveriSonum);
      //Serial.print("slaveno:");Serial.print(tv);Serial.print(" slaveismi:");Serial.print(slaves[tv]);
      //Serial.print(" slavetx:");Serial.println(slavestx[tv]);

      if (txv.indexOf("RZ=") > -1 && txv.indexOf("?") > -1) {
        String TXdegiskenAdi = slaves[tv] + txv.substring(txv.indexOf("=")+1,txv.indexOf("?"));
       
        for (int i=11;i<31;i++){
          if (degis[i] == TXdegiskenAdi) {
            break;
          }
          if (degis[i] == "") {
            degis[i] = TXdegiskenAdi;
            // Serial.print(i); Serial.print(" - TXdegiskenAdi:<");Serial.print(TXdegiskenAdi);Serial.println(">");
            TXdegiskenAdi="";
            break;
          }
        }
      }
    }
  }
  txv = "";
  if (txveri.lastIndexOf(";") + 1 != txveri.length()) txveri += ";";
  Serial.println("txveri gönderildi. Veri:" + txveri);
  unoSerial.println(txveri);
}


/*

    String gelenHat="";
  while (unoSerial.available() > 0) {
    char c = unoSerial.read();
    if (c == '\n') {
      rxBuffer.trim();
      gelenHat = rxBuffer;
      rxBuffer = "";  // İşlem bitince hemen temizle
    } else if (c != '\r') {
      rxBuffer += c;
    }
  }



  //if (unoSerial.available()) {
    if (gelenHat.length()>0) {
    // unoSerial.readStringUntil('\n');
    rxBuffer="";
    */