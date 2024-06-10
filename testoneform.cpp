#include "testoneform.h"
#include "ui_testoneform.h"

TestOneForm::TestOneForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TestOneForm) {
    ui->setupUi(this);
}

TestOneForm::~TestOneForm() {
    delete ui;
}

Ui::TestOneForm* TestOneForm::get_UI() {
    return ui;
}
