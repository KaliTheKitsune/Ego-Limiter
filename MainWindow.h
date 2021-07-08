//
// Created by kalic on 07/06/2021.
//

#ifndef EGO_LIMITER_MAINWINDOW_H
#define EGO_LIMITER_MAINWINDOW_H

#include <QtWidgets>

class MainWindow : public QWidget{

Q_OBJECT

public slots:
    void push_ego();

public:
    MainWindow();
    ~MainWindow();

    int get_xp() const;
    void set_xp(int new_xp);
    int get_level();
    void set_level(int lvl);
    void increment_level();
    static int get_level_xp(const int& new_level);
    static std::string humanize_number(const int& number);

protected:
    // widgets
    QPushButton *pb_clickme;
    QProgressBar *pb_level;
    QLabel *l_level;
    QLabel *l_xp;
    QGraphicsDropShadowEffect *shadow_effect;

    // vars
    int xp;
    int level;

    // functions
    void set_label_xp(const int& xp);
    void set_label_level(const int& lvl);
    void update_progress_bar(int value);
    void refresh_shadow(const int& blur = 25);

    //save functions
    void save() const;
    void load();
};


#endif //EGO_LIMITER_MAINWINDOW_H
