apt update
apt install cmake g++ -y
apt install zip wget curl git -y


echo "Install coverage... "
apt install gcovr -y

echo "Installing easy-logger... "
wget https://github.com/ahmad88me/easy-logger/archive/v1.0.zip
unzip v1.0.zip
rm  v1.0.zip
cd easy-logger-1.0;make install;cd ..;rm -Rf easy-logger-1.0

echo "Installing tabular-parser... "
wget https://github.com/ahmad88me/tabular-parser/archive/v1.2.zip
unzip v1.2.zip
rm v1.2.zip
cd tabular-parser-1.2;make install;cd ..;rm -Rf tabular-parser-1.2

echo "Installing tada-hdt-entity... "
wget https://github.com/oeg-upm/tada-hdt-entity/archive/v1.3.zip
unzip v1.3.zip
rm v1.3.zip
cd tada-hdt-entity-1.3;make install;cd ..;rm -Rf tada-hdt-entity-1.3

echo "update linker caches..."
ldconfig
