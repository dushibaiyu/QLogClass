#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    qDebug() << "on_pushButton_clicked() this qDebug();";
    qWarning() << "on_pushButton_clicked() this is QWarnning";
    QLogDebug << "on_pushButton_clicked() this is a debug";
    QLogTest << "on_pushButton_clicked() this is a test";
    QLogWarning << "on_pushButton_clicked() this is a QLogWarning";
}
