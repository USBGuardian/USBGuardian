#include "details.h"
#include "ui_details.h"
#include <QDebug>

Details::Details(const QString &report, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Details)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);

    ui->buttonBox->clear();
    ui->buttonBox->addButton(QDialogButtonBox::Close);

    QStringList virusesList = report.split('\n', QString::SkipEmptyParts);

    foreach (QString virus, virusesList) {
        if (virus.contains("FOUND") || virus.contains("Removed")) {
            QListWidgetItem *item = new QListWidgetItem(virus.replace("FOUND", "INFECTED"));
            ui->listDetails->addItem(item);
        }
    }
}

Details::~Details()
{
    delete ui;
}
