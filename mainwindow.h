#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QVBoxLayout>

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
    void slot_durationChanged(qint64 duration);
    void slot_setPosition(qint64 pos);
    void slot_positionChanged(qint64 pos);
    void slot_playerStateChanged(QMediaPlayer::State state);

    void on_pushButton_play_clicked();

    void on_pushButton_stop_clicked();

private:
    QString millisecond2time(qint64 mill);

private:
    Ui::MainWindow *ui;
    // action
    void on_open_file();
    //
    QMediaPlayer *player;
    QVideoWidget *vWidget;
    QVBoxLayout  *vlayout;
};

#endif // MAINWINDOW_H
