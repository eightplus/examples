#include "mainwindow.h"

#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QDebug>
#include <QThread>
#include <QProgressBar>
#include <QFileDialog>
#include <QMap>
#include <QDir>
#include <QFile>
#include <QTime>
#include <QtConcurrent/QtConcurrentMap>

#include <functional>
#include <unistd.h>

const int imageSize = 100;
typedef QMap<QString, int> WordCount;

namespace {

void my_fun1(int &value)
{
    qDebug() << "run my_fun1 thread id:" << QThread::currentThreadId();
    value += 1;
}

int my_func2(int value)
{
    qDebug() << "run my_func2 thread id:" << QThread::currentThreadId();
    return value - 1;
}

int my_func3(int value)
{
    qDebug() << "run my_func3 thread id:" << QThread::currentThreadId();
    return value * 2;
}

void my_sum(int &result, const int &intermediate)
{
    qDebug() << "  run my_sum thread id:" << QThread::currentThreadId();
    result += intermediate;
}

/*QImage image_scale(const QString &imageName)
{
    //在线程中执行，所以线程id与主进程的不一样
    static int count = 0;
    qDebug() << "run image_scale thread id:" << QThread::currentThreadId();
    QImage image(imageName);
    count += 3;
    sleep(count);

    return image.scaled(QSize(imageSize, imageSize), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
}*/

QStringList findAllFiles(const QString &startDir, const QStringList &filters)
{
    QStringList names;
    QDir dir(startDir);

    const auto files = dir.entryList(filters, QDir::Files);
    for (const QString &file : files)
        names += startDir + '/' + file;

    const auto subdirs =  dir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot);
    for (const QString &subdir : subdirs) {
        names += findAllFiles(startDir + '/' + subdir, filters);
    }

    return names;
}



//使用单线程计算
WordCount countWordsWithSingleThread(const QStringList &files)
{
    WordCount wordCount;
    for (const QString &file : files) {
        QFile f(file);
        f.open(QIODevice::ReadOnly);
        QTextStream textStream(&f);
        while (!textStream.atEnd()) {
            const auto words =  textStream.readLine().split(' ');
            for (const QString &word : words)
                wordCount[word] += 1;
        }
    }

    return wordCount;
}

//使用多线程并行计算
WordCount countWordsWithMultiThreads(const QString &file)
{
    WordCount wordCount;

    QFile f(file);
    f.open(QIODevice::ReadOnly);
    QTextStream textStream(&f);

    while (!textStream.atEnd()) {
        const auto words =  textStream.readLine().split(' ');
        for (const QString &word : words)
            wordCount[word] += 1;
    }

    return wordCount;
}

void reduceWordCounts(WordCount &result, const WordCount &w)
{
    QMapIterator<QString, int> i(w);
    while (i.hasNext()) {
        i.next();
        result[i.key()] += i.value();
    }
}

}

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , m_watcher(new QFutureWatcher<QImage>(this))
{
    qDebug() << "MainWindow constructor thread id:" << QThread::currentThreadId();

    this->setWindowTitle(tr("QtConcurrent::map"));
    this->resize(600, 454);

    m_mapBtn = new QPushButton(tr("TestMap"));
    m_mappedBtn = new QPushButton(tr("TestMapped"));
    m_mappedReducedBtn = new QPushButton(tr("TestMappedReduced"));
    m_mappedReducedBtn2 = new QPushButton(tr("TestMappedReduced2"));
    QHBoxLayout *testbtnLayout = new QHBoxLayout();
    testbtnLayout->addWidget(m_mapBtn);
    testbtnLayout->addWidget(m_mappedBtn);
    testbtnLayout->addWidget(m_mappedReducedBtn);
    testbtnLayout->addWidget(m_mappedReducedBtn2);
    testbtnLayout->addStretch();
    testbtnLayout->setSpacing(10);
    testbtnLayout->setMargin(0);

    m_openBtn = new QPushButton(tr("Open Images"));
    m_cancelBtn = new QPushButton(tr("Cancel"));//点击取消按钮后，任务取消，后面没有执行的任务都会触发finished信号
    m_cancelBtn->setEnabled(false);
    m_pauseBtn = new QPushButton(tr("Pause/Resume"));
    m_pauseBtn->setEnabled(false);
    m_progressBar = new QProgressBar(this);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(m_openBtn);
    buttonLayout->addWidget(m_cancelBtn);
    buttonLayout->addWidget(m_pauseBtn);
    buttonLayout->addStretch();
    buttonLayout->setSpacing(10);
    buttonLayout->setMargin(0);

    m_imagesLayout = new QGridLayout();

    m_mainLayout = new QVBoxLayout();
    m_mainLayout->addLayout(testbtnLayout);
    m_mainLayout->addLayout(buttonLayout);
    m_mainLayout->addWidget(m_progressBar);
    m_mainLayout->addLayout(m_imagesLayout);
    m_mainLayout->addStretch();
    m_mainLayout->setSpacing(50);
    m_mainLayout->setContentsMargins(10, 10, 10, 10);
    setLayout(m_mainLayout);

    this->initConections();
}

MainWindow::~MainWindow()
{
    m_watcher->cancel();
    m_watcher->waitForFinished();
}

