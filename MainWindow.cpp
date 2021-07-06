//
// Created by kalic on 07/06/2021.
//

#include "MainWindow.h"
#include <random>

MainWindow::MainWindow() {
    setFixedSize(1040, 585);

    //
    //initialising widgets
    //

    // Progress bar
    pb_level = new QProgressBar(this);
    pb_level->setFixedSize(215,455);
    pb_level->move(65, 65);
    pb_level->setOrientation(Qt::Vertical);

    // init level
    l_level = new QLabel(this);
    l_level->setFixedSize(450, 100);
    l_level->move(300, 100);
    l_level->setTextFormat(Qt::RichText);
    l_level->setFont(QFont("Roboto"));
    level = new int;
    set_level(1);

    // init xp
    l_xp = new QLabel(this);
    l_xp->setFixedSize(450, 100);
    l_xp->move(300, 200);
    l_xp->setTextFormat(Qt::RichText);
    l_xp->setFont(QFont("Roboto"));
    xp = new int;
    set_xp(0);

    // push button
    pb_clickme = new QPushButton("INSÉRER DE L'EGO",this);
    pb_clickme->move(520, 420);
    pb_clickme->setFixedSize(250, 100);

    connect(pb_clickme, SIGNAL(clicked(bool)), this, SLOT(push_ego()));
}


int MainWindow::get_level_xp(const int *new_level){
    return 5 * (*new_level ^ 2) + (50 *  *new_level) + 100;
}

int MainWindow::get_xp() const{
    return *xp;
}

void MainWindow::set_xp(int new_xp) {
    // set xp
    *xp = new_xp;

    //update progress bar max
    pb_level->setValue(*xp);

    //updating level label
    set_label_xp(xp);
}

int MainWindow::get_level(){
    return *level;
}

void MainWindow::set_level(int lvl){
    // set level
    *level = lvl;

    //update progress bar max
    pb_level->setMaximum(get_level_xp(level));

    //updating level label
    set_label_level(level);
}

void MainWindow::increment_level() {
    //computing new xp value
    set_xp(*xp - get_level_xp(level));
    set_level(*level + 1);
}

void MainWindow::push_ego() {
    // generate a random xp number between 10 and 25 (included)
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(10,25);
    int added_xp = distribution(generator);

    // updating xp level
    set_xp(*xp + added_xp);

    qDebug() << "xp ="<< *xp;

    // checking if level has been completed
    if (*xp >= get_level_xp(level)) {
        increment_level();
    }

    //updating progress bar
    pb_level->setValue(*xp);

    //updating the label
    set_label_xp(xp);
}

void MainWindow::set_label_xp(int *xp) {
    // preparing max_xp value
    int max_xp = get_level_xp(level);

    // setting up the label
    QString text = QString("<span style='font-size:25pt'>Ego <span style='font-size:50pt'>(%1/%2)</span></span>").arg(*xp).arg(max_xp);

    // updating the label
    l_xp->setText(text);
}

void MainWindow::set_label_level(int *lvl) {
    // setting up the label
    QString text = QString("<span style='font-size:25pt'>Niveau <span style='font-size:50pt'>%1</span></span>").arg(*lvl);

    // updating the label
    l_level->setText(text);
}

