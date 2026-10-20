String sonheader;
bool kucukmenu;
// Parametrenin başına '&' ekledik, böylece bağlantı RAM'de kopyalanmaz.
void httpheader(WiFiClient& xilent) {
  xilent.print(F("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\n"));
  xilent.print(F("<head><meta name='viewport' content='width=device-width, initial-scale=1'>\r\n"));
  xilent.print(F("<meta charset='UTF-8'>\r\n"));
  xilent.print(F("<link rel='icon' href='data:,'>\r\n"));

  // CSS Kısmı Düzenlendi: Butonların ve yazıların alt satıra kırılmasını engellemek için 'white-space: nowrap;' eklendi
  xilent.print(F("<style>html {font-family:Helvetica;display:inline-block;margin:0px auto;text-align:left;}\r\n"));
  xilent.print(F(".butmen{background-color:#0000;border:none;color:white; padding:6px 8px;font-size:20px; margin:2px;cursor:pointer; white-space: nowrap;}\r\n"));
  xilent.print(F(".butmenbs{background-color:#0000;border:none;color:white; padding:4px 4px;font-size:22px; margin:2px;cursor:pointer;}\r\n"));
  xilent.print(F("table {border-collapse: collapse;}\r\n"));  // Tablo boşluklarını sıfırlar
  xilent.print(F("</style>\r\n</head>\r\n"));

  xilent.printf_P(PSTR("<body bgcolor='#%s'>\r\n"), butonpbgcol.c_str());

  if (kucukmenu == false) {
    xilent.print(F("<br><table style='width:100%;'><tr><td style='border:2px solid black;color:black; vertical-align: top;'>\r\n"));

    // DÜZELTME: width:%%10; değil, menü yazılarının rahat sığması için %18 (18%%) yapıldı ve genişlik HTML sonuna alındı.
    xilent.printf_P(PSTR("<table style='width:100%%;'><tr><td bgcolor='%s' style='vertical-align: top; width:18%%;'><table bgcolor='%s' style='border:1px solid #%s; width:100%%;'><td style='border:0px solid black; align:center;'>\r\n"),
                    butonpbgcol.c_str(), butonpbgcol.c_str(), menutextcol.c_str());

    xilent.printf_P(PSTR("<a href='%s/kucukmenu'><button class='button butmen'>&ensp;<font color='%s'>☰ </font><font size='3' color='%s'>MENU</font></button></a>\r\n"),
                    usertoken.c_str(), menutextcol.c_str(), menutextcol.c_str());

    xilent.printf_P(PSTR("<tr><td bgcolor='%s' style='border:1px solid %s;'>\r\n"), butonpbgcol.c_str(), butonpbgcol.c_str());
    xilent.printf_P(PSTR("<a href='%s/Menu0'><button class='button butmen'>🏠 <font size='3' color='%s'>Anasayfa</font></button></a></td></tr>\r\n"), usertoken.c_str(), menutextcol.c_str());

    xilent.printf_P(PSTR("<tr><td bgcolor='%s' style='border:1px solid %s;'>\r\n"), butonpbgcol.c_str(), butonpbgcol.c_str());
    xilent.printf_P(PSTR("<a href='%s/'><button class='button butmen'>🧐 <font size='3' color='%s'>Durum</font></button></a></td></tr>\r\n"), usertoken.c_str(), menutextcol.c_str());

    xilent.printf_P(PSTR("<tr><td bgcolor='%s' style='border:1px solid %s;'>\r\n"), butonpbgcol.c_str(), butonpbgcol.c_str());
    xilent.printf_P(PSTR("<a href='%s/pnayar'><button class='button butmen'>🧮 <font size='3' color='%s'>Pin ayar</font></button></a></td></tr>\r\n"), usertoken.c_str(), menutextcol.c_str());

    xilent.printf_P(PSTR("<tr><td bgcolor='%s' style='border:1px solid %s;'>\r\n"), butonpbgcol.c_str(), butonpbgcol.c_str());
    xilent.printf_P(PSTR("<a href='%s/ssidset'><button class='button butmen'>🛜 <font size='3' color='%s'>Wifi ayar</font></button></a></td></tr>\r\n"), usertoken.c_str(), menutextcol.c_str());

    xilent.printf_P(PSTR("<tr><td bgcolor='%s' style='border:1px solid %s;'>\r\n"), butonpbgcol.c_str(), butonpbgcol.c_str());
    xilent.printf_P(PSTR("<a href='%s/myssidnameayar'><button class='button butmen'>📓 <font size='3' color='%s'>Cihaz adı</font></button></a></td></tr>\r\n"), usertoken.c_str(), menutextcol.c_str());

    xilent.printf_P(PSTR("<tr><td bgcolor='%s' style='border:1px solid %s;'>\r\n"), butonpbgcol.c_str(), butonpbgcol.c_str());
    xilent.printf_P(PSTR("<a href='%s/mqttipayar'><button class='button butmen'>🦟 <font size='3' color='%s'>MQTT ayar</font></button></a></td></tr>\r\n"), usertoken.c_str(), menutextcol.c_str());

    xilent.printf_P(PSTR("<tr><td bgcolor='%s' style='border:1px solid %s;'>\r\n"), butonpbgcol.c_str(), butonpbgcol.c_str());
    xilent.printf_P(PSTR("<a href='%s/firebaseset'><button class='button butmen'>🔥 <font size='3' color='%s'>Firebase ayar</font></button></a></td></tr>\r\n"), usertoken.c_str(), menutextcol.c_str());

    xilent.printf_P(PSTR("<tr><td bgcolor='%s' style='border:1px solid %s;'>\r\n"), butonpbgcol.c_str(), butonpbgcol.c_str());
    xilent.printf_P(PSTR("<a href='%s/kulupayr'><button class='button butmen'>👨🏻‍💻 <font size='3' color='%s'>Şifre ayar</font></button></a></td></tr>\r\n"), usertoken.c_str(), menutextcol.c_str());

    xilent.print(F("</td></tr></table><br><br><br><br><br><br><br><br><br><br><br><br></td>\r\n"));

  } else {

    xilent.print(F("<br><table style='width:100%;'><tr><td style='border:2px solid black;color:black; vertical-align: top;'>\r\n"));

    // DÜZELTME: width:%8; hatası width:6%%; (yüzde 6) olarak düzeltildi (Sadece ikonların sığacağı kadar dar)
    xilent.printf_P(PSTR("<table style='width:100%%;'><tr><td bgcolor='%s' style='vertical-align: top; width:6%%;'><table bgcolor='%s' style='border:1px solid #%s; width:100%%;'><td style='border:1px solid %s; align:center;'>\r\n"),
                    butonpbgcol.c_str(), butonpbgcol.c_str(), menutextcol.c_str(), butonpbgcol.c_str());

    xilent.printf_P(PSTR("<span title='Menüyü genişletir butonların etiketleri görünür.'><a href='%s/buyukmenu'><button class='button butmen'>&ensp;<font color='%s'>☰ </font></button></a></span>\r\n"), usertoken.c_str(), menutextcol.c_str());

    xilent.printf_P(PSTR("<tr><td bgcolor='%s' style='border:1px solid %s;'><span title='Ana sayfaya dön'><a href='%s/Menu0'><button class='button butmen'>🏠</button></a></span></td></tr>\r\n"), butonpbgcol.c_str(), butonpbgcol.c_str(), usertoken.c_str());
    xilent.printf_P(PSTR("<tr><td bgcolor='%s' style='border:1px solid %s;'><span title='Durum sayfası'><a href='%s/'><button class='button butmen'>🧐</button></a></span></td></tr>\r\n"), butonpbgcol.c_str(), butonpbgcol.c_str(), usertoken.c_str());
    xilent.printf_P(PSTR("<tr><td bgcolor='%s' style='border:1px solid %s;'><span title='Pin ve program ayarları'><a href='%s/pnayar'><button class='button butmen'>🧮</button></a></span></td></tr>\r\n"), butonpbgcol.c_str(), butonpbgcol.c_str(), usertoken.c_str());
    xilent.printf_P(PSTR("<tr><td bgcolor='%s' style='border:1px solid %s;'><span title='Wifi`ye bağlanma ayarları'><a href='%s/ssidset'><button class='button butmen'>🛜</button></a></span></td></tr>\r\n"), butonpbgcol.c_str(), butonpbgcol.c_str(), usertoken.c_str());
    xilent.printf_P(PSTR("<tr><td bgcolor='%s' style='border:1px solid %s;'><span title='Benim SSID adım'><a href='%s/myssidnameayar'><button class='button butmen'>📓</button></a></span></td></tr>\r\n"), butonpbgcol.c_str(), butonpbgcol.c_str(), usertoken.c_str());
    xilent.printf_P(PSTR("<tr><td bgcolor='%s' style='border:1px solid %s;'><span title='MQTT ile bağlanılacak server ip girişi'><a href='%s/mqttipayar'><button class='button butmen'>🦟</button></a></span></td></tr>\r\n"), butonpbgcol.c_str(), butonpbgcol.c_str(), usertoken.c_str());
    xilent.printf_P(PSTR("<tr><td bgcolor='%s' style='border:1px solid %s;'><span title='Firebase bağlantı ayarları'><a href='%s/firebaseset'><button class='button butmen'>🔥</button></a></span></td></tr>\r\n"), butonpbgcol.c_str(), butonpbgcol.c_str(), usertoken.c_str());
    xilent.printf_P(PSTR("<tr><td bgcolor='%s' style='border:1px solid %s;'><span title='admin, kullanıcı şifre değiştirme sayfası'><a href='%s/kulupayr'><button class='button butmen'>👨🏻‍💻</button></a></span></td></tr>\r\n"), butonpbgcol.c_str(), butonpbgcol.c_str(), usertoken.c_str());

    xilent.print(F("</td></tr></table><br><br><br><br><br><br><br><br><br><br><br><br></td>\r\n"));
  }

  // DÜZELTME: Genişlik hataları düzeltildi, sağ taraftaki içerik ekranı kalan tüm boşluğu kaplasın diye width:82%% (büyük menüde) ve 94%% (küçük menüde) olacak şekilde tarayıcı otomatik ayarlar.
  xilent.printf_P(PSTR("<td bgcolor='#%s' style='border:4px solid #%s; vertical-align:top; width:2%%;'> </td> <td style='border:2px solid %s; vertical-align:top; pading:10px;'>\r\n"),
                  butonpbgcol.c_str(), butonpbgcol.c_str(), butonpbgcol.c_str());

  xilent.printf_P(PSTR("&emsp; <font size='6' align='center' color='#%s'>%s</font>&emsp; - &emsp;\r\n"),
                  menutextcol.c_str(), esphostname.c_str());

  if (header.indexOf("/Menu1") > -1 || (Menu == 1 && header.indexOf("/ HTTP/1.1") > -1)) xilent.print(F("Durum sayfası"));
  if (header.indexOf("/pnayar") > -1) xilent.print(F("Pin ayar sayfası"));
  if (header.indexOf("/ssidset") > -1) xilent.print(F("Bağlantı ayar sayfası"));
  if (header.indexOf("/myssidnameayar") > -1) xilent.print(F("Cihaz adı ayar sayfası"));
  if (header.indexOf("/mqttipayar") > -1) xilent.print(F("MQTT ayar sayfası"));
  if (header.indexOf("/firebaseset") > -1) xilent.print(F("Firebase ayar sayfası"));
  if (header.indexOf("/kulupayr") > -1) xilent.print(F("Kullanıcı sayfası"));

  xilent.printf_P(PSTR("<hr style='height:8px;border-width:1;color: #%s;background-color: #%s'>\r\n"),
                  menutextcol.c_str(), menutextcol.c_str());
}



