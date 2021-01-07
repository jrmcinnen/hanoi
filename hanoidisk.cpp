/* Kiekko-luokan toteutus.
 *
 * Ohjelman tekijä:
 * Nimi: Jere Mäkinen
 * Opiskelijanumero: 273090
 * UserID: makinenj
 * Email: jere.makinen@tuni.fi
*/

#include "hanoidisk.hh"

Hanoidisk::Hanoidisk(int x, int y, int size, QGraphicsScene* scene):
    x_(x), y_(y), size_(size), scene_(scene)
{
    print_ = scene_->addRect(x_, y_, size_, DISK_HEIGHT);
}

int Hanoidisk::get_size() const
{
    return size_;
}

QGraphicsRectItem* Hanoidisk::get_disk_item()
{
    return print_;
}

void Hanoidisk::move_disk(int x, int y)
{
    print_->moveBy(x - x_, y - y_);
    x_ = x;
    y_ = y;
}
