#include "Logic/Types.h"
#include <QAbstractListModel>
#include <memory>

class BusinessLogic;

class PatientList : public QAbstractListModel {
  Q_OBJECT
  Q_PROPERTY(
      bool loadingPatients MEMBER loading_patients_ NOTIFY loadingPatients)
public:
  enum PatientRoles {
    idRole = Qt::UserRole + 1,
    posRole,
    nameRole,
    surnameRole,
    emailRole,
    dobRole
  };
  PatientList(QObject *parent, std::shared_ptr<BusinessLogic> &business_logic);

  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  QVariant data(const QModelIndex &index,
                int role = Qt::DisplayRole) const override;
  QHash<int, QByteArray> roleNames() const override;

public slots:
  void InsertPatients(const QVariantList &, int total_patients,
                      int page_number);
  void deletePatient(const QString &uid);
  void refreshPatientList();
  void sendPatientFromList(QString uuid);
  void fillServerDummyPatients();

signals:
  void loadingPatients();
  void LoadMorePatients() const;

private:
  void RequestAdditionalPatients();
  void ToggleLoading(bool load);
  void ResetModel();

private:
  bool loading_patients_;
  int total_patients_;
  int current_page_;
  QVector<seed::Patient> patient_list_;
  std::shared_ptr<BusinessLogic> business_logic_;
};
