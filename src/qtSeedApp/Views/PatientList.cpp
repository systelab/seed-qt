#include "PatientList.h"
#include "Logic/BusinessLogic.h"

QHash<int, QByteArray> PatientList::roleNames() const {
  QHash<int, QByteArray> roles;

  roles[idRole] = "id";
  roles[posRole] = "pos";
  roles[nameRole] = "name";
  roles[surnameRole] = "surname";
  roles[emailRole] = "email";
  roles[dobRole] = "dob";

  return roles;
}

PatientList::PatientList(QObject *parent,
                         std::shared_ptr<BusinessLogic> &business_logic)
    : QAbstractListModel(parent), loading_patients_(false), total_patients_(0),
      current_page_(0), business_logic_(business_logic) {
  QObject::connect(business_logic_.get(), &BusinessLogic::SendPatientList, this,
                   &PatientList::InsertPatients);

  QObject::connect(this, &PatientList::LoadMorePatients, this,
                   &PatientList::RequestAdditionalPatients);
}

int PatientList::rowCount(const QModelIndex & /* parent */) const {
  return total_patients_;
}

QVariant PatientList::data(const QModelIndex &index, int role) const {
  if (!index.isValid())
    return QVariant();

  if (index.row() >= total_patients_ || index.row() < 0)
    return QVariant();

  // Check if there are still patients to request from server
  if ((patient_list_.size() - index.row() < 4) &&
      patient_list_.size() != total_patients_)
    emit LoadMorePatients();

  switch (role) {
  case PatientRoles::posRole:
    return patient_list_.at(index.row()).id.left(3);
  case PatientRoles::idRole:
    return patient_list_.at(index.row()).id;
  case PatientRoles::nameRole:
    return patient_list_.at(index.row()).name;
  case PatientRoles::surnameRole:
    return patient_list_.at(index.row()).surname;
  case PatientRoles::emailRole:
    return patient_list_.at(index.row()).email;
  case PatientRoles::dobRole: {
    QString dob(patient_list_.at(index.row()).dateOfBirth);
    return dob.left(4) + "/" + dob.mid(4, 2) + "/" + dob.mid(6, 2);
  }
  default:
    return QVariant();
  }
}

void PatientList::InsertPatients(const QVariantList &patients,
                                 int total_patients, int page_number) {
  ToggleLoading(false);

  // If patients are in server's first page ->Clear list before inserting new
  // patients
  if (current_page_ == 0 && !patient_list_.empty()) {
    ResetModel();
  }

  current_page_ = page_number;
  total_patients_ = total_patients;

  // Do not call beginInsertRows if there are no patients in server side.
  if (total_patients_ <= 0)
    return;

  beginInsertRows(QModelIndex(), patient_list_.size(),
                  patient_list_.size() + patients.size() - 1);

  for (auto i : patients) {
    Patient patient = i.value<seed::Patient>();
    patient_list_.push_back(patient);
  }
  endInsertRows();
}

void PatientList::deletePatient(const QString &uid) {
  business_logic_->DeletePatient(uid);
}

void PatientList::refreshPatientList() { business_logic_->GetPatientList(0); }

void PatientList::sendPatientFromList(QString uuid) {
  for (auto patient : patient_list_) {
    if (patient.id == uuid) {
      business_logic_->GetPatientFromList(patient);
      return;
    }
  }
}

void PatientList::RequestAdditionalPatients() {
  // Exit function if we are already loagind new patients
  if (loading_patients_)
    return;

  current_page_++;
  business_logic_->GetPatientList(current_page_);
  ToggleLoading(true);
}

void PatientList::ToggleLoading(bool load) {
  loading_patients_ = load;
  emit loadingPatients();
}

void PatientList::ResetModel() {
  total_patients_ = 0;
  beginResetModel();
  patient_list_.clear();
  endResetModel();
}

void PatientList::fillServerDummyPatients() {
  business_logic_->FillServerDummyPatients();
}
