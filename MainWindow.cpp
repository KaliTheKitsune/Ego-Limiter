//
// Created by kalic on 07/06/2021.
//

#include "MainWindow.h"
#include <random>
#include <fstream>
#include <cstdlib>

MainWindow::MainWindow() {
    //
    //setting up styling
    //
    qApp->setStyleSheet(R"(
        QWidget { background-color: #212121 }
        QLabel { color: #FFFFFF; }
        QPushButton {
            background: #EF5DA8;
            color: #FFFFFF;
            border-radius: 15px;
            font-family: Roboto;
            font-style: normal;
            font-weight: normal;
            font-size: 20px;
            line-height: 29px;
        }
        QProgressBar {
            background: rgba(255, 255, 255, 0.05);
            border-radius: 20px;
            border: none;
        }
        QProgressBar::chunk {
            background: qlineargradient( x1:0 y1:0, x2:0 y2:1, stop: 0 rgba(120, 121, 241, 0.25), stop: 1 #EF5DA8);
            border-radius: 20px;
        }
    )");

    //
    //initialising widgets
    //

    setFixedSize(1040, 585);

    // Progress bar
    pb_level = new QProgressBar(this);
    pb_level->setFixedSize(215,455);
    pb_level->move(65, 65);
    pb_level->setOrientation(Qt::Vertical);
    pb_level->setTextVisible(false);
    // add shadows
    refresh_shadow();
    pb_level->setGraphicsEffect(shadow_effect);

    // init level
    l_level = new QLabel(this);
    l_level->setFixedSize(650, 100);
    l_level->move(350, 100);
    l_level->setTextFormat(Qt::RichText);

    // init xp
    l_xp = new QLabel(this);
    l_xp->setFixedSize(650, 100);
    l_xp->move(350, 200);
    l_xp->setTextFormat(Qt::RichText);

    // push button
    pb_clickme = new QPushButton("INSÉRER DE L'EGO",this);
    pb_clickme->setFlat(true);
    pb_clickme->move(520, 420);
    pb_clickme->setFixedSize(250, 100);
    // add shadows
    refresh_shadow();
    pb_clickme->setGraphicsEffect(shadow_effect);

    connect(pb_clickme, SIGNAL(clicked(bool)), this, SLOT(push_ego()));

    //
    //Loading up dat file
    //
    load();
}

int MainWindow::get_level_xp(const int& new_level){
    return (new_level ^ 2) + (50 *  new_level) + 100;
}

int MainWindow::get_xp() const{
    return xp;
}

void MainWindow::set_xp(int new_xp) {
    // set xp
    xp = new_xp;

    //update progress bar
    update_progress_bar(xp);

    //updating level label
    set_label_xp(xp);
}

int MainWindow::get_level(){
    return level;
}

void MainWindow::set_level(int lvl){
    // set level
    level = lvl;

    //update progress bar max
    pb_level->setMaximum(get_level_xp(level));

    //updating level label
    set_label_level(level);
}

void MainWindow::increment_level() {
    //computing new xp value
    set_xp(xp - get_level_xp(level));
    set_level(level + 1);
}

void MainWindow::push_ego() {
    // generate a random xp number between 15 and 35 (included)
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(15,35);
    int added_xp = distribution(generator);

    // updating xp level
    set_xp(xp + added_xp);

    // checking if level has been completed
    if (xp >= get_level_xp(level)) {
        increment_level();
    }
}

void MainWindow::set_label_xp(const int& xp) {
    // preparing max_xp value
    int max_xp = get_level_xp(level);

    // setting up the label
    QString text = QString("<span style='font-size:25pt'>Ego <span style='font-size:50pt'>(%1/%2)</span></span>");
    text = text.arg(QString::fromStdString(humanize_number(xp)),
                    QString::fromStdString(humanize_number(max_xp)));

    // updating the label
    l_xp->setText(text);
}

void MainWindow::set_label_level(const int& lvl) {
    // setting up the label
    QString text = QString("<span style='font-size:25pt'>Niveau <span style='font-size:50pt'>%1</span></span>");
    text = text.arg(QString::fromStdString(humanize_number(lvl)));

    // updating the label
    l_level->setText(text);
}

void MainWindow::update_progress_bar(int value) {
    int percentage = value * 100 / pb_level->maximum();

    // value is too low and break the round edges
    if (percentage < 10) {
        value = 10 * pb_level->maximum() / 100;
    }
    //value is too hight and hide the progress bar
    else if (value > pb_level->maximum()) {
        value = pb_level->maximum();
    }

    pb_level->setValue(value);
    pb_level->repaint(); //some time the progressbar just freezes so...
}

std::string MainWindow::humanize_number(const int &number) {
    std::string u;
    std::string r;
    auto n = (float)number;

    if (number < 0) return ">0";
    else if (number >= 1000000000) {
        r = std::to_string(n/1000000000);
        u = " G";
    }
    else if (number >= 1000000) {
        r = std::to_string(n/1000000);
        u = " M";
    }
    else if (number >= 1000) {
        r = std::to_string(n/1000);
        u = " K";
    }
    else return std::to_string(number);

    r = r.substr(0, r.find('.')+3) + u;

    return r;
}

void MainWindow::refresh_shadow(const int& blur) {
    shadow_effect = new QGraphicsDropShadowEffect;
    shadow_effect->setBlurRadius(blur);
    shadow_effect->setXOffset(50);
    shadow_effect->setYOffset(50);
    shadow_effect->setColor(QColor(0, 0, 0, 70));
}

void MainWindow::save() const
{
    std::ofstream of;
    char buffer[11];

    of.open("save.dat", std::ios::binary | std::ios::out);

    sprintf(buffer, "%d", xp);
    of.write(buffer, 11);
    sprintf(buffer,  "%d", level);
    of.write(buffer, 11);
    of.close();
}

void MainWindow::load()
{
    std::ifstream inf;
    char* xp_buffer = new char[11];
    char* level_buffer = new char[11];

    inf.open("save.dat", std::ios::binary | std::ios::in);
    inf.read(xp_buffer, 11);
    inf.read(level_buffer, 11);
    inf.close();

    set_level(strtol(level_buffer, nullptr, 10));
    set_xp(strtol(xp_buffer, nullptr, 10));

    delete[] xp_buffer;
    delete[] level_buffer;
}

MainWindow::~MainWindow() {
    save();
}
