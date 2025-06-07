# Viikkoraportti 4

4. viikolla käytetyt tunnit: 16

Kirjoitin koodin, joka käy läpi miinaharavakentästä muodostettuja rajoitteita.
Triviaalien rajoitteiden ratkaisu ja ali- ja ylijoukkoja käyttämällä rajoitteiden sieventäminen on melkein valmis.

Opin, että kun käyttää osoittimia dynaamisissa tietorakenteissa oleviin alkioihin, pitää ottaa huomioon, säilyykö alkio samassa muistiosoitteessa koko elinkaarensa ajan.
Tallennan rajoitteet c++ vektoriin, joten kun vektorin koko kasvoi, kaikki osoittimet osoitteisiin sen sisälle hajosivat, jos muistinhallinta siirsi vektorin eri paikkaan.
Ratkaisin ongelman std::unique_ptr osoittimilla, jotka tallentavat varsinaisen alkion kekomuistiin ja korvaavat sen paikan vektorissa.

Eniten haasteita on tuottanut algoritmin sisäisen kulun seuraaminen ja debuggaaminen.

Seuraavaksi aion tehdä peruuttavan haun kaikille ratkaisuille.
