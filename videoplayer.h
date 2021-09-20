#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QString>
#include <QThread>
#include <QMutex>
#include <QImage>
#include <QWaitCondition>
#include <opencv2/core/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

class videoPlayer : public QThread
{
    Q_OBJECT

public:
    videoPlayer(QObject *parent = 0);
    ~videoPlayer();
    bool loadVideo(string fileName);
    void play(bool type);
    bool isStopped() const;
    void Stop();
    QString             source;

private:
    cv::VideoCapture    capture;
    int                 frameRate;
    bool                stop;
    QMutex              mutex;
    QWaitCondition      condition;
    Mat                 frame;
    Mat                 RGBframe;
    QImage              img;

signals:
    void processedImage(const QImage &image);
    void videoEnded();

protected:
    void run();
    void msleep(int ms);
};

#endif // VIDEOPLAYER_H
