#include "trainingform.h"
#include "ui_trainingform.h"

TrainingForm::TrainingForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TrainingForm) {
    ui->setupUi(this);
}

TrainingForm::~TrainingForm() {
    delete ui;
}

Ui::TrainingForm* TrainingForm::get_UI() {
    return ui;
}
