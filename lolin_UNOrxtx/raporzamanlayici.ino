
void raporzamanlayicivoid() {
  if (raporsayaci < millis()) {
    raporsayaci = raporzamanlayici + millis();
    // raporlama yap

              //if(waiting==true)Serial.println("waiting");
              //else Serial.println("Going");

    for (int i = 0; i < toplamAnalog; i++) {
      if (analogRapor[i] == true) {

        if (analogPinDeg[i] != analogPinEdeg[i]) {
          analogPinEdeg[i] = analogPinDeg[i];
          txdata = "E!>" + CHZadi + "?" + analogIsimleri[i] + "=" + analogPinDeg[i];
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
          txdata = "E!>" + CHZadi + "?" + diIsimleri[i] + "=" + diPinDeg[i];
          slaveTxvoid();
          txdata="";
          break;
        }
      }
    }
  }
}