#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QThread>
#include <QFileSystemWatcher>
#include <QFile>
#include <QFileInfo>
#include <QProcess>
#include <QStorageInfo>
#include <QTimer>
#include <QKeyEvent>
#include <QTextStream>
#include <QRegularExpression>

#include "details.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // Go to home view
    ui->stackedWidget->setCurrentIndex(0);

    processProg = new QProcess(this);
    process = new QProcess(this);
    imgSliderId = 0;
    scanDone = false;
    removeAction = false;

    // Detect any modification on report.log file
    watcher = new QFileSystemWatcher(this);
    connect(watcher, SIGNAL(fileChanged(const QString &)), this, SLOT(fileChanged(const QString &)));
    watcher->addPath("/opt/USBGuardian/logs/report.log");

    // switch every 5 seconds the image on the analysis view
    QTimer *timer = new QTimer(this);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(switchImgSlider()));
    timer->start(5000);
}

void MainWindow::switchImgSlider()
{
    // if you want to add more image just add them in this list
    QList<QString> sensiList;
    sensiList.append("sensi-1.png");
    sensiList.append("sensi-2.png");
    sensiList.append("sensi-3.png");

    if (imgSliderId < sensiList.length()) {
        ui->graphicsView_imgSlider->setStyleSheet("border-image: url(:/resources/assets/" + sensiList.at(imgSliderId) + ");");
        imgSliderId++;
    }
    else {
        imgSliderId = 0;
    }
}

// Read the file statistics.txt and show data in the Information view
void MainWindow::loadInformationView(const QString &path)
{
    process->start("clamscan", QStringList() << "--version");
    process->waitForFinished();
    ui->label_databaseVersion_value->setText(process->readAllStandardOutput());

    QString fileContent = readFile(path);
    QRegExp rx("([a-zA-Z% ]+): ([a-zA-Z0-9%. ]*)\\n");
    QMap<QString, QString> statistics;

    int pos = 0;
    while ((pos = rx.indexIn(fileContent, pos)) != -1) {
        statistics.insert(rx.cap(1), rx.cap(2));
        pos += rx.matchedLength();
    }

    ui->label_nbDevicesScanned_value->setText(statistics.value("Number of devices scanned"));
    ui->label_nbMalwaresDetected_value->setText(statistics.value("Number of malwares detected"));
    ui->label_nbFilesScanned_value->setText(statistics.value("Number of files scanned"));
    ui->label_percentInfectedDevices_value->setText(statistics.value("% of infected devices"));
    ui->label_averageTimeScan_value->setText(statistics.value("Average time of scan"));
}

