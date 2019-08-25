#include "authdialog.h"

#include <QIcon>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>
#include <QLabel>

AuthDialog::AuthDialog(QWidget *parent) :
    QDialog (parent)
  , _ok(nullptr)
  , _cancel(nullptr)
  , _username(nullptr)
  , _password(nullptr)
  , _save(nullptr)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    _save = new QCheckBox(this);
    _ok = new QPushButton(this);
    _cancel = new QPushButton(this);

    _save->setText(tr("Save"));
    _ok->setText(tr("Ok"));
    _cancel->setText(tr("Cancel"));

    _username = new QLineEdit(this);
    _password = new QLineEdit(this);

    _password->setEchoMode(QLineEdit::Password);

    QLabel *userLabel = new QLabel(this);
    QLabel *passLabel = new QLabel(this);

    userLabel->setText(tr("Username"));
    passLabel->setText(tr("Password"));

    QWidget *buttonWidget = new QWidget(this);
    QHBoxLayout *buttonLayout = new QHBoxLayout(buttonWidget);

    QWidget *userWidget = new QWidget(this);
    QHBoxLayout *userLayout = new QHBoxLayout(userWidget);

    QWidget *passWidget = new QWidget(this);
    QHBoxLayout *passLayout = new QHBoxLayout(passWidget);

    userLayout->addWidget(userLabel);
    userLayout->addWidget(_username);

    passLayout->addWidget(passLabel);
    passLayout->addWidget(_password);

    buttonLayout->addWidget(_save);
    buttonLayout->addWidget(_ok);
    buttonLayout->addWidget(_cancel);

    mainLayout->addWidget(userWidget);
    mainLayout->addWidget(passWidget);
    mainLayout->addWidget(buttonWidget);
    mainLayout->addStretch();

    connect(_ok,
            SIGNAL(released()),
            this,
            SLOT(handleOk()));
    connect(_cancel,
            SIGNAL(released()),
            this,
            SLOT(handleCancel()));
}

const QString AuthDialog::getUsername()
{
    return _username->text();
}

const QString AuthDialog::getPassword()
{
    return _password->text();
}

bool AuthDialog::saveAuth()
{
    return _save->isChecked();
}

void AuthDialog::handleOk()
{
    QDialog::accept();
}

void AuthDialog::handleCancel()
{
    QDialog::reject();
}
