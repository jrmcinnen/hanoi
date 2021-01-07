/* Graafisen käyttöliittymän toteutus
 *
 * Ohjelman tekijä:
 * Nimi: Jere Mäkinen
 * Opiskelijanumero: 273090
 * UserID: makinenj
 * Email: jere.makinen@tuni.fi
*/


#include "mainwindow.hh"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene_ = new QGraphicsScene(this);
    timer_ = new QTimer;

    move_timer_ = new QTimer;

    int left_margin = 20;
    int top_margin = 80;

    ui->graphicsView->setGeometry(left_margin, top_margin,
                                   BORDER_RIGHT + 2, BORDER_DOWN + 2);
    ui->graphicsView->setScene(scene_);

    scene_->setSceneRect(0, 0, BORDER_RIGHT - 1, BORDER_DOWN - 1);

    ui->lcdNumberMin->setStyleSheet("background:black");
    ui->lcdNumberSec->setStyleSheet("background:black");
    ui->lcdNumberMin->setPalette(Qt::white);
    ui->lcdNumberSec->setPalette(Qt::white);

    // Tappien luominen.
    rod_a_ = new Hanoirod(BORDER_RIGHT/6,BORDER_DOWN-ROD_HEIGHT,
                        scene_);
    rod_b_ = new Hanoirod(BORDER_RIGHT/2,BORDER_DOWN-ROD_HEIGHT,
                        scene_);
    rod_c_ = new Hanoirod(BORDER_RIGHT*5/6,BORDER_DOWN-ROD_HEIGHT,
                        scene_);
    // Tappien värit.
    QBrush yellow_brush(Qt::yellow);
    QBrush cyan_brush(Qt::cyan);
    QBrush magenta_brush(Qt::magenta);

    rod_a_->get_rod_item()->setBrush(yellow_brush);
    rod_b_->get_rod_item()->setBrush(cyan_brush);
    rod_c_->get_rod_item()->setBrush(magenta_brush);

    // Kiekojen luominen pelilaudalle ja asettaminen aloitustappiin.
    QBrush dark_yellow_brush(Qt::darkYellow);

    for ( int i = DISK_AMMOUNT; i > 0; --i ) {
        Hanoidisk* new_disk = new Hanoidisk(rod_a_->get_x()-
                                            ((SMALLEST_DISK_WIDTH*20/19*i)
                                             -ROD_WIDTH)/2,
                                            rod_a_->get_top(),
                                        SMALLEST_DISK_WIDTH*20/19*i, scene_);
        rod_a_->add_disk(new_disk);
        rod_a_->get_topdisk()->get_disk_item()->setBrush(dark_yellow_brush);
    }    

    // Siirtonapit otetaan aluksi pois käytöstä, samoin myös pause- ja
    // reset-napit.
    ui->a_to_bButton->QWidget::setDisabled(true);
    ui->a_to_cButton->QWidget::setDisabled(true);
    ui->b_to_aButton->QWidget::setDisabled(true);
    ui->b_to_cButton->QWidget::setDisabled(true);
    ui->c_to_aButton->QWidget::setDisabled(true);
    ui->c_to_bButton->QWidget::setDisabled(true);
    ui->pauseButton->QWidget::setDisabled(true);
    ui->resetButton->QWidget::setDisabled(true);

    connect(ui->closeButton, &QPushButton::clicked, this, &MainWindow::close);

    connect(timer_, &QTimer::timeout, this, &MainWindow::count);

    connect(ui->startButton, &QPushButton::clicked, this,
            &MainWindow::start);

    connect(ui->pauseButton, &QPushButton::clicked, this,
            &MainWindow::stop);
    connect(ui->pauseButton, &QPushButton::clicked, this,
            &MainWindow::disable_all_buttons);

    connect(ui->resetButton, &QPushButton::clicked, this,
            &MainWindow::reset);

    connect(ui->auto_solveButton, &QPushButton::clicked, this,
            &MainWindow::start_auto_solve);
    connect(move_timer_, &QTimer::timeout, this, &MainWindow::auto_move_disk);

    connect(ui->reset_best_timeButton, &QPushButton::clicked, this,
            &MainWindow::reset_best_time);

    connect(ui->a_to_bButton, &QPushButton::clicked, this,
            &MainWindow::a_to_b);
    connect(ui->a_to_cButton, &QPushButton::clicked, this,
            &MainWindow::a_to_c);
    connect(ui->b_to_aButton, &QPushButton::clicked, this,
            &MainWindow::b_to_a);
    connect(ui->b_to_cButton, &QPushButton::clicked, this,
            &MainWindow::b_to_c);
    connect(ui->c_to_aButton, &QPushButton::clicked,this,
            &MainWindow::c_to_a);
    connect(ui->c_to_bButton, &QPushButton::clicked, this,
            &MainWindow::c_to_b);

    // Aiemman parhaan ajan lukeminen.
    std::ifstream best_time_file(BEST_TIME_FILE_NAME);
    std::string line;
    while ( std::getline(best_time_file, line)) {
        if( line != "" ) {
        QString current_best_time = QString::fromStdString(line);
        ui->best_timelabel->setText(current_best_time);
        }
    }
    best_time_file.close();
}