void MainWindow::initConections()
{
    connect(m_mapBtn, SIGNAL(clicked()), this, SLOT(onMapBtnClicked()));
    connect(m_mappedBtn, SIGNAL(clicked()), this, SLOT(onMappedBtnClicked()));
    connect(m_mappedReducedBtn, SIGNAL(clicked()), this, SLOT(onMappedReducedBtnClicked()));
    connect(m_mappedReducedBtn2, SIGNAL(clicked()), this, SLOT(onMappedReducedBtn2Clicked()));
    connect(m_openBtn, SIGNAL(clicked()), this, SLOT(onOpenBtnClicked()));
    connect(m_cancelBtn, &QPushButton::clicked, m_watcher, &QFutureWatcher<QImage>::cancel);
    connect(m_pauseBtn, &QPushButton::clicked, m_watcher, &QFutureWatcher<QImage>::togglePaused);
    connect(m_watcher, SIGNAL(progressRangeChanged(int,int)), this, SLOT(onSetProgressRange(int,int)));
    connect(m_watcher, SIGNAL(progressValueChanged(int)), this, SLOT(onProgressValueChanged(int)));
    connect(m_watcher, &QFutureWatcher<QImage>::resultReadyAt, this, &MainWindow::showImage);//connect(m_watcher, SIGNAL(resultReadyAt(int)), SLOT(showImage(int))); resultReadyAt() 表示 index 对应位置的处理结果已准备就绪，所以连接该信号至槽函数 showImage()，可以显示处理完的图片
    connect(m_watcher, &QFutureWatcher<QImage>::finished, this, &MainWindow::finished);
}

void MainWindow::onSetProgressRange(int minimum, int maximum)
{
    //监控了几张图片，则maximum就是几，minimum=0
    qDebug() << "onSetProgressRange minimum:" << minimum << ", maximum:" << maximum;
    m_progressBar->setRange(minimum, maximum);
}

void MainWindow::onProgressValueChanged(int value)
{
    qDebug() << "progressValueChanged value:" << value;
    m_progressBar->setValue(value);
}

void MainWindow::onOpenBtnClicked()
{
    if (m_watcher->isRunning()) {
        m_watcher->cancel();
        m_watcher->waitForFinished();
    }

    QStringList files = QFileDialog::getOpenFileNames(this, tr("Select Images"),
                            QStandardPaths::writableLocation(QStandardPaths::PicturesLocation), "*.jpg *.png");

    if (files.isEmpty())
        return;

    qDeleteAll(m_labels);
    m_labels.clear();

    int dim = qSqrt(qreal(files.count())) + 1;
    for (int i = 0; i < dim; ++i) {
        for (int j = 0; j < dim; ++j) {
            QLabel *imageLabel = new QLabel;
            imageLabel->setFixedSize(imageSize,imageSize);
            m_imagesLayout->addWidget(imageLabel,i,j);
            m_labels.append(imageLabel);
        }
    }

    std::function<QImage(const QString&)> image_scale = [imageSize](const QString &imageName) {
        //在线程中执行，所以线程id与主进程的不一样
        static int count = 0;
        qDebug() << "run image_scale thread id:" << QThread::currentThreadId();
        QImage image(imageName);
        count += 3;
        sleep(count);
        return image.scaled(QSize(imageSize, imageSize), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    };

    auto future = QtConcurrent::mapped(files, image_scale);
    m_watcher->setFuture(future);

    m_openBtn->setEnabled(false);
    m_cancelBtn->setEnabled(true);
    m_pauseBtn->setEnabled(true);
}

void MainWindow::showImage(int index)
{
    m_labels[index]->setPixmap(QPixmap::fromImage(m_watcher->resultAt(index)));
}

void MainWindow::finished()
{
    qDebug() << "finished.......";
    m_openBtn->setEnabled(true);
    m_cancelBtn->setEnabled(false);
    m_pauseBtn->setEnabled(false);
}

void MainWindow::onMapBtnClicked()
{
    QList<int> m_list;
    m_list << 1 << 3 << 5 << 7 << 9;

    QFuture<void> future = QtConcurrent::map(m_list, my_fun1);
    future.waitForFinished();
    qDebug() << "m_list: "<< m_list;
}

void MainWindow::onMappedBtnClicked()
{
    QList<int> m_list;
    m_list << 2 << 4 << 6 << 8 << 10;

    QFuture<int> future = QtConcurrent::mapped(m_list, my_func2);
    future.waitForFinished();
    qDebug()<<"m_list: " << m_list;
    qDebug()<<"new list: " << future.results();
}


void MainWindow::onMappedReducedBtnClicked()
{
    QList<int> m_list;
    m_list<< 1 << 2 << 3 << 4 << 5;

    QFuture<int> future = QtConcurrent::mappedReduced(m_list, my_func3, my_sum);
    future.waitForFinished();

    qDebug() << "result:" << future.result();
}

void MainWindow::onMappedReducedBtn2Clicked()
{
    QStringList files = findAllFiles(QDir::homePath(), QStringList() << "*.cpp" << "*.h");

    // Single thread
    QTime time1;
    time1.start();
    WordCount total1 = countWordsWithSingleThread(files);
    qDebug() << "Single thread find" << total1.size() << "words, Using " << time1.elapsed() / 1000.0 << "seconds";

    // Multi thread
    QTime time2;
    time2.start();
    WordCount total2 = QtConcurrent::mappedReduced(files, countWordsWithMultiThreads, reduceWordCounts);
    qDebug() << "Multi thread find " << total2.size() << "words, Using " << time2.elapsed() / 1000.0 << "seconds";
}
