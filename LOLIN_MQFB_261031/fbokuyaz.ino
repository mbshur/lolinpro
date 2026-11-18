int fbsayac = 0;
String tamUrl;

// --- API_KEY Kontrolü Yapan Yardımcı Fonksiyon ---
String urlAuthEkle(String hamUrl) {
  // 1. ADIM: Sizin hazır fonksiyonunuzla tüm %3A, %2F gibi tarayıcı kodlamalarını çözüyoruz
  hamUrl = Karakterduzeltfunc(hamUrl); 
  hamUrl.trim();
  
  // 2. ADIM: https:// kısmını bozmadan, aradaki hatalı çift eğik çizgileri (//) temizliyoruz
  if (hamUrl.indexOf(F("://")) > -1) {
    String protokol = hamUrl.substring(0, hamUrl.indexOf(F("://")) + 3);
    String adres = hamUrl.substring(hamUrl.indexOf(F("://")) + 3);
    while (adres.indexOf(F("//")) > -1) {
      adres.replace(F("//"), F("/"));
    }
    hamUrl = protokol + adres;
  }

  // 3. ADIM: API_KEY varsa, onu da temizleyip URL sonuna güvenli bir şekilde ekliyoruz
  if (API_KEY.length() > 5) {
    API_KEY = Karakterduzeltfunc(API_KEY); // API Key içinde de olabilecek karakterleri düzeltir
    API_KEY.trim();
    
    if (hamUrl.indexOf('?') > -1) {
      hamUrl = hamUrl + "&auth=" + API_KEY;
    } else {
      hamUrl = hamUrl + "?auth=" + API_KEY;
    }
  }
  
  hamUrl.trim();
  return hamUrl;
}

void fbsayacoku()
{
  String anaUrl = DATABASE_URL;
  String hedefYol = "/" + YOL + "/r/" + esphostname;
  tamUrl = urlAuthEkle(anaUrl + hedefYol + ".json"); 
  
  String resul = firebaseGetData(tamUrl.c_str());
  Serial.print("fbsayacoku result: "); Serial.println(resul);
  
  if(resul == "null" || resul == "") {
    fbdataguncelle();
  }
  else
  {
    int resulInt = resul.toInt();
    if(resulInt == 9) { return; }

    if(resulInt >= 5 && resulInt < 9) {
      fbpinstatelerioku();
      fbsayacguncelle();
    }
    
    if(psco == true || psci == true) {
      fbpinstateleriyaz();
    }
  }
  fbtimer = millis();                          
  fbisleniyor = false;
}

void fbsayacguncelle()
{  
  String anaUrl = DATABASE_URL;
  String hedefYol = "/" + YOL + "/r/" + esphostname;
  String tamUrl = urlAuthEkle(anaUrl + hedefYol + ".json"); 
  
  fbsayac += 1;
  if(fbsayac > 4) fbsayac = 0;

  firebasePutData(tamUrl.c_str(), String(fbsayac));
  fbtimer = millis(); 
  fbisleniyor = false;
}

void fbdataguncelle()
{
  Serial.println("fbdataguncelle ------------");
  fbsayacguncelle();
  fbpinayarlariyaz();
  fbpinstateleriyaz();
  fbisleniyor = false;
}

void fbpinstatelerioku()
{
  if(YOL == "") dosyaokufbyol();

  String anaUrl = DATABASE_URL;
  String hedefYol = "/" + YOL + "/pins/" + esphostname + "pin";
  String tamUrl = urlAuthEkle(anaUrl + hedefYol + ".json"); 

  String resul = firebaseGetData(tamUrl.c_str());

  if(resul == "null" || resul == "")
  {
    fbdataguncelle();
  }
  else
  {
    String reslt = resul;
    if(reslt.indexOf("ACL:") > -1 && reslt.indexOf("ACL:") < 2) {
      reslt = reslt.substring(reslt.indexOf(",") + 1);
    }
    
    // Sonsuz döngü ve çökme riskini önleyen güvenli parser (while)
    while (reslt.indexOf(":") > -1 && reslt.indexOf(",") > -1)
    {
      int idxIkiNokta = reslt.indexOf(":");
      int idxVirgul = reslt.indexOf(",");
      
      String pnm = reslt.substring(0, idxIkiNokta);
      String pns = reslt.substring(idxIkiNokta + 1, idxVirgul);
      reslt = reslt.substring(idxVirgul + 1);
      
      Serial.println(pnm + " " + pns);
      
      for(int hh = 0; hh < pinsayisi; hh++)
      {
        if(pnm == pinname[hh])
        { 
          if(pinmode[hh] != "INP")
          {
            Serial.println(pns + "*" + fbPinState[hh]);
            Serial.print("psco: "); Serial.println(psco ? "true" : "false");
            
            // 🎯 FLICKER ÖNLEME KİLİDİ: Lokal değişim varsa (psco veya psci true ise) 
            // Firebase'deki eski veri lokal durumu ezemez!
            if (psco == false && psci == false && pns != fbPinState[hh])
            {
              PinState[hh] = pns;
              fbPinState[hh] = pns;
            }
          }
          break;
        }
      }
    }
    

    Serial.print("psco: "); Serial.print(psco); Serial.print("  psci: "); Serial.println(psci);
    
    // 🎯 YENİ SIRALAMA BURASI: Önce Firebase'e güncel veriyi gönderiyoruz
    if(psco == true || psci == true) { 
      fbpinstateleriyaz(); 
    }

    // Donanımı ve röleleri en son tetikliyoruz (Flicker'ı önleyen kritik adım)
    programrun(); 
    pinuygula(); 
  }
  fbtimer = millis(); 
  fbisleniyor = false;
} // Fonksiyonun bittiği süslü parantez

