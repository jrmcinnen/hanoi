/* Kiekko
 *
 * Luokka, joka vastaa Hanoin torni -pelin yhtä kiekkoa.
 * Sen ominaisuuksia ovat levyn koordinaatit ja sen koko,
 * lisäksi kiekon kuva ja alusta, jolla se sijaitsee. Kiekkoa
 * on mahdollista siirtää uusiin koordinaatteihin. Lisäksi luokalla
 * on yksinkertaisia palautusfunktioita.
 *
 * Ohjelman tekijä:
 * Nimi: Jere Mäkinen
 * Opiskelijanumero: 273090
 * UserID: makinenj
 * Email: jere.makinen@tuni.fi
*/

#include "mainwindow.hh"

#ifndef HANOIDISK_HH
#define HANOIDISK_HH

const int DISK_HEIGHT = 15;
const int SMALLEST_DISK_WIDTH = 20;

class Hanoidisk
{
public:
    Hanoidisk(int x, int y, int size, QGraphicsScene* scene);

    // Palauttaa levyn koon.
    int get_size() const;

    // Palauttaa kiekon kuvan.
    QGraphicsRectItem* get_disk_item();

    // Siirtää levyn uusiin koordinaatteihin.
    void move_disk(int x, int y);

private:
    // Levyn koordinaatit
    int x_;
    int y_;

    // Levyn koko
    int size_;

    // Piirtoalusta, jolla levy sijaitsee.
    QGraphicsScene* scene_;

    // Levyn kuva alustalla.
    QGraphicsRectItem* print_;
};

#endif // HANOIDISK_HH
