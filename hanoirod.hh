/* Tappi
 *
 * Luokka, joka vastaa Hanoin torni -pelin yhtä tappia.
 * Tapin ominaisuuksia ovat sen koordinaatit, sen ylimmäin levyn sijainti,
 * tapin kuva ja alusta, jolla se sijaitsee. Tappi myös tietää, mitä kiekkoja
 * siinä on. Tämä tarkastelu on toteutettu linkitettynä listana. Yhtenä omi-
 * naisuuten on osoitin ylimpään kiekkoon.
 *
 * Tappiin voi lisätä uuden kiekon ja ylimmän kiekon voi siirtää toiseen
 * tappiin. Lisäksi voidaan tarkastella, onko jokin siirto pelin sääntöjen
 * rajoissa sallittu ja onko tapissa yhtään kiekkoa.
 *
 * Ohjelman tekijä:
 * Nimi: Jere Mäkinen
 * Opiskelijanumero: 273090
 * UserID: makinenj
 * Email: jere.makinen@tuni.fi
*/

#ifndef HANOIROD_HH
#define HANOIROD_HH

#include "hanoidisk.hh"

const int ROD_HEIGHT = 220;
const int ROD_WIDTH = 5;

class Hanoidisk;

class Hanoirod
{
public:
    Hanoirod(int x, int y, QGraphicsScene* scene);
    ~Hanoirod();

    // Koordinattiarvojen palautukset.
    int get_x() const;
    int get_y() const;
    int get_top() const;

    // Ylimmän kiekon koko
    int get_topdisk_size() const;

    // Palauttaa ylimmänkiekon.
    Hanoidisk* get_topdisk();

    // Palauttaa tangon kuvan.
    QGraphicsRectItem* get_rod_item();

    // Tarkistaa onko tanko tyhjä.
    bool is_rod_empty() const;

    // Asettaa uuden arvon ylimmän levyn koordinaatille.
    void set_top_coordinate( int transition );

    // Lisää kiekon tankoon.
    void add_disk(Hanoidisk* disk_to_be_added);

    // Siirtää kiekon tangon ylimmän kiekon toiseen tankoon.
    void move_topdisk(Hanoirod* target);

    // Tarkistaa onko siirto sallittu.
    bool is_move_legal(Hanoirod* target) const;

private:
    // Linkitetty listarakenne tangossa olevien levyjen käsittelyyn.
    // Muodostuu kiekko-oliosta ja osoittimesta seuravaan kiekkoon.
    struct Disk {
        Hanoidisk* disk;
        Disk* next;
    };
    // Osoitin tapin ylimpään kiekkoon.
    Disk* top_ = nullptr;

    // Ylimmän levyn y-koordinaatti.
    int top_coordinate_ ;

    // Piirtoalusta, jolla tanko on.
    QGraphicsScene* scene_;
    // Tangon kuva alustalla.
    QGraphicsRectItem* print_;

    // Tangon koordinaatit
    const int x_;
    const int y_;

    // Poistaa kiekon tangosta.
    void remove_disk();
};

#endif // HANOIROD_HH
