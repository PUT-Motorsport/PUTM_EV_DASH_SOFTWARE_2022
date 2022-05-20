#include "canraw.h"
#include "ui_canraw.h"

CanRaw::CanRaw(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CanRaw)
{
    ui->setupUi(this);
    refreshTimer = new QTimer();
    QObject::connect(refreshTimer, &QTimer::timeout, this, &CanRaw::sniff);
}

CanRaw::~CanRaw()
{
    delete ui;
    delete refreshTimer;
}

void CanRaw::navigate([[maybe_unused]]buttonStates navigation)
{
    refreshTimer->stop();
    this->done(QDialog::Accepted);
}

void CanRaw::raiseError(QString const &errorMessage)
{
    ui->error->setText("Error: " + errorMessage);
    QTimer::singleShot(3000, [this] () {
            ui->error->setText("");
    });
}

void CanRaw::startSniffing()
{
    refreshTimer->start(refreshTime);
}

void CanRaw::sniff()
{
    if (currentLines > maxLines) {
        ui->textLogs->clear();
        currentLines = 0;
    }

    if (newLine == logger.canLine)
        return;

    newLine = logger.canLine;      //FIXME: Probably not the cleanest way to do it
    ui->textLogs->append(newLine);
    currentLines++;
}


