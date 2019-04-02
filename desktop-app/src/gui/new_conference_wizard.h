#ifndef NEW_CONFERENCE_WIZARD_H
#define NEW_CONFERENCE_WIZARD_H

#include <iostream>

#include <QWizard>
#include <QString>
#include <QFileDialog>
#include <QMessageBox>
#include "ui_new_conference_wizard.h"

#include "yaml-cpp/yaml.h"

#include "conference.h"

 
namespace Ui {
class NewConferenceWizard;
}
 
class NewConferenceWizard : public QWizard
{
    Q_OBJECT
 
public:
    explicit NewConferenceWizard(QWidget *parent = 0);
    ~NewConferenceWizard();
 
private:
    Ui::NewConferenceWizard *ui;

    bool requireConfigField(const YAML::Node & config, std::string config_field);

private slots:
    void browseConfigFile();
};


#endif
