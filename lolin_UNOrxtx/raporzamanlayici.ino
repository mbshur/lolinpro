
void raporzamanlayicivoid()
{
  if(raporsayaci < millis())
  {
    raporsayaci = raporzamanlayici + millis();
    // raporlama yap



    for (int i = 0;i < toplamAnalog;i++){
    if(raporlanacakcihazismi==analogIsimleri[i])
    {
      int an = analogRead(analogPin[i]);
      raporstr ="<ESP!>"+ CHZadi + ":AN=" + raporlanacakcihazismi + "?" + an;
    break;
    }
    }

    for (int i = 0;i < toplamDi;i++){
    if(raporlanacakcihazismi==diIsimleri[i])
    {
      int di= digitalRead(diPin[i]);
      raporstr ="<ESP?>"+ CHZadi + ":AN=" + raporlanacakcihazismi + "?" + di;
      break;
    }
    }



    if(raporstr!=""){
      txdata = raporstr;
      raporstr="";
    }

  }
}