MainWindow::~MainWindow()
{
    delete timer_;
    delete move_timer_;

    rod_a_->~Hanoirod();
    rod_b_->~Hanoirod();
    rod_c_->~Hanoirod();

    delete rod_a_;
    delete rod_b_;
    delete rod_c_;

    delete ui;
}

void MainWindow::disable_all_buttons()
{
    ui->a_to_bButton->QWidget::setDisabled(true);
    ui->a_to_cButton->QWidget::setDisabled(true);
    ui->b_to_aButton->QWidget::setDisabled(true);
    ui->b_to_cButton->QWidget::setDisabled(true);
    ui->c_to_aButton->QWidget::setDisabled(true);
    ui->c_to_bButton->QWidget::setDisabled(true);
}

void MainWindow::count()
{
    ++seconds_;
    if (seconds_ == 60 ){
        ++minutes_;
        seconds_ = 0;
    }

    if (minutes_ == 60){
        minutes_ = 0;
    }

    ui->lcdNumberMin->display(minutes_);
    ui->lcdNumberSec->display(seconds_);
}


void MainWindow::start()
{
    timer_->start(1000);
    ui->startButton->QWidget::setDisabled(true);
    ui->auto_solveButton->QWidget::setDisabled(true);
    ui->pauseButton->QWidget::setEnabled(true);
    check_allowed_moves();
}

void MainWindow::stop()
{
    timer_->stop();
    ui->startButton->QWidget::setEnabled(true);
    ui->pauseButton->QWidget::setDisabled(true);
}

void MainWindow::reset()
{
    QBrush dark_yellow_brush(Qt::darkYellow);
    if ( rod_b_->is_rod_empty() ) {
        while( not rod_c_->is_rod_empty() ) {
            rod_c_->move_topdisk(rod_b_);
        }
        while(not rod_b_->is_rod_empty() ) {
            rod_b_->move_topdisk(rod_a_);
            rod_a_->get_topdisk()->get_disk_item()->
                    setBrush(dark_yellow_brush);
        }

    } else if ( rod_c_->is_rod_empty() ) {
        while( not rod_b_->is_rod_empty() ) {
            rod_b_->move_topdisk(rod_c_);
        }
        while( not rod_c_->is_rod_empty() ) {
            rod_c_->move_topdisk(rod_a_);
            rod_a_->get_topdisk()->get_disk_item()->
                    setBrush(dark_yellow_brush);
        }
    }
    ui->startButton->QWidget::setEnabled(true);
    ui->auto_solveButton->QWidget::setEnabled(true);
    ui->resetButton->QWidget::setDisabled(true);

    ui->game_wonlabel->setText("");
    seconds_ = 0;
    minutes_ = 0;

    ui->lcdNumberMin->display(minutes_);
    ui->lcdNumberSec->display(seconds_);
    ui->best_timelabel->setStyleSheet("color:black");
}

void MainWindow::start_auto_solve()
{
    ui->startButton->QWidget::setDisabled(true);
    ui->auto_solveButton->QWidget::setDisabled(true);
    is_auto_mode_on_ = true;

    move_timer_->start(500);
    timer_->start(1000);
}

void MainWindow::auto_move_disk()
{
    if ( last_move_ == "bc" ) {
        if ( rod_a_->is_move_legal(rod_b_) ) {
            a_to_b();
        } else if( rod_b_->is_move_legal(rod_a_) ) {
            b_to_a();
        }
        last_move_ = "ab";

    } else if ( last_move_ == "ab" ) {
        if ( rod_a_->is_move_legal(rod_c_) ) {
            a_to_c();
        } else if ( rod_c_->is_move_legal(rod_a_) ) {
            c_to_a();
        }
        last_move_ = "ac";

    } else if ( last_move_ == "ac" ) {
        if ( rod_b_->is_move_legal(rod_c_) ) {
            b_to_c();
        } else if ( rod_c_->is_move_legal(rod_b_) ) {
            c_to_b();
        }
        last_move_ = "bc";
    }

    disable_all_buttons();

    if ( is_game_won() ) {
        move_timer_->stop();
        timer_->stop();
        is_auto_mode_on_ = false;
    }
}

void MainWindow::a_to_b()
{
    QBrush dark_cyan_brush(Qt::darkCyan);
    rod_a_->get_topdisk()->get_disk_item()->setBrush(dark_cyan_brush);
    move_disk(rod_a_, rod_b_);
}

void MainWindow::a_to_c()
{
    QBrush dark_magenta_brush(Qt::darkMagenta);
    rod_a_->get_topdisk()->get_disk_item()->setBrush(dark_magenta_brush);
    move_disk(rod_a_, rod_c_);
}

void MainWindow::b_to_a()
{
    QBrush dark_yellow_brush(Qt::darkYellow);
    rod_b_->get_topdisk()->get_disk_item()->setBrush(dark_yellow_brush);
    move_disk(rod_b_, rod_a_);
}

