#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <rayjack/hittable_list.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void renderClicked();
   
private:
    Ui::MainWindow *ui;
    HittableList createWorld();
};
#endif // MAINWINDOW_H
