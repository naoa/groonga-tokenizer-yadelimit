
#CentOS6

```
docker run -v /usr/local/src:/usr/local/src -i -t centos:centos6 /bin/bash
rpm -ivh http://packages.groonga.org/centos/groonga-release-1.1.0-1.noarch.rpm
yum update -y
yum install -y groonga-devel autoconf automake libtool wget tar gcc-c++ make mecab-devel libedit-devel rpm-build rpmdevtools

cd /usr/local/src/groonga-tokenizer-yadelimit
make clean
sh clean.sh
sh autogen.sh
./configure && make

mkdir -p ~/rpmbuild/{BUILD,SRPMS,SPECS,SOURCES,RPMS}
cp packages/rpm/centos/groonga-tokenizer-yadelimit.spec ~/rpmbuild/SPECS/

cd /usr/local/src
rm -rf groonga-tokenizer-yadelimit-1.0.0
cp -rf groonga-tokenizer-yadelimit groonga-tokenizer-yadelimit-1.0.0
tar zcvf groonga-tokenizer-yadelimit-1.0.0.tar.gz groonga-tokenizer-yadelimit-1.0.0
mv groonga-tokenizer-yadelimit-1.0.0.tar.gz ~/rpmbuild/SOURCES/
rpmbuild -ba ~/rpmbuild/SPECS/groonga-tokenizer-yadelimit.spec
mv ~/rpmbuild/RPMS/x86_64/groonga-tokenizer-yadelimit-1.0.0-1.el6.x86_64.rpm /usr/local/src/groonga-tokenizer-yadelimit/public/centos/6/
```

#CentOS7

```
docker run -v /usr/local/src:/usr/local/src -i -t centos:centos7 /bin/bash
…
mv ~/rpmbuild/RPMS/x86_64/groonga-tokenizer-yadelimit-1.0.0-1.el7.centos.x86_64.rpm /usr/local/src/groonga-tokenizer-yadelimit/public/centos/7/```
```

#Fefora20

```
docker run -v /usr/local/src:/usr/local/src -i -t fedora:20 /bin/bash
rpm -ivh http://packages.groonga.org/fedora/groonga-release-1.1.0-1.noarch.rpm
yum update -y
yum install -y groonga-devel autoconf automake libtool wget tar gcc-c++ make mecab-devel libedit-devel rpm-build rpmdevtools
…
cp packages/rpm/fedora/groonga-tokenizer-yadelimit.spec ~/rpmbuild/SPECS/
…
mv ~/rpmbuild/RPMS/x86_64/groonga-tokenizer-yadelimit-1.0.0-1.fc20.x86_64.rpm /usr/local/src/groonga-tokenizer-yadelimit/public/fedora/20/
```

#Debian(wheezy)

```
docker run -v /usr/local/src:/usr/local/src -i -t naoa/groonga-build-debian-wheezy /bin/bash
docker run -v /usr/local/src:/usr/local/src -i -t debian:wheezy /bin/bash

echo "deb http://packages.groonga.org/debian/ wheezy main" >> /etc/apt/sources.list.d/groonga.list
echo "deb-src http://packages.groonga.org/debian/ wheezy main" >> /etc/apt/sources.list.d/groonga.list

apt-get update
apt-get install -y --allow-unauthenticated groonga-keyring
apt-get update
apt-get install -y groonga libgroonga-dev wget tar build-essential zlib1g-dev liblzo2-dev libmsgpack-dev libzmq-dev libevent-dev libmecab-dev autoconf automake libtool lsb-release aptitude devscripts

cd /usr/local/src/groonga-tokenizer-yadelimit
make clean
sh clean.sh
sh autogen.sh
./configure && make

rm -rf ~/build
mkdir -p ~/build

cd /usr/local/src

rm -rf groonga-tokenizer-yadelimit-1.0.0
cp -rf groonga-tokenizer-yadelimit groonga-tokenizer-yadelimit-1.0.0
tar zcvf groonga-tokenizer-yadelimit-1.0.0.tar.gz groonga-tokenizer-yadelimit-1.0.0
mv groonga-tokenizer-yadelimit-1.0.0.tar.gz ~/build/groonga-tokenizer-yadelimit_1.0.0.orig.tar.gz
cd ~/build
tar xfz groonga-tokenizer-yadelimit_1.0.0.orig.tar.gz
cd groonga-tokenizer-yadelimit-1.0.0/
cp -rf packages/debian .
debuild -us -uc -b
cd ..
mv ~/build/groonga-tokenizer-yadelimit_1.0.0-1_amd64.deb /usr/local/src/groonga-tokenizer-yadelimit/public/debian/wheezy/
```

#Debian(jessie)

```
docker run -v /usr/local/src:/usr/local/src -i -t naoa/groonga-build-debian-jessie /bin/bash
docker run -v /usr/local/src:/usr/local/src -i -t debian:jessie /bin/bash

echo "deb http://packages.groonga.org/debian/ jessie main" >> /etc/apt/sources.list.d/groonga.list
echo "deb-src http://packages.groonga.org/debian/ jessie main" >> /etc/apt/sources.list.d/groonga.list
…
mv ~/build/groonga-tokenizer-yadelimit_1.0.0-1_amd64.deb /usr/local/src/groonga-tokenizer-yadelimit/public/debian/jessie/
```

#Ubuntu(precise)

```
docker run -v /usr/local/src:/usr/local/src -i -t naoa/groonga-build-ubuntu-precise /bin/bash
docker run -v /usr/local/src:/usr/local/src -i -t ubuntu:precise /bin/bash

apt-get update
apt-get -y install software-properties-common python-software-properties
apt-get update
add-apt-repository -y universe
add-apt-repository -y ppa:groonga/ppa
apt-get update
apt-get install -y groonga libgroonga-dev wget tar build-essential zlib1g-dev liblzo2-dev libmsgpack-dev libzmq-dev libevent-dev libmecab-dev autoconf automake libtool lsb-release aptitude devscripts
apt-get install -y dh-make
…
mv ~/build/groonga-tokenizer-yadelimit_1.0.0-1_amd64.deb /usr/local/src/groonga-tokenizer-yadelimit/public/ubuntu/precise/
```

#Ubuntu(trusty)

```
docker run -v /usr/local/src:/usr/local/src -i -t naoa/groonga-build-ubuntu-trusty /bin/bash
docker run -v /usr/local/src:/usr/local/src -i -t ubuntu:trusty /bin/bash

…
mv ~/build/groonga-tokenizer-yadelimit_1.0.0-1_amd64.deb /usr/local/src/groonga-tokenizer-yadelimit/public/ubuntu/trusty/
```

