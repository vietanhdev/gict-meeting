#include "new_conference_wizard.h"

NewConferenceWizard::NewConferenceWizard(QWidget *parent)
    : QWizard(parent), ui(new Ui::NewConferenceWizard) {
    ui->setupUi(this);

    // Connect buttons
    connect(ui->configFileBrowseBtn, SIGNAL(released()), this,
            SLOT(browseConfigFile()));
}

NewConferenceWizard::~NewConferenceWizard() { delete ui; }


bool NewConferenceWizard::requireConfigField(const YAML::Node & config, std::string config_field) {
    if (!config["conference_name"]) {
        std::string error_msg = "Wrong format configuration file - Missing:" + config_field;
        QMessageBox::critical(
            this, "Config Error",
            error_msg.c_str()
            );
        return false;
    }
    return true;
}

void NewConferenceWizard::browseConfigFile() {

    configLoaded = false;

    // Open config file
    QString configFile = QFileDialog::getOpenFileName(this,
    tr("Open configuration file for this meeting"), NULL, tr("iCT Meeting Config File (*.yaml)"));
    ui->configFilePath->setText(configFile);

    // Load config file
    YAML::Node config = YAML::LoadFile(configFile.toStdString());

    // Check config fields
    if (!requireConfigField(config, "conference_name")) return;
    if (!requireConfigField(config, "secret_key")) return;
    if (!requireConfigField(config, "server_ip")) return;
    if (!requireConfigField(config, "video_up_port")) return;
    if (!requireConfigField(config, "video_down_port")) return;
    if (!requireConfigField(config, "audio_up_port")) return;
    if (!requireConfigField(config, "audio_down_port")) return;


    Conference &new_conference = Conference::instance();
    
    new_conference.setName(config["conference_name"].as<std::string>());
    new_conference.setSecretKey(config["secret_key"].as<std::string>());
    new_conference.setServerIp(config["server_ip"].as<std::string>());
    new_conference.setVideoUpPort(config["video_up_port"].as<std::string>());
    new_conference.setVideoDownPort(config["video_down_port"].as<std::string>());
    new_conference.setAudioUpPort(config["audio_up_port"].as<std::string>());
    new_conference.setAudioDownPort(config["audio_down_port"].as<std::string>());
    new_conference.setClientId(config["client_id"].as<std::string>());
    new_conference.setClientName(config["client_name"].as<std::string>());


    // Review config
    QString configConfirm = QString( "Configuration Review: \n"
                                "Conference Name: %1\n"
                                "Server IP: %2\n"
                                "video_up_port: %3\n"
                                "video_down_port: %4\n"
                                "audio_up_port: %5\n"
                                "audio_down_port: %6\n"
                                "client_id: %7\n"
                                "client_name: %8\n"
                                "\nClick 'Finish' to Join the conference."
                                 ).arg( QString::fromUtf8(new_conference.getName().c_str()))
                                 .arg( QString::fromUtf8(new_conference.getServerIp().c_str()))
                                 .arg( QString::fromUtf8(new_conference.getVideoUpPort().c_str()))
                                 .arg( QString::fromUtf8(new_conference.getVideoDownPort().c_str()))
                                 .arg( QString::fromUtf8(new_conference.getAudioUpPort().c_str()))
                                 .arg( QString::fromUtf8(new_conference.getAudioDownPort().c_str()))
                                 .arg( QString::fromUtf8(new_conference.getClientId().c_str()))
                                 .arg( QString::fromUtf8(new_conference.getClientName().c_str()));
    ui->conferenceConfigReview->setText(configConfirm);

    configLoaded = true;

}

void NewConferenceWizard::done(int result) {
    if (result) {
        if (configLoaded) {
            std::cout << "START STREAMING" << std::endl;
            this->destroy();
        } else {
            QMessageBox::critical(
                this, "Config Error",
                "Please choose a configuration file."
            );
        }
    } else {
        this->destroy();
    }
}