void MainWindow::b_to_c()
{
    QBrush dark_magenta_brush(Qt::darkMagenta);
    rod_b_->get_topdisk()->get_disk_item()->setBrush(dark_magenta_brush);
    move_disk(rod_b_, rod_c_);
}

void MainWindow::c_to_a()
{
    QBrush dark_yellow_brush(Qt::darkYellow);
    rod_c_->get_topdisk()->get_disk_item()->setBrush(dark_yellow_brush);
    move_disk(rod_c_, rod_a_);
}

void MainWindow::c_to_b()
{
    QBrush dark_cyan_brush(Qt::darkCyan);
    rod_c_->get_topdisk()->get_disk_item()->setBrush(dark_cyan_brush);
    move_disk(rod_c_, rod_b_);
}

void MainWindow::move_disk(Hanoirod *from, Hanoirod *to)
{
    from->move_topdisk(to);
    check_allowed_moves();
    if ( is_game_won() ) {
        game_won();
    }
}

void MainWindow::reset_best_time()
{
    std::ofstream best_time_file(BEST_TIME_FILE_NAME);
    best_time_file << DEFAULT_BEST_TIME << std::endl;
    best_time_file.close();
    ui->best_timelabel->setText(QString::fromStdString(DEFAULT_BEST_TIME));
    ui->best_timelabel->setStyleSheet("color:black");
}

void MainWindow::check_allowed_moves()
{
    if ( rod_a_->is_move_legal(rod_b_) ) {
        ui->a_to_bButton->setEnabled(true);
    } else {
        ui->a_to_bButton->setDisabled(true);
    }

    if ( rod_a_->is_move_legal(rod_c_) ) {
        ui->a_to_cButton->setEnabled(true);
    } else {
        ui->a_to_cButton->setDisabled(true);
    }

    if ( rod_b_->is_move_legal(rod_a_) ) {
        ui->b_to_aButton->setEnabled(true);
    } else {
        ui->b_to_aButton->setDisabled(true);
    }

    if ( rod_b_->is_move_legal(rod_c_) ) {
        ui->b_to_cButton->setEnabled(true);
    } else {
        ui->b_to_cButton->setDisabled(true);
    }

    if ( rod_c_->is_move_legal(rod_a_) ) {
        ui->c_to_aButton->setEnabled(true);
    } else {
        ui->c_to_aButton->setDisabled(true);
    }

    if ( rod_c_->is_move_legal(rod_b_) ) {
        ui->c_to_bButton->setEnabled(true);
    } else {
        ui->c_to_bButton->setDisabled(true);
    }
}

bool MainWindow::is_game_won()
{
    if ( (rod_a_->is_rod_empty() and rod_b_->is_rod_empty()) or
         (rod_a_->is_rod_empty() and rod_c_->is_rod_empty())) {
        return true;
    } else {
        return false;
    }
}

void MainWindow::game_won()
{
    disable_all_buttons();
    ui->pauseButton->setDisabled(true);
    ui->resetButton->setEnabled(true);
    if( not is_auto_mode_on_ ) {
        ui->game_wonlabel->setText(GAME_WON_MESSAGE);
        timer_->stop();
        if( is_best_time() ) {
            set_new_best_time();
            ui->best_timelabel->setStyleSheet("color:green");
        } else {
            ui->best_timelabel->setStyleSheet("color:red");
        }
    } else {
        ui->game_wonlabel->setText(AUTO_SOLVE_MESSAGE);
    }
}
bool MainWindow::is_best_time()
{
    std::ifstream best_time_file(BEST_TIME_FILE_NAME);
    std::string line;
    std::getline(best_time_file, line);
    if ( line == DEFAULT_BEST_TIME ) {
        best_time_file.close();
        return true;
    } else {
    std::vector<std::string> best_time = split(line);
        if ( std::stoi(best_time.at(0)) < minutes_ ) {
            best_time_file.close();
            return false;
        } else if ( std::stoi(best_time.at(1)) <= seconds_ ) {
            best_time_file.close();
            return false;
        } else {
            best_time_file.close();
            return true;
        }
    }
}

void MainWindow::set_new_best_time()
{
    std::ofstream best_time_file(BEST_TIME_FILE_NAME);
    std::string new_best_time = "";
    if ( seconds_ < 10 ) {
        new_best_time = std::to_string(minutes_) + ":"
            + "0" + std::to_string(seconds_);
    } else {
        new_best_time = std::to_string(minutes_) + ":"
            + std::to_string(seconds_);
    }
    best_time_file << new_best_time << std::endl;
    best_time_file.close();

    ui->best_timelabel->setText(QString::fromStdString(new_best_time));
}

std::vector<std::string> MainWindow::split(const std::string text)
{
    std::vector<std::string> split_text;
    std::string::size_type start = 0;
    for( ;; ) {

        std::string::size_type spot = text.find(':', start);

        if ( spot == std::string::npos ) {
            split_text.push_back(text.substr(start, text.back()));
            return split_text;

        } else {
            split_text.push_back(text.substr(start, spot-start));
            start = spot+1;
        }
    }
}

