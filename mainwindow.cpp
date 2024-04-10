#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <sys/types.h>
#include <sys/resource.h>
#include <signal.h>
#include <QMessageBox>
#include <QProcess>
#include <sched.h>
#include <unistd.h>
#include <QSortFilterProxyModel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    tableView = findChild<QTableView*>("processList");
    model = new QStandardItemModel(this);
    model->setColumnCount(6);
    model->setHorizontalHeaderLabels({"Nome do processo", "% CPU", "PID", "PPID", "Prioridade", "Status"});

    tableView->setModel(model);
    tableView->setColumnWidth(0,170);

    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()), this, SLOT(updateProcessList()));
    timer->start(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_killerBtn_clicked()
{
    bool ok;
    pid_t pid = static_cast<pid_t>(ui->PID->text().toInt(&ok));

    if(ok){
        int response = kill(pid,SIGKILL);

        if(response == 0){
            QMessageBox::information(nullptr, "Aviso", "Processo morto!!!");
        }else {
            QMessageBox::information(nullptr, "Aviso", "Erro ao matar processo!!!");

        }
    } else {
        QMessageBox::information(nullptr, "Aviso", "Erro na conversão do PID!!!");
    }
}


void MainWindow::on_stopBtn_clicked()
{
    bool ok;
    pid_t pid = static_cast<pid_t>(ui->PID->text().toInt(&ok));

    if(ok){
        int response = kill(pid,SIGSTOP);

        if(response == 0){
            QMessageBox::information(nullptr, "Aviso", "Processo pausado!!!");
        }else {
            QMessageBox::information(nullptr, "Aviso", "Erro ao pausar processo!!!");

        }
    } else {
        QMessageBox::information(nullptr, "Aviso", "Erro na conversão do PID!!!");
    }
}


void MainWindow::on_contBtn_clicked()
{
    bool ok;
    pid_t pid = static_cast<pid_t>(ui->PID->text().toInt(&ok));

    if(ok){
        int response = kill(pid,SIGCONT);

        if(response == 0){
            QMessageBox::information(nullptr, "Aviso", "Processo retomado!!!");
        }else {
            QMessageBox::information(nullptr, "Aviso", "Erro ao continuar processo!!!");

        }
    } else {
        QMessageBox::information(nullptr, "Aviso", "Erro na conversão do PID!!!");
    }
}


void MainWindow::on_priorityBtn_clicked()
{
    bool ok1, ok2;
    pid_t pid = static_cast<pid_t>(ui->PID->text().toInt(&ok1));
    int p =  static_cast<pid_t>(ui->priorityInput->text().toInt(&ok2));

    if(ok1 && ok2){
        int response = setpriority(PRIO_PROCESS,pid,p);

        if(response == 0){
            QMessageBox::information(nullptr, "Aviso", "Prioridade do processo alterada!!!");
        }else {
            QMessageBox::information(nullptr, "Aviso", "Erro ao tentar alterar a prioridade!!!");

        }
    } else {
        QMessageBox::information(nullptr, "Aviso", "Erro na conversão do PID!!!");
    }
}


void MainWindow::on_cpuBtn_clicked()
{
    bool ok, ok1;
    pid_t pid = static_cast<pid_t>(ui->PID->text().toInt(&ok));
    cpu_set_t cpu;
    CPU_ZERO(&cpu);
    CPU_SET(static_cast<int>(ui->cpuInput->text().toInt(&ok1)), &cpu);

    int response = sched_setaffinity(pid, sizeof(cpu_set_t), &cpu);

    if(response == 0){
        QMessageBox::information(nullptr, "Aviso", "Processo alocado para a(s) CPU(s) " + ui->cpuInput->text());
    } else {
        QMessageBox::information(nullptr, "Aviso", "Erro na alocação de CPU " + response);
    }
}

void MainWindow::updateProcessList()
{
    model->clear();
    model->setHorizontalHeaderLabels({"Nome do processo", "% CPU", "PID", "PPID", "Prioridade", "Status"});
    tableView->setColumnWidth(0,170);

    PROCTAB* proc = openproc(PROC_FILLSTAT);
    proc_t* processInfo;
    while((processInfo = readproc(proc, nullptr)) != nullptr)
    {
        QList<QStandardItem*> rowItems;
        rowItems.append(new QStandardItem(QString::fromUtf8(processInfo->cmd)));
        rowItems.append(new QStandardItem(QString::number(processInfo->pcpu)));
        rowItems.append(new QStandardItem(QString::number(processInfo->tid)));
        rowItems.append(new QStandardItem(QString::number(processInfo->ppid)));
        rowItems.append(new QStandardItem(QString::number(processInfo->priority)));
        rowItems.append(new QStandardItem(QString(processInfo->state)));
        if(QString::fromUtf8(processInfo->cmd).startsWith(ui->filterInput->text(), Qt::CaseInsensitive))
        {
            model->appendRow(rowItems);
        }
        freeproc(processInfo);
    }
    closeproc(proc);
}


//void MainWindow::on_filterBtn_clicked()
//{
    //QString filt = ui->filterInput->text();

    //proxyModel->setFilterRegExp(filt);
//}

