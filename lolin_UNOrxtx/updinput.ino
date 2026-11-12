
// --- 🎯 SENSÖRLERİ OKUYUP ESP'YE FORMALİZE PAKET FIRLATAN MOTOR ---
void updinput() {
  // Asenkron zaman kontrolü: Arduino'yu dondurmadan, periyodik olarak çalışır

  // 🚨 2. SENARYO (Gelişmiş): Canlı Dijital Giriş (DI) ve Akıllı Arıza Bildirimi
  for (int i = 0; i < toplamDi; i++) {
    diPinDeg[i] = digitalRead(diPin[i]);
    delay(1);
  }

  for (int i = 0; i < toplamAnalog; i++) {
    analogPinDeg[i] = analogRead(analogPin[i]);
    delay(1);
  }

}