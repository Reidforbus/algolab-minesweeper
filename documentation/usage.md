# Käyttöohje

## Asennus

Sinulla tulee olla asennettuna cmake ja jokin C++- kääntäjä.

Kun olet kloonannut repositorion aja repositorion kansiossa seuraavat komennot:

```bash
cmake .
```


Tämä lataa ohjelman käyttämän ui-kirjaston ja luo Makefilen.

```bash
make -j
```

Tämä kääntää lähdekoodin ohjelmaksi.

## Ohjelman ajaminen

Ohjelmaa ajetaan terminaalissa komennolla ```./minesweeper``` repositorion kansiossa.

Lisäämällä komennon perään lipun ```-s``` ja kokonaisluvun, voit antaa pelilaudan generoivalle algoritmille aloitussyötteen.

```bash
./minesweeper -s 1329813523
```

Ilman tätä pelilauta generoidaan satunnaisesti.
Pelilaudan muut parametrit, kuten koko ja miinojen määrä on muutettavissa lähdekoodin main.cpp tiedostossa.
 *Muista ajaa ```make``` uudestaan, jos muutat parametrejä!*

Tilanteissa, joissa algoritmi jää jumiin eikä pysty ratkaisemaan miinaharavaa pidemmälle, on käyttäjän mahdollista antaa seuraava siirto.

Siirrot annetaan muodossa: ```toiminto rivi sarake```

Mahdollisia toimintoja:

- ```o```: Avaa annetun ruudun
- ```f```: Asettaa lipun annettuun ruutuun
- ```q```: Lopetta pelin

Eli esimerkiksi komento: ```o 1 1``` avaa vasemman yläkulman ruudun.

## Testien ajaminen

Testit voi ajaa komennolla ```ctest```

Testin kattavuusraporttia varten täytyy olla käytössä Clang tai GNU -kääntäjä sekä gcovr asennettuna. gcovr-ohjelman voi asentaa komennolla ```pip install gcovr```.

Komennolla ```make coverage``` ohjelma ajaa testit ja generoi testien kattavuusraportin coverage-kansioon, ```coverage.html``` nimiseen tiedostoon.