int hsay = 0;
void htpcl(WiFiClient xilent) {
  header.reserve(2048);
  header = "";
  if (xilent) {

    // If a new xilent connects,
    Serial.println("New Client. fben:" + String(fben));  // print a message out in the serial port
    String currentLine = "";                             // make a String to hold incoming data from the xilent
    xcurrentTime = 0;
    while (xilent.connected() && xcurrentTime <= timeoutTime) {  // loop while the xilent's connected
      xcurrentTime += 1;
      if (xilent.available()) {  // if there's bytes to read from the xilent,

        for (int q = 0; q < 2048; q++) {
          char c = xilent.read();  // read a byte, then

          //Serial.print((int)c);
          if (int(c) == 255) break;
          //Serial.print(c);  // print it out the serial monitor
          header += c;
        }

        hsay += 1;
        Serial.println(hsay);
        String Hosttmp = header.substring(header.indexOf("Host:") + 5, header.length());
        Host = Hosttmp.substring(0, Hosttmp.indexOf("\n"));

        if (header.indexOf(" /favicon.ico") > -1) {
          header = sonheader;
        }

        if (header.indexOf(" HTTP/1.1") > -1) {
          header = header.substring(0, header.indexOf(" HTTP/1.1") + 9);

          if (header != headerold) {
            if (header.indexOf("favicon.ico") < 0) headerold = header;
          }

          reConnectsayac = millis();



          IPAddress lip = WiFi.localIP();
          String lipStr = String(lip[0]) + '.' + String(lip[1]) + '.' + String(lip[2]) + '.' + String(lip[3]);





          creator = "M";
          // login kısmı
          //////////////////
          //////////////////
          //////////////////
          // http://192.168.2.84/auth:?capt=2927&unme=admin&pwrd=1234
          // bilgi geldiyse aut = 1 yapmak için


          String xo2;

          header = Karakterduzeltfunc(header);

          if (header.indexOf("/gerT") > -1) {
            xilent.println("HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n");
            xilent.println(YOL + "[" + esphostname + "]" + lipStr);
            xilent.println("\r\n\r\n");
            return;
          }

          if (header.indexOf("/payM") > -1) {
            xilent.println("HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n");
            xilent.println("payĞ" + pinayar + "<br><br>");
            String pis = "";
            if (ACLilanciyim == true) { pis += "ACL:" + String(ACL) + ","; }

            for (int k = 0; k < 11; k++) {
              if (pinname[k] != "") {
                pis += pinname[k] + ":" + PinState[k] + ",";
              }
            }



            xilent.println("pinĞ" + pis + "<br><br>");
            xilent.println("rĞ" + String(fbsayac) + "<br><br>");
            xilent.println("\r\n\r\n");
            if (pinayarchg == true) pinayarchg = false;
            if (habpchg == true) habpchg = false;
            return;
          }

          /*
          if (header.indexOf("/ser:") > -1) {

            IPAddress lip = WiFi.localIP();
            String lipStr = String(lip[0]) + '.' + String(lip[1]) + '.' + String(lip[2]) + '.' + String(lip[3]);
            String gd = YOL + "[" + esphostname + "]" + lipStr;
            xilent.println("HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n");
            xilent.println(gd);
            xilent.println("\r\n\r\n");

            Serial.println("Server geldi          ");

            IPAddress remip = xilent.remoteIP();
            String remoteip = String(remip[0]) + '.' + String(remip[1]) + '.' + String(remip[2]) + '.' + String(remip[3]);

            Serial.println(remoteip);
            htserverkaydet(remoteip);
            xilent.println("\r\n\r\n");
            return;
          }
*/





          //           http://192.168.2.138/auth:?capt=2046&unme=admin&pwrd=1111
          Serial.println(header);

          bool menu1girilemez = false;









          // =================================================================
          // 1. ADIM: KORUMA DUVARINDAN HEMEN ÖNCE İSTEĞİ YAKALAMA
          // =================================================================
          if (header.indexOf("/lisanskontrol") > -1) {
            int lskodIdx = header.indexOf("lskod=");
            if (lskodIdx != -1) {
              // "lskod=" ifadesinden sonraki kısmı alıyoruz (6 karakter sonrasından başlar)
              int startIdx = lskodIdx + 6;

              // HTTP isteğinde parametre bitişini (boşluk veya & işareti) buluyoruz
              int endIdx = header.indexOf(' ', startIdx);
              int ampIdx = header.indexOf('&', startIdx);

              // Eğer & işareti boşluktan önce geliyorsa bitiş noktası olarak & işaretini seç
              if (ampIdx != -1 && ampIdx < endIdx) {
                endIdx = ampIdx;
              }

              // URL'den gelen ham lisans kodunu kesip alıyoruz
              String girilenLisans = header.substring(startIdx, endIdx);
              girilenLisans.trim();

              // Daha önce yazdığımız doğrulama ve LittleFS'e kaydetme motorunu çağırıyoruz
              webAktivasyonKoduKaydet(girilenLisans);

              // Eğer aktivasyon başarılı olduysa cihazorjinal "EVET" olacaktır.
              // Kullanıcıyı bilgilendirmek veya sayfayı tazelemek için yönlendirme (HTTP Redirect) yapıyoruz:
              xilent.print(F("HTTP/1.1 303 See Other\r\nLocation: /\r\n\r\n"));
              xilent.abort();
              return;
            }
          }

          // =================================================================
          // 2. ADIM: KORUMA DUVARI (Yukarıdaki filtreden hemen sonra gelir)
          // =================================================================
          if (!cihazorjinal) {
            aut = 0;   // Yetkileri sıfırla
            Menu = 9;  // Doğrudan Lisans Menüsüne yönlendir
          }

// -----------------------------------------------------------------
// 3. MENÜ 9: Lisans Giriş Ekranı HTML Çıktısı
// -----------------------------------------------------------------
if (Menu == 9) {
  if (header.indexOf("/") > -1) {

    // Güvenlik motorumuzdan o anki MAC adresine göre parazitli kodu üretiyoruz
    String gercekMac = WiFi.macAddress();
    gercekMac.replace(":", "");

    // Anlık olarak 1-4 arası random sayı üretip talep kodunu hazırlıyoruz (max 5 hariçtir, 1,2,3,4 üretir)
    int anlikRnd = random(1, 5);
    String ekranaBasilacakTalepKodu = talepKoduUret(gercekMac, anlikRnd);

    // HTML Başlangıcı ve CSS (Tamamen Flash bellekte - F makrosu ile)
    xilent.print(F("<!DOCTYPE HTML>\r\n<html>\r\n<head>\r\n"
                   "<meta name='viewport' content='width=device-width, initial-scale=1'>\r\n"
                   "<meta charset='UTF-8'>\r\n"
                   "<style>\r\n"
                   "body { font-family: sans-serif; background-color: #f4f4f4; text-align: center; padding: 20px; }\r\n"
                   ".box { background: white; padding: 20px; border-radius: 8px; box-shadow: 0px 0px 10px rgba(0,0,0,0.1); max-width: 350px; margin: auto; }\r\n"
                   "input[type='text'] { width: 90%; padding: 8px; margin: 10px 0; border: 1px solid #ccc; border-radius: 4px; text-align: center; text-transform: uppercase; }\r\n"
                   "input[type='submit'] { width: 95%; padding: 10px; border: none; border-radius: 4px; cursor: pointer; font-weight: bold; }\r\n"
                   ".btn-aktif { background-color: #4CAF50; color: white; }\r\n"
                   ".btn-aktif:hover { background-color: #45a049; }\r\n"
                   ".btn-devam { background-color: #008CBA; color: white; }\r\n"
                   ".btn-devam:hover { background-color: #007B9A; }\r\n"
                   "</style>\r\n</head>\r\n<body>\r\n<div class='box'>\r\n"));

    // --------------------------------=================================
    // SENARYO A: AKTİVASYON BAŞARILI İSE (Sizin eklediğiniz harika kısım)
    // --------------------------------=================================
    if (cihazorjinal) {
      Menu = 0; // Menüyü ana sayfaya serbest bırakıyoruz
      
      // Başarılı uyarısı ve Mavi renkli şık bir DEVAM ET butonu basıyoruz
      xilent.printf_P(PSTR("<h2><font color='#4CAF50'>AKTİVASYON BAŞARILI</font></h2>\r\n"
                           "<p style='color:gray; font-size:14px; font-weight:bold;'>%s</p><br>\r\n"
                           "<form autocomplete='off' action='/' method='post'>\r\n"
                           "<input type='submit' class='btn-devam' value='SİSTEME GİRİŞ YAP'>\r\n"
                           "</form>\r\n"), pinayarerrorlog.c_str());
      
      xilent.print(F("</div>\r\n</body>\r\n</html>\r\n")); // div kutusu ve HTML güvenle kapatıldı
      xilent.println();
      xilent.abort();
      pinayarerrorlog="";
      return;
    } 
    // --------------------------------=================================
    // SENARYO B: CİHAZ LİSANS BEKLİYOR İSE (Normal Kilit Ekranı)
    // --------------------------------=================================
    else {
      // Başlık ve Hata Mesajı Alanı (Eğer önceden hatalı girildiyse uyarı basar)
      xilent.printf_P(PSTR("<h2><font color='#FF0000'>LİSANS GEREKLİ</font></h2>\r\n"
                           "<p style='color:gray; font-size:13px;'>Cihaz klonlanmış veya henüz aktif edilmemiş.</p>\r\n"
                           "<p style='color:red; font-weight:bold;'>%s</p>\r\n"), pinayarerrorlog.c_str());

      // Talep Kodunu Gösterdiğimiz Alan
      xilent.printf_P(PSTR("<div style='background:#eee; padding:10px; border-radius:4px; font-weight:bold; letter-spacing:1px; color:#333; font-size:16px;'>\r\n"
                           "Talep Kodu:<br><span style='color:#007BFF;'>%s</span>\r\n"
                           "</div><br>\r\n"), ekranaBasilacakTalepKodu.c_str());

      // Lisans Giriş Formu
      xilent.printf_P(PSTR("<form autocomplete='off' action='%s/lisanskontrol' method='get'>\r\n"
                           "<label style='font-size:14px; font-weight:bold;'>Aktivasyon Kodunu Girin:</label><br>\r\n"
                           "<input type='text' name='lskod' placeholder='702BA...' maxlength='20' required><br><br>\r\n"
                           "<input type='submit' class='btn-aktif' value='AKTİF ET'>\r\n"
                           "</form>\r\n"), usertoken.c_str());

      // HTML Kapanışı
      xilent.print(F("</div>\r\n</body>\r\n</html>\r\n"));
      xilent.println();
      xilent.abort();
      return;
    }
  }
}



          //aut = 1;

          if (header.indexOf("/logcancel") > -1) {
            aut = 0;
            usertoken = "";
          }

          String lipstrhost = String(lip[0]) + '.' + String(lip[1]) + '.' + String(lip[2]);


          IPAddress remip = xilent.remoteIP();
          String remoteip = String(remip[0]) + '.' + String(remip[1]) + '.' + String(remip[2]) + '.' + String(remip[3]);



          if (header.indexOf("/kucukmenu") > -1) {
            kucukmenu = true;
            header = sonheader;
          }
          if (header.indexOf("/buyukmenu") > -1) {
            kucukmenu = false;
            header = sonheader;
          }


          /*
          if (aut == 0 && remoteip.indexOf(lipstrhost) == 0) // && header.indexOf("/SEND>data:")>-1)
          { 
            aut = 1;
            //header=header.substring(header.indexOf("/SEND>data:")+4,header.length());
          }
          */



          if (header.indexOf("/pinayarsil") > -1) {
            pinayar = "";
            dosyaYazpinayar();
            header = "/ HTTP/1.1";
          }

          if (header.indexOf("/programsil") > -1) {
            programdata = "";
            dosyaYazprogram(programdata);
            header = "/ HTTP/1.1";
          }

          if (header.indexOf("/usersil") > -1) {
            users = "admin,1111,2\na,1,1\n";
            Menu = 0;
            aut = 0;
            usernamelbl = "";
            usertoken = "";
            dosyaYazusers();
            dosyaOkuusers();
            header = header.substring(0, header.indexOf("/userssil")) + "/logout" + header.substring(header.indexOf("/userssil") + 9, header.length());
          }

          if (header.indexOf("/nevarsasil") > -1) {
            LittleFS.remove("/program.txt");
            delay(10);
            LittleFS.remove("/pinayar.txt");
            delay(10);
            LittleFS.remove("/ssidpass.txt");
            delay(10);
            LittleFS.remove("/httpserverip.txt");
            delay(10);
            LittleFS.remove("/mqttip.txt");
            delay(10);
            LittleFS.remove("/myssidname.txt");
            delay(10);
            LittleFS.remove("/fben.txt");
            delay(10);
            LittleFS.remove("/fburl.txt");
            delay(10);
            LittleFS.remove("/fbapi.txt");
            delay(10);
            LittleFS.remove("/fbyol.txt");
            delay(10);
            LittleFS.remove("/fbusername.txt");
            delay(10);
            LittleFS.remove("/fbuserpass.txt");
            delay(10);
            LittleFS.remove("/habp.txt");
            delay(10);
            LittleFS.remove("/usrnamepass.txt");
            delay(10);
            ESP.reset();
          }




          if (header.indexOf("/login") > -1) aut = -1;

          if (header.indexOf("/logout") > -1) {
            aut = 0;
            for (int u = 0; u < MAX_USER; u++) {
              if (header.substring(header.indexOf("/tokn:"), header.indexOf("/tokn:") + 10) == usertokn[u]) {
                usertoken = "";
                usertokn[u] = "";
                usernamelbl = "";
                header = header.substring(0, header.indexOf("/tokn:")) + header.substring(header.indexOf("/tokn:") + 10, header.length());
                break;
              }
            }
          }



          if (header.indexOf("SPAYPIN") > -1) aut = 1;

          Serial.println(xilent.remoteIP());

          if (header.indexOf("favicon.ico") < 0) sonheader = header;

          if (header.indexOf("/tokn:") > -1) {
            aut = 0;
            for (int u = 0; u < MAX_USER; u++) {

              if (header.substring(header.indexOf("/tokn:"), header.indexOf("/tokn:") + 10) == usertokn[u]) {
                if (userLevels[u] == 2) userlogouttime[u] = millis();
                usernamelbl = usernames[u];
                aut = userLevels[u];
                usertoken = usertokn[u];
                header = header.substring(0, header.indexOf("/tokn:")) + header.substring(header.indexOf("/tokn:") + 10, header.length());
                break;
              }
            }
          }



          if (aut < 1) {
            if (header.indexOf("/auth:") > -1) {
              String capttmp;
              String unmetmp;
              String pwrdtmp;
              if (header.indexOf("?capt=") > -1) {
                capttmp = header.substring((header.indexOf("capt=") + 5), (header.indexOf("&unme")));
                xo2 = capttmp + "<br>";

                if (header.indexOf("&unme=") > -1) {
                  unmetmp = header.substring((header.indexOf("&unme=") + 6), (header.indexOf("&pwrd")));
                  xo2 += unmetmp + "<br>";
                }

                if (header.indexOf("&pwrd=") > -1) {
                  pwrdtmp = header.substring((header.indexOf("&pwrd=") + 6), (header.indexOf(" HTTP")));
                  xo2 += pwrdtmp + "<br>";
                }
              }
              /*
                String dizgi1=capttmp+unmetmp+pwrdtmp;
                String dizgi2=capt+unme+pwrd;
                
                uint8_t dizgi1len=dizgi1.length()+1;
                char diz1[dizgi1len];
                dizgi1.toCharArray(diz1,dizgi1len);

                uint8_t dizgi2len=dizgi2.length()+1;
                char diz2[dizgi2len];
                dizgi2.toCharArray(diz2,dizgi2len);
                
                for(int dfg=0;dfg<dizgi2len;dfg++)
                {
                  Serial.print(">");Serial.print((int)diz1[dfg]);Serial.print("<");Serial.print(diz1[dfg]);Serial.print("-");Serial.print(diz2[dfg]);Serial.print(">");Serial.println((int)diz2[dfg]);
                  if((int)diz1[dfg]== (int)diz2[dfg])
                  {
                    Serial.print(dfg);Serial.print(" doğru ");Serial.println(diz2[dfg]);
                  }
                }
*/

              if (capt == capttmp) {
                if (users.length() < 1) {
                  users = "admin,1111,2\na,1,1\n";
                  dosyaYazusers();
                  dosyaOkuusers();
                }

                for (int u = 0; u < MAX_USER; u++) {
                  if (usernames[u] == unmetmp && passwords[u] == pwrdtmp) {
                    usertokn[u] = "/tokn:" + capt;
                    usertoken = "/tokn:" + capt;
                    aut = userLevels[u];
                    if (userLevels[u] == 2) userlogouttime[u] = millis();
                    usernamelbl = usernames[u];
                    break;
                  }
                }
                capt = "";
                if (aut == 0) { xo2 += "<br> HATALI GiRiS."; }
              }
            }
          }





          // =================================================================
          // KOMUT 1: /autok GELDİĞİNDE (aut=2 durumunu sabitle ve kaydet)
          // =================================================================
          if (header.indexOf("/autok") > -1) {
            dosya.close();
            LittleFS.remove("/autdurum.txt");
            dosya = LittleFS.open("/autdurum.txt", "w+");
            if (dosya) {
              dosya.print("2");
              dosya.flush();
              dosya.close();
            }

            aut = 2;  // Çalışma anında anlık olarak yetkiyi yükselt

            // Ekrana kısa bir bilgi mesajı basıp ana sayfaya yönlendiriyoruz
            xilent.print(F("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n"));
            xilent.print(F("<head><meta charset='UTF-8'></head>\r\n"));
            xilent.print(F("<html><body><h3>Kalıcı giriş aktif edildi (aut=2). Yönlendiriliyorsunuz...</h3>"
                           "<script>setTimeout(function(){ window.location.href='/'; }, 1500);</script></body></html>"));
            xilent.println();
            xilent.abort();
            return;
          }

          // =================================================================
          // KOMUT 2: /autno GELDİĞİNDE (durumu sıfırla veya dosyayı sil)
          // =================================================================
          if (header.indexOf("/autno") > -1) {
            dosya.close();
            if (LittleFS.exists("/autdurum.txt")) {
              LittleFS.remove("/autdurum.txt");  // Dosyayı tamamen siliyoruz
            }

            aut = 0;   // Çalışma anında yetkiyi sıfırla
            Menu = 0;  // Menüyü de varsayılana çek (gerekiyorsa)

            // Ekrana bilgi basıp giriş sayfasına yönlendiriyoruz
            xilent.print(F("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n"));
            xilent.print(F("<head><meta charset='UTF-8'></head>\r\n"));
            xilent.print(F("<html><body><h3>Kalıcı giriş kapatıldı (aut=0). Giriş ekranına yönlendiriliyorsunuz...</h3>"
                           "<script>setTimeout(function(){ window.location.href='/'; }, 1500);</script></body></html>"));
            xilent.println();
            xilent.abort();
            return;
          }









          if (aut < 1) {
            if (header.indexOf("/") > -1) {
              capt = String(random(1000, 9999));

              // IP adreslerini String nesnelerine dönüştürüp RAM harcamak yerine
              // doğrudan WiFi kütüphanesinden alıp printf içinde basacağız.
              IPAddress gip = WiFi.gatewayIP();
              IPAddress dns = WiFi.dnsIP();
              IPAddress ip = WiFi.softAPIP();

              // HTML Başlangıcı ve CSS/Meta etiketleri tamamen Flash bellekte
              xilent.print(F("<!DOCTYPE HTML>\r\n<html>\r\n<head><meta name='viewport' xo='width=device-width, initial-scale=1'>\r\n<META charset='UTF-8'>\r\n</head>\r\n<body><br>\r\n"));

              // Dinamik Hostname basımı
              xilent.printf_P(PSTR("<font size='3' color='#FF0000'>%s</font><br>informations<br>\r\n"), esphostname.c_str());
              // xo2 içeriği (xo2 global veya lokal bir String ise)
              
              xilent.println(xo2);
              xilent.print(F("<br><br>\r\n"));

              // IP Bilgilerini basıyoruz. %d.%d.%d.%d formatı sayesinde
              // RAM'de tek bir String bile oluşturmadan IPAddress nesnesini doğrudan ekrana basıyoruz.
              xilent.printf_P(PSTR("Local IP : %d.%d.%d.%d<br>\r\n"), lip[0], lip[1], lip[2], lip[3]);  // Not: Kodunda lipStr tanımı vardı ama lip nesnesi yukarıda eksikti. Eğer lip yukarıda tanımlıysa bu satır çalışır.
              xilent.printf_P(PSTR("Local Gateway : %d.%d.%d.%d<br>\r\n"), gip[0], gip[1], gip[2], gip[3]);
              xilent.printf_P(PSTR("Access IP : %d.%d.%d.%d</td>\r\n"), ip[0], ip[1], ip[2], ip[3]);

              // O devasa Form yapısını tek satırda Flash belleğe gömüyoruz.
              // capt değişkenini (%s) dinamik olarak formun içine yerleştiriyoruz.
              xilent.printf_P(PSTR("<form autocomplete='off' action='%s/auth:' method='get'><br><label>%s<br><label>kodu gir: </label><input name='capt' style='width:70px;' maxlength='4'><br><label>Username: </label><input name='unme' style='width:70px;' maxlength='6'><label><br>password: </label><input name='pwrd' type='password' style='width:70px;' maxlength='4'><input type='submit' style='width:60px;background-color:lightgreen;'></form>\r\n"), usertoken.c_str(), capt.c_str());

              // Vazgeç butonu ve HTML kapanışı
              xilent.printf_P(PSTR("<br><br> <div align='left'><form action='%s/logcancel' method='POST'><input type='submit' value='vazgeç' style='width:60px;background-color:pink;'></form></div><br><br><br></body>\r\n"), usertoken.c_str());

              xilent.println();
              xilent.abort();
              return;
            }
          }


          /////////////////////
          /////////////////////////////////
          if (header.indexOf("/Menu0") > -1) { Menu = 0; }
          //if (header.indexOf("/Menu0") > -1){ Menu = 0;aut=0}
          if (header.indexOf("/Menu1") > -1 && aut > 1) {
            Menu = 1;
            menu1girilemez = false;
          }

          if (header.indexOf("/Menu1") > -1 && aut < 2) {
            Menu = 0;
            menu1girilemez = true;
          }

          //////////////////
          //////////////////


          creator += "ur";


          int headerset = 1;
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the xilent HTTP request, so send a response:  /*





          ///////////////////////////////////////////////////////////////
          ///////////////////////////////////////////////////////////////

          String strtmp = header;
          if (header.indexOf("/SEND>") > -1) {
            String htpServerip = "";
            String htpport = "";
            String port = "";
            String Sdata = "";
            if (strtmp.indexOf("http://") > -1) {
              htpServerip = strtmp.substring(strtmp.indexOf("http://"), strtmp.indexOf(",data:"));
              String porttmp = htpServerip.substring(htpServerip.indexOf("http://") + 7, htpServerip.length());
              if (porttmp.indexOf(":") > -1) {
                int portbas = porttmp.indexOf(":");
                htpServerip = strtmp.substring(strtmp.indexOf("http://"), portbas);
                port = strtmp.substring(portbas, strtmp.length());
              }
              if (strtmp.indexOf("SPAYPIN") > -1) Sdata = "/SEND>" + strtmp.substring(strtmp.indexOf("data:") + 5, strtmp.indexOf("SPAYPIN"));
              else Sdata = "/SEND>" + strtmp.substring(strtmp.indexOf("data:") + 5, strtmp.indexOf(" HTTP/1.1"));
            } else {
              if (strtmp.indexOf("SPAYPIN") > -1) Sdata = strtmp.substring(strtmp.indexOf("data:") + 5, strtmp.indexOf("SPAYPIN"));
              Sdata = strtmp.substring(strtmp.indexOf("data:") + 5, strtmp.length());
            }

            Serial.print("Sdata: ");
            Serial.println(Sdata);

            if (htpServerip.length() > 0) {
              sendserver80(htpServerip, port, Sdata);
            } else {
              for (int z = 0; z < 11; z++) {
                if (Sdata.length() < 1) break;
                String pnm;
                String pns;

                if (Sdata.indexOf(":") > -1) {
                  pnm = Sdata.substring(0, Sdata.indexOf(":"));
                  if (Sdata.indexOf(",") > -1) {
                    pns = Sdata.substring(Sdata.indexOf(":") + 1, Sdata.indexOf(","));
                    Sdata = Sdata.substring(Sdata.indexOf(",") + 1, Sdata.length());
                  } else {
                    pns = Sdata.substring(Sdata.indexOf(":") + 1, Sdata.indexOf(" HTTP/1.1"));
                    Sdata = "";
                  }
                  Serial.println(pnm + " pnm1  ---  1pns " + pns);
                  if (pnm.indexOf("VR") == 0) {
                    Serial.println(pnm + " pnm2  ---  2pns " + pns);
                    for (int vrr = 0; vrr < 6; vrr++) {
                      if (pnm.indexOf("VR" + (String)vrr) == 0) {
                        VRP[vrr] = pns;
                      }
                    }
                  } else
                    for (int hh = 0; hh < 11; hh++) {
                      Serial.print("pnm=");
                      Serial.print(pnm);
                      Serial.print("pinname[hh]=");
                      Serial.println(pinname[hh]);
                      if (pnm == pinname[hh]) {
                        Serial.println(pnm + " pnm3  ---  3pns " + pns);
                        PinState[hh] = pns;
                        updateoutput();
                        break;
                      }
                    }
                }
              }
            }
          }

          /////////////////////////////////////////////////////////
          /////////////////////////////////////////////////////////////////////
          //////////////////////////////////////////////////////
          ////////////////////////////////////////////////////////////////



          //////////////////////////////////////////
          if (header.indexOf("SPAYPIN") > -1) {
            xilent.println("HTTP/1.1 200 OK\r\n");
            xilent.println(lipStr + "=");
            for (int x = 0; x < pinsayisi + 1; x++) {
              if (pinmode[x] == "OUT" || pinmode[x] == "INP") {
                String gonrd = pinname[x] + "|" + pinmode[x] + "|" + pinsignaltype[x] + "|" + pinminvalue[x] + "|" + pinval[x] + "|" + pinmaxvalue[x] + "|" + acilseviyesi[x] + "|" + acildeger[x] + "|" + pinlabel[x] + "|" + PinState[x] + "<br>";
                xilent.println(gonrd);
              }
            }
            xilent.println("EndText");
            return;
          }

          //////////////////////////////////////////
          ehabp = habp;

          if (header.indexOf("habp=0") > -1) habp = 0;

          if (header.indexOf("habp=1") > -1) habp = 1;

          //if (header.indexOf("habp=fir") > -1) habp = 2;

          if (header.indexOf("habp=2") > -1) habp = 2;

          if (header.indexOf("habp=3") > -1) habp = 3;
          //        Serial.println(header);
          //        Serial.print(ehabp);  Serial.print(" ehabp ");Serial.println(habp);

          if (header.indexOf("/mqttipayar?is=") > -1) {
            MQTTip = header.substring((header.indexOf("?is=") + 4), (header.indexOf(" HTTP/")));
            mqttipkaydet(MQTTip);
            ehabp = 10000;
          }
          if (header.indexOf("/mqttipayar?yol=") > -1) {
            YOL = header.substring((header.indexOf("?yol=") + 5), (header.indexOf(" HTTP/")));
            dosyayazfbyol();
            ehabp = 10000;
          }


          if ((ehabp > 0 && habp == 0) || (ehabp == 0 && habp > 0)) {
            xilent.println("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE html><html><head><title>" + esphostname + "  -> " + lipStr + " - ESP8266-Pro.v4</title>");
            xilent.println("<meta name='viewport' content='width=device-width, initial-scale=1'>");
            xilent.println("<meta charset='UTF-8'>");
            if (sayfayenile > 0) xilent.println("<script> setTimeout(function(){window.location.href = '/';}, 10000);</script>");
            //xilent.println("<link rel='icon' href='data:,'>");
            xilent.println("</head><body> Resetleniyor... Sayfayı yeniden ");
            xilent.printf_P(PSTR("<a href='%s/'><button>yüklemek</button></a> için 10 saniye bekleyin!\r\n"), usertoken.c_str());
            xilent.println("</body></html>");
            delay(1000);
            xilent.abort();
            delay(10);
            if (ehabp != 10000) dosyayazhabp();
            delay(1000);
            ESP.reset();
          }

          if (ehabp != habp) {
            if (ehabp == 2 && habp == 3) {
              String mqyol = "/" + YOL + "/FBSERVER";
              String mqdat = "KILLME>" + esphostname;
              rutingonder10s = millis();
              mqttsend(mqyol, mqdat);
            }

            if (ehabp == 3 && habp == 2) {
              if (mqttclient.connected()) {
                IPAddress lip = WiFi.localIP();
                mylocalip = String(lip[0]) + '.' + String(lip[1]) + '.' + String(lip[2]) + '.' + String(lip[3]);

                String mqyol = "/" + YOL + "/FBSERVER";
                String mqdat = "PAYNEW>" + WiFi.macAddress() + "<" + esphostname + ">" + mylocalip;
                rutingonder10s = millis();
                mqttsend(mqyol, mqdat);
              }
            }
            dosyayazhabp();
          }


          if (Menu == 1) {


            creator += "at";

            httpheader(xilent);

if (header.indexOf(F("/pnayar")) > -1 || header.indexOf(F("/programkayit")) > -1 || header.indexOf(F("/uprogramkayit")) > -1 || header.indexOf(F("/umoddegis")) > -1) {


  // 🎯 SİZİN HARİKA FİKRİNİZ: Mod değiştirme isteği geldiyse işi yap ve header'ı ez!
  if (header.indexOf(F("/umoddegis")) > -1) {
    uzunMetrajVar = !uzunMetrajVar; // Durumu anında tersine çevir (true/false)
    
    // LittleFS kalıcı hafızaya umod durumunu kaydediyoruz
    dosya.close();
    dosya = LittleFS.open("/umod.txt", "w+");
    if (dosya) {
      dosya.print(uzunMetrajVar ? "1" : "0");
      dosya.close();
    }
    
    // 🚀 SİHİRLİ SATIR: Gelen header'ı ceket değiştirir gibi havada değiştiriyoruz!
    // Kod aşağıya doğru akmaya devam edecek ve aşağıdaki 'if(header.indexOf("/pnayar")>-1)' 
    // şartına tam uyum sağlayarak sayfayı anında yeni modla tazeleyecektir.
    header = "GET /" + usertoken + "/pnayar HTTP/1.1";
  }


  // =================================================================
  // --- VERİ İŞLEME VE DOSYA YAZMA BÖLÜMÜ ---
  // =================================================================
  
  // 1. Standart Program Kayıt İsteyi
  if (header.indexOf(F("/programkayit?is=")) > -1) {
    String programdatatm = header.substring(header.indexOf(F("kayit?is=")) + 9, header.indexOf(F(" HTTP/1.1")));
    xilent.print(F("<br>program"));
    dosyaYazprogram(programdatatm);
    if (programerrorlog != "") xilent.print(F(" yazılmadı. Hatalı giriş var.<br>"));
    else xilent.print(F(" yazıldı.<br>"));
  }

  // 2. Yeni Eklenen: Uzun Metraj Şov Programı Kayıt İsteği
  if (header.indexOf(F("/uprogramkayit?is=")) > -1) {
    String uprogramdatatm = header.substring(header.indexOf(F("uprogramkayit?is=")) + 17, header.indexOf(F(" HTTP/1.1")));
    xilent.print(F("<br>uzun metraj program"));
    dosyazuprogram(uprogramdatatm); // Uzun metrajı kaydeden fonksiyonunuz
    if (uprogramerrorlog != "") xilent.print(F(" yazılmadı. Hatalı giriş var.<br>"));
    else xilent.print(F(" yazıldı.<br>"));
  }

  // 3. Pin Ayarları Kayıt İsteği
  if (header.indexOf(F("/pnayar?is=")) > -1) {
    pinayartmp = header.substring(header.indexOf(F("?is=")) + 4, header.indexOf(F(" HTTP/1.1")));
    xilent.print(F("<br>pinayar"));
    dosyaYazpinayar();
    dosyaOkupinayar();
    if (pinayarerrorlog != "") xilent.print(F(" yazılmadı. Hatalı giriş var.<br>"));
    else xilent.print(F(" yazıldı.<br>"));
  }

  // =================================================================
 // --- HTML ARAYÜZ ÇIKTI BÖLÜMÜ ---
  xilent.print(F("<style>\r\n"
                 "  .btn-mod { padding: 10px 15px; border: none; border-radius: 4px; font-weight: bold; cursor: pointer; color: white; transition: 0.3s; }\r\n"
                 "</style>\r\n"
                 "<div style='background: white; padding: 15px; border-radius: 6px; margin-bottom: 15px; text-align: left; box-shadow: 0 2px 5px rgba(0,0,0,0.05);'>\r\n"
                 "  <span style='font-weight: bold; font-size: 15px;'>Çalışma Modu: </span>"));

if (uzunMetrajVar) {
  // onclick içindeki tırnak kaçırmalarına ve JavaScript sözdizimine dikkat:
  xilent.printf_P(PSTR("<button class='btn-mod' style='background-color: #4CAF50;' "
                       "onclick=\"window.location.href='http://%s/%s/umoddegis'\">🚀 UZUN METRAJ MODU (AKTİF)</button>"), 
                  WiFi.localIP().toString().c_str(), usertoken.c_str());
} else {
  xilent.printf_P(PSTR("<button class='btn-mod' style='background-color: #555;' "
                       "onclick=\"window.location.href='http://%s/%s/umoddegis'\">📝 STANDART MOD (AKTİF)</button>"), 
                  WiFi.localIP().toString().c_str(), usertoken.c_str());
}
  
  xilent.print(F("  <p style='font-size: 12px; color: gray; margin: 5px 0 0 0;'>Modu değiştirmek için butona tıklayın. Sistem otomatik yenilenecektir.</p>\r\n"
                 "</div>\r\n"));


  // TABLO 1: PIN AYARLARI TABLOSU (Aynen korunuyor)
  xilent.printf_P(PSTR("<br><br><table> <caption>PIN AYARLARI</caption> <tr> <td scope='col'>pin ayarları:</td><td scope='col'>Pin Hata</td></tr>\r\n"
                       "<td><form method='get' action='%s/pnayar'>\r\n"
                       "<textarea name='is' id='id' cols='40' rows='10'>%s</textarea><br>\r\n"
                       "<input type='submit' value='Pinayar Kaydet'>\r\n"
                       "</form>\r\n"
                       "</td><td style='font-size:12px;vertical-align:top;'>"),
                  usertoken.c_str(), pinayartmp.c_str());

  xilent.println(pinayarerrorlog);

  xilent.print(F("</td></tr></table>\r\n"
                 "<label style='font-size:14px;'>\r\n"
                 "Altta bir ornek verilmistir.<br>\r\n"
                 "<a href='https://github.com/mbshur/lolinpro/raw/main/lolinhelp/turkce/lolin_tr.pdf' target='_blank'>Yardım [TR]</a><br>\r\n"
                 "D1|OUT|DIG|0|0|1|0|0|Sarj1_pirizi<br>\r\n"
                 "</label><br>\r\n"));

  // TABLO 2: DİNAMİK PROGRAM GİRİŞİ TABLOSU
  // Eğer uzunMetrajVar true ise Şov Editörünü, false ise Standart Editörü basıyoruz
  if (uzunMetrajVar) {
    // 🎯 Uzun Metraj Dünyası (/uprogramkayit ve uprogramdata)
    xilent.printf_P(PSTR("<table><caption>UZUN METRAJ ŞOV GİRİŞİ</caption>\r\n"
                         "<tr><td scope='col'>Şov Senaryosu (uprogram):<br>\r\n"
                         "<form method='get' action='%s/uprogramkayit'>\r\n"
                         "<textarea name='is' id='id' cols='60' rows='15'>%s</textarea><br>\r\n"
                         "<input type='submit' value='Şovu Kaydet'>\r\n"
                         "</form>\r\n"
                         "</td></tr><br>\r\n"),
                    usertoken.c_str(), uprogramdata.c_str());

    if (uprogramerrorlog.length() < 1) {
      xilent.print(F("<td style='font-size:12px;vertical-align:top;'>Hata yok</td>\r\n"));
    } else {
      xilent.printf_P(PSTR("<td scope='col' style='font-size:12px;vertical-align:top;'>Şov Hatası: %s</td>\r\n"), uprogramerrorlog.c_str());
    }
  } 
  else {
    // 🎯 Standart Kısa Program Dünyası (/programkayit ve programdata)
    xilent.printf_P(PSTR("<table><caption>STANDART PROGRAM GİRİŞİ</caption>\r\n"
                         "<tr><td scope='col'>Program (program.txt):<br>\r\n"
                         "<form method='get' action='%s/programkayit'>\r\n"
                         "<textarea name='is' id='id' cols='60' rows='15'>%s</textarea><br>\r\n"
                         "<input type='submit' value='Programı Kaydet'>\r\n"
                         "</form>\r\n"
                         "</td></tr><br>\r\n"),
                    usertoken.c_str(), programdata.c_str());

    if (programerrorlog.length() < 1) {
      xilent.print(F("<td style='font-size:12px;vertical-align:top;'>Hata yok</td>\r\n"));
    } else {
      xilent.printf_P(PSTR("<td scope='col' style='font-size:12px;vertical-align:top;'>Program Hatası: %s</td>\r\n"), programerrorlog.c_str());
    }
  }

  // Tablo kapatma etiketleri
  xilent.print(F("</tr><br></table><br><br><br><br>\r\n"));
}



            creator += " B";
            //Serial.println(header);


            creator += "EK";

            if (header.indexOf(F("/ssidset")) > -1) {

              // --- 1. AYARLARI YAZMA VE RESET ATMA BÖLÜMÜ ---
              if (header.indexOf(F("/ssidset?")) > -1) {
                if (header.indexOf(F("ssid=")) > -1) {
                  ssid = header.substring(header.indexOf(F("ssid=")) + 5, header.indexOf(F("&")));
                  Serial.println(ssid);
                }
                if (header.indexOf(F("&pass=")) > -1) {
                  pass = header.substring(header.indexOf(F("&pass=")) + 6, header.indexOf(F(" HTTP")));
                  Serial.println(pass);
                  dosyayazssidpass();

                  // Kullanıcıya bilgi verip bağlantıyı kibarca sonlandırıyoruz ve reset atıyoruz
                  xilent.print(F("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML><html><body>Ayarlar kaydedildi. Cihaz yeniden baslatiliyor...</body></html>\r\n"));
                  xilent.println();
                  delay(500);  // Verinin tarayıcıya ulaşması için çok kısa bir süre tanıyoruz
                  ESP.reset();
                }
              }

              // --- 2. IP ADRESLERİNİ DOĞRUDAN OKUMA (SIFIR RAM) ---
              IPAddress gip = WiFi.gatewayIP();
              IPAddress dns = WiFi.dnsIP();
              IPAddress ip = WiFi.softAPIP();

              // --- 3. HTML ARAYÜZÜ ÇIKTI BÖLÜMÜ (SIFIR RAM) ---

              // İlk tablo başlığı ve IP bilgileri şablonu
              // Not: lipStr basmaya çalıştığın değişken localIP ise doğrudan lip nesnesiyle değiştirebiliriz.
              // Burada tanımlı olduğunu varsaydığımız lip (WiFi.localIP()) kullanılmıştır.
              // IPAddress nesnelerinin içindeki her bir baytı, [1]... diyerek tek tek çıkartıyoruz
              xilent.printf_P(PSTR("<br><br><table style='border:2px solid green;width:500px'><tr><td style='border:1px solid black;width:250px'>\r\n"
                                   "Bilgiler<br><br>\r\n"
                                   "Local IP : %d.%d.%d.%d<br>\r\n"
                                   "Local Gateway : %d.%d.%d.%d<br>\r\n"
                                   "Access IP : %d.%d.%d.%d</td>\r\n"
                                   "<td style='border:1px solid black;width:250px'>\r\n"
                                   "<form action='%s/ssidsetscanwifi' method='POST'><input type='submit' value='Wifi tara'></form>\r\n"),
                              lip[0], lip[1], lip[2], lip[3],
                              gip[0], gip[1], gip[2], gip[3],
                              ip[0], ip[1], ip[2], ip[3], usertoken.c_str());

              if (header.indexOf(F("scanwifi HTTP")) > -1) {
                rescanwifi = true;
              }

              // Tarama yönlendirmeleri ve bulunan Wi-Fi listesi tablosu
              xilent.printf_P(PSTR("<br>Wifi tara tıkladıktan sonra 10 sn bekle<br>\r\n"
                                   "<br>Sonra tarama sonucu butonuna tıkla<br>\r\n"
                                   "<form action='%s/ssidset' method='POST'><input type='submit' value='Tarama sonucu'></form>\r\n"
                                   "</td></tr></table>\r\n"
                                   "<p>\r\n"
                                   "<table style='border:2px solid black;width:500px'><tr><td style='border:1px solid black;width:500px'>\r\n"
                                   "Tarama sonucunda bulunabilen Wifi cihazları listesi</td><tr><td><br>\r\n"),
                              usertoken.c_str());

              // Bulunan Wi-Fi ağlarının string listesini (st) güvenle basıyoruz
              xilent.println(st);

              // Formun geri kalanı ve input alanları
              xilent.printf_P(PSTR("</td><tr></table>\r\n"
                                   "Kaydettikten sonra cihaza resetlenir.<br>\r\n"
                                   "<form action='%s/ssidset' method='get'><label>SSID: </label><input name='ssid' maxlength='32'><label>PASS: </label><input name='pass' maxlength='32'><input type='submit' value='Kaydet'></form>\r\n"
                                   "<br><br><br><br>\r\n"),
                              usertoken.c_str());
            }












            if (header.indexOf(F("/myssidnameayar")) > -1) {

              // --- 1. AYARLARI YAZMA BÖLÜMÜ ---
              if (header.indexOf(F("/myssidnameayar?is=")) > -1) {
                myssidyazilimi = header.substring(header.indexOf(F("?is=")) + 4, header.indexOf(F(" HTTP/")));
                Serial.println(F("baradan geçtim)"));
                dosyayazmyssidname();
              }

              // --- 2. HTML ARAYÜZ ÇIKTI BÖLÜMÜ (SIFIR RAM) ---
              // Form yapısını ve input etiketini tek bir şablon olarak Flash'a gömüyoruz.
              // Wi-Fi ismini (%s) çift tırnak içerisine (value='%s') alarak HTML standartlarına uygun hale getirdik.
              xilent.printf_P(PSTR("<br><br><form method='get' action='%smyssidnameayar'>\r\n"
                                   "<label>Benim wifi ismim (SSID) : </label>\r\n"
                                   "<input name='is' style='width:100px;' maxlength='32' value='%s'>\r\n"
                                   "<input type='submit'>\r\n"
                                   "</form>\r\n"
                                   "<br><br><br><br>\r\n"),
                              usertoken.c_str(), myssidyazilimi.c_str());
            }


            /*
if (header.indexOf(F("/serveripayar")) > -1) {

  // --- 1. AYARLARI YAZMA BÖLÜMÜ ---
  if (header.indexOf(F("/serveripayar?is=")) > -1) {
    htServerip = header.substring(header.indexOf(F("?is=")) + 4, header.indexOf(F(" HTTP/")));
    htserverkaydet(htServerip);
  }

  // --- 2. HTML ARAYÜZ ÇIKTI BÖLÜMÜ (SIFIR RAM) ---
  // Form yapısını, input alanını ve "Boş ise işlem yapılmaz." uyarısını tek bir blok halinde Flash'a alıyoruz.
  xilent.printf_P(PSTR("<br><br><form method='get' action='%sserveripayar'>\r\n"
                       "<label>Haberleşilecek cihaz ip no : </label>\r\n"
                       "<input name='is' style='width:100px;' maxlength='32' value='%s'>\r\n"
                       "<input type='submit'>\r\n"
                       "</form>\r\n"
                       "(Boş ise işlem yapılmaz.)<br><br><br><br>\r\n"), 
                  htServerip.c_str());
}
*/









            if (header.indexOf(F("/mqttipayar")) > -1) {

              // --- 1. VERİ KAYDETME VE YENİDEN BAŞLATMA LOGIC BÖLÜMÜ ---
              bool tetiklendi = false;

              // MQTT Yolu (YOL) parametresi geldi mi?
              if (header.indexOf(F("/mqttipayar?yol=")) > -1) {
                YOL = header.substring(header.indexOf(F("?yol=")) + 5, header.indexOf(F(" HTTP/")));
                // dosyayazyol(YOL); // Eğer bir kaydetme fonksiyonun varsa buraya ekleyebilirsin
                tetiklendi = true;
              }

              // MQTT Sunucu IP (is) parametresi geldi mi?
              if (header.indexOf(F("/mqttipayar?is=")) > -1) {
                MQTTip = header.substring(header.indexOf(F("?is=")) + 4, header.indexOf(F(" HTTP/")));
                // dosyayazmqttip(MQTTip); // Eğer bir kaydetme fonksiyonun varsa buraya ekleyebilirsin
                tetiklendi = true;
              }

              // Eğer iki ayardan biri değiştiyse kullanıcıya bilgi verip cihazı resetliyoruz
              if (tetiklendi) {
                xilent.print(F("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML><html><body>Ayarlar kaydedildi. Cihaz yeniden baslatiliyor...</body></html>\r\n"));
                xilent.println();
                delay(500);  // Verinin tarayıcıya ulaşması için zaman tanıyoruz
                ESP.reset();
                return;
              }

              // --- 2. HTML ARAYÜZ ÇIKTI BÖLÜMÜ (SIFIR RAM) ---
              // İki ayrı formu ve uyarı metinlerini tek bir büyük şablon halinde Flash belleğe alıyoruz.
              // Dinamik olan YOL ve MQTTip değişkenlerini %s ve .c_str() ile güvenli tırnaklar ('%s') içine gömüyoruz.
              xilent.printf_P(PSTR("<br>Dikkat!!! MQTT server ip ya da Yol kaydedilir veya değiştirilirse Cihaz yeniden başlatılacak.<br><br>\r\n"
                                   "MQTT yolu ile firebase yolu aynıdır.<br>\r\n"
                                   "<form method='get' action='%smqttipayar'>\r\n"
                                   "<label>Abone olunacak MQTT yolu: </label>\r\n"
                                   "<input name='yol' style='width:100px;' maxlength='32' value='%s'>\r\n"
                                   "<input type='submit'>\r\n"
                                   "</form><br><br>\r\n"
                                   "<form method='get' action='%smqttipayar'>\r\n"
                                   "<label>Bağlanılacak MQTT server ip: </label>\r\n"
                                   "<input name='is' style='width:100px;' maxlength='32' value='%s'>\r\n"
                                   "<input type='submit'>\r\n"
                                   "</form><br><br><br><br>\r\n"),
                              usertoken.c_str(),
                              YOL.c_str(),
                              MQTTip.c_str());
            }


            creator += "T";
            //Serial.println(header);



            creator += "AŞ";

            /*
            if (header.indexOf("/Boardtype?is=") > -1) {
              String Boardtypetmp = header.substring((header.indexOf("?is=") + 4), (header.indexOf("&mq=")));
              Boardtype = Boardtypetmp.toInt();
              dosyaYazBoardtype();
              Serial.println("Write flash file ssidname;Boardtype;");
              Serial.println(Boardtype);
              httpheader(xilent);
              xilent.println("<form method='get' action='%s/Boardtype'><label>ESP Board tipi ( Esp-01 icin 0  Esp-lolin icin 1) : </label><input name='is' length=32 value=");
              xilent.println(Boardtype);
              xilent.println("><input type='submit'>");
              xilent.println("</form>");
              if (Boardtype == 0) xilent.println("<form method='get' action='%sSERVERayar'><label>My SSID Name: </label><input name='is' length=32 value=");
              if (Boardtype == 1) xilent.println("<form method='get' action='%sSERVERayar'><label>My SSID Name: </label><input name='is' length=32 value=");
              xilent.println(myssidyazilimi);
              xilent.println("><br>");
              xilent.println("<label>Baglanilacak SERVER adressi: </label><input name='mq' length=32 value=");
//              xo += SERVERip;
              xilent.println("><input type='submit'>");
              xilent.println("</form>");
              xilent.println("<br>");
            }
*/


            if (header.indexOf(F("/kulupayr")) > -1) {
              Serial.println(F("kulpayr a girdim"));
              dosyaOkuusers();

              // --- 1. VERİ KAYDETME BÖLÜMÜ ---
              if (header.indexOf(F("/kulupayr?unme=")) > -1) {
                users = header.substring(header.indexOf(F("?unme=")) + 6, header.indexOf(F(" HTTP/")));
                dosyaYazusers();
                dosyaOkuusers();
                Serial.println(F("Write flash file ussdoc"));
              }

              // --- 2. HTML ARAYÜZ ÇIKTI BÖLÜMÜ (SIFIR RAM) ---
              // Form yapısını tek bir şablon olarak Flash belleğe gömüyoruz.
              // unme ve pwrd değişkenlerini %s kullanarak düzgün HTML tırnakları içerisine enjekte ediyoruz.
              xilent.printf_P(PSTR("<br><br>\r\n"
                                   "<form method='get' action='%s/kulupayr'>\r\n"
                                   "<textarea name='unme' id='unme' cols='40' rows='10'>%s</textarea>\r\n"
                                   "<input type='submit'>\r\n"
                                   "</form>\r\n"
                                   "<label>En fazla 5 kullanıcı kaydedilebilir. <br>Formatı aşağıdaki gibidir.<br>Kullanıcı_Adı, Kullanıcı_şifresi, Kullanıcı_yetkisi<br><br> Yetki  2-Yönetici, 1-Kullanıcı <br><br>Örnek:<br>admin,1111,2<br>murat,1234,1<br></label><br><br><br>\r\n"),
                              usertoken.c_str(), users.c_str());
            }










            // firebase web kısmı başı
            Serial.println(header);

            if (header.indexOf(F("/firebaseset")) > -1) {
              // --- 1. ÖNCE PROTOKOL DEĞİŞİKLİĞİ VE RESET KONTROLÜ (BAĞLANTIYI KOPARMAMAK İÇİN EN ÜSTE ALINDI) ---
              if (header.indexOf(F("fireb=Off")) > -1) {
                fben = false;
                dosyayazfben();
                xilent.print(F("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML><html><body>Firebase Kapatildi. Yeniden baslatiliyor...</body></html>\r\n"));
                xilent.println();
                delay(500);
                ESP.reset();
                return;
              }
              if (header.indexOf(F("fireb=On")) > -1) {
                fben = true;
                dosyayazfben();
                xilent.print(F("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML><html><body>Firebase Acildi. Yeniden baslatiliyor...</body></html>\r\n"));
                xilent.println();
                delay(500);
                ESP.reset();
                return;
              }

              dosyaokufburl();
              dosyaokufbyol();

              xilent.print(F("<br><br>Realtime Database Ayarlari<br>\r\n"));

              String errstring = "";
              if (header.indexOf(F("?fburl=")) > -1) {
                String hdrt = header;
                header = Karakterduzeltfunc(hdrt);

                DATABASE_URL = header.substring(header.indexOf(F("?fburl=")) + 7, header.indexOf(F("&fbapi=")));
                if (DATABASE_URL.indexOf(F(".")) > -2 && DATABASE_URL.indexOf(F(".")) < 2) {
                  errstring += F("URL Yanlis girilmis.<br>");
                }
                if (DATABASE_URL.length() == 0 || DATABASE_URL == "") {
                  errstring += F("URL boş birakilmis.<br>");
                }

                YOL = header.substring(header.indexOf(F("&fbyol=")) + 7, header.indexOf(F(" HTTP/1.1")));
                if (YOL.length() > 10) {
                  errstring += F("YOL Yanlış.<br>");
                }
                if (YOL.length() < 1 || YOL == "") {
                  errstring += F(" YOL boş.<br>");
                }

                if (errstring == "") {
                  dosyayazfburl();
                  dosyayazfbyol();
                }
              }

              if (fben == true && DATABASE_URL != "") {
                errstring = F("Firebase Ayarları yapılmış.");
              }

              // --- 2. HTML ARAYÜZ ÇIKTI BÖLÜMÜ (SIFIR RAM) ---

              // Ana form alanını ve çizgileri tek kalıp halinde Flash belleğe gömüyoruz
              xilent.printf_P(PSTR("<hr style='height:4px;border-width:0;color:black;background-color:gray'><br>\r\n"
                                   "<form method='get' action='%s/firebaseset'>\r\n"
                                   "<label>FBRTD nin URL 'i yazın <br> URL : </label>\r\n"
                                   "<input name='fburl' id='fburl' style='width:350px;' value='%s'><br><br>\r\n"
                                   "<label>Database deki Yol (Örnek: Balıkesir ev1: bev1, yazlık 10Yz1, gibi kısa kodlar kullanın)<br><br>MQTT yolu ile firebase yolu aynıdır.<br> YOL : </label>\r\n"
                                   "<input name='fbyol' id='fbyol' style='width:70px;' value='%s'><input type='submit'>\r\n"
                                   "</form>\r\n"
                                   "<hr style='height:4px;border-width:0;color:black;background-color:gray'>\r\n"),
                              usertoken.c_str(), DATABASE_URL.c_str(), YOL.c_str());

              // Firebase Açık/Kapalı Seçim Kombosu (Select Menu)
              if (DATABASE_URL != "") {
                xilent.printf_P(PSTR("<form action='%s/firebaseset'><label for='option'>Firebase protokolu:</label>\r\n"
                                     "<select id='fireb' name='fireb'>\r\n"
                                     "<option value='Off' %s>Kapali</option>\r\n"
                                     "<option value='On' %s>Acik</option>\r\n"
                                     "</select><input type='submit' value='Submit'></form>\r\n"),
                                (!fben ? "selected" : ""), (fben ? "selected" : ""), usertoken.c_str());
              }

              // Hata veya durum mesajını en alta dinamik olarak basıyoruz
              xilent.printf_P(PSTR("<br><br>%s<br><br>\r\n"), errstring.c_str());
            }
            //firebase web kısmı sonu











            // haberleşme protokolu 1 kapalı, 2 MQTT, 3 esp diğer espine mqttile bilgi git gel yapacak diğeri firebase ile onun adına haberleşecek...
            if (header.indexOf(F("/habp?habp")) > -1 || header.indexOf(F("/ HTTP/1.1")) > -1 || header.indexOf(F("/Menu1")) > -1) {

              // --- 1. PROTOKOL DEĞİŞTİRME VE RESET TETİKLEYİCİSİ (EN ÜSTE ALINDI) ---
              if (header.indexOf(F("/habp?habp=")) > -1) {
                String habpStr = header.substring(header.indexOf(F("?habp=")) + 6, header.indexOf(F(" HTTP/")));
                habp = habpStr.toInt();

                // dosyayazhabp(); // Eğer kaydetme fonksiyonun varsa burayı aktif edebilirsin

                xilent.print(F("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML><html><body>Protokol degistirildi. Cihaz yeniden baslatiliyor...</body></html>\r\n"));
                xilent.println();
                delay(500);  // Tarayıcıya paketin ulaşması için bekle
                ESP.reset();
                return;
              }

              // IP adreslerini doğrudan donanımdan çekiyoruz (Daha önce öğrendiğimiz gibi %d için)
              IPAddress gip = WiFi.gatewayIP();
              IPAddress dns = WiFi.dnsIP();
              IPAddress ip = WiFi.softAPIP();

              // --- 2. DURUM BİLGİLERİNİ BAŞTAN UYARLAMA ---
              // Wi-Fi durum metnini ve pin durum metnini RAM harcamadan hazırlıyoruz
              const char* wifiDurum = (WiFi.status() == WL_CONNECTED) ? "Baglanilan Wifi" : "Kayitli Wifi";
              const char* pinDurum = (pinayar.length() < 2) ? "Pin ayarlari yapilmamis!" : "Pin ayarlari tamam";

              // --- 3. HTML ÇIKTI BÖLÜMÜ (SIFIR RAM) ---
              xilent.print(F("<br>\r\n"));

              // Aktif haberleşme protokol durum mesajları
              if (habp == 2) {
                xilent.print(F("<font size='3' color='#000000'>MQTT ile FBSERVER haberleşmesi yüzünden firebase çalışıyor.</font><br><br>\r\n"));
              }
              if (fben == true && habp == 3) {
                xilent.print(F("<font size='3' color='#000000'>Firebase çalışıyor.</font><br><br>\r\n"));
              }

              // Sistem IP ve Cihaz Durum Bilgilerini Tek Büyük printf_P Bloğunda Basıyoruz
              xilent.printf_P(PSTR("Local IP : %d.%d.%d.%d<br>\r\n"
                                   "Local Gateway : %d.%d.%d.%d<br>\r\n"
                                   "Access IP : %d.%d.%d.%d<br><br>\r\n"
                                   "Wifi ismim : %s<br>\r\n"
                                   "%s : %s"),
                              lip[0], lip[1], lip[2], lip[3],
                              gip[0], gip[1], gip[2], gip[3],
                              ip[0], ip[1], ip[2], ip[3],
                              esphostname.c_str(),
                              wifiDurum, ssid.c_str());

              // Eğer Wi-Fi bağlıysa yanına sinyal gücünü (RSSI) de sıfır RAM ile basıyoruz
              if (WiFi.status() == WL_CONNECTED) {
                xilent.printf_P(PSTR(" Sinyal gücü : %d"), WiFi.RSSI());
              }
              xilent.print(F("<br>\r\n"));

              // Diğer MQTT ve Firebase yollarını tek şablonda birleştiriyoruz
              xilent.printf_P(PSTR("%s<br>\r\n"
                                   "Bağlanılacak MQTT server ip: %s<br>\r\n"
                                   "MQTT subscribe yolu: %s<br>\r\n"
                                   "Firebase data ve MQTT subscribe yolu: %s<br>\r\n"
                                   "Firebase url : %s<br><br>\r\n"
                                   "Diğer cihazlarla haberleşilecek protokol (kapalı) <---> (MQTT ya da MQTT&Firebase) değiştirildiğinde ESP restart yapılacak!!!<br>\r\n"),
                              pinDurum, MQTTip.c_str(), YOL.c_str(), YOL.c_str(), DATABASE_URL.c_str());

              // Protokol Seçim Kombo Menüsü (Select Menu) - selected durumları Ternary (? :) ile RAM tüketmeden çözüldü
              xilent.printf_P(PSTR("<br><form action='%s/habp'><label style='font-size: 12px;'>Diğer cihazlarla haberleşilecek protokol:\r\n"
                                   "<select id='habp' name='habp'>\r\n"
                                   "<option value='0' %s>Kapali</option>\r\n"
                                   "<option value='1' %s>MQTT</option>\r\n"
                                   "<option value='2' %s>MQTT&FBSERVER</option>\r\n"
                                   "<option value='3' %s>MQTT&Firebase</option>\r\n"
                                   "</select><input type='submit' value='Submit'> değiştirildiğinde ESP restart yapılacak!!!</label></form>\r\n"),
                              usertoken.c_str(),
                              (habp <= 0 ? "selected" : ""),
                              (habp == 1 ? "selected" : ""),
                              (habp == 2 ? "selected" : ""),
                              (habp == 3 ? "selected" : ""));



              // Dikkat Uyarısı ve Yardım Linki
              xilent.print(F("<br><font size='3' color='#FF0000'>Dikkat Ayarlar yapılıp kaydedilirken ESP ye bağlı tüm cihazların gücünü kapatın.<br>\r\n"
                             "Kayıttan sonra Reset anında pinlere İstek dışında enerji yollanabilir.<br>\r\n"
                             "cihazlar istek dışı çalışabilir.</font><br><br>\r\n"
                             "<a href='https://github.com/mbshur/lolinpro/raw/main/lolinhelp/turkce/lolin_tr.pdf' target='_blank'>Yardım [TR]</a><br>\r\n"));

              xilent.println();
            }
            // habp sonu ////////////





            // Clear the header variable
            // Telif bilgisi, MAC adresi ve tablo kapanış etiketlerini tek bir şablonla Flash'tan basıyoruz
            xilent.printf_P(PSTR("coded by %s. ESP kontrol (v4\r\n"
                                 "<label style='font-size:1px;'>%s</label>04.01.2026)<br><br>\r\n"
                                 "</td></table></td></tr></table>\r\n"),
                            creator.c_str(),
                            WiFi.macAddress().c_str());

            xilent.println();
            xilent.println();
            /////xilent.abort();;
          }



          if (Menu == 0) {


            creator += "at";
            //                              Serial.print("butonpbgcol:");
            //                          Serial.println(butonpbgcol);
            //dosyaOkupinayar();

            //xo+="Connection: close";

            // LOW İKEN RÖLE ÇALIŞIYOR

            // turns the GPIOs on   Motor
            // İşlemler

            String Headerparcala = header;
            int indis = -1;
            String pinismi = "";
            String pinkomut;
            String PWMdeger;
            String Analoggelendeger;
            String Digitalgelendeger;

            //Serial.print("Headerparcala :"); Serial.println(Headerparcala);


            //butonactcol butonpascol butonayrcol butonpbgcol


            //String butonactcol = "#d1ca03";
            //String butonpascol = "#A3A3A3";
            //String butonayrcol = "#20d3c8";
            //String menutextcol = "000000";
            //String butonpbgcol = "#ffb12a";


            if (Headerparcala.indexOf("/butonactcol?is=") > -1) {
              Headerparcala = Headerparcala.substring(Headerparcala.indexOf("/butonactcol?is=") + 16, Headerparcala.length());
              butonactcol = Headerparcala.substring(0, Headerparcala.indexOf(" HTTP"));
              butonactcolyaz();
            }

            if (Headerparcala.indexOf("/butonpascol?is=") > -1) {
              Headerparcala = Headerparcala.substring(Headerparcala.indexOf("/butonpascol?is=") + 16, Headerparcala.length());
              butonpascol = Headerparcala.substring(0, Headerparcala.indexOf(" HTTP"));
              butonpascolyaz();
            }

            if (Headerparcala.indexOf("/butonayrcol?is=") > -1) {
              Headerparcala = Headerparcala.substring(Headerparcala.indexOf("/butonayrcol?is=") + 16, Headerparcala.length());
              butonayrcol = Headerparcala.substring(0, Headerparcala.indexOf(" HTTP"));
              butonayrcolyaz();
            }



            if (Headerparcala.indexOf("/menutextcol?is=") > -1) {
              Headerparcala = Headerparcala.substring(Headerparcala.indexOf("/menutextcol?is=") + 16, Headerparcala.length());
              menutextcol = Headerparcala.substring(0, Headerparcala.indexOf(" HTTP"));
              menutextcolyaz();
            }


            if (Headerparcala.indexOf(" /butonpbgcol") > -1) {
              Headerparcala = Headerparcala.substring(Headerparcala.indexOf("/butonpbgcol") + 16, Headerparcala.length());
              //Serial.println(Headerparcala);
              butonpbgcol = Headerparcala.substring(0, Headerparcala.indexOf(" HTTP"));
              butonpbgcolyaz();
            }

            creator += " BE";

            //LOLİN için
            if (pinayar.length() > 3) {
              if (Headerparcala.indexOf("GET /D") > -1) {
                htpcldepindegisti = true;
                if (header.indexOf("?pwm=") > -1) {
                  Headerparcala = Headerparcala.substring(Headerparcala.indexOf(" /D") + 2, Headerparcala.length());
                  pinismi = Headerparcala.substring(0, Headerparcala.indexOf("?"));
                } else if (header.indexOf("?ser=") > -1) {
                  Headerparcala = Headerparcala.substring(Headerparcala.indexOf(" /D") + 2, Headerparcala.length());
                  pinismi = Headerparcala.substring(0, Headerparcala.indexOf("?"));
                } else {
                  Headerparcala = Headerparcala.substring(Headerparcala.indexOf(" /D") + 2, Headerparcala.length());
                  pinismi = Headerparcala.substring(0, Headerparcala.indexOf("/"));
                }


                int pinismiint;
                if (pinismi.length() > 1)
                  pinismiint = pinismi.substring(1, 2).toInt();
                else pinismiint = pinismi.toInt();
                /*
                for(int qq=0;qq<sizeof(Pin)+1qq++){
                  Serial.print( qq ); Serial.println("       " + pinsignaltype[qq]);
                }
              Serial.println(pinsignaltype[pinismiint]);
*/
                // pinsinyaline göre parametre eklenecek PWM için PWM degeri
                if (pinsignaltype[pinismiint] == "PWM") {
                  // Serial.println(Headerparcala);
                  Headerparcala = Headerparcala.substring(Headerparcala.indexOf("?pwm=") + 5, Headerparcala.length());
                  pinkomut = "";
                  PWMdeger = Headerparcala.substring(0, Headerparcala.indexOf(" HTTP"));
                  //Serial.println(PWMdeger);
                  //Serial.println("pwmdeger:::::::::::::::::::::: " + PWMdeger);
                  PinState[pinismiint] = PWMdeger;
                } else if (pinsignaltype[pinismiint] == "SER") {
                  //Serial.println(Headerparcala);
                  Headerparcala = Headerparcala.substring(Headerparcala.indexOf("?ser=") + 5, Headerparcala.length());
                  pinkomut = "";
                  PWMdeger = Headerparcala.substring(0, Headerparcala.indexOf(" HTTP"));
                  //Serial.println(PWMdeger);
                  //Serial.println("pwmdeger:::::::::::::::::::::: " + PWMdeger);
                  PinState[pinismiint] = PWMdeger;
                } else {
                  Headerparcala = Headerparcala.substring(Headerparcala.indexOf(":") + 1, Headerparcala.length());
                  pinkomut = Headerparcala.substring(0, Headerparcala.indexOf(" HTTP"));
                }


                //Serial.print("pinkomut : ");Serial.println(pinkomut);




                //Programtakip(programdata);
              }







              creator += "K";




              if (header.indexOf("GET /A") > -1) {
                htpcldepindegisti = true;
                if (header.indexOf("?pwm=") > -1) {
                  Headerparcala = Headerparcala.substring(Headerparcala.indexOf(" /A") + 2, Headerparcala.length());
                  pinismi = Headerparcala.substring(0, Headerparcala.indexOf("?"));
                } else if (header.indexOf("?ser=") > -1) {
                  Headerparcala = Headerparcala.substring(Headerparcala.indexOf(" /A") + 2, Headerparcala.length());
                  pinismi = Headerparcala.substring(0, Headerparcala.indexOf("?"));
                } else {
                  Headerparcala = Headerparcala.substring(Headerparcala.indexOf(" /A") + 2, Headerparcala.length());
                  pinismi = Headerparcala.substring(0, Headerparcala.indexOf("/"));
                }


                int pinismiint;
                if (pinismi.length() > 1)
                  pinismiint = pinismi.substring(1, 2).toInt();
                else pinismiint = pinismi.toInt();


                if (pinismi.indexOf("A") > -1) pinismiint += 9;



                Serial.print(pinismi + " ");

                if (pinmode[pinismiint] == "OUT" & pinsignaltype[pinismiint] == "DIG") {

                  if (pinkomut == "1" || pinkomut == "HIGH" || pinkomut == "ON") {
                    PinState[pinismiint] = "1";
                  }

                  if (pinkomut == "0" || pinkomut == "LOW" || pinkomut == "OFF") {
                    PinState[pinismiint] = "0";
                  }
                }


                if (pinmode[pinismiint] == "OUT" && pinsignaltype[pinismiint] == "PWM") {
                  int PWMdegerint = PWMdeger.toInt();
                  PinState[pinismiint] = PWMdeger;
                  if (PinState[pinismiint] != ePinState[pinismiint]) {
                    Outpwm(pinismi, PWMdegerint);
                  }
                }
                //if(habp == 3)zamanfark = 3;
                //else zamanfark = 1555;
              }



              if (header.indexOf((" /rr120")) > -1) sayfayenile = 120;
              if (header.indexOf((" /rr30")) > -1) sayfayenile = 30;
              if (header.indexOf((" /rr10")) > -1) sayfayenile = 10;
              if (header.indexOf((" /rr0")) > -1) sayfayenile = 0;

              pinkomut.toUpperCase();
              //Serial.print("pinkomut.toUpperCase()"); Serial.println(pinkomut);
              //dosyaOkupinayar();

              int pinismiint;
              if (pinismi.length() > 1)
                pinismiint = pinismi.substring(1, 2).toInt();
              else pinismiint = pinismi.toInt();

              if (pinismi.indexOf("A") > -1) pinismiint += 9;


              creator += "T";
              Serial.print(pinismi + " ");

              if (pinmode[pinismiint] == "OUT" & pinsignaltype[pinismiint] == "DIG") {

                if (pinkomut == "1" || pinkomut == "HIGH" || pinkomut == "ON") {
                  PinState[pinismiint] = "1";
                }

                if (pinkomut == "0" || pinkomut == "LOW" || pinkomut == "OFF") {
                  PinState[pinismiint] = "0";
                }
              }


              if (pinmode[pinismiint] == "OUT" && pinsignaltype[pinismiint] == "PWM") {
                int PWMdegerint = PWMdeger.toInt();
                PinState[pinismiint] = PWMdeger;
                if (PinState[pinismiint] != ePinState[pinismiint]) {
                  //Outpwm(pinismi, PWMdegerint);
                  //Programtakip(programdata);
                }
              }

              if (pinmode[pinismiint] == "OUT" && pinsignaltype[pinismiint] == "SER") {
                int PWMdegerint = PWMdeger.toInt();
                if (PWMdegerint > 180) PWMdegerint = 180;
                PinState[pinismiint] = String(PWMdegerint);
                if (PinState[pinismiint] != ePinState[pinismiint]) {
                  //myservo[pinismiint].write(PWMdegerint);
                  //Programtakip(programdata);
                }
              }



              creator += "A";
            }

            if (pinayar.length() > 0) updateinput();
            if (pinayar.length() > 0 && programdata.length() > 0 && psci==true) programrun();
            if (pinayar.length() > 0) updateoutput();
            creator += "Ş";

            // --- ANA SAYFA BAŞLANGICI VE CSS OPTİMİZASYONU (SIFIR RAM) ---

            // IPAddress nesnesini doğrudan donanımdan çekiyoruz (Pointer hatasını engellemek için)
            IPAddress lip = WiFi.localIP();

            // 1. HTML Başlığı, Meta Etiketleri ve Dinamik Title Alanı
            xilent.printf_P(PSTR("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE html><html><head>\r\n"
                                 "<title>%s -> %d.%d.%d.%d - ESP8266-Pro.v4</title>\r\n"
                                 "<meta name='viewport' content='width=device-width, initial-scale=1'>\r\n"
                                 "<meta charset='UTF-8'>\r\n"
                                 "<link rel='icon' href='data:,'>\r\n"),
                            esphostname.c_str(),
                            lip[0], lip[1], lip[2], lip[3]);

            // 2. Dinamik Sayfa Yenileme (Script) Optimizasyonu
            if (sayfayenile > 0) {
              xilent.printf_P(PSTR("<script>setTimeout(function(){window.location.href = '/';}, %d);</script>\r\n"),
                              (sayfayenile * 1000));
            }

            // 3. Devasa CSS Bloğu ve Dinamik Renklerin (%s) Tek Seferde Flash'tan Basılması
            xilent.printf_P(PSTR("<style>\r\n"
                                 "html{ font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\r\n"
                                 ".button { background-color: #%s; border: 2px solid yellow; border-radius: 10px 10px 10px 10px; color: white; padding: 2px 2px; text-decoration: none; font-size: 24px; margin: 1px; cursor: pointer;}\r\n"
                                 ".butoff { background-color: #%s; border: 2px solid gray; border-radius: 10px 10px 10px 10px; color: white; padding: 2px 2px; text-decoration: none; font-size: 24px; margin: 1px; cursor: pointer;}\r\n"
                                 ".butayr { background-color: #%s; border: 2px solid gray; border-radius: 6px 6px 10px 10px; color: black; padding: 2px 2px; text-decoration: none; font-size: 28px; margin: 1px; cursor: pointer;}\r\n"
                                 "body { background-color: #%s; }\r\n"
                                 "</style>\r\n"
                                 "</head>\r\n"),
                            butonactcol.c_str(),
                            butonpascol.c_str(),
                            butonayrcol.c_str(),
                            butonpbgcol.c_str());

            // --- ANA SAYFA ÜST DURUM VE YENİLEME BUTONLARI BÖLÜMÜ (SIFIR RAM) ---

            // 1. Body Başlangıcı ve Giriş/Çıkış Butonları
            xilent.print(F("<body>\r\n"));

            if (aut < 1) {
              xilent.printf_P(PSTR("<div align='left'><form action='%s/login' method='POST'><input type='submit' value='Giriş' style='width:60px;background-color:white;'></form></div>\r\n"), usertoken.c_str());
            }
            if (aut >= 1) {
              xilent.printf_P(PSTR("<div align='left'><form action='%s/logout' method='POST'><input type='submit' value='Çıkış' style='width:60px;background-color:white;'></form></div>\r\n"), usertoken.c_str());
            }

            xilent.printf_P(PSTR("<div align='center'>\r\n"
                                 "<label style='font-size: 10px;'>Sayfayı yenileme için alttaki düğmeyi kullanabilirsiniz</label>\r\n"),
                            usertoken.c_str());

            // Sayfayı yenileme durumu mesajı
            if (sayfayenile > 0) {
              xilent.printf_P(PSTR("<label style='font-size: 10px;'> : Sayfayı yenile Açık %d saniyede bir yenilenecek</label>\r\n"), sayfayenile);
            } else {
              xilent.print(F("<label style='font-size: 10px;'> : Sayfayı yenile Kapalı</label>\r\n"));
            }
            if (usernamelbl != "") {
              String sev = "";
              if (aut == 1) sev = "🤵";
              if (aut == 2) sev = "🕵️‍♂️";
              xilent.printf_P(PSTR("<label style='font-size: 10px;'> USER :%s %s</label>\r\n"), sev.c_str(), usernamelbl.c_str());
            }
            // 2. Sayfa Yenileme Süre Butonları Tablosu
            // Butonların aktiflik durumlarına göre arka plan renk stillerini (? :) Ternary yapısıyla tek kalıpta basıyoruz
            xilent.printf_P(PSTR("<table style='border:5px solid black;width:00px'><tr>\r\n"
                                 "<td style='border:0px solid black;width:90px; align:center; '></td>\r\n"  // Orijinal koddaki 90p hatası 90px yapıldı
                                 "<td style='border:1px solid black;width:250px; align:center; '><form action='%s/rr120' method='POST'><input type='submit' value='120s' style='width:50px; %s'></form></td>\r\n"
                                 "<td style='border:1px solid black;width:250px; align:center; '><form action='%s/rr30' method='POST'><input type='submit' value='30s' style='width:50px; %s'></form></td>\r\n"
                                 "<td style='border:1px solid black;width:250px; align:center; '><form action='%s/rr10' method='POST'><input type='submit' value='10s' style='width:50px; %s'></form></td>\r\n"
                                 "<td style='border:1px solid black;width:250px; align:center; '><form action='%s/rr0' method='POST'><input type='submit' value='Kapat' style='width:50px;'></form></td>\r\n"
                                 "</tr></table></div>\r\n"),
                            (sayfayenile == 120 ? "background-color:yellow;" : ""),
                            (sayfayenile == 30 ? "background-color:yellow;" : ""),
                            (sayfayenile == 10 ? "background-color:yellow;" : ""), usertoken.c_str(), usertoken.c_str(), usertoken.c_str(), usertoken.c_str());

            // 3. Cihaz Bilgileri ve Bağlantı Durumu Göstergeleri
            // esphostname içindeki son eki (tire işaretinden sonrasını) String substring ile RAM harcamak yerine doğrudan buluyoruz
            int dashIndex = esphostname.indexOf("-");
            const char* sonekPtr = (dashIndex > -1) ? esphostname.c_str() + dashIndex + 1 : esphostname.c_str();

            xilent.printf_P(PSTR("<label style='font-size: 20px;'>%s</label> &emsp;&emsp;\r\n"), sonekPtr);


// anasayfada Çalışma modu başı
  // 🎯 SİZİN HARİKA FİKRİNİZ: Mod değiştirme isteği geldiyse işi yap ve header'ı ez!
  if (header.indexOf(F("/umoddegis")) > -1) {
    uzunMetrajVar = !uzunMetrajVar; // Durumu anında tersine çevir (true/false)
    
    // LittleFS kalıcı hafızaya umod durumunu kaydediyoruz
    dosya.close();
    dosya = LittleFS.open("/umod.txt", "w+");
    if (dosya) {
      dosya.print(uzunMetrajVar ? "1" : "0");
      dosya.close();
    }
    
    // 🚀 SİHİRLİ SATIR: Gelen header'ı ceket değiştirir gibi havada değiştiriyoruz!
    // Kod aşağıya doğru akmaya devam edecek ve aşağıdaki 'if(header.indexOf("/pnayar")>-1)' 
    // şartına tam uyum sağlayarak sayfayı anında yeni modla tazeleyecektir.
    if(Menu==1) header = "GET /" + usertoken + "/pnayar HTTP/1.1";
    if(Menu==0) header = "GET /" + usertoken + "/ HTTP/1.1";
  }




if (uzunMetrajVar) {
  xilent.print(F("<style>\r\n"
                 "  .btn-mod { padding: 10px 15px; border: none; border-radius: 4px; font-weight: bold; cursor: pointer; color: white; transition: 0.3s; }\r\n"
                 "</style>\r\n"
                 "<div style='background: white; padding: 15px; border-radius: 6px; margin-bottom: 15px; text-align: left; box-shadow: 0 2px 5px rgba(0,0,0,0.05);'>\r\n"
                 "  <span style='font-weight: bold; font-size: 15px;'>Çalışma Modu: </span>"));
}
if (uzunMetrajVar) {
  // onclick içindeki tırnak kaçırmalarına ve JavaScript sözdizimine dikkat:
  xilent.printf_P(PSTR("<button class='btn-mod' style='background-color: #4CAF50;' "
                       "onclick=\"window.location.href='http://%s/%s/umoddegis'\">🚀 UZUN METRAJ MODU (AKTİF)</button>"), 
                  WiFi.localIP().toString().c_str(), usertoken.c_str());
} else {
  /*
  xilent.printf_P(PSTR("<button class='btn-mod' style='background-color: #555;' "
                       "onclick=\"window.location.href='http://%s/%s/umoddegis'\">📝 STANDART MOD (AKTİF)</button>"), 
                  WiFi.localIP().toString().c_str(), usertoken.c_str());
  */
}
// anasayfada Çalışma modu sonu
if (uzunMetrajVar) {
  xilent.print(F("  <p style='font-size: 12px; color: gray; margin: 5px 0 0 0;'>Modu değiştirmek için butona tıklayın. Sistem otomatik yenilenecektir.</p>\r\n"
                 "</div>\r\n"));
}








            xilent.print(F("<br>AUT:\r\n"));
            xilent.println(String(aut));
            xilent.print(F("<br>\r\n"));
            if (lip[0] == 0) {
              xilent.print(F(" Not connected.\r\n"));
            } else {
              // IP adresini pointer sorunu yaratmadan %d formatıyla güvenle basıyoruz
              xilent.printf_P(PSTR("<label style='font-size: 15px;'> Ipadr: %d.%d.%d.%d</label>\r\n"),
                              lip[0], lip[1], lip[2], lip[3]);

              if (habp > 0 && mqtterror == true) {
                xilent.printf_P(PSTR("<br><label style='font-size: 15px;'> MQTT bağlantı sorunu! %s</label>\r\n"), MQTTip.c_str());
              }
            }

            // Acil seviyesi göstergesi
            xilent.printf_P(PSTR("Acil seviyesi: %s\r\n"), ACL.c_str());
            xilent.printf_P(PSTR("Token : %s\r\n"), usertoken.c_str());
            if (pinayarerrorlog != "") {
              xilent.printf_P(PSTR("<br>Pin ayar hatası: %s<br>\r\n"), pinayarerrorlog.c_str());
            }

            if (programerrorlog != "") {
              xilent.printf_P(PSTR("<br>Program hatası: %s<br>\r\n"), programerrorlog.c_str());
            }


            if (pinayar.length() > 3) {
              int pindolusay = -1;
              for (int m = 9; m > 0; m--) {
                if (pinname[m] != "") {
                  pindolusay = m + 1;
                  break;
                }
              }
              if (pindolusay > -1)
                for (int x = 0; x < pindolusay; x++) {
                  if (pinlabel[x].length() > 1) {
                    xilent.print(F("<hr style='height:6px;border-width:1;color:black;background-color:black'>\r\n"));
                    if (pinmode[x] == "OUT" && pinsignaltype[x] == "DIG") {
                      String pinstatesakla;
                      if (acildeyim[x] == true) {
                        pinstatesakla = PinState[x];
                        PinState[x] = acildeger[x];
                        xilent.print(F("Bu pin Acil durum etkisinde<br>\r\n"));
                      }
                      xilent.printf_P(PSTR("<label style='font-size:0px;'>Dijital çıkış - [%s]= %s</label> %s\r\n"),
                                      pinname[x].c_str(), PinState[x].c_str(), pinlabel[x].c_str());

                      if (PinState[x] == "") PinState[x] = "0";
                      String pinstatesahte = PinState[x];
                      bool yildizli;
                      if (pinlabel[x].indexOf("*") + 1 == pinlabel[x].length()) yildizli = true;
                      else yildizli = false;
                      if (yildizli == true) {
                        if (PinState[x] == "1") pinstatesahte = "0";
                        else pinstatesahte = "1";
                      }
                      if (pinstatesahte == "0") {
                        if (yildizli == false) {
                          xilent.printf_P(PSTR("<a href='%s/%s:1'><button class='button butoff'>⬛</button></a>\r\n"), usertoken.c_str(), pinname[x].c_str());
                        } else {
                          xilent.printf_P(PSTR("<a href='%s/%s:0'><button class='button buton'>🟩</button></a> ❕\r\n"), usertoken.c_str(), pinname[x].c_str());
                        }
                      }
                      if (pinstatesahte == "1") {
                        if (yildizli == false) {
                          xilent.printf_P(PSTR("<a href='%s/%s:0'><button class='button buton'>🟩</button></a>\r\n"), usertoken.c_str(), pinname[x].c_str());
                        } else {
                          xilent.printf_P(PSTR("<a href='%s/%s:1'><button class='button butoff'>⬛</button></a> ❕\r\n"), usertoken.c_str(), pinname[x].c_str());
                        }
                      }
                      if (acildeyim[x] == true) {
                        PinState[x] = pinstatesakla;
                      }
                    }
                    if (pinmode[x] == "OUT" && pinsignaltype[x] == "BUZ") {
                      xilent.printf_P(PSTR("<label style='color: #0A4A4A;'>Ses frekansı çıkış - %s [%s] 🎹  Buzzer pini<br></label>\r\n"),
                                      pinlabel[x].c_str(), pinname[x].c_str());
                    }



                    if (pinmode[x] == "OUT" && pinsignaltype[x] == "PWM") {
                      String pinstatesakla;
                      if (acildeyim[x] == true) {
                        pinstatesakla = PinState[x];
                        PinState[x] = acildeger[x];
                        xilent.print(F("Bu pin Acil durum etkisinde<br>\r\n"));
                      }

                      // Minimum ve Maksimum Değer etiketlerini Flash bellekten basıyoruz
                      xilent.printf_P(PSTR("<label style='font-size: 12px;'> min: %s-max: %s</label><br>\r\n"),
                                      pinminvalue[x].c_str(), pinmaxvalue[x].c_str());

                      // HTML Slider ve JavaScript Yükleyici Şablonu Tek Seferde Flash'tan Basılıyor
                      xilent.printf_P(PSTR("PWM çıkışı - <div class='slidecontainer'>\r\n"
                                           "<form method='get' id='form%s' action='%s/%s'>%s [%s]"
                                           "<input type='range' min='%s' max='%s' value='%s' class='slider' name='pwm' id='myRange%s'>\r\n"
                                           "<span id='demo%s'></span>\r\n"
                                           "<input type='submit' style='width:50px;' value='>'>\r\n"
                                           "</form></div>\r\n"
                                           "<script>\r\n"
                                           "var slider%s = document.getElementById('myRange%s');\r\n"
                                           "var output%s = document.getElementById('demo%s');\r\n"
                                           "output%s.innerHTML = slider%s.value;\r\n"
                                           "slider%s.oninput = function() { output%s.innerHTML = this.value; };\r\n"
                                           "slider%s.onmouseup = function () { document.getElementById('form%s').submit(); };\r\n"
                                           "</script>\r\n"),
                                      pinname[x].c_str(), usertoken.c_str(), pinname[x].c_str(), pinlabel[x].c_str(), pinname[x].c_str(),
                                      pinminvalue[x].c_str(), pinmaxvalue[x].c_str(), PinState[x].c_str(), pinname[x].c_str(),
                                      pinname[x].c_str(),
                                      pinname[x].c_str(), pinname[x].c_str(), pinname[x].c_str(), pinname[x].c_str(),
                                      pinname[x].c_str(), pinname[x].c_str(), pinname[x].c_str(), pinname[x].c_str(),
                                      pinname[x].c_str(), pinname[x].c_str());

                      Outpwm(pinname[x], PinState[x].toInt());

                      if (acildeyim[x] == true) {
                        PinState[x] = pinstatesakla;
                      }
                    }


                    if (pinmode[x] == "OUT" && pinsignaltype[x] == "SER") {
                      String pinstatesakla;
                      if (acildeyim[x] == true) {
                        pinstatesakla = PinState[x];
                        PinState[x] = acildeger[x];
                        xilent.print(F("Bu pin Acil durum etkisinde<br>\r\n"));
                      }

                      // Servo form yapısını tek bir printf_P kalıbıyla Flash bellekten basıyoruz.
                      // Değişkeni value='%s' şeklinde güvenli tırnak içine aldık.
                      xilent.printf_P(PSTR("<br><br><form method='get' action='%s/%s'>\r\n"
                                           "<label>Servo sinyali - %s [%s] </label>\r\n"
                                           "<input name='ser' maxlength='3' value='%s'>\r\n"
                                           "<input type='submit'>\r\n"
                                           "</form>\r\n"),
                                      usertoken.c_str(), pinname[x].c_str(), pinlabel[x].c_str(), pinname[x].c_str(), PinState[x].c_str());

                        if (servovar[x] && myservo[x] != NULL) 
                        {
                          int aciDeperi = constrain(PinState[x].toInt(), 0, 180); 
                          
                          // 🎯 Pointer nesnesi olduğu için artık '.' yerine '->' kullanıyoruz:
                          myservo[x]->write(aciDeperi); 
                          
                          psco = true; 
                          ePinState[x] = PinState[x]; 
                        }

                      // Seri port logunu da printf_P ile optimize ederek Flash'a taşıdık
                      Serial.printf_P(PSTR("myservo[x].write(PinState[x].toInt()); :%d\n"), PinState[x].toInt());

                      if (acildeyim[x] == true) {
                        PinState[x] = pinstatesakla;
                        PinState[x] = acildeger[x];
                      }
                    }


                    // Bellek Canavarları Engellendi: angg1 ve angg2 ham literal olarak Flash'ta bırakıldı,
                    // RAM'e (String nesnesine) dönüştürülüp hafıza tüketen kopyaları silindi.

                    if (pinmode[x] == "INP" && pinsignaltype[x] == "ANG") {
                      float analogokunandeger = analogRead(getGPIOPin(pinname[x]));

                      if (pinlabel[x].indexOf("DC") > -1) {
                        float dcv;
                        if (analogokunandeger < 200) {
                          dcv = analogokunandeger * 0.015;
                        } else if (analogokunandeger >= 200 && analogokunandeger < 231) {
                          dcv = 2.80 + ((analogokunandeger - 200) / 18.5);
                        } else if (analogokunandeger >= 231 && analogokunandeger < 250) {
                          dcv = 4.52 + ((analogokunandeger - 231) / 14.4);
                        } else if (analogokunandeger >= 250 && analogokunandeger < 260) {
                          dcv = 5.50 + ((analogokunandeger - 250) / 14.4);
                        } else if (analogokunandeger >= 260 && analogokunandeger < 331) {
                          dcv = 6.47 + ((analogokunandeger - 260) / 15.6);
                        } else {
                          analogokunandeger = analogokunandeger - 180;
                          dcv = analogokunandeger * 0.0737;
                        }

                        // Seri port çıktısını printf_P ile tamamen Flash bellek üzerinden yapıyoruz
                        Serial.printf_P(PSTR("değer: %.2f Volt karşılığı: %.2f\n"), analogokunandeger, dcv);

                        // Float değerini RAM dostu olacak şekilde anlık biçimlendirip String dizisine yazıyoruz
                        char buf[10];
                        dtostrf(dcv, 1, 2, buf);
                        PinState[x] = buf;
                      } else {
                        PinState[x] = String((int)analogokunandeger);
                      }

                      // Dinamik çıktı satırını printf_P ile sıfır RAM kullanarak basıyoruz
                      xilent.printf_P(PSTR("<label style='color: #3A3A00;'> Analog ölçüm - %s [%s]= %s</label><br>\r\n"),
                                      pinlabel[x].c_str(), pinname[x].c_str(), PinState[x].c_str());
                    }


                    if (pinmode[x] == "INP" && pinsignaltype[x] == "DIG") {
                      PinState[x] = String(digitalRead(getGPIOPin(pinname[x])));
                      xilent.printf_P(PSTR("<br> Digital giriş - %s [%s]= %s<br>\r\n"),
                                      pinlabel[x].c_str(), pinname[x].c_str(), PinState[x].c_str());
                    }

                    if (pinmode[x] == "INP" && pinsignaltype[x].indexOf("DHT") == 0) {
                      String t = "";
                      String h = "";
                      int tIndex = PinState[x].indexOf('t');
                      int hIndex = PinState[x].indexOf('h');

                      if (tIndex > -1 && hIndex > tIndex) {
                        t = PinState[x].substring(tIndex + 1, hIndex);
                        h = PinState[x].substring(hIndex + 1);
                      }

                      // HTML Çıktısını printf_P ile tamamen Flash bellekten sıfır RAM yüküyle basıyoruz
                      // HTML standartlarına uyması için yüzde işareti %% şeklinde kaçış karakteriyle güncellendi
                      xilent.printf_P(PSTR("<label style='color: #9A3A00;'> Sıcaklık</label> ve <label style='color: #002A9A;'> Nem </label>- %s [%s]= %s C° %s0%% <br>\r\n"),
                                      pinlabel[x].c_str(), pinname[x].c_str(), t.c_str(), h.c_str());
                    }


                    if (pinmode[x] == "INP" && pinsignaltype[x] == "HCE") {
                      xilent.printf_P(PSTR("<p style='margin-bottom:3px'> </p> Mesafe sensörü - %s [%s]= %s cm.<br>\r\n"),
                                      pinlabel[x].c_str(), pinname[x].c_str(), PinState[x].c_str());
                    }
                  }
                }
            }

            // Sabit HTML çizgisi ve düzen etiketleri tamamen Flash belleğe taşındı
            xilent.print(F("<hr style='height:5px;border-width:1;color:black;background-color:black'>\r\n"
                           " ⫘ <div align='left'>\r\n"));


            if (progmsg != "" || errorlog != "") {
              xilent.print(F("<table><td style='border:2px solid black;width:400px; align:center;'>\r\n"));
              if (progmsg != "") {
                xilent.print(F("<label style='font-size: 12px;'>Program müdahalesi ile olanlar aşağıdadır</label><br>\r\n"));
                xilent.printf_P(PSTR("<label style='font-size: 10px;'>%s</label><br>\r\n"), progmsg.c_str());
              }
              if (errorlog != "") {
                xilent.printf_P(PSTR("<br>Hata: %s<br>\r\n"), errorlog.c_str());
              }
              xilent.print(F("</td></table>\r\n"));
            }
            xilent.printf_P(PSTR(" </div><p><span title='Ayarlar sayfasına giriş'><a href='%s/Menu1'><button class='button butayr'>⚙️</button></a></span></p>\r\n"), usertoken.c_str());

            if (menu1girilemez == true) {
              xilent.print(F("<style> @keyframes blinker {50% { opacity: 0; }}</style><span style=\"color: red; animation: blinker 1s linear infinite;\">YETKİSİZ GİRİLEMEZ!... ADMİN GRİŞİ YAPMADAN AYARLAR KISMINA GİREMEZSİNİZ</span><br><br><br>\r\n"));
            }

            if (RXTXerrorlog != "") {
              xilent.printf_P(PSTR("<style> @keyframes blinker {50% { opacity: 0; }}</style><span style=\"color: red; animation: blinker 1s linear infinite;\">RXTX HATASI %s</span><br><br><br>\r\n"), RXTXerrorlog.c_str());
            }






            // Seçim kombo menüsü ve durumları tek bir printf_P şablonuyla Flash bellek üzerinden basılıyor
            xilent.printf_P(PSTR("<br><form action='%s/habp'><label style='font-size: 12px;'>Diğer cihazlarla haberleşilecek protokol:\r\n"
                                 "<select id='habp' name='habp'>\r\n"
                                 "<option value='0' %s>Kapali</option>\r\n"
                                 "<option value='1' %s>MQTT</option>\r\n"
                                 "<option value='2' %s>MQTT&FBSERVER</option>\r\n"
                                 "<option value='3' %s>MQTT&Firebase</option>\r\n"
                                 "</select><input type='submit' value='Submit'> değiştirildiğinde ESP restart yapılacak!!!</label></form>\r\n"),
                            usertoken.c_str(),
                            (habp <= 0 ? "selected" : ""),
                            (habp == 1 ? "selected" : ""),
                            (habp == 2 ? "selected" : ""),
                            (habp == 3 ? "selected" : ""));
            //            xilent.println("<label style='font-size: 12px;'>");
            //            if (fben == 1) xilent.println("<form action='%s/firebaseset?fireb=Off' method='POST'>Firebase Açık<input type='submit' value='FB Kapat'>! Reset çekilir !</form></label>");
            //            if (fben == 0) xilent.println("<form action='%s/firebaseset?fireb=On' method='POST'>Firebase Kapalı<input type='submit' value='FB Aç'>! Reset çekilir !</form></label>");

            /*
for(int v=1;v<10;v++)
{
xilent.println(String(v) + " mqyol:");xilent.println(mqyol[v]+" -- ");
xilent.println("degmq:");xilent.println(degisenmq[v]+" <br>");
}
*/
            // --- 1. SİSTEM VE CİHAZ BİLGİLERİ ---
            xilent.printf_P(PSTR("<br>habp:%d<br><br>\r\n"
                                 "coded by %s. ESP kontrol (v4\r\n"
                                 "<label style='font-size:1px;'>%s</label>04.01.2026)<br><br>\r\n"
                                 "istemci: %s<br>\r\n"
                                 "Acil durum seviyesi: %s<br>\r\n"
                                 "<div align='left'>\r\n"),
                            habp, creator.c_str(), WiFi.macAddress().c_str(), remoteip.c_str(), ACL.c_str());

            // --- 2. RENK AYAR FORMLARI ( value='%s' GÜVENLİĞİ İLE ) ---
            xilent.printf_P(PSTR("<form method='get' action='%s/butonactcol'><label>Acik konumundaki buton rengi. #</label><input name='is' maxlength='6' style='width:50px' value='%s'><input type='submit'>d1ca03</form><br>\r\n"
                                 "<form method='get' action='%s/butonpascol'><label>Kapalı konumunda buton rengi  #</label><input name='is' maxlength='6' style='width:50px' value='%s'><input type='submit'>A3A3A3</form><br>\r\n"
                                 "<form method='get' action='%s/butonayrcol'><label>Ayar, pwm gönder buton rengi.  #</label><input name='is' maxlength='6' style='width:50px' value='%s'><input type='submit'>20d3c8</form><br>\r\n"
                                 "<form method='get' action='%s/menutextcol'><label>Ayar sayfası menü yazı rengi.  #</label><input name='is' maxlength='6' style='width:50px' value='%s'><input type='submit'>000000</form><br>\r\n"
                                 "<form method='get' action='%s/butonpbgcol'><label>Bu sayfanin arka planının rengi #</label><input name='is' maxlength='6' style='width:50px' value='%s'><input type='submit'>ffb12a</form></td><br><br><br>\r\n"),
                            usertoken.c_str(), butonactcol.c_str(), usertoken.c_str(), butonpascol.c_str(), usertoken.c_str(), butonayrcol.c_str(), usertoken.c_str(), menutextcol.c_str(), usertoken.c_str(), butonpbgcol.c_str());

            // --- 3. HATA SIFIRLAMA VE LİNK TABLOSU ---
            xilent.print(F("<div><table><td style='border:2px solid black;width:400px; align:center;'>\r\n"));
            if (progmsg != "") {
              xilent.print(F("<label style='font-size: 12px;'>Hata ile kaşılaşıyorsanız ilgili kayıtları silmek için aşağıdaki urlleri girin</label><br>\r\n"));
            }
            // lipStr yerine %d.%d.%d.%d kalıbı getirilerek RAM sızıntısı sıfırlandı
            xilent.printf_P(PSTR("<label style='font-size: 10px;'>Pin ayarı hatalı oldu pinayarlarını sil diyorsanız &emsp; http://%d.%d.%d.%d/pinayarsil</label><br>\r\n"
                                 "<label style='font-size: 10px;'>Program ayarı hatalı oldu program sil diyorsanız &emsp; http://%d.%d.%d.%d/programsil</label><br>\r\n"
                                 "<label style='font-size: 10px;'>Haberleşme protokolü hatalı diyorsanız &emsp; http://%d.%d.%d.%d/habp=0</label><br>\r\n"
                                 "<label style='font-size: 10px;'>Tüm kayıtları sil diyorsanız &emsp; http://%d.%d.%d.%d/nevarsasil</label><br>\r\n"
                                 "</td></table></div>\r\n"),
                            lip[0], lip[1], lip[2], lip[3],
                            lip[0], lip[1], lip[2], lip[3],
                            lip[0], lip[1], lip[2], lip[3],
                            lip[0], lip[1], lip[2], lip[3]);

            // --- 4. HAFIZA VE LITTLEFS SİSTEM RAPORU ---
            xilent.printf_P(PSTR("<br> Free heap size : %d bytes<br><br>\n"), ESP.getFreeHeap());

            FSInfo info;
            LittleFS.info(info);
            size_t totalBytes = info.totalBytes;
            size_t usedBytes = info.usedBytes;
            size_t freeBytes = totalBytes - usedBytes;

            xilent.printf_P(PSTR("File Kullanilan Alan: %d KB<br>\n"
                                 "File Kalan Bos Alan: %d KB<br>\n"
                                 "File Sys Toplam Alan: %d KB<br>\n"),
                            usedBytes / 1024, freeBytes / 1024, totalBytes / 1024);

            /*                        butonactcol butonpascol butonayrcol butonpbgcol
xilent.println(".button { background-color: #d1ca03; border: none; color: white; padding: 10px 10px;text-decoration: none; font-size: 10px; margin: 1px; cursor: pointer;}");
xilent.println(".butoff {background-color: #A3A3A3; border: none; color: white; padding: 10px 10px;text-decoration: none; font-size: 10px; margin: 1px; cursor: pointer;}");
xilent.println(".butayr {background-color: #20d3c8; border: none; color: black; padding: 10px 10px;text-decoration: none; font-size: 10px; margin: 1px; cursor: pointer;}");
xilent.println("</style>");

xilent.println("<style>body {background-color: #ffb12a;} </style>");  //darkblue, lightblue, #b0b0b0 gibi

*/


            const char* ocz = R"ocz(<script>var uri = window.location.toString();
if (uri.indexOf("?") > 0) {
var clean_uri = uri.substring(0, uri.indexOf("?"));
window.history.replaceState({}, document.title, clean_uri);
}</script>)ocz";

            String ocz2 = String(ocz);

            //xilent.println(ocz2);
            // url variable hidden url , url hide

            // menu0bitti
            xilent.println("<br>");
            xilent.println("<script>");
            xilent.println("function ekraniKilitle() {");
            // Sayfa içeriğini tamamen silip temiz bir beyaz ekran yapar ("Clear" mantığı)
            xilent.println("  document.body.innerHTML = '<h1 style='font-family:Helvetica,Arial; text-align:center; margin-top:20%; color:#333333;'>Lütfen Bekleyin...</h1>';");
            xilent.println("}");

            // TIKLAMA YAKALAYICI: Sayfadaki tüm butonları ve linkleri otomatik dinler
            xilent.println("document.addEventListener('click', function(e) {");
            xilent.println("  var t = e.target;");
            xilent.println("  while (t && t != document.body) {");
            xilent.println("    if (t.tagName == 'A' || t.tagName == 'BUTTON') {");
            xilent.println("      ekraniKilitle();");
            // Butonun içindeki linke gitmesine engel olmaması için window.stop() veya preventDefault kullanmıyoruz
            xilent.println("      break;");
            xilent.println("    }");
            xilent.println("    t = t.parentNode;");
            xilent.println("  }");
            // Sayfanın ESP'ye (yeni linke) yönlenmesini 10 milisaniye geciktirerek
            // ekran temizleme efektinin kullanıcının gözünde anında oluşmasını sağlarız.
            xilent.println("}, true);");
            xilent.println("</script>");

            xilent.println("</body></html>");









            // --- 5. URL GİZLEME VE EKRAN KİLİTLEME JAVASCRIPT KAPANIKLARI (FİNAL) ---
            // Bellek sızıntısı yapan ocz2 String kopyası tamamen silindi.
            // Ham kural (ocz) doğrudan Flash bellek üzerinden tarayıcıya fırlatılıyor.
            xilent.print(ocz);

            // Ekran kilitleme loader JS şablonu ve HTML döküman sonu tek parça halinde Flash'a kilitlendi
            xilent.print(F("<br><script>\r\n"
                           "function ekraniKilitle() {\r\n"
                           "  document.body.innerHTML = '<h1 style='font-family:Helvetica,Arial; text-align:center; margin-top:20%; color:#333333;'>Lütfen Bekleyin...</h1>';\r\n"
                           "}\r\n"
                           "document.addEventListener('click', function(e) {\r\n"
                           "  var t = e.target;\r\n"
                           "  while (t && t != document.body) {\r\n"
                           "    if (t.tagName == 'A' || t.tagName == 'BUTTON') {\r\n"
                           "      ekraniKilitle();\r\n"
                           "      break;\r\n"
                           "    }\r\n"
                           "    t = t.parentNode;\r\n"
                           "  }\r\n"
                           "}, true);\r\n"
                           "</script>\r\n"
                           "</body></html>\r\n\r\n"));


            //xilent.abort();
            //xilent.flush();
            //xilent.stop();
            if (header.indexOf("/resetle") > -1) ESP.reset();
          }
          yield();
        }
      }
    }
  }
  header = "";
}