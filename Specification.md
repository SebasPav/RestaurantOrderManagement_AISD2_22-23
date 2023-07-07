# Dane
## Menu
Menu będzie wykorzystywała trzewo binarne. 
W menu dane są: ID, nazwa, deskrypcja, cena, categoria, subkategoria, ingredienty, waga, calorie

## Zamówienia 
Zamówienia wykorzystuje drzewa AVL.
W zamówieniach dane są: ID, status zamówienia, numer stolu, ID produktu (Menu).

### Platnosci
Platności sa powiązane z zamówieniami, jakoż oni nie mogą istnieć bez zamówienia.
W płatnościach dane są: ID, ID zamówienia, data, suma, status płatności, metoda płatności.

## Stoly
Odłegoci miedzy stołami będzie zachowana w grafach. Odległości pomiędzy wierzchołkami jest obliczana algorytmem Dijksty.


# Functions
## Zarządzanie sceną
Zarzadzanie funkcjami będzie prowadzone poprzez zarzadzanie scena w kazdym z menu. Jest możliwe generować dane poprzez komputer.

## Zarządzanie menu
Dodawaj, edytuj i usuwaj pozycje menu, definiuj kategorie i podkategorie pozycji menu, określaj ceny, waga, kalorie.

## Zarządzanie zamówieniami
Twórz, edytuj i usuwaj zamówienia, śledź status zamówienia: oczekujące, w toku, dostarczone lub anulowane, przypisuj zamówienia do stolików.

## Zarządzanie płatnościami
Przetwarzaj i śledź płatności za zamówienia, obsługuj różne metody płatności: gotówka, karta kredytowa, cyfrowe portfele.
