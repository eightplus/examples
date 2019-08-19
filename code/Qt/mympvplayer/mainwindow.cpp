#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileInfo>
#include <QApplication>
#include <QAction>
#include <QMimeData>
#include <QDesktopWidget>
#include <QDir>
#include <QFileDialog>
#include <QtCore/qmath.h>
#include <QDebug>
#include <QDragEnterEvent>
#include <QDropEvent>

#include "core.h"
#include "bottomwidget.h"
#include "videowindow.h"
#include "utils.h"
#include "preferences.h"
#include "global.h"

using namespace Global;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setFocusPolicy(Qt::ClickFocus);
    this->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    this->resize(900, 600);
    this->setWindowTitle(tr("MyMpvPlayer"));
    this->setAcceptDrops(true);

    global_init();

    this->createMenu();
    this->createVideoWindow();
    this->createCore();
    this->createBottomToolBar();

    m_centralLayout = new QVBoxLayout(ui->centralWidget);
    m_centralLayout->setContentsMargins(0, 0, 0, 0);
    m_centralLayout->setMargin(1);
    m_centralLayout->setSpacing(0);
    m_centralLayout->addWidget(m_playerWindow);
    m_centralLayout->addWidget(m_bottomToolbar);

    ui->centralWidget->setFocusPolicy(Qt::StrongFocus);
    ui->centralWidget->setFocus();

    this->move((QApplication::desktop()->screenGeometry(0).width() - this->width()) / 2, (QApplication::desktop()->screenGeometry(0).height() - this->height()) / 2);
}

MainWindow::~MainWindow()
{
    global_end();

    if (m_core) {
        delete m_core;
        m_core = nullptr;
    }
    if (m_mainMenu) {
        delete m_mainMenu;
        m_mainMenu = nullptr;
    }
    if (m_playerWindow) {
        delete m_playerWindow;
        m_playerWindow = nullptr;
    }
    if (m_bottomToolbar) {
        delete m_bottomToolbar;
        m_bottomToolbar = nullptr;
    }
    if (m_centralLayout) {
        delete m_centralLayout;
        m_centralLayout = nullptr;
    }
    delete ui;
}

void MainWindow::createMenu()
{
    m_mainMenu = new QMenu(this);
    m_openAct = new QAction(this);
    m_openAct->setShortcut(QKeySequence("Ctrl+F"));
    m_openAct->setText(tr("Open &File..."));
    connect(m_openAct, SIGNAL(triggered()), this, SLOT(openFile()));
    m_mainMenu->addAction(m_openAct);
}

void MainWindow::createVideoWindow()
{
    m_playerWindow = new VideoWindow(ui->centralWidget);
    m_playerWindow->setContentsMargins(0, 0, 0, 0);
    connect(m_playerWindow, &VideoWindow::rightClicked, this, [=] () {
        m_mainMenu->move(QCursor::pos());
        m_mainMenu->show();
    });
}

void MainWindow::createCore()
{
    m_core = new Core(m_playerWindow, this);
    connect(m_core, SIGNAL(widgetsNeedUpdate()), this, SLOT(updateWidgets()));
    connect(m_core, &Core::showFrame, this, [=] (int frame) {
        qDebug() << "Frame changed: " << frame;
    });

    connect(m_core, SIGNAL(showTime(double, bool)), this, SLOT(gotCurrentTime(double, bool)));
    connect(m_core, SIGNAL(showMessage(QString)), this, SLOT(displayMessage(QString)));
    connect(m_core, SIGNAL(stateChanged(Core::State)), this, SLOT(displayState(Core::State)));
    connect(m_core, SIGNAL(stateChanged(Core::State)), this, SLOT(togglePlayAction(Core::State)));
    connect(m_core, SIGNAL(mediaStartPlay()), this, SLOT(newMediaLoaded()), Qt::QueuedConnection);
    connect(m_playerWindow, SIGNAL(wheelUp()), m_core, SLOT(wheelUp()));
    connect(m_playerWindow, SIGNAL(wheelDown()), m_core, SLOT(wheelDown()));
}

void MainWindow::newMediaLoaded()
{
    QFileInfo fi(m_core->mdat.m_filename);
    if (fi.exists()) {
        QString name = fi.fileName();
        this->setWindowTitle(name);
    }
}

void MainWindow::displayMessage(QString message) {
    qDebug() << message;
}

void MainWindow::displayState(Core::State state)
{
    switch (state) {
        case Core::Playing:
            break;
        case Core::Paused:
            break;
        case Core::Stopped:
            this->setWindowTitle("");
            break;
    }
}

void MainWindow::createBottomToolBar()
{
    m_bottomToolbar = new BottomWidget(this);
    m_bottomToolbar->setFixedHeight(79);
    m_bottomToolbar->setLayoutDirection(Qt::LeftToRight);
    connect(m_bottomToolbar, SIGNAL(requestVolumeChanged(int)), m_core, SLOT(setVolume(int)));
    connect(m_core, SIGNAL(volumeChanged(int)), m_bottomToolbar, SIGNAL(valueChanged(int)));
    connect(m_bottomToolbar, &BottomWidget::toggleStop, this, [=] {
        m_core->stop();
    });
    connect(m_bottomToolbar, &BottomWidget::togglePlayPause, this, &MainWindow::onPlayPause);
    connect(m_bottomToolbar, SIGNAL(toggleMute()), this, SLOT(onMute()));
    connect(m_bottomToolbar, SIGNAL(posChanged(int)), m_core, SLOT(goToPosition(int)));
    connect(m_core, SIGNAL(positionChanged(int)), m_bottomToolbar, SLOT(setPos(int)));
    connect(m_bottomToolbar, SIGNAL(wheelUp()), m_core, SLOT(wheelUp()));
    connect(m_bottomToolbar, SIGNAL(wheelDown()), m_core, SLOT(wheelDown()));
    connect(m_core, SIGNAL(newDuration(double)), m_bottomToolbar, SLOT(setDuration(double)));
}

