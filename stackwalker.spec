Summary: Convert minidump to JSON
Name: stackwalker

Version: 2016.06
Release: 1%{?dist}
License: BSD

Group: System Environment/Base
URL: https://github.com/olear/stackwalker

Source: %{name}-%{version}.tar.xz
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root

BuildRequires: libcurl-devel
Requires: libcurl

%description
Convert minidump to JSON.

%prep
%setup

%build
cd breakpad
./configure
make %{?_smp_mflags}
cd ..
cd minidump-stackwalk
make
cd ..
head -n32 minidump-stackwalk/stackwalker.cc > LICENSE

%install
mkdir -p %{buildroot}/usr/bin
strip -s minidump-stackwalk/stackwalker
cp minidump-stackwalk/stackwalker %{buildroot}/usr/bin/

%clean
%{__rm} -rf %{buildroot}

%files
%defattr(-,root,root,-)
/usr/bin/stackwalker
%doc LICENSE

%changelog
