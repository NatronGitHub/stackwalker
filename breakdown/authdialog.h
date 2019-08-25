#ifndef AUTHDIALOG_H
#define AUTHDIALOG_H

#include <QObject>
#include <QDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QCheckBox>

class AuthDialog : public QDialog
{
    Q_OBJECT

public:
    AuthDialog(QWidget *parent = nullptr);

public slots:
    const QString getUsername();
    const QString getPassword();
    bool saveAuth();

private:
    QPushButton *_ok;
    QPushButton *_cancel;
    QLineEdit *_username;
    QLineEdit *_password;
    QCheckBox *_save;

private slots:
    void handleOk();
    void handleCancel();
};

#endif // AUTHDIALOG_H
