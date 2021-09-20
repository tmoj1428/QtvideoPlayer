#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QMouseEvent>
#include <QListWidgetItem>
#include "videoplayer.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void playVideo();

private:
    Ui::MainWindow                      *ui;
    videoPlayer                         *Player;
    int                                 counter = 1;
    QMap<QString, QString>              source;
    int                                 currentVideo;

private slots:
    void updatePlayerUI(QImage img);

    void on_play_pushButton_clicked();
    void on_load_pushButton_clicked();
    void on_actionVideo_Player_triggered();
    void on_actionCamera_triggered();
    void playNewVideo(QListWidgetItem *item);
    void on_addPlayList_pushButton_clicked();
    void on_next_pushButton_clicked();
    void on_prev_pushButton_clicked();
    void videoEnd();
    void playVideo(int current);
};
#endif // MAINWINDOW_H
