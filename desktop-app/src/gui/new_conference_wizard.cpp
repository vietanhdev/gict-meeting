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
    if (!config[config_field]) {
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
    if (!requireConfigField(config, "client_id")) return;
    if (!requireConfigField(config, "client_name")) return;
    if (!requireConfigField(config, "client_auth_key")) return;


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
    new_conference.setClientAuthKey(config["client_auth_key"].as<std::string>());


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
                                 .arg(  QString::fromUtf8(new_conference.getServerIp().c_str()))
                                 .arg( new_conference.getVideoUpPort() )
                                 .arg( new_conference.getVideoDownPort() )
                                 .arg( new_conference.getAudioUpPort() )
                                 .arg( new_conference.getAudioDownPort() )
                                 .arg( new_conference.getClientId() )
                                 .arg( QString::fromUtf8(new_conference.getClientName().c_str()));
    ui->conferenceConfigReview->setText(configConfirm);

    configLoaded = true;

}

void NewConferenceWizard::done(int result) {
    if (result) {
        if (configLoaded) {
            this->destroy();
            if (!VideoStreamService::instance().startStreaming()) {
                QMessageBox::critical(
                    this, "Error",
                    "Could not join the conference - Image streaming error."
                );
            }
            if (!AudioStreamService::instance().startStreaming()) {
                QMessageBox::critical(
                    this, "Error",
                    "Could not join the conference - Audio streaming error."
                );
            }
            emit newConferenceJoined();
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