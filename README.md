# WikiSearch
Proyecto 2020-1 de UTEC-CS3102

## Goal
Google-like search engine for Wikipedia articles.

## Demo
[Here](https://drive.google.com/file/d/1N9Nu2maqrgRKkS9oCKuiCW6tHnPwLecP/view?usp=sharing)

## Data Processing
1. `ZimManager.h`: Read data from zim file
2. `htmlParser.h`: Parse wikipedia article html
3. `Preprocess.h`: Preprocess text and get a word count

### Run
After installing the requirements and in `DataProcessing` directory.
```bash
make
./processing data/zim/wiki-mini.zim
# word count of each article in wiki-mini.zim
```

If `error while loading shared libraries`, run `/sbin/ldconfig -v`

#### Examples
The `examples` directory contains examples for each main component.
The command to execute an example can be found on the first line of the file.

### Requirements
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


## WikiSearch Website

### Features
#### Search
* Search through Wikipedia articles indexed in a B+Tree.
* Indexes are generated with this [script](../DataProcessing/genIndexServer.cpp) in Data Processing.
* Get a small description and keywords of each result.
* Search results are fetched from the [Crow search server](server.cpp).

#### Autocomplete
* Powered by a pregenerated trie in JSON with the [JSON-Trie format](https://github.com/fluorine/JSON-Trie).
* JSON Tries are available in assets. More tries can be generated from a list of words with [this script](../DataProcessing/jsonTrie.cpp), which uses the [Trie class](../DataProcessing/include/Trie.h#L42).

#### Ads
* [A Flask app](ads_server.py) scrapes Linio and gets the first n products.
* Links and prices to Linio products are displayed in WikiSearch along with Wikipedia results.

### Run
#### Frontend
Install dependencies and start React app
```
npm i
npm run start
```
#### Search server
Compile and start the server. Crow framework is required.
```
make
./server
```
#### Ads server
Install Python 3 and Flask and start app
```
python3 ads_server.py
```

### Requirements
* Python Flask
* npm
* Crow dependencies
From [mrozigor/crow](https://github.com/mrozigor/crow#installing-missing-dependencies) forked from the original [crow](https://github.com/ipkn/crow)

**Ubuntu**
```
sudo apt-get install build-essential libtcmalloc-minimal4 && sudo ln -s /usr/lib/libtcmalloc_minimal.so.4 /usr/lib/libtcmalloc_minimal.so
```
**OSX**
```
brew install boost google-perftools
```

