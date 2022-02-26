#include "logs.h"
#include "ui_logs.h"

Logs::Logs(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Logs)
{
    ui->setupUi(this);
    refreshTimer = new QTimer();
    QObject::connect(refreshTimer, &QTimer::timeout, this, &Logs::sniff);
}

Logs::~Logs()
{
    delete ui;
    delete refreshTimer;
}

void Logs::navigate(Navigation pressed)
{
    refreshTimer->stop();
    this->done(QDialog::Accepted);
}

void Logs::raiseError(const QString &errorMessage, int errorCode)
{
    ui->error->setText("Error " + QString::number(errorCode) + ": " + errorMessage);
    QTimer::singleShot(3000, [this] () {
            ui->error->setText("");
    });
}

void Logs::startSniffing()
{
    refreshTimer->start(refreshTime);
}

void Logs::sniff()
{
    if (currentLines > maxLines) {
        currentLines = 0;
        ui->textLogs->setText("");
    }
    if (newLine == Logger::logLine)
        return;
    newLine = Logger::logLine;
    ui->textLogs->append(newLine);
    currentLines++;
}