void MainWindow::onPlayPause()
{
    m_core->playOrPause(0);
}

void MainWindow::onMute()
{
    bool muted = pref->global_volume ? pref->mute : m_core->mset.mute;
    m_core->mute(!muted);
    if (muted && pref->volume <= 0) {
        m_core->setVolume(50, true);
    }

    m_bottomToolbar->onMutedChanged(muted, pref->volume);
}

void MainWindow::onCloseWindow()
{
    if (m_core->state() != Core::Stopped) {
        m_core->stop();
    }

    qApp->quit();
}

void MainWindow::togglePlayAction(Core::State state)
{
    if (state == Core::Playing) {
        m_bottomToolbar->onMusicPlayed();
    }
    else if (state == Core::Stopped) {
        m_bottomToolbar->onMusicPause();
    }
    else {
        m_bottomToolbar->onMusicPause();
    }
}

void MainWindow::openFile()
{
    QStringList _video;
    _video << "avi" << "vfw" << "divx"
       << "mpg" << "mpeg" << "m1v" << "m2v" << "mpv" << "dv" << "3gp"
       << "mov" << "mp4" << "m4v" << "mqv"
       << "dat" << "vcd"
       << "ogg" << "ogm" << "ogv" << "ogx"
       << "asf" << "wmv"
       << "bin" << "iso" << "vob"
       << "mkv" << "nsv" << "ram" << "flv"
       << "rm" << "swf"
       << "ts" << "rmvb" << "dvr-ms" << "m2t" << "m2ts" << "mts" << "rec" << "wtv"
       << "f4v" << "hdmov" << "webm" << "vp8"
       << "bik" << "smk" << "m4b" << "wtv"
       << "part";

    QString s_filter;
    for (int n=0; n < _video.count(); n++) {
        s_filter = s_filter + "*." + _video.at(n) + " ";
    }
    if (!s_filter.isEmpty()) s_filter = " (" + s_filter + ")";

    QString s = QFileDialog::getOpenFileName(
                       this, tr("Choose a file"), QDir::homePath(),
                       tr("Video") + s_filter +";;" +
                       tr("All files") +" (*.*)" );
    if (!s.isEmpty()) {
        openFile(s);
    }
}

void MainWindow::openFile(QString file)
{
   if (!file.isEmpty()) {
        QString extension = QFileInfo(file).suffix().toLower();
        if (!QFileInfo(file).isDir() && (extension != "iso") && (extension != "m3u") && (extension != "m3u8") && (extension != "pls")) {

            if (file.startsWith("file:")) {
                file = QUrl(file).toLocalFile();
            }
            m_core->openFile(file);
        }
    }
}

void MainWindow::gotCurrentTime(double sec, bool flag)
{
    QString time;
    QString all_time;
    if (flag) {
        time = "00:00:00";
        all_time = " / 00:00:00";
    }
    else {
        static int last_second = 0;
        if (qFloor(sec) == last_second) return; // Update only once per second
        last_second = qFloor(sec);

        time = Utils::formatTime((int) sec);
        all_time = " / " +  Utils::formatTime((int) m_core->mdat.duration);
    }
    if (m_bottomToolbar) {
        m_bottomToolbar->displayTime(time, all_time);
    }
}

void MainWindow::updateWidgets()
{
    ui->centralWidget->setFocus();

    bool muted = pref->mute;
    m_bottomToolbar->onMutedChanged(muted, pref->volume);

}

void MainWindow::resizeMainWindow(int w, int h)
{
    if (isFullScreen())
        return;

    int diff_width = this->width() - ui->centralWidget->width();
    int diff_height = this->height() - ui->centralWidget->height();

    int new_width = w + diff_width;
    int new_height = h + diff_height;

    int minimum_width = minimumSizeHint().width();
    if (new_width < minimum_width) {
        new_width = minimum_width;
    }

    resize(new_width, new_height);
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent *e)
{
    QStringList files;

    if (e->mimeData()->hasUrls()) {
        QList <QUrl> l = e->mimeData()->urls();
        QString s;
        for (int n=0; n < l.count(); n++) {
            if (l[n].isValid()) {
                if (l[n].scheme() == "file")
                    s = l[n].toLocalFile();
                else
                    s = l[n].toString();
                files.append(s);
            }
        }
    }

    QStringList file_list;

    if (files.count() > 0) {
        files.sort();

        QStringList _subtitles;
        _subtitles << "srt" << "sub" << "ssa" << "ass" << "idx" << "txt" << "smi"
               << "rt" << "utf" << "aqt" << "vtt";
        QString s;
        for (int n=0; n < _subtitles.count(); n++) {
            if (!s.isEmpty()) s = s + "|";
            s = s + "^" + _subtitles.at(n) + "$";
        }

        QRegExp ext_sub(s);
        ext_sub.setCaseSensitivity(Qt::CaseInsensitive);

        foreach (QString file, files) {
            QFileInfo fi(file);
            if (fi.isDir()) {

            }
            else
            if (ext_sub.indexIn(fi.suffix()) > -1) {
                // Subtitle file
            }
            else {
                file_list << file;
            }
        }

        if (file_list.count() > 0 && QFile::exists(file_list[0])) {
            openFile(file_list[0]);
        }
    }
}

void MainWindow::closeEvent(QCloseEvent * e)
{
    this->onCloseWindow();
    e->accept();
}
