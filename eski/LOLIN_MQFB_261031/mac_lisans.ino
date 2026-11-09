// 2 ile 5 arasında rastgele bir sayı üretir (Örn: 3)
int rastgeleSayiUret() {
  return random(1, 5); 
}

String talepKoduUret(String temizMac, int rnd) {
  String uretilen = String(rnd); // En başa yine Rnd sayıyı koyuyoruz (Örn: 4)
  
  // Sizin güncellediğiniz 10 elemanlı harika parazit havuzu (O harfi dahil)
  const char parazitHavuzu[] = "GOHJKLVNMY";
  const int parazitBoyutu = 10;

  // Gerçek MAC karakterleri için 16'lı temiz havuzumuz
  const char hexHavuz[] = "0123456789ABCDEF";
  const int hexHavuzBoyutu = 16;

  // Rnd sayısının tek mi çift mi olduğunu anlıyoruz
  bool rndTekSayi = (rnd % 2 != 0);

  for (int i = 0; i < temizMac.length(); i++) {
    char mevcutChar = temizMac[i];
    int havuzIdx = -1;

    // Karakterin 16'lı havuzdaki yerini bul
    for (int j = 0; j < hexHavuzBoyutu; j++) {
      if (hexHavuz[j] == mevcutChar) {
        havuzIdx = j;
        break;
      }
    }

    // Eğer karakter havuzda bulunduysa dinamik kaydırma yap
    if (havuzIdx != -1) {
      int yeniIdx = havuzIdx;

      // Mantık: Her karakterde bir artı, bir eksi işlem yapılacak.
      // Başlangıç yönünü (artı mı eksi mi olacağını) Rnd'nin tek/çift durumu belirler.
      if (rndTekSayi) {
        // Rnd Tek ise: i=0 (artı), i=1 (eksi), i=2 (artı)... şeklinde gider
        if (i % 2 == 0) {
          yeniIdx = (havuzIdx + rnd) % hexHavuzBoyutu; // Artı yönü
        } else {
          yeniIdx = (havuzIdx - rnd + hexHavuzBoyutu) % hexHavuzBoyutu; // Eksi yönü
        }
      } 
      else {
        // Rnd Çift ise: i=0 (eksi), i=1 (artı), i=2 (eksi)... şeklinde başlar
        if (i % 2 == 0) {
          yeniIdx = (havuzIdx - rnd + hexHavuzBoyutu) % hexHavuzBoyutu; // Eksi yönü
        } else {
          yeniIdx = (havuzIdx + rnd) % hexHavuzBoyutu; // Artı yönü
        }
      }
      
      // Şifrelenmiş yeni gerçek karakteri ekle
      uretilen += hexHavuz[yeniIdx];
    } 
    else {
      // Havuz dışı bir durum olursa koru
      uretilen += mevcutChar;
    }
    
    // Araya tamamen rastgele bir göz boyama parazit harfi seçip yerleştiriyoruz
    int rastgeleSecim = random(0, parazitBoyutu);
    uretilen += parazitHavuzu[rastgeleSecim];
  }
  return uretilen;
}

// Gerçek MAC adresini Rnd kadar kaydırıp 9-Rnd kuralına göre DOĞRU cevabı hesaplar
String dogruCevapHesapla(String temizMac, int rnd) {
  int karsilikRnd = 9 - rnd; // Örn: 9 - 4 = 5
  String dogruCevap = String(karsilikRnd); // En başa '5' yerleşir
  
  // Sadece temiz 16'lı (Hexadecimal) havuz kullanıyoruz
  const char havuz[] = "0123456789ABCDEF";
  const int havuzBoyutu = 16;

  // Gerçek MAC karakterlerini havuz üzerinde Rnd kadar ileri kaydırıyoruz
  for (int i = 0; i < temizMac.length(); i++) {
    char mevcutChar = temizMac[i];
    int havuzIdx = -1;

    // Mevcut karakterin havuzdaki yerini buluyoruz
    for (int j = 0; j < havuzBoyutu; j++) {
      if (havuz[j] == mevcutChar) {
        havuzIdx = j;
        break;
      }
    }

    // Karakter havuzda bulunduysa kaydırma işlemini yap
    if (havuzIdx != -1) {
      // % 16 sayesinde F'den sonra otomatik olarak en başa (0'a) döner
      int yeniIdx = (havuzIdx + rnd) % havuzBoyutu;
      dogruCevap += havuz[yeniIdx];
    } else {
      dogruCevap += mevcutChar;
    }
  }
  return dogruCevap;
}



