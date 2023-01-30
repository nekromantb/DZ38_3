#include <QApplication>
#include <QtGui/QtGui>
#include <QPushButton>
#include <QVBoxLayout>
#include <QSlider>
#include <QPixmap>
#include <QLabel>
#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsBlurEffect>
#include <QGraphicsPixmapItem>
#include <QFileDialog>

QImage blurImage (QImage source, qreal radius)
{
    if (source.isNull()) return QImage();
    QGraphicsScene scene;
    QGraphicsPixmapItem item;
    item.setPixmap(QPixmap::fromImage(source));

    auto *blur =  new QGraphicsBlurEffect;
    blur->setBlurRadius(radius);
    item.setGraphicsEffect(blur);
    scene.addItem(&item);
    QImage result(source.size(), QImage::Format_ARGB32);
    result.fill(Qt::transparent);
    QPainter painter(&result);
    scene.render(&painter, QRectF(), QRectF(0,0, source.width(), source.height()));
    return result;
}

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    auto *window = new QWidget;
    auto *box = new QVBoxLayout(window);
    auto *image = new QLabel;
    auto *slider = new QSlider(Qt::Horizontal);
    auto *openButton = new QPushButton("Open");

    QImage source_image;

    window->setMinimumSize(300, 300);

    slider->setMaximumHeight(30);
    openButton->setMaximumHeight(30);

    slider->setMaximum(10);
    slider->setMinimum(0);

    box->addWidget(image);
    box->addWidget(slider);
    box->addWidget(openButton);

    QObject::connect(openButton, &QPushButton::clicked, [&source_image, image](){
        auto file_path = QFileDialog::getOpenFileName(nullptr,
                                     "Open png Image",
                                     "G:\\work\\Prog\\progs\\DZ38_3",
                                     "png files (*.png)");
        source_image = QImage(file_path);
        image->setPixmap(QPixmap::fromImage(source_image).scaled(
                image->width(),
                image->height(),
                Qt::KeepAspectRatio
                ));
    });

    QObject::connect(slider, &QSlider::sliderMoved, [&source_image, slider, image](int newValue){
        image->setPixmap(QPixmap::fromImage(blurImage(source_image, qreal(newValue)).scaled(
                image->width(),
                image->height(),
                Qt::KeepAspectRatio
                )));
    });



    window->show();
    return QApplication::exec();
}
