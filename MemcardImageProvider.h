#pragma once

#include <QQuickImageProvider>
#include <Application.h>

class ColorImageProvider : public QQuickImageProvider
{
public:
    ColorImageProvider(Application* app) : _app(app), QQuickImageProvider(QQuickImageProvider::Pixmap) {
    }

  /*  QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override {
        int width = 16;
        int height = 16;

        if (size){
           *size = QSize(width, height);
        }
        auto imageData = _app->getImage(0);
        QImage img(imageData.data(), 16, 16, QImage::Format_RGB888);
        return img;
    }*/

    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize) override{
       int width = 16;
       int height = 16;

       if (size){
          *size = QSize(width, height);
       }

       int fileId = 0;
       int imgId = 0;

        if(id.contains('/')){
            auto parts = id.split('/');
            if(parts.size() == 2){
                fileId = parts[0].toInt();
                imgId = parts[1].toInt();

            }
        }

       auto imageData = _app->getImage(fileId, imgId);
       QImage img(imageData.data(), 16, 16, QImage::Format_RGB888);
       return QPixmap::fromImage(img);
    }
private:
    Application* _app;
};