void dosyaokucihazmac() {
  dosya.close();
  
  // 1. Gerçek donanım MAC adresini alıp iki noktaları temizliyoruz
  String gercekMac = WiFi.macAddress();
  gercekMac.replace(":", ""); 

  // 2. Dosyadan kayıtlı aktivasyon kodunu okuyoruz
  dosya = LittleFS.open("/cihazmac.txt", "r");
  if (dosya) {
    String gecicicihazmac = dosya.readStringUntil('\n');
    gecicicihazmac.trim();
    cihazmac = gecicicihazmac;
    dosya.close();
  }

  // 3. Dosyadan okunan kod, bu cihazın üretebileceği olası doğru cevaplardan biriyle eşleşiyor mu?
  if (cihazmac == dogruCevapHesapla(gercekMac, 2) || cihazmac == dogruCevapHesapla(gercekMac, 3) || 
      cihazmac == dogruCevapHesapla(gercekMac, 4) || cihazmac == dogruCevapHesapla(gercekMac, 5)) {
    
    cihazorjinal = true;
    Serial.println(F("Lisans Doğrulandı. Cihaz Orijinal."));
  } 
  else {
    // Klonlanmış veya henüz aktif edilmemiş cihaz!
    cihazorjinal = false;
    
    int anlikRnd = rastgeleSayiUret();
    String ekranaBasilacakTalepKodu = talepKoduUret(gercekMac, anlikRnd);
    
    // HTML sayfasında kullanıcıya göstermek için hata loguna talep kodunu gömüyoruz
    pinayarerrorlog = "Cihaz Aktif Değil! Lütfen Aktivasyon Kodunu Girin.";
    programerrorlog = "Talep Kodu: " + ekranaBasilacakTalepKodu;

    // Konsola da RAM dostu olarak basalım
    Serial.printf_P(PSTR("\n!!! AKTIVASYON GEREKLI !!!\n"));
    Serial.printf_P(PSTR("Arayüzde Gösterilecek Kod: %s\n\n"), ekranaBasilacakTalepKodu.c_str());
  }
}


void webAktivasyonKoduKaydet(String gelenKod) {
  gelenKod.trim();
  gelenKod.toUpperCase(); // <-- Kullanıcı küçük harf yazsa bile otomatik olarak BÜYÜK harfe çevrilir.
  
  String gercekMac = WiFi.macAddress();
  gercekMac.replace(":", "");

  if (gelenKod.length() > 0) {
    // İlk karakterden (Örn: 5) karşı tarafın Rnd değerini (4) geri hesaplıyoruz
    int karsilikRnd = gelenKod[0] - '0'; // İlk karakterin sayısal değeri
    int orijinalRnd = 9 - karsilikRnd;   // 9 - 5 = 4 (Orijinal Rnd bulundu)
    
    // Güvenli havuz mantığına sahip fonksiyonumuzla olması gereken gerçek kodu hesapla
    String beklenenKod = dogruCevapHesapla(gercekMac, orijinalRnd);
    
    if (gelenKod == beklenenKod) {
      // BAŞARILI! Kodu LittleFS dosya sistemine yazıyoruz

      dosya.close();
      LittleFS.remove("/cihazmac.txt");
      dosya = LittleFS.open("/cihazmac.txt", "w+");
      if (dosya) {
        dosya.print(gelenKod);
        dosya.flush();
        dosya.close();
        cihazorjinal = true;
      }
      
      pinayarerrorlog = "Aktivasyon Başarılı! Cihaz orijinal moda geçti.";
      programerrorlog = "";
      
      // CRITICAL: Cihazı fişten çekip takmaya gerek kalmasın diye hafızadaki kodu 
      // anında okutup 'cihazorjinal = true' durumuna çekiyoruz ve kilit anında açılıyor!
      dosyaokucihazmac(); 
      
      setup2(); // Sistem ayarlarını yenile

    }
    else {
      // Kod hatalıysa kullanıcıya hata logunu göster
      pinayarerrorlog = "Hatalı Aktivasyon Kodu Girdiniz! Lütfen tekrar deneyin.";
    }
  }
}