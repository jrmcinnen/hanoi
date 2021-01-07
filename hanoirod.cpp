/* Tappi-luokan toteutus
 *
 * Ohjelman tekijä:
 * Nimi: Jere Mäkinen
 * Opiskelijanumero: 273090
 * UserID: makinenj
 * Email: jere.makinen@tuni.fi
*/

#include "hanoirod.hh"

Hanoirod::Hanoirod(int x, int y, QGraphicsScene* scene):
    top_(nullptr), top_coordinate_(BORDER_DOWN-DISK_HEIGHT), scene_(scene),
    x_(x), y_(y)
{
    print_ = scene_->addRect(x_, y_, ROD_WIDTH, ROD_HEIGHT);
}

Hanoirod::~Hanoirod()
{
    while( top_ != nullptr ) {
        Disk* disk_to_be_removed = top_;
        Hanoidisk* disk = disk_to_be_removed->disk;
        top_ = top_->next;

        delete disk;
        disk = nullptr;

        delete disk_to_be_removed;
        disk_to_be_removed = nullptr;
    }
}

int Hanoirod::get_x() const
{
    return x_;
}

int Hanoirod::get_y() const
{
    return y_;
}

int Hanoirod::get_top() const
{
    return top_coordinate_;
}

int Hanoirod::get_topdisk_size() const
{
    // Tyhjälle tapille palautetaan arvoksi nolla siirtojen
    // tarkastelua varten.
    if ( is_rod_empty() ) {
        return 0;

    } else {
        return top_->disk->get_size();
    }
}

QGraphicsRectItem* Hanoirod::get_rod_item()
{
    return print_;
}

Hanoidisk* Hanoirod::get_topdisk()
{
    return top_->disk;
}

bool Hanoirod::is_rod_empty() const
{
    if ( top_ == nullptr ) {
        return true;

    } else {
        return false;
    }
}

void Hanoirod::add_disk(Hanoidisk* disk_to_be_added)
{
    Disk* new_disk = new Disk{disk_to_be_added, nullptr};

    if ( top_ == nullptr ){
        top_ = new_disk;

    } else {
        new_disk->next = top_;
        top_ = new_disk;
    }
    top_coordinate_ -= DISK_HEIGHT;
}

void Hanoirod::remove_disk()
{
    // Tyhäjästä tapista ei voida poistaa kiekkoa.
    if ( top_ != nullptr ) {
        Disk* disk_to_be_removed = top_;
        top_ = top_->next;
        top_coordinate_ += DISK_HEIGHT;

        delete disk_to_be_removed;
        disk_to_be_removed = nullptr;
    }
}

void Hanoirod::move_topdisk(Hanoirod* target)
{
    // Tyhjästä tapista, ei voida siirtää kiekkoa. Muuten siirtoja ei
    // tässä rajoiteta.
    if( top_ != nullptr ) {
        top_->disk->move_disk(target->get_x() -
                              (top_->disk->get_size()-ROD_WIDTH)/2,
                               target->get_top());
        target->add_disk(top_->disk);
        remove_disk();
    }
}

// Siirto on sallittu, jos kohde tappi on tyhjä tai sen ylin kiekko on
// suurempi kuin siirrettävä kiekko.
bool Hanoirod::is_move_legal(Hanoirod* target) const
{
    if ( is_rod_empty() ) {
        return false;

    } else if ( target->get_topdisk_size() == 0 ) {
        return true;

    } else if ( get_topdisk_size() < target->get_topdisk_size() ) {
        return true;

    } else {
        return false;
    }
}
