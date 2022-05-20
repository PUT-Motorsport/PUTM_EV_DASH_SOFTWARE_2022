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

void Logs::navigate([[maybe_unused]]buttonStates navigation)
{
    refreshTimer->stop();
    this->done(QDialog::Accepted);
}

void Logs::raiseError(const QString &errorMessage)
{
    ui->error->setText("Error: " + errorMessage);
    QTimer::singleShot(3000, [this] () {
            ui->error->setText("");
    });
}

void Logs::startSniffing()
{
    logger.forceFlush();
    QFile log("log.txt");
    if (not(log.open(QIODevice::ReadOnly | QIODevice::Text)))
            qDebug() << "Failed to open log as read only";
    qDebug() << "Lines: " << logger.logLinesCount;
    if ((logger.logLinesCount - 10) > 0)
        log.seek(logger.logLinesCount - 10);
    while (not(log.atEnd())) {
        qDebug() << "Appended";
        ui->textLogs->append(log.readLine().split('\n').at(0));
    }
    refreshTimer->start(refreshTime);
}

void Logs::sniff()
{
    if (currentLines > maxLines) {
        currentLines = 0;
        ui->textLogs->setText("");
    }
    if (newLine == logger.logLine)
        return;
    newLine = logger.logLine;
    ui->textLogs->append(newLine);
    currentLines++;
}
