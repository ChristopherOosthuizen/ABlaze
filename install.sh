
#!/bin/sh
git clone https://github.com/google/googletest.git Google_tests/googletest
mkdir build
cd build
cmake ../
make
cd ../

echo -e "\ninstall:">> Makefile
echo -e "\tcp build/ablaze /usr/bin/" >> Makefile
echo -e "\trm -rf /usr/lib/ablaze/" >> Makefile
echo -e "\tcp -R Lib/ /usr/lib/ablaze/" >> Makefile