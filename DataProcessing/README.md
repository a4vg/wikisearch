# Data Processing
1. `ZimManager.h`: Read data from zim file
2. `htmlParser.h`: Parse wikipedia article html
3. `Preprocess.h`: Preprocess text and get a word count

## Run
After installing the requirements and in `DataProcessing` directory.
```bash
make
./processing data/zim/wiki-mini.zim
# word count of each article in wiki-mini.zim
```

If `error while loading shared libraries`, run `/sbin/ldconfig -v`

### Examples
The `examples` directory contains examples for each main component.
The command to execute an example can be found on the first line of the file.

## Requirements
* zimlib
```bash
wget http://ftp.debian.org/debian/pool/main/z/zimlib/zimlib_1.2.orig.tar.gz -O zimlib-1.2.tar.gz
tar xf zimlib-1.2.tar.gz
cd zimlib-1.2
./configure
make
make install
```

* my-html
```bash
git clone https://github.com/lexborisov/myhtml.git
cd myhtml
make
make test
sudo make install
```