void MainWindow::fileChanged(const QString &path)
{
    QFileInfo fileInfo(path);
    QString fileName = fileInfo.fileName();
    if (fileName == "report.log") {
        analyzeReport(path);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::readFile(const QString &path)
{
    QFile file(path);
    QString result;
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        while (!stream.atEnd()) {
            QString line = stream.readLine();
            result += line + "\n";
        }
        file.close();
    }
    return result;
}

void MainWindow::analyzeReport(const QString &path)
{
    QString fileContent = readFile(path);
    QRegExp rx("(USB stick format:[ ])([a-zA-Z0-9]*)");
    QMap<QString, QString> format;

    int pos = 0;
    // From the file report.log get the format of usb key
    while ((pos = rx.indexIn(fileContent, pos)) != -1) {
        format.insert("format", rx.cap(2));
        pos += rx.matchedLength();
    }
    // Format list supported by the solution
    QList<QString> formatList;
    formatList.append("VFAT");
    formatList.append("FAT32");
    formatList.append("FAT16");

    if (fileContent.contains("USB stick format: Unsupported format"))
    {
        // Go to error view and then after 5 seconds go back to home view
        ui->stackedWidget->setCurrentIndex(4);
        QTimer::singleShot(5000, this, SLOT(gotoHomeView()));
    } else if (fileContent.contains("SCAN SUMMARY") && fileContent.contains("End of analysis")) {
        // If the analysis is over go to report view
        loadReportView(path);
        scanDone = false;
    }
    else if ((formatList.contains(format.value("format")) && !removeAction && !fileContent.contains("End of analysis")) || (formatList.contains(format.value("format")) && !removeAction && !fileContent.contains("End of analysis")) )
    {
      // if no analysis was previously done for the current usb device start a new one
        scanDone = false;
        ui->progressBar->setValue(0);
        ui->label_analyze_view_title->setText("Scanning your USB drive");
        // Go to analysis view
        ui->stackedWidget->setCurrentIndex(1);
        // everytime the script progress print on stdout start the function processProgOutput()
        connect(processProg, SIGNAL(readyReadStandardOutput()), this, SLOT(processProgOutput()));
        processProg->start("/opt/USBGuardian/scripts/progress.sh", QStringList());
    }
}
// Show the view depending on the result of analysis OK or Malware found
void MainWindow::loadReportView(const QString &path)
{
    QString fileContent = readFile(path);
    QRegExp rx("([A-Z][a-zA-Z_ ]*):[ ]([a-zA-Z0-9.:() ]*)(\\n)*");
    QMap<QString, QString> map;
    // Separate each lines
    int pos = 0;
    while ((pos = rx.indexIn(fileContent, pos)) != -1) {
        map.insert(rx.cap(1), rx.cap(2));
        pos += rx.matchedLength();
    }

    QPixmap pixSuccess(":/resources/assets/icons/check.png");
    QPixmap pixFailure(":/resources/assets/icons/danger.png");
    int widthReport = ui->img_report_state->width() - 20;
    int heightReport = ui->img_report_state->height() - 20;

    QRegularExpression regexInfectedFiles("^0$");
    QRegularExpressionMatch matchExpression = regexInfectedFiles.match(map.value("Infected files"));
    // false if infected
    bool isMatch = matchExpression.hasMatch();

    QString removedPattern= "Removed";
    // If the button remove malware was pushed and the malware removed
    if(fileContent.contains(removedPattern)){
        ui->img_report_state->setPixmap(pixSuccess.scaled(widthReport, heightReport, Qt::KeepAspectRatio));
        ui->label_report_state_title->setText("Malware removed");
        ui->label_report_information_details->setText("You can now press the eject button to retrieve your USB Device. Remember to scan it often.");
        ui->btnFormat->hide();
        ui->btnRemove->hide();
        ui->btnShowInfectedFiles->show();
    } else if (isMatch) { // If no malware were found
        ui->img_report_state->setPixmap(pixSuccess.scaled(widthReport, heightReport, Qt::KeepAspectRatio));
        ui->label_report_state_title->setText("No malware detected");
        ui->label_report_information_details->setText("You can now press the eject button to retrieve your USB Device. Remember to scan it often.");
        ui->btnFormat->hide();
        ui->btnRemove->hide();
        ui->btnShowInfectedFiles->hide();
    } else {
        // else a malware has been found
        ui->img_report_state->setPixmap(pixFailure.scaled(widthReport, heightReport, Qt::KeepAspectRatio));
        ui->label_report_state_title->setText("WARNING: " + map.value("Infected files") + " malware found");
        ui->btnFormat->show();
        ui->btnRemove->show();
        ui->btnShowInfectedFiles->show();
        ui->label_report_information_details->setText("We strongly advise you to remove malwares or format your USB Device. If any doubt please contact your security expert");
    }

    ui->label_report_time->setText("Total time: " + map.value("Time"));
    ui->label_report_files->setText("Files analyzed: " + map.value("Scanned files"));
    // Show report view
    ui->stackedWidget->setCurrentIndex(3);
}

void MainWindow::processProgOutput(){
    QString fileOkPercentage = processProg->readAllStandardOutput();
    if (fileOkPercentage.contains("USB Removed") || fileOkPercentage.contains("No such file or directory")){
        runGuardianShellScript("ejectUSB");
        // show error view saying that usb was removed during clamav scan
        exceptionMessageUsbRemoved();
        return;
    }
    // Clear the data, output format sample: 85%\n ==> chop(3) => 85
    fileOkPercentage.chop(3);
    if(fileOkPercentage != ""){
      // Set the percentage value to the data cleared
        ui->progressBar->setValue(fileOkPercentage.toInt());
        if (ui->progressBar->value() >= 99 && scanDone == false){
            scanDone = true;
            processProg->close();
        }
    }
}

void MainWindow::on_btn_close_statisticsView_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_btn_information_clicked()
{
    loadInformationView("/opt/USBGuardian/statistics.txt");
    ui->stackedWidget->setCurrentIndex(5);
}

void MainWindow::on_btn_restart_welcomeView_clicked()
{
    confirmDeviceRestart();
}

void MainWindow::on_btn_restart_analyzeView_clicked()
{
    confirmDeviceRestart();
}

void MainWindow::on_btn_restart_ejectView_clicked()
{
    confirmDeviceRestart();
}

void MainWindow::on_btn_restart_reportView_clicked()
{
    confirmDeviceRestart();
}

void MainWindow::on_btn_restart_exceptionView_clicked()
{
    confirmDeviceRestart();
}

void MainWindow::confirmDeviceRestart()
{
    QMessageBox messageBox;
    messageBox.setText("Are you sure you want to restart the device?");
    messageBox.setIcon(QMessageBox::Warning);
    messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);

    int exec = messageBox.exec();
    switch (exec) {
    case QMessageBox::Yes:
        runGuardianShellScript("reboot");
        break;
    case QMessageBox::Cancel:
        break;
    default:
        break;
    }
}

