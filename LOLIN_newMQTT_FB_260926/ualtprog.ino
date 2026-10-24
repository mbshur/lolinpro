// 🎯 DERLEYİCİYE BİLDİRİM: Bu pointer projenin ana dosyasında zaten var, onu kullan diyoruz
extern String* aktifTmrDeger; // int* yerine String* yapıldı

// --- 4. ALT MODÜL DOSYASINI SATIR SATIR İŞLEYEN YENİ MOTOR ---
void altDosyaYurut() {
  // 🎯 GÜNCEL POINTER KİLİDİ: String içeriğini tam sayıya (.toInt()) çevirip kontrol ediyoruz
  if (aktifTmrDeger != nullptr && (*aktifTmrDeger).toInt() > 0) { 
    yield();
    return; // Zamanlayıcı metni içindeki sayı 0 olana kadar bekler!
  }

    // 🎯 TEST LOGU: Kodun buraya kadar ulaşıp ulaşmadığını kesinleştirelim
  Serial.print(F("[uprog-alt] altDosyaYurut() MOTORU TETİKLENDİ. Dosya acik mi? "));
  Serial.println(ualt_dosya ? "EVET" : "HAYIR");

  // 1. Güvenlik Kontrolü: Alt dosya açık değilse bayrağı kapat ve çık
  if (!ualt_dosya) {
    altDosyaCalisiyor = false;
    Serial.println(F("[uprog-alt] HATA: Alt dosya hafizada bulunamadi!"));
    return;
  }

// 🎯 ALT DOSYA POINTER ZAMANLAYICI KİLİDİ
// Önce pointer'ın boş (nullptr) olmadığını, sonra işaret ettiği adresin değerinin 0'dan büyük olduğunu kontrol eder
if (aktifTmrDeger != nullptr && (*aktifTmrDeger).toInt() > 0) { 
  yield();
  return; // Zamanlayıcı sıfırlanana kadar fonksiyonu burada kes, YENİ SATIRA GEÇME!
}

  // 2. Alt dosyada okunacak satır var mı?
  if (ualt_dosya.available()) {
    String altSatir = ualt_dosya.readStringUntil('\n');
    altSatir.trim();

    // Boş satırları ve yorum satırlarını doğrudan pas geç
    if (altSatir == "" || altSatir.indexOf("//") == 0) {
      yield();
      return; 
    }

    Serial.print(F("[uprog-alt] Modul Satiri Okundu: '"));
    Serial.print(altSatir);
    Serial.println(F("'"));

    // 🎯 SİZİN HARİKA ENTEGRASYONUNUZ:
    // Okunan alt senaryo satırını doğrudan sizin mevcut yorumlama motorunuza paslıyoruz
    satirp = altSatir; 
    
    // Not: loopSatirNo'yu bozmamak için alt dosya takibini kendi içinde yapabilirsiniz, 
    // veya mevcut satirsayisip değişkenine atayabilirsiniz:
    satirsayisip = 999; 

    // Motoru tetikliyoruz: Sizin mevcut satirislem fonksiyonunuz bu satırı da tıpkı ana program gibi yorumlar
    ifsonuc = false;
    Serial.println(altSatir); // Uno donanım hattına (HW Serial) doğrudan gönderir

    satirislem();

    // 🎯 KUSURSUZ PARANTEZ ATLAMA MOTORU (Alt modül içindeki IF kontrolleri için de birebir çalışır)
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
    // --- 3. Alt dosyadaki tüm satırlar bitti: Kapat ve Ana Orkestraya Dön!
    ualt_dosya.close();
    altDosyaCalisiyor = false;
    
    // 🎯 SİHİRLİ DOKUNUŞ: Hafızada asılı kalan zamanlayıcı pointer'ını tamamen boşa çıkar!
    // Böylece ana programdaki D1=0; gibi komutlar çakışma yaşamadan pini sıfırlayabilir.
    aktifTmrDeger = nullptr; 
    
    Serial.print(F("[uprog-alt] Modul bitti: "));
    Serial.print(aktifAltDosyaAdi);
    Serial.println(F(". Ana programa kesintisiz geri donuluyor..."));
    
    aktifAltDosyaAdi = "";
  }
  
  yield(); // ESP8266 Wi-Fi arka plan işleri için nefes alsın
}