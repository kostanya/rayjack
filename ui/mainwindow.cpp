#include "mainwindow.h"
#include "../build/ui_mainwindow.h"

#include <rayjack/camera.h>
#include <rayjack/hittable_list.h>
#include <rayjack/material.h>
#include <rayjack/sphere.h>

#include <QPixmap>
#include <QThread>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Connect the renderClicked() slot to the button click
    connect(ui->renderButton, &QPushButton::clicked, this, &MainWindow::renderClicked);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::renderClicked(){
    QString message = "Rendering image...";
    ui->statusbar->showMessage(message);
    QApplication::processEvents(); // UI updates
    QThread::msleep(100); // Wait for 100 milliseconds

    HittableList world = createWorld();

    int imgHeight = ui->line_imageHeight->text().toInt();
    int imgWidth = ui->line_imageWidth->text().toInt();
    int samplePerPixel = ui->line_samplePerPixel->text().toInt();
    int maxRayBounce = ui->line_maxRayBounce->text().toInt();
    float verticalFieldOfView = ui->line_verticalField->text().toFloat();
    float lookFrom_x = ui->line_lookFrom_x->text().toFloat();
    float lookFrom_y = ui->line_lookFrom_y->text().toFloat();
    float lookFrom_z = ui->line_lookFrom_z->text().toFloat();
    float lookAt_x = ui->line_lookAt_x->text().toFloat();
    float lookAt_y = ui->line_lookAt_y->text().toFloat();
    float lookAt_z = ui->line_lookAt_z->text().toFloat();
    float viewUp_x = ui->line_viewUp_x->text().toFloat();
    float viewUp_y = ui->line_viewUp_y->text().toFloat();
    float viewUp_z = ui->line_viewUp_z->text().toFloat();
    float defocusAngle = ui->line_defocusAngle->text().toFloat();
    float focusDistance = ui->line_focusDist->text().toFloat();
    uint32_t numOfThreads = ui->line_numThreads->text().toUInt();

    Camera cam;

    cam.imageHeight = imgHeight;
    cam.imageWidth = imgWidth;
    cam.samplesPerPixel = samplePerPixel;
    cam.maxRayBounce = maxRayBounce;
    cam.verticalFieldOfView = verticalFieldOfView;
    cam.lookFrom = point3(lookFrom_x, lookFrom_y, lookFrom_z);
    cam.lookAt = point3(lookAt_x, lookAt_y, lookAt_z);
    cam.viewUp = vec3(viewUp_x, viewUp_y, viewUp_z);
    cam.defocusAngle = defocusAngle;
    cam.focusDist = focusDistance;
    cam.numThreads = numOfThreads;

    float renderTime=cam.render(world);
    message = "Render completed in " + QString::number(renderTime) + " seconds";
    ui->statusbar->showMessage(message);
    QPixmap pix("render_result_thread_deneme.png");
    if (!pix.isNull()) {
        ui->image->setPixmap(pix);
    } else {
        message = "Image not loaded";
        ui->statusbar->showMessage(message);
    }

}

HittableList MainWindow::createWorld() {
    HittableList world;
    
    auto ground_material = std::make_shared<Lambertian>(color(0.5, 0.5, 0.5));
    world.add(std::make_shared<Sphere>(point3(0, -1000, 0), 1000, ground_material));

    for (int a = -1; a < 1; a++) {
        for (int b = -1; b < 1; b++) {
            auto choose_mat = randomFloat();
            point3 center(a + 0.9 * randomFloat(), 0.2, b + 0.9 * randomFloat());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                std::shared_ptr<Material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = random3() * random3();
                    sphere_material = std::make_shared<Lambertian>(albedo);
                    world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = random3(0.5, 1);
                    auto fuzz = randomFloat(0, 0.5);
                    sphere_material = std::make_shared<Metal>(albedo, fuzz);
                    world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = std::make_shared<Dielectric>(1.5);
                    world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = std::make_shared<Dielectric>(1.5);
    world.add(std::make_shared<Sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = std::make_shared<Lambertian>(color(0.4, 0.2, 0.1));
    world.add(std::make_shared<Sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = std::make_shared<Metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(std::make_shared<Sphere>(point3(4, 1, 0), 1.0, material3));
    
    return world;

}

