#ifndef PATIENTHELPER_H
#define PATIENTHELPER_H

#include "Logic/Types.h"
#include <QDate>
#include <QObject>

class PatientHelper : public QObject {
  Q_OBJECT
  Q_PROPERTY(QString id MEMBER id_ NOTIFY patientChanged)
  Q_PROPERTY(QString name MEMBER name_ NOTIFY patientChanged)
  Q_PROPERTY(QString surname MEMBER surname_ NOTIFY patientChanged)
  Q_PROPERTY(QString email MEMBER email_ NOTIFY patientChanged)
  Q_PROPERTY(QString dobYear MEMBER dobYear_ NOTIFY patientChanged)
  Q_PROPERTY(QString dobMonth MEMBER dobMonth_ NOTIFY patientChanged)
  Q_PROPERTY(QString dobDay MEMBER dobDay_ NOTIFY patientChanged)
  Q_PROPERTY(QString coor MEMBER coor_ NOTIFY patientChanged)
  Q_PROPERTY(QString street MEMBER street_ NOTIFY patientChanged)
  Q_PROPERTY(QString city MEMBER city_ NOTIFY patientChanged)
  Q_PROPERTY(QString zip MEMBER zip_ NOTIFY patientChanged)

public:
  PatientHelper(QObject *parent = nullptr) : QObject(parent) {
    initializePatient();
  }

  seed::Patient getPatient() {
    seed::Patient p;
    p.id = id_;
    p.name = name_;
    p.surname = surname_;
    p.email = email_;
    p.dateOfBirth = dobYear_ + dobMonth_ + dobDay_ + "T000000";
    p.coordinates = coor_;
    p.street = street_;
    p.city = city_;
    p.zip = zip_;
    return p;
  }

  void setPatient(seed::Patient patient) {
    id_ = patient.id;
    name_ = patient.name;
    surname_ = patient.surname;
    email_ = patient.email;
    dobYear_ = patient.dateOfBirth.left(4);
    dobMonth_ = patient.dateOfBirth.mid(4, 2);
    dobDay_ = patient.dateOfBirth.mid(6, 2);
    coor_ = patient.coordinates;
    street_ = patient.street;
    city_ = patient.city;
    zip_ = patient.zip;
  }

  void initializePatient() {
    id_ = "";
    name_ = "";
    surname_ = "";
    email_ = "";
    coor_ = "";
    street_ = "";
    city_ = "";
    zip_ = "";

    dobYear_ = QString::number(QDate::currentDate().year());
    dobMonth_ = QString::number(QDate::currentDate().month());
    if (dobMonth_.length() < 2)
      dobMonth_.prepend("0");
    dobDay_ = QString::number(QDate::currentDate().day());
    if (dobDay_.length() < 2)
      dobDay_.prepend("0");
  }

signals:
  void patientChanged();

private:
  QString id_;
  QString name_;
  QString surname_;
  QString email_;
  QString dobYear_;
  QString dobMonth_;
  QString dobDay_;
  QString coor_;
  QString street_;
  QString zip_;
  QString city_;
};

#endif // PATIENTHELPER_H
