#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QMessageBox>
#include <QFileSystemWatcher>
#include <QProcess>
#include "details.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void fileChanged(const QString &path);
    QString readFile(const QString &path);
	void gotoHomeView();
    void ejectUSB();
    void removeVirus();
    void formatUSB();
    void processProgOutput();
    void on_btn_close_statisticsView_clicked();
    void on_btn_information_clicked();
    void on_btn_restart_welcomeView_clicked();
    void on_btn_restart_analyzeView_clicked();
    void on_btn_restart_ejectView_clicked();
    void on_btn_restart_reportView_clicked();
    void on_btn_restart_exceptionView_clicked();
    void on_btnEject_clicked();
    void on_btnRemove_clicked();
    void on_btnFormat_clicked();
    void on_btnShowInfectedFiles_clicked();
    void switchImgSlider();
    void on_btn_cancel_analyze_clicked();
    void exceptionMessageUsbRemoved();
	
    void on_btn_close_exceptionView_clicked();

private:
    Ui::MainWindow *ui;
    QFileSystemWatcher * watcher;
    QProcess *processProg;
    void loadInformationView(const QString &path);
    void loadReportView(const QString &path);
    void analyzeReport(const QString &path);
    void confirmDeviceRestart();
    void runGuardianShellScript(const QString &scriptName);
    QProcess *process;
    Details *details;
    int imgSliderId;
    bool removeAction;
    bool scanDone;
};

#endif // MAINWINDOW_H
