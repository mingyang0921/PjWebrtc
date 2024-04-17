#include "QtWidgetsTest.h"

#include <QApplication>
#include <QComboBox>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSpacerItem>
#include <QTextEdit>
#include <QVBoxLayout>

QtWidgetsTest::QtWidgetsTest(QWidget *parent)
	: QWidget(parent)
{
    initUI();
}

QtWidgetsTest::~QtWidgetsTest()
{}


void QtWidgetsTest::initUI()
{
    QVBoxLayout* verticalLayout_father;
    QHBoxLayout* horizontalLayout_up;
    QVBoxLayout* verticalLayout_local_camera;
    QHBoxLayout* horizontalLayout_local_camera_switch;
    QSpacerItem* horizontalSpacer_local_camera_left;
    QLabel* label_local_camera_info;
    QComboBox* comboBox_cocal_camera_check;
    QPushButton* pushButton_local_camera_makesure;
    QSpacerItem* horizontalSpacer_local_camera_right;
    QLabel* label_local_camera_show;
    QVBoxLayout* verticalLayout_local_sdp;
    QHBoxLayout* horizontalLayout_local_sdp_title;
    QSpacerItem* horizontalSpacer_local_sdp_title_left;
    QLabel* label_local_sdp_title_info;
    QSpacerItem* horizontalSpacer_local_sdp_title_right;
    QTextEdit* textEdit_local_sdp_text;
    QVBoxLayout* verticalLayout_local_button;
    QSpacerItem* verticalSpacer_local_button_up;
    QPushButton* pushButton_local_button_press;
    QSpacerItem* verticalSpacer_local_button_down;
    QFrame* line;
    QHBoxLayout* horizontalLayout_down;
    QVBoxLayout* verticalLayout_remote_camera;
    QHBoxLayout* horizontalLayout_remote_camera_title;
    QSpacerItem* horizontalSpacer_remote_camera_title_left;
    QLabel* label_remote_camera_title_info;
    QSpacerItem* horizontalSpacer_remote_camera_title_right;
    QLabel* label_remote_camera_show;
    QVBoxLayout* verticalLayout_remote_sdp;
    QHBoxLayout* horizontalLayout_remote_sdp;
    QSpacerItem* horizontalSpacer_remote_sdp_left;
    QLabel* label_remote_sdp_info;
    QSpacerItem* horizontalSpacer_remote_sdp_right;
    QTextEdit* textEdit_remote_sdp_text;
    QVBoxLayout* verticalLayout_remote_button;
    QSpacerItem* verticalSpacer_remote_button_up;
    QPushButton* pushButton_remote_button_press;
    QSpacerItem* verticalSpacer_remote_button_down;

    this->resize(937, 623);

    verticalLayout_father = new QVBoxLayout(this);
    verticalLayout_father->setObjectName(QString::fromUtf8("verticalLayout_father"));
    verticalLayout_father->setContentsMargins(0, 0, 0, 0);

    horizontalLayout_up = new QHBoxLayout();
    horizontalLayout_up->setObjectName(QString::fromUtf8("horizontalLayout_up"));

    verticalLayout_local_camera = new QVBoxLayout();
    verticalLayout_local_camera->setObjectName(QString::fromUtf8("verticalLayout_local_camera"));
    horizontalLayout_local_camera_switch = new QHBoxLayout();
    horizontalLayout_local_camera_switch->setObjectName(QString::fromUtf8("horizontalLayout_local_camera_switch"));
    horizontalSpacer_local_camera_left = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout_local_camera_switch->addItem(horizontalSpacer_local_camera_left);

    label_local_camera_info = new QLabel();
    label_local_camera_info->setObjectName(QString::fromUtf8("label_local_camera_info"));

    horizontalLayout_local_camera_switch->addWidget(label_local_camera_info);

    comboBox_cocal_camera_check = new QComboBox();
    comboBox_cocal_camera_check->setObjectName(QString::fromUtf8("comboBox_cocal_camera_check"));

    horizontalLayout_local_camera_switch->addWidget(comboBox_cocal_camera_check);

    pushButton_local_camera_makesure = new QPushButton();
    pushButton_local_camera_makesure->setObjectName(QString::fromUtf8("pushButton_local_camera_makesure"));

    horizontalLayout_local_camera_switch->addWidget(pushButton_local_camera_makesure);

    horizontalSpacer_local_camera_right = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout_local_camera_switch->addItem(horizontalSpacer_local_camera_right);


    verticalLayout_local_camera->addLayout(horizontalLayout_local_camera_switch);

    label_local_camera_show = new QLabel();
    label_local_camera_show->setObjectName(QString::fromUtf8("label_local_camera_show"));

    verticalLayout_local_camera->addWidget(label_local_camera_show);


    horizontalLayout_up->addLayout(verticalLayout_local_camera);

    verticalLayout_local_sdp = new QVBoxLayout();
    verticalLayout_local_sdp->setObjectName(QString::fromUtf8("verticalLayout_local_sdp"));
    horizontalLayout_local_sdp_title = new QHBoxLayout();
    horizontalLayout_local_sdp_title->setObjectName(QString::fromUtf8("horizontalLayout_local_sdp_title"));
    horizontalSpacer_local_sdp_title_left = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout_local_sdp_title->addItem(horizontalSpacer_local_sdp_title_left);

    label_local_sdp_title_info = new QLabel();
    label_local_sdp_title_info->setObjectName(QString::fromUtf8("label_local_sdp_title_info"));

    horizontalLayout_local_sdp_title->addWidget(label_local_sdp_title_info);

    horizontalSpacer_local_sdp_title_right = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout_local_sdp_title->addItem(horizontalSpacer_local_sdp_title_right);


    verticalLayout_local_sdp->addLayout(horizontalLayout_local_sdp_title);

    textEdit_local_sdp_text = new QTextEdit();
    textEdit_local_sdp_text->setObjectName(QString::fromUtf8("textEdit_local_sdp_text"));

    verticalLayout_local_sdp->addWidget(textEdit_local_sdp_text);


    horizontalLayout_up->addLayout(verticalLayout_local_sdp);

    verticalLayout_local_button = new QVBoxLayout();
    verticalLayout_local_button->setObjectName(QString::fromUtf8("verticalLayout_local_button"));
    verticalSpacer_local_button_up = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    verticalLayout_local_button->addItem(verticalSpacer_local_button_up);

    pushButton_local_button_press = new QPushButton();
    pushButton_local_button_press->setObjectName(QString::fromUtf8("pushButton_local_button_press"));

    verticalLayout_local_button->addWidget(pushButton_local_button_press);

    verticalSpacer_local_button_down = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    verticalLayout_local_button->addItem(verticalSpacer_local_button_down);


    horizontalLayout_up->addLayout(verticalLayout_local_button);


    verticalLayout_father->addLayout(horizontalLayout_up);

    line = new QFrame();
    line->setObjectName(QString::fromUtf8("line"));
    line->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 24, 55);"));
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);

    verticalLayout_father->addWidget(line);

    horizontalLayout_down = new QHBoxLayout();
    horizontalLayout_down->setObjectName(QString::fromUtf8("horizontalLayout_down"));
    verticalLayout_remote_camera = new QVBoxLayout();
    verticalLayout_remote_camera->setObjectName(QString::fromUtf8("verticalLayout_remote_camera"));
    horizontalLayout_remote_camera_title = new QHBoxLayout();
    horizontalLayout_remote_camera_title->setObjectName(QString::fromUtf8("horizontalLayout_remote_camera_title"));
    horizontalSpacer_remote_camera_title_left = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout_remote_camera_title->addItem(horizontalSpacer_remote_camera_title_left);

    label_remote_camera_title_info = new QLabel();
    label_remote_camera_title_info->setObjectName(QString::fromUtf8("label_remote_camera_title_info"));

    horizontalLayout_remote_camera_title->addWidget(label_remote_camera_title_info);

    horizontalSpacer_remote_camera_title_right = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout_remote_camera_title->addItem(horizontalSpacer_remote_camera_title_right);


    verticalLayout_remote_camera->addLayout(horizontalLayout_remote_camera_title);

    label_remote_camera_show = new QLabel();
    label_remote_camera_show->setObjectName(QString::fromUtf8("label_remote_camera_show"));

    verticalLayout_remote_camera->addWidget(label_remote_camera_show);


    horizontalLayout_down->addLayout(verticalLayout_remote_camera);

    verticalLayout_remote_sdp = new QVBoxLayout();
    verticalLayout_remote_sdp->setObjectName(QString::fromUtf8("verticalLayout_remote_sdp"));
    horizontalLayout_remote_sdp = new QHBoxLayout();
    horizontalLayout_remote_sdp->setObjectName(QString::fromUtf8("horizontalLayout_remote_sdp"));
    horizontalSpacer_remote_sdp_left = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout_remote_sdp->addItem(horizontalSpacer_remote_sdp_left);

    label_remote_sdp_info = new QLabel();
    label_remote_sdp_info->setObjectName(QString::fromUtf8("label_remote_sdp_info"));

    horizontalLayout_remote_sdp->addWidget(label_remote_sdp_info);

    horizontalSpacer_remote_sdp_right = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    
    horizontalLayout_remote_sdp->addItem(horizontalSpacer_remote_sdp_right);


    verticalLayout_remote_sdp->addLayout(horizontalLayout_remote_sdp);

    textEdit_remote_sdp_text = new QTextEdit();
    textEdit_remote_sdp_text->setObjectName(QString::fromUtf8("textEdit_remote_sdp_text"));

    verticalLayout_remote_sdp->addWidget(textEdit_remote_sdp_text);


    horizontalLayout_down->addLayout(verticalLayout_remote_sdp);

    verticalLayout_remote_button = new QVBoxLayout();
    verticalLayout_remote_button->setObjectName(QString::fromUtf8("verticalLayout_remote_button"));
    verticalSpacer_remote_button_up = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    verticalLayout_remote_button->addItem(verticalSpacer_remote_button_up);

    pushButton_remote_button_press = new QPushButton();
    pushButton_remote_button_press->setObjectName(QString::fromUtf8("pushButton_remote_button_press"));
    
    verticalLayout_remote_button->addWidget(pushButton_remote_button_press);

    verticalSpacer_remote_button_down = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    verticalLayout_remote_button->addItem(verticalSpacer_remote_button_down);


    horizontalLayout_down->addLayout(verticalLayout_remote_button);


    verticalLayout_father->addLayout(horizontalLayout_down);

    label_local_camera_info->setText(QApplication::translate("Widget", "\345\252\222\344\275\223\346\272\220\351\200\211\346\213\251", nullptr));
    pushButton_local_camera_makesure->setText(QApplication::translate("Widget", "\347\241\256\350\256\244", nullptr));
    label_local_camera_show->setText(QApplication::translate("Widget", "TextLabel", nullptr));
    label_local_sdp_title_info->setText(QApplication::translate("Widget", "\346\234\254\345\234\260SDP", nullptr));
    pushButton_local_button_press->setText(QApplication::translate("Widget", "\345\220\257\345\212\250", nullptr));
    label_remote_camera_title_info->setText(QApplication::translate("Widget", "\345\257\271\347\253\257\345\252\222\344\275\223", nullptr));
    label_remote_camera_show->setText(QApplication::translate("Widget", "TextLabel", nullptr));
    label_remote_sdp_info->setText(QApplication::translate("Widget", "\345\257\271\347\253\257SDP", nullptr));
    pushButton_remote_button_press->setText(QApplication::translate("Widget", "\350\277\236\346\216\245", nullptr));

}