void MainWindow::on_btnRemove_clicked()
{
    QMessageBox messageBox;
    messageBox.setText("Are you sure you want to remove malwares? Infected files will be erased");
    messageBox.setIcon(QMessageBox::Warning);
    messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);

    int exec = messageBox.exec();
    switch (exec) {
    case QMessageBox::Yes:
        removeVirus();
        break;
    case QMessageBox::Cancel:
        break;
    default:
        break;
    }
}

void MainWindow::removeVirus(){
    ui->progressBar->setValue(0);
    ui->stackedWidget->setCurrentIndex(1);
    ui->label_analyze_view_title->setText("Removing malwares");
    scanDone = false;
    removeAction = true;
    QProcess pScan;
    pScan.startDetached("python3 /opt/USBGuardian/scripts/scanAndRemove.py", QStringList());
    processProg->start("/opt/USBGuardian/scripts/progress.sh", QStringList());
}

void MainWindow::on_btnFormat_clicked()
{
    QMessageBox messageBox;
    messageBox.setWindowTitle("Format the USB Drive?");
    messageBox.setText("Are you sure you want to format the USB drive ? All the data will be erased");
    messageBox.setIcon(QMessageBox::Warning);
    messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);

    int exec = messageBox.exec();
    switch (exec) {
    case QMessageBox::Yes:
        formatUSB();
        break;
    case QMessageBox::Cancel:
        break;
    default:
        break;
    }
}

void MainWindow::formatUSB(){
    QProcess pFormat;
    pFormat.start("python3 /opt/USBGuardian/scripts/formatUSB.py", QStringList());
    pFormat.waitForFinished();
    QMessageBox messageBox;
    messageBox.setText("Your USB drive have been successfully formatted.");
    messageBox.setIcon(QMessageBox::Information);
    messageBox.setStandardButtons(QMessageBox::Ok);
    int exec = messageBox.exec();
    switch (exec) {
    case QMessageBox::Ok:
        break;
    default:
        break;
    }
}

void MainWindow::runGuardianShellScript(const QString &scriptName)
{
    QProcess process;
    process.start("/opt/USBGuardian/scripts/" + scriptName + ".sh", QStringList());
    process.waitForFinished();
}

void MainWindow::on_btnShowInfectedFiles_clicked()
{
    QString report = readFile("/opt/USBGuardian/logs/report.log");
    details = new Details(report, this);
    details->show();
}

void MainWindow::ejectUSB(){
    runGuardianShellScript("ejectUSB");
    ui->stackedWidget->setCurrentIndex(2);
    QTimer::singleShot(5000, this, SLOT(gotoHomeView() ));
}

void MainWindow::on_btnEject_clicked()
{
    ejectUSB();
}

void MainWindow::gotoHomeView(){
    scanDone = false;
    removeAction = false;
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_btn_cancel_analyze_clicked()
{
    ejectUSB();
}

void MainWindow::exceptionMessageUsbRemoved()
{
    ui->label_exception_title->setText("USB drive has been removed. Scan has been canceled.");
    ui->stackedWidget->setCurrentIndex(4);
}

void MainWindow::on_btn_close_exceptionView_clicked()
{
    gotoHomeView();
}
