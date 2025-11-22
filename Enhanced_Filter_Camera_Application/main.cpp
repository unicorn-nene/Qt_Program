    #include <QApplication>
#include <QMainWindow>
#include <QLabel>
#include <QVBoxLayout>
#include <QMediaDevices>
#include <QCameraDevice>            // 负责采集摄像头画面
#include <QMediaCaptureSession>     // 负责将不同每条对象连接到 Session
#include <QVideoSink>               // 接受视频数据并输出画面
#include <QCamera>
#include <QPushButton>
#include <QCheckBox>
#include <QImageCapture>
#include <QFileDialog>

// Utility function
QImage toGrayScale(const QImage &image)
{
    return image.convertToFormat(image.Format_Grayscale8);
}

int main(int argc, char *argv[])
{

    // >>>>>>>>>>>>> UI Setup Begin >>>>>>>>>>>
    QApplication a (argc, argv);

    QMainWindow window{};

    QWidget *centralWidget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    QLabel *videoLabel = new QLabel("Camera Feed");
    videoLabel->setMaximumSize(720, 1080);
    videoLabel->setStyleSheet("background-color: black");
    videoLabel->setAlignment(Qt::AlignVCenter);
    layout->addWidget(videoLabel);

    QCheckBox *bwCheckBox = new QCheckBox("Black and white Mode");
    layout->addWidget(bwCheckBox);

    QPushButton *takePictureButton = new QPushButton("Take Picture");
    layout->addWidget(takePictureButton);
    // <<<<<<<<<<<<< UI Setup End <<<<<<<<<<<<<<


    // >>>>>>>>>>>>> CameraMedia Setup Begin >>>>>>>>>>>
    QCameraDevice cameraDevice = QMediaDevices::defaultVideoInput();
    QCamera *camera = new QCamera(cameraDevice);

    QMediaCaptureSession * session = new QMediaCaptureSession();
    QVideoSink *videoSink = new QVideoSink();
    QImageCapture *imageCapture = new QImageCapture();

    session->setCamera(camera);
    session->setVideoSink(videoSink);
    session->setImageCapture(imageCapture);

    QImage currentFrame;

    QObject::connect(videoSink, &QVideoSink::videoFrameChanged,[&](const QVideoFrame &frame)
                     {
                         if(!frame.isValid())
                             return;

                         QVideoFrame cloneFrame(frame);
                         cloneFrame.map(QVideoFrame::ReadOnly);
                         QImage image = cloneFrame.toImage();
                         cloneFrame.unmap();

                         // 顺时针旋转 90度
                         QTransform transform;
                         transform.rotate(90);
                         image = image.transformed(transform);

                         currentFrame = bwCheckBox->isChecked() ? toGrayScale(image) : image;
                         videoLabel->setPixmap(QPixmap::fromImage(currentFrame).scaled(videoLabel->size(),
                                                                                       Qt::KeepAspectRatio,
                                                                                       Qt::SmoothTransformation));
                     });
    // <<<<<<<<<<<<< CameraMedia Setup End <<<<<<<<<<<<<<


    // >>>>>>>>>>>>> Screen shot save Begin >>>>>>>>>>>
    QObject::connect(takePictureButton, &QPushButton::clicked, [&]()
                     {
                         if(currentFrame.isNull())
                             return;

                         // default dir
                         QString defaultPath = "E:/我的图片/picture/image.png";

                         // save dialog
                         // QString path = QFileDialog::getSaveFilename(nullptr, "Save Image", "Images (*.png *.jpg)");
                         QString path = QFileDialog::getSaveFileName(nullptr,
                                                                     "Save Image",
                                                                     defaultPath,
                                                                     "PNG Image (*.png);;JPEG Image (*.jpg *jpeg)");

                         if(!path.isEmpty())
                         {
                             QFileInfo info(path);
                             if(info.suffix().isEmpty())
                                 path += ".png";
                             currentFrame.save(path);
                         }
                     });

    // <<<<<<<<<<<<< Screen shot save End <<<<<<<<<<<<<<

    camera->start();

    window.setCentralWidget(centralWidget);
    window.setWindowTitle("Camera App");
    window.resize(720, 1430);
    window.show();

    return a.exec();
}
