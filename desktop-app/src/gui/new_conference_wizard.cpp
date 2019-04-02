#include "new_conference_wizard.h"

NewConferenceWizard::NewConferenceWizard(QWidget *parent)
    : QWizard(parent), ui(new Ui::NewConferenceWizard) {
    ui->setupUi(this);

    // Connect buttons
    connect(ui->configFileBrowseBtn, SIGNAL(released()), this,
            SLOT(browseConfigFile()));
}

NewConferenceWizard::~NewConferenceWizard() { delete ui; }

NewConferenceWizard::browseConfigFile() {
    // configFilePath
}