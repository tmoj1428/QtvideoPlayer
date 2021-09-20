#include <QDebug>
#include <QMessageBox>

#include "videoplayer.h"

videoPlayer::videoPlayer(QObject *parent) : QThread(parent)
{
    stop = true;
}

bool videoPlayer::loadVideo(string file)
{
    source = QString::fromStdString(file);

    capture.open(file);
    if (capture.isOpened())
    {
        frameRate = (int) capture.get(CAP_PROP_FPS);
        return true;
    }
    else
        return false;
}

void videoPlayer::play(bool type)
{
    if (type)
    {
        int deviceID = 0;             // 0 = open default camera
        int apiID = cv::CAP_ANY;      // 0 = autodetect default API
        // open selected camera using selected API
        capture.open(deviceID, apiID);
        // check if we succeeded
        if (!capture.isOpened()) {
            qDebug() << "ERROR! Unable to open camera\n";
            return;
        }
        source = "";
    }else{
        if (source.isEmpty())
        {
            return;
        }
    }


    if (!isRunning()) {
        if (isStopped()){
            stop = false;
        }
        start(LowPriority);
    }
}

void videoPlayer::run()
{
    int delay = (1000/frameRate);
    while(!stop){
        if (!capture.read(frame))
        {
            emit videoEnded();
            stop = true;
        }else {
            if (frame.channels() == 3){
                cv::cvtColor(frame, RGBframe, COLOR_BGR2RGB);
                img = QImage((const unsigned char*)(RGBframe.data),
                                  RGBframe.cols,RGBframe.rows,QImage::Format_RGB888);
            }
            else
            {
                img = QImage((const unsigned char*)(frame.data),
                                     frame.cols,frame.rows,QImage::Format_Indexed8);
            }
            emit processedImage(img);
            this->msleep(delay);
        }
    }
}


videoPlayer::~videoPlayer()
{
    mutex.lock();
    stop = true;
    capture.release();
    condition.wakeOne();
    mutex.unlock();
    wait();
}
void videoPlayer::Stop()
{
    stop = true;
}
void videoPlayer::msleep(int ms){
    struct timespec ts = { ms / 1000, (ms % 1000) * 1000 * 1000 };
    nanosleep(&ts, NULL);
}
bool videoPlayer::isStopped() const{
    return this->stop;
}
