Procesy:
B procesów typu Babcia
S procesów typu Studentka

Zasoby:
P słoików - maks liczba słoików
K konfitur - maks liczba konfitur

Babcie ubiegają się o słoik, tworzą konfiturę i zwiększają liczbę dostępnych konfitur.
Studentki ubiegają się o konfiturę, zjadają ją, po czym oddają pusty słoik.

Algorytm:

- Proces Bi generuje lokalny znacznik czasu i wysyła wiadomość REQ_JAR ze swoim znacznikiem czasowym do wszystkich innych procesów B.
- Kolejne procesy B dopisuje proces Bi do lokalnej kolejki na podstawie znaczników czasowych (w przypadku remisu - na podstawie id). Następnie odsyłają OK_JAR wraz ze swoim znacznikiem czasowym.
- Proces Bi wie, że może wejść do sekcji krytycznej i dostać słoik, jeśli:
  1. dostanie OK_JAR od wszystkich innych procesów B ze znacznikami czasowymi większymi od swojego
  2. znajduje się wśród P najstarszych procesów w lokalnej kolejce słoików
  3. lokalna ilość dostępnych konfitur jest mniejsza niż K
  4. lokalna ilość dostępnych słoików jest większa niż 0
- Proces Bi po wejściu do sekcji krytycznej symuluje tworzenie konfitury, po którym zwiększa ilość dostępnych konfitur i zmniejsza ilość pustych słoików wysyłając wiadomość NEW_JAM do wszystkich S i B.
- Proces Bi opuszcza sekcje krytyczną i usuwa się z lokalnych kolejek.
- Proces Si, wysyła wiadomość REQ_JAM ze swoim znacznikiem czasowym do wszystkich innych procesów S.
- Pozostałe procesy S dopisują Si do lokalnej kolejki i odsyłają OK_JAM ze swoim znacznikiem.
- Proces Si może wejść do sekcji krytycznej i otrzymać konfiturę, jeśli:
  1. otrzyma OK_JAM od wszystkich innych S
  2. znajduje się na pozycji w kolejce mniejszej niż ilość dostępnych konfitur.
  3. lokalna ilość konfitur jest większa niż 0.
- Po wejściu do sekcji krytycznej, Si symuluje jedzenie konfitury, następnie zmniejsza lokalną liczbę konfitur o 1 i wysyła wiadomość
  REL_JAR do procesów B informując o zwolnieniu słoika.
- Po zjedzeniu konfitury Si opuszcza sekcje krytyczną i usuwa się z lokalnych kolejek.

- Po otrzymaniu NEW_JAM, każdy proces zwiększa lokalną liczbę dostępnych konfitur i zmniejsza ilość pustych słoików i usuwa z kolejki proces,
  który wysłał wiadomość.
- Po otrzymaniu REL_JAR, B zwiększa lokalną liczbę pustych słoików i sprawdza, czy spełnia warunki wejścia do sekcji krytycznej.