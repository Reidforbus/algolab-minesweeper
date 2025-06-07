# Käyttöohje

## Asennus

Sinulla tulee olla asennettuna cmake ja jokin C++- kääntäjä

Kun olet kloonannut repositorion aja repositorion kansiossa seuraavat komennot:

- ```cmake .```
Tämä lataa ohjelman käyttämän ui-kirjaston ja luo Makefilen.
- ```make```
Tämä kääntää lähdekoodin ohjelmaksi.

## Ohjelman ajaminen

Ohjelmaa ajetaan terminaalissa komennolla ```./Minesweeper``` repositorion kansiossa.
Lisäämällä komennon perään lipun ```-s``` ja kokonaisluvun, voit antaa pelilaudan generoivalle algoritmille aloitussyötteen.
Ilman tätä pelilauta generoidaan satunnaisesti.
Pelilaudan muut parametrit, kuten koko ja miinojen määrä on muutettavissa lähdekoodin main.cpp tiedostossa.

Algoritmiltä puuttuu vielä kyky tehdä arvauksia, joten käyttäjän on valittava ensimmäinen siirto.
Myös tilanteissa, joissa algoritmi jää jumiin eikä pysty ratkaisemaan miinaharavaa pidemmälle, on käyttäjän mahdollista antaa seuraava siirto.

Siirrot annetaan muodossa: toiminto rivi sarake
Mahdollisia toimintoja:
- 'o': Avaa annetun ruudun
- 'f': Asettaa lipun annettuun ruutuun
- 'q': Lopetta pelin

Eli esimerkiksi komento: "q 1 1" avaa vasemman yläkulman ruudun.

## Testaajalle

Aloitussyötteellä 2 ja avaamalla ruudun (1, 1) peli saadaan pelattua loppuun :)
