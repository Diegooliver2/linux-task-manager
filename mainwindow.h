#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QTableView>
#include <QTimer>
#include <proc/readproc.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_killerBtn_clicked();

    void on_stopBtn_clicked();

    void on_contBtn_clicked();

    void on_priorityBtn_clicked();

    void on_cpuBtn_clicked();

    //void on_filterBtn_clicked();

    void updateProcessList();

private:
    Ui::MainWindow *ui;
    QStandardItemModel *model;
    QTableView* tableView;
    QTimer* timer;
};
#endif // MAINWINDOW_H
