#include "BusinessLogic.h"
#include <QDebug>
#include "Comms.h"
#include "DbManager.h"

BusinessLogic::BusinessLogic() : is_user_logger_(false) {
  comms_ = std::unique_ptr<IComms>(new Comms);
  db_ = std::unique_ptr<IDb>(new DbManager);

  QObject::connect(comms_.get(), &IComms::SendPatients, this,
                   &BusinessLogic::ProcessPatients);

  QObject::connect(comms_.get(), &IComms::LoginSuccess, this,
                   &BusinessLogic::ProcessLoginSuccess);

  QObject::connect(comms_.get(), &IComms::ReportCommsError, this,
                   &BusinessLogic::ProcessCommsError);
}

void BusinessLogic::StartUp() {
  db_->StartUp();

  CommsConfiguration conf;
  db_->GetCommConfiguration(conf);
  comms_->SetCommsAddress(conf);

  //Auto Login
  comms_->Login("Systelab", "Systelab");
}

void BusinessLogic::ShutDown() {
  QCoreApplication::processEvents();
  db_.reset();
  comms_.reset();
}

void BusinessLogic::LoginUser(QString user, QString pass) {
  comms_->Login(user, pass);
}

void BusinessLogic::ProcessLoginSuccess(QString user) {
  QString user_logged(user);
  is_user_logger_ = true;
  emit SendLoginStatus(true);
}

void BusinessLogic::LogOut() {
  is_user_logger_ = false;
}

void BusinessLogic::GetPatientList(int page) {
  comms_->GetPatientList(page);
}

void BusinessLogic::ProcessPatients(QVector<Patient> patients, int total_patients, int page_number) {
  emit SendPatientList(patients, total_patients, page_number);
}

void BusinessLogic::SaveNewPatient(const Patient &patient) {
  comms_->PostPatient(patient);
}

void BusinessLogic::UpdatePatient(const Patient &patient) {
  comms_->PutPatient(patient);
}

void BusinessLogic::DeletePatient(const QString &uid) {
  comms_->DeletePatient(uid);
}

void BusinessLogic::ProcessCommsError(int errorCode, QString errorSummary) {
  if (errorCode == 401) {
    is_user_logger_ = false;
    emit SendLoginStatus(false);
  }
  qDebug() << "errorCode" << errorCode;
  qDebug() << "errorSummary" << errorSummary;
}

void BusinessLogic::GetPatientFromList(const Patient &patient) {
  emit SendPatient(patient);
}

void BusinessLogic::UpdateConfiguration(const CommsConfiguration &conf) {
  if(db_->UpdateCommConfiguration(conf))
      comms_->SetCommsAddress(conf);
}

void BusinessLogic::GetConfiguration(CommsConfiguration &conf) {
  db_->GetCommConfiguration(conf);
}

bool BusinessLogic::IsUserLogger() {
  return is_user_logger_;
}
