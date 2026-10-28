// --- ⏱️ TERSİNE HABERLEŞME ZAMANLAYICI DEĞİŞKENLERİ ---
unsigned long inputZamanHafizasi = 0;
const unsigned long INPUT_PERIYODU = 500; // Sensörler kaç milisaniyede bir okunup ESP'ye yollansın? (Örn: 500ms)

// --- 🎯 SENSÖRLERİ OKUYUP ESP'YE FORMALİZE PAKET FIRLATAN MOTOR ---
void updinput() {
  // Asenkron zaman kontrolü: Arduino'yu dondurmadan, periyodik olarak çalışır
  if (millis() - inputZamanHafizasi < INPUT_PERIYODU) {
    return; // Zamanı gelmediyse fonksiyondan çık, ana döngüyü yorma
  }
  inputZamanHafizasi = millis(); // Zamanlayıcıyı güncelle

  // Eğer sistemde tanımlanmış hiçbir analog giriş yoksa boşuna işlem yapma
  if (toplamAnalog == 0) return;

  // Hat düzeni: "CihazAdı:AN:Metinler;" formatında bir tren oluşturacağız
  // Örnek Hedef: "UNO1:AN:POT1=512|SENS2=1023;"
  String inputPaketi = CHZadi + ":AN:";

  // Tüm tanımlı analog pinleri sırayla dönüp okuyoruz
  for (int i = 0; i < toplamAnalog; i++) {
    int okunacakPin = analogPin[i];
    int analogDeger = analogRead(okunacakPin); // Fiziksel pini oku (0 - 1023 arası)

    // Sensör adını ve değerini pakete ekle (Örn: "POT1=512")
    inputPaketi += analogIsimleri[i] + "=" + String(analogDeger);

    // Eğer son eleman değilse araya boru '|' işareti koyarak birleştir
    if (i < toplamAnalog - 1) {
      inputPaketi += "|";
    }
  }

  // Paketin sonuna endüstriyel bitiş karakterimiz olan noktalı virgülü ekliyoruz
  inputPaketi += ";";

  // 🚀 OLUŞAN PAKETİ DONANIMSAL SERİ PORTTAN ESP (LOLIN) HATTINA FIRLAT!
  Serial.println(inputPaketi); 
}