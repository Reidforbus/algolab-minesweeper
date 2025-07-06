# Toteutusdokumentti

## Rakenne

Ohjelma sisältää seuraavat osat:

- Pelimoottori [Minegame](https://github.com/Reidforbus/algolab-minesweeper/blob/main/src/minesweeper/game.cpp)
    - Generoi pelilaudan ja ylläpitää sen tilaa
    - Ottaa vastaan avaus- ja liputussiirtoja

- Terminaalikäyttöliittymä [GameTUI](https://github.com/Reidforbus/algolab-minesweeper/blob/main/src/minesweeper/tui.cpp)
    - Tulostaa pelilaudan tilan

- Tekoäly [CSPAI](https://github.com/Reidforbus/algolab-minesweeper/blob/main/src/minesweeper/aicore.cpp)
    - Laskee miinojen ja vapaiden ruutujen sijainnit
    - Sisältää CSP luokan [CSPGraph](https://github.com/Reidforbus/algolab-minesweeper/blob/main/include/minesweeper/csp.hpp), joka abstraktoi miinaharavakentän muuttujien summiksi
    - Jos CSP luokka ei saa laskettua siirtoa, avaamattomista ruuduista arvotaan yksi avattavaksi

## Käytetyt algoritmit

- Syvyyshaku
    - Yhtälöiden sievennys ja ratkaisuiden etsintä etenee yhtälöiden verkossa syvyyshaun tavoin
    - Kun yhtälöiden sieventäminen ei tuota siirtoa, etsitään jokainen mahdollinen kaikki rajoitukset täyttävä ratkaisu. 
    Tämä käyttää peruuttavaa syvyyshakua $O(n!)$. Tämän tehottomuus on suurin pullonkaula algoritmin suoritusajassa, varsinkin kun pelissä tulee vastaan pitkä ketju miinoja.
    - Pelimoottori avaa tyhjät ruudut käyttäen syvyyshakua

- Verkon komponenttien haku
    - Ennen kaikkien ratkaisuiden etsintää, yhtälöiden verkko jaetaan komponentteihinsa


## Parannus- ja kehitysideat

Kaikkien ratkaisuiden hakua voisi parantaa esim. järjestämällä komponentit kasvavaan järjestykseen ja tallentamalla haun tuloksen, jos siirtoja ei löydy, koska toistaiseksi algoritmi tekee laskun uudestaan, vaikka komponentin tila ei ole muuttunut.

Myös haun sisäistä toimintaa voisi parantaa, että peruuttaminen tapahtuisi aiemmin.


## Kielimallien käyttö

Käytin ChatGPT:tä algoritmien ymmärtämisen apuna, eniten constraint satisfaction problem tekniikoiden ymmärtämisessä. 
Siitä oli myös apua oikean terminologian löytämisessä, kun yritti etsiä materiaaleja aiheeseen liittyen.

Yhden muistinhallintaongleman ratkaisu löytyi ChatGPT:ltä.

## Lähteet

- [Becerra - Algorithmic approaches to playing minesweeper](https://dash.harvard.edu/server/api/core/bitstreams/7312037d-80a6-6bd4-e053-0100007fdf3b/content)
- [Studholme - Minesweeper as a constraint satisfaction problem](https://www.cs.toronto.edu/~cvs/minesweeper/minesweeper.pdf)
