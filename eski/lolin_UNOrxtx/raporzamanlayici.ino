
void raporzamanlayicivoid() {
  if (raporsayaci < millis()) {
    raporsayaci = raporzamanlayici + millis();
    // raporlama yap



    for (int i = 0; i < toplamAnalog; i++) {
      if (analogRapor[i] == true) {

        if (analogPinDeg[i] != analogPinEdeg[i]) {
          analogPinEdeg[i] = analogPinDeg[i];
          txdata = "<ESP!>" + CHZadi + ":AN=" + analogIsimleri[i] + "?" + analogPinDeg[i];
          slaveTxvoid();
          txdata="";
          break;
        }
      }
    }

    for (int i = 0; i < toplamDi; i++) {
      if (diRapor[i] == true) {

        if (diPinDeg[i] != diPinEdeg[i]) {
          diPinEdeg[i] = diPinDeg[i];
          txdata = "<ESP?>" + CHZadi + ":AN=" + diIsimleri[i] + "?" + diPinDeg[i];
          slaveTxvoid();
          txdata="";
          break;
        }
      }
    }
  }
}