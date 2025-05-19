# Määrittelydokumentti

Aion toteuttaa kurssiprojektina tekoälyalgoritmin pelaamaan miinaharavaa. Suoritan kurssin osana TKT-kandiohjelmaa.

## Algoritmi

Algoritmin on kehittänyt Chris Studholme. 
Algoritmi lähestyy miinaharavaa rajoitelaskennan ongelmana (CSP), missä jokaisen avatun ruudun arvo esitetään avaamattomien naapurien summana.
Näistä joukoista etsitään osajoukkoja ja niitä käyttäen yhtälöitä voidaan sieventää.
Triviaaleiksi sieventyneet ruudut voidaan merkitä miinaksi tai avata.
Jos päädytään tilanteeseen, jossa sieventäminen ei riitä, yhtälöt ryhmitellään yhteisten ruutujen mukaan.
Tämän jälkeen ryhmille lasketaan kaikki mahdolliset ratkaisut käyttäen peruuttavaa syvyyshakua.
Kun kaikki ratkaisut on laskettu, etsitään näistä kaikki ruudut, joiden arvo on sama kaikissa ratkaisuissa.
Jos nämä eivät riitä jatkamaan ratkaisua, joudutaan arvaamaan seuraava avattava ruutu.
Lähteessä algoritmi käyttää todennäköisyyksiä arvauksien tekemiseen, mutta aion jättää arvaamisen satunnaiseksi valinnaksi.

Algoritmi saa syötteenä alkuun pelilaudan mitat ja miinojen määrän laudalla. Algoritmi antaa pelimoottorille seuraavan avattavan ruudun.
Tämän jälkeen pelimoottori palauttaa avatun ruudun arvon tai keskeyttää pelin, jos avattu ruutu sisälsi miinan.

Kokonaisuudessaan algoritmin aikavaativuus on lähteen mukaan eksponentiaalinen pelilaudan kokoon nähden, kun miinojen tiheys pidetään samana (0.2).

## Ohjemointikielet

Toteutuskieli: C++
Vertaisarvointiin soveltuvat kielet: C++, Python, Java

## Dokumentaatio

Projektin dokumentaatio kirjoitetaan suomeksi, koodi ja sen kommentit englanniksi.

## Lähteet

- [Becerra - Algorithmic approaches to playing minesweeper](https://dash.harvard.edu/server/api/core/bitstreams/7312037d-80a6-6bd4-e053-0100007fdf3b/content)
- [Studholme - Minesweeper as a constraint satisfaction problem](https://www.cs.toronto.edu/~cvs/minesweeper/minesweeper.pdf)
