#ifndef NEW_CONFERENCE_WIZARD_H
#define NEW_CONFERENCE_WIZARD_H

#include <QWizard>
#include "ui_new_conference_wizard.h"
 
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

private slots:
    void browseConfigFile();
};


#endif