void fbpinstateleriyaz()
{
  String anaUrl = DATABASE_URL;
  String hedefYol = "/" + YOL + "/pins/" + esphostname + "pin";
  String tamUrl = urlAuthEkle(anaUrl + hedefYol + ".json"); 
  
  String dats = "";
  if(ACLilanciyim == true) dats = "ACL:" + String(ACL) + ",";

  // Hafıza taşma hatası düzeltildi (h < pinsayisi)
  for(int h = 0; h < pinsayisi; h++)
  {
    if(pinname[h].length() > 0)
    {
      Serial.print(String(h) + " -----> " + pinname[h] + " = "); Serial.println(PinState[h]);
      if(pinsignaltype[h].indexOf("DHT") > -1 && PinState[h].length() < 2) return;
      if(pinsignaltype[h].indexOf("HCE") > -1 && PinState[h].length() < 2) return;
      
      if(PinState[h] == "") PinState[h] = "0";
      
      int b = pinname[h].length() + 1;
      char cvc[b];
      pinname[h].toCharArray(cvc, b);
      
      if(pinname[h].length() > 0 && (int)cvc[0] != 10) {
        dats += pinname[h] + ":" + PinState[h] + ",";
      }
    }
  }
  
  firebasePutData(tamUrl.c_str(), dats);

  for(int hh = 0; hh < pinsayisi; hh++)
  {
    fbPinState[hh] = PinState[hh];
  }
  psco = false;
  psci = false;

  fbtimer = millis(); 
  fbisleniyor = false;
}

void fbpinayarlarioku()
{
  String anaUrl = DATABASE_URL;
  String hedefYol = "/" + YOL + "/pays/" + esphostname + "pay";
  String tamUrl = urlAuthEkle(anaUrl + hedefYol + ".json"); 

  String resul = firebaseGetData(tamUrl.c_str());

  // String eşleştirme standardı düzeltildi
  String dats = "[" + pinayar + "]";
  dats.replace("\n", "][");
  dats.replace("[]", "");

  if(resul == "null" || resul == "")
  {
    fbpinayarlariyaz();
  }
  else
  {
    Serial.print("Resul: "); Serial.println(resul);
    Serial.print("Dats : "); Serial.println(dats);
    if(resul != dats) {
      fbpinayarlariyaz();
    } else {
      Serial.println("Ayarlar aynı, işlem yapılmadı.");
    }
  }

  fbtimer = millis(); 
  fbisleniyor = false;
}

void fbpinayarlariyaz()
{
  String anaUrl = DATABASE_URL;
  String hedefYol = "/" + YOL + "/pays/" + esphostname + "pay";
  String tamUrl = urlAuthEkle(anaUrl + hedefYol + ".json"); 
  
  String dats = "[" + pinayar + "]";
  dats.replace("\n", "][");
  dats.replace("[]", "");
  
  firebasePutData(tamUrl.c_str(), dats);
  fbpinstateleriyaz();
  
  fbtimer = millis(); 
  fbisleniyor = false;
}

void fbdeletesayac()
{
  // İhtiyaç halinde kullanılabilir
}














































/*
//          if(sayPtakipicin>100){
//            if(programdata.length()>1)Programtakip(programdata);
//            sayPtakipicin=0;
//          }
//          sayPtakipicin+=1;


   // save
  DynamicJsonDocument saveDoc(1024);
  saveDoc["name"] = "Device 1";
  saveDoc["temperature"] = 30.00;
  saveDoc["location"][0] = 48.756080;
  saveDoc["location"][1] = 2.302038;
  String saveJSONData;
  serializeJson(saveDoc, saveJSONData);
  int saveResponseCode = firebaseRealtime.save("devices", "1", saveJSONData);
  Serial.println("\nSave - response code: " + String(saveResponseCode));
  saveDoc.clear();

  // update
  DynamicJsonDocument updateDoc(1024);
  updateDoc["temperature"] = 35.00;
  String updateJSONData;
  serializeJson(updateDoc, updateJSONData);
  int updateResponseCode = firebaseRealtime.save("devices", "1", updateJSONData, true);
  Serial.println("\nUpdate - response code: " + String(updateResponseCode));
  updateDoc.clear();

  // fetch
  DynamicJsonDocument fetchDoc(1024);
  int fetchResponseCode = firebaseRealtime.fetch("devices", "1", fetchDoc);
  float temp = fetchDoc["temperature"];
  String name = fetchDoc["name"];
  float lat = fetchDoc["location"][0];
  float lon = fetchDoc["location"][1];
  Serial.println("\nFetch - response code: " + String(fetchResponseCode));
  Serial.println("Name: " + name + ", Temp: " + String(temp) + ", Lat: " + String(lat) + ", Lon: " + String(lon));
  fetchDoc.clear();


    // delete
  // int deleteResponseCode = remove("devices", "1");
  // Serial.println("\nDelete response code: " + String(deleteResponseCode));

*/