#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QUrl>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    player = new QMediaPlayer;
    vWidget = new QVideoWidget;
    //根据label尺寸设置播放区域
    vWidget->resize(ui->label_player->size());
    vlayout = new QVBoxLayout;
    //vlayout->setMargin(1);
    vlayout->addWidget(vWidget);
    ui->label_player->setLayout(vlayout);
    player->setVideoOutput(vWidget); // player先和vWidget关联起来，后面对player加载视频（音频）

    //设置主窗口标题
    setWindowTitle(tr("Sweetheart Player"));
    // 打开文件
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::on_open_file);

    connect(player, &QMediaPlayer::durationChanged, this, &MainWindow::slot_durationChanged);// 获取时长->进度条长度
    connect(ui->horizontalSlider, &QSlider::sliderMoved, this, &MainWindow::slot_setPosition); //进度条->媒体进度
    connect(player, &QMediaPlayer::positionChanged, this, &MainWindow::slot_positionChanged); // 媒体进度->进度条位置
    connect(player, &QMediaPlayer::stateChanged, this, &MainWindow::slot_playerStateChanged); // 更改"播放"(暂停)按钮文字

}

MainWindow::~MainWindow()
{
    delete ui;
}

// 获取歌曲的长度,设置进度条长度
void MainWindow::slot_durationChanged(qint64 duration)
{
    ui->horizontalSlider->setRange(0, duration);
    //qDebug() << "duration:" << duration << endl;
    // 设置总时长标签
    QString totalTime = "/" + millisecond2time(duration);
    ui->label_total_time->setText(totalTime);
}

// 设置媒体播放位置
void MainWindow::slot_setPosition(qint64 pos)
{
    player->setPosition(pos);
}

// 根据媒体播放进度修改滑动条进度
void MainWindow::slot_positionChanged(qint64 pos)
{
    ui->horizontalSlider->setValue(pos);
    // 设置当前播放时间标签
    ui->label_current_time->setText(millisecond2time(pos));
}

//根据播放状态改变按钮文字
void MainWindow::slot_playerStateChanged(QMediaPlayer::State state)
{
    if (state == QMediaPlayer::PlayingState) {
        ui->pushButton_play->setText("暂停");
    } else {
        ui->pushButton_play->setText("播放");
    }
}

void MainWindow::on_open_file(){
    //选择视频文件
    QString filename = QFileDialog::getOpenFileName(this, tr("select file"), ".");
    if (0 == filename.length()) { //没有选中文件则返回
        return;
    }
    //设置播放器
    player->setMedia(QUrl::fromLocalFile(filename));
    player->play();
    qDebug() << player->duration() << endl;
}

// 播放与暂停切换
// 按钮文字由player的stateChanged信号的槽函数更改
void MainWindow::on_pushButton_play_clicked()
{
    if (player->state() == QMediaPlayer::PlayingState) {
        player->pause();
    } else {
        player->play();
    }
}

void MainWindow::on_pushButton_stop_clicked()
{
    player->stop();
}

// 将毫秒转换为时间00:00
QString MainWindow::millisecond2time(qint64 mill)
{
    QString minuteString;
    QString secondString;

    qint64 seconds = mill / 1000;
    qint64 minute = seconds / 60;
    qint64 second = seconds % 60;

    if (second < 10) { //为了美观补0
        secondString.append("0");
    }
    secondString.append(QString::number(second));

    if (minute < 10) {
        minuteString.append("0");
    }
    minuteString.append(QString::number(minute));

    QString timeString;
    timeString.append(minuteString).append(":").append(secondString);

    return timeString;
}
