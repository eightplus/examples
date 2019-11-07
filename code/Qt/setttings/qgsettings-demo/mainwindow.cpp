#include "mainwindow.h"
#include "preferences.h"
#include "ui_mainwindow.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QTimer>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_pref(new Preferences)
{
    ui->setupUi(this);

    this->setWindowTitle(tr("QGSettings"));
    this->resize(660, 368);

    ui->m_ageSpinBox->setMaximum(120);
    ui->m_ageSpinBox->setMinimum(1);
    ui->m_maleRadioButton->setObjectName("male");
    ui->m_femaleradioButton->setObjectName("female");
    m_buttonGroup = new QButtonGroup(this);
    m_buttonGroup->addButton(ui->m_maleRadioButton);
    m_buttonGroup->addButton(ui->m_femaleradioButton);

    this->move((QApplication::desktop()->screenGeometry(0).width() - this->width()) / 2, (QApplication::desktop()->screenGeometry(0).height() - this->height()) / 2);

    this->bindConnections();

    QTimer::singleShot(100, this, [=] { this->loadConfData(); });
}

MainWindow::~MainWindow()
{
    if (m_pref) {
        delete m_pref;
        m_pref = nullptr;
    }
}

void MainWindow::bindConnections()
{
    connect(m_pref, &Preferences::send_string, this, [=] (Preferences::ValueType type, const QString &value) {
        if (type == Preferences::NameType) {
            ui->m_nameLineEdit->setText(value);
        }

    });
    connect(m_pref, &Preferences::send_int, this, [=] (Preferences::ValueType type, int value) {
        if (type == Preferences::AgeType) {
            if (value <= 0 || value > 120)
                value = 18;
            ui->m_ageSpinBox->setValue(value);
        }
    });
    connect(m_pref, &Preferences::send_bool, this, [=] (Preferences::ValueType type, bool value) {
        if (type == Preferences::SexType) {
            if (value)
                ui->m_maleRadioButton->setChecked(true);
            else
                ui->m_femaleradioButton->setChecked(true);
        }
    });

    connect(ui->m_nameBtn, &QPushButton::clicked, this, [=] () {
        if (!ui->m_nameLineEdit->text().isEmpty())
            m_pref->set_string_value("name", ui->m_nameLineEdit->text());
    });
    connect(ui->m_ageSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onAgeValueChanged(int)));
    connect(ui->m_maleRadioButton, SIGNAL(clicked(bool)), this, SLOT(onRadioButtonClicked()));
    connect(ui->m_femaleradioButton, SIGNAL(clicked(bool)), this, SLOT(onRadioButtonClicked()));
}

void MainWindow::loadConfData()
{
    m_pref->load_data();
}

void MainWindow::onAgeValueChanged(int value)
{
    m_pref->set_int_value("age", value);
}

void MainWindow::onRadioButtonClicked()
{
    QObject *obj = sender();
    if (QRadioButton *pbtn = qobject_cast<QRadioButton*>(obj)) {
        QString obj_name = pbtn->objectName();
        if (obj_name == "male") {
            m_pref->set_bool_value("male", true);
        }
        else if(obj_name == "female") {
            m_pref->set_bool_value("male", false);
        }
    }
}
