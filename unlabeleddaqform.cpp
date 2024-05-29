#include "unlabeleddaqform.h"
#include "ui_unlabeleddaqform.h"

UnlabeledDAQForm::UnlabeledDAQForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UnlabeledDAQForm) {
    ui->setupUi(this);
}

UnlabeledDAQForm::~UnlabeledDAQForm() {
    delete ui;
}

Ui::UnlabeledDAQForm* UnlabeledDAQForm::get_UI() {
    return ui;
}
