/* Hanoin torni
 *
 * Hanoin torni pelin graafinen käyttöliittymä.
 * Käyttöliittymä koostuu piirtoalusta ja painikkeista, joilla kiekkoja voi
 * liikutella, pelin aloitus- ja keskeytyspainikkeet. Painikkeet löytyvät myös
 * automaattiseen ratkaisuun, alkutilanteen palautukseen ja ennätysajan nollaa-
 * miseen. Piirtoalueelle piirretään kolme tappia ja vakioarvon mukainen määrä
 * kiekkoja.
 *
 * Ohjelman tekijä:
 * Nimi: Jere Mäkinen
 * Opiskelijanumero: 273090
 * UserID: makinenj
 * Email: jere.makinen@tuni.fi
*/

#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMainWindow>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QTimer>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "hanoirod.hh"

const int DISK_AMMOUNT = 6;

const int BORDER_UP = 0;
const int BORDER_DOWN = 260;
const int BORDER_LEFT = 0;
const int BORDER_RIGHT = 680;

const QString GAME_WON_MESSAGE = "Congratulations! You won the game!";
const QString AUTO_SOLVE_MESSAGE = "Auto solve completed.";
const std::string BEST_TIME_FILE_NAME = "best_time.txt";
const std::string DEFAULT_BEST_TIME = "xx:xx";

class Hanoirod;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    // Asettaa siirtopainikkeet pois päältä.
    void disable_all_buttons();    

    // Peliajan näyttäminen.
    void count();   

    // Pelin alkaessa käynnistää ajanoton ja aktivoi siirtopainikkeet,
    // sekä pause-painikkeen.
    void start();

    // Pysäyttää ajanoton.
    void stop();

    // Palauttaa pelin alkuasetelman. Pelin voi aloittaa alusta tai käyttää
    // automaattista ratkaisua.
    void reset();

    // Aloittaa pelin automaattisen ratkaisun.
    void start_auto_solve();

    // Suorittaa siirrot automaattisessa ratkaisussa.
    void auto_move_disk();

    // Kiekkojen siirrot. Siirtofunktioita kutsutaan painikkeen mukaisille
    // tapeille ja kiekkojen värejä muutetaan kohdetapin mukaan.
    void a_to_b();
    void a_to_c();
    void b_to_a();
    void b_to_c();
    void c_to_a();
    void c_to_b();

    // Parhaan ajan nollaaminen. Ennätykseksi asetetaan oletusarvo.
    void reset_best_time();

private:
    Ui::MainWindow *ui;

    QGraphicsScene* scene_;
    QTimer* timer_;
    QPainter* painter_;

    int seconds_ = 0;
    int minutes_ = 0;

    // Tapit
    Hanoirod* rod_a_;
    Hanoirod* rod_b_;
    Hanoirod* rod_c_;

    // Apumuuttujat automatisoituun ratkaisuun.
    std::string last_move_ = "bc";
    bool is_auto_mode_on_ = false;

    // Ajastin automaattisia siirtoja varten.
    QTimer* move_timer_;

    // Toimenpiteet kiekkoa siirrettäessä. Varsinasen siirron lisäksi tarkis-
    // tetaan muiden funktioiden avulla seuraavat lailliset siirrot ja onko
    // peli voitettu.
    void move_disk(Hanoirod* from, Hanoirod* to);

    // Sallittujen siirtonappien käyttöönotto ja käytöstä pois otto.
    void check_allowed_moves();

    // Tarkistaa onko peli voitettu.
    bool is_game_won();

    // Toimenpiteet kun peli voitetaan.
    void game_won();

    // Tarkistaa onko uusi aika myös uusi ennätysaika.
    bool is_best_time();

    // Muutta ennätysajan uuteen.
    void set_new_best_time();

    // Perinteinen split-funktio. Käytetään tarkastellessa, oliko uusi aika
    // ennätys.
    std::vector<std::string> split(const std::string text);
};

#endif // MAINWINDOW_HH
