#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <QDebug>
#include <QLabel>
#include <QFileDialog>
#include <QMessageBox>
#include <QDrag>
#include <QMimeData>
#include <QMouseEvent>

#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace cv;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Player = new videoPlayer();
    QObject::connect(Player, SIGNAL(processedImage(QImage)),
                                this, SLOT(updatePlayerUI(QImage)));
    QObject::connect(ui->listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
                                this, SLOT(playNewVideo(QListWidgetItem*)));
    QObject::connect(Player, SIGNAL(videoEnded()),
                                this, SLOT(videoEnd()));
}

void MainWindow::updatePlayerUI(QImage img)
{
    if (!img.isNull())
    {
        ui->label->setAlignment(Qt::AlignCenter);
        ui->label->setPixmap(QPixmap::fromImage(img).scaled(ui->label->size()
                                           ,Qt::KeepAspectRatio, Qt::FastTransformation));
    }
}

MainWindow::~MainWindow()
{
    delete Player;
    delete ui;
}

void MainWindow::playVideo(int current)
{
    QString filename = source[ui->listWidget->item(current)->text()];
    ui->listWidget->setCurrentRow(current);
    if (!Player->loadVideo(filename.toAscii().data()))
    {
        QMessageBox msgBox;
        msgBox.setText("The selected video could not be opened!");
        msgBox.exec();
    }else{
        currentVideo = ui->listWidget->currentRow();
    }
}

void MainWindow::on_play_pushButton_clicked()
{
    if (!ui->actionCamera->isChecked() && !ui->actionVideo_Player->isChecked())
    {
        QMessageBox::warning(this, "Error", "Please select a type!");
        return;
    }else
    {
        if (Player->isStopped())
        {
            if (ui->actionCamera->isChecked())
            {
                Player->play(true);
                ui->play_pushButton->setText(tr("Stop"));
            }
            else if (ui->actionVideo_Player->isChecked() && ui->listWidget->count() != 0)
            {
                if (Player->source.isEmpty()){
                    playVideo(0);
                }
                Player->play(false);
                ui->play_pushButton->setText(tr("Stop"));
            }
            else
                QMessageBox::warning(this, "Error", "Please load a video!");
        }else
        {
            Player->Stop();
            ui->play_pushButton->setText(tr("Play"));
        }
    }
}

void MainWindow::on_load_pushButton_clicked()
{
    if (!ui->actionCamera->isChecked())
    {
        if (Player->isRunning())
        {
            Player->Stop();
            ui->play_pushButton->setText(tr("Play"));
        }

        QString filename = QFileDialog::getOpenFileName(this,
                                              tr("Open Video"), ".",
                                              tr("Video Files (*.avi *.mpg *.mp4)"));
        if (!filename.isEmpty()){
            if (!Player->loadVideo(filename.toAscii().data()))
            {
                QMessageBox msgBox;
                msgBox.setText("The selected video could not be opened!");
                msgBox.exec();
            }
            QStringList fileNameList = filename.split("/");
            source[fileNameList.last()] = filename;
            ui->listWidget->insertItem(0, fileNameList.last());
            ui->listWidget->setCurrentRow(0);
            if (Player->isStopped())
            {
                Player->play(false);
                ui->play_pushButton->setText(tr("Stop"));
            }
        }
        else{
            QMessageBox::warning(this, "Error", "Please select a video to play!");
        }
    }
}

void MainWindow::on_actionVideo_Player_triggered()
{
    if (ui->actionVideo_Player->isChecked())
    {
        if (Player->isRunning())
        {
            Player->Stop();
            ui->play_pushButton->setText(tr("Play"));
        }
        QThread::msleep(1000);
        if (ui->listWidget->count() != 0)
        {
            playNewVideo(ui->listWidget->item(0));
            Player->play(false);
            ui->play_pushButton->setText(tr("Stop"));
        }
        ui->actionCamera->setChecked(false);
    }
}

void MainWindow::on_actionCamera_triggered()
{
    if (ui->actionCamera->isChecked())
    {
        if (Player->isRunning())
        {
            Player->Stop();
            ui->play_pushButton->setText(tr("Play"));
        }
        QThread::msleep(2000);
        if (Player->isStopped())
        {
            Player->play(true);
            ui->play_pushButton->setText(tr("Stop"));
        }
        ui->actionVideo_Player->setChecked(false);
    }
}

void MainWindow::playNewVideo(QListWidgetItem *item)
{
    if (!ui->actionCamera->isChecked())
    {
        if (Player->isRunning())
        {
            Player->Stop();
            ui->play_pushButton->setText(tr("Play"));
        }
        QThread::msleep(1000);
        QString filename = source[item->text()];
        if (!Player->loadVideo(filename.toAscii().data()))
        {
            QMessageBox msgBox;
            msgBox.setText("The selected video could not be opened!");
            msgBox.exec();
        }
        if (Player->isStopped())
        {
            Player->play(false);
            ui->play_pushButton->setText(tr("Stop"));
        }
    }
}

void MainWindow::on_addPlayList_pushButton_clicked()
{
    QStringList filenames = QFileDialog::getOpenFileNames(this,
                                          tr("Open Video"), ".",
                                          tr("Video Files (*.avi *.mpg *.mp4)"));
    if (!filenames.isEmpty()){
        for (const auto& filename : filenames) {
            QStringList fileNameList = filename.split("/");
            if (source[fileNameList.last()].contains(filename))
                continue;
            source[fileNameList.last()] = filename;
            ui->listWidget->addItem(fileNameList.last());
        }
    }
}

void MainWindow::on_next_pushButton_clicked()
{
    if (!ui->actionCamera->isChecked())
    {
        if (Player->isRunning())
        {
            Player->Stop();
            ui->play_pushButton->setText(tr("Play"));
        }
        QThread::msleep(1000);
        if (currentVideo + 1 < ui->listWidget->count())
        {
            playVideo(currentVideo + 1);
            if (Player->isStopped())
            {
                Player->play(false);
                ui->play_pushButton->setText(tr("Stop"));
            }
        }
        else{
            QMessageBox::warning(this, "Error", "There is no next video!");
        }
    }
}

void MainWindow::on_prev_pushButton_clicked()
{
    if (!ui->actionCamera->isChecked())
    {
        if (Player->isRunning())
        {
            Player->Stop();
            ui->play_pushButton->setText(tr("Play"));
        }
        QThread::msleep(1000);
        if (currentVideo > 0)
        {
            playVideo(currentVideo - 1);
            if (Player->isStopped())
            {
                Player->play(false);
                ui->play_pushButton->setText(tr("Stop"));
            }
        }
        else{
            QMessageBox::warning(this, "Error", "There is no Previous video!");
        }
    }
}

void MainWindow::videoEnd()
{
    QImage img("./pics/end.jpg");
    ui->label->setAlignment(Qt::AlignCenter);
    ui->label->setPixmap(QPixmap::fromImage(img).scaled(ui->label->size()
                                       ,Qt::KeepAspectRatio, Qt::FastTransformation));
    Player->Stop();
    ui->play_pushButton->setText(tr("Play"));
    playVideo(ui->listWidget->currentRow());
}
