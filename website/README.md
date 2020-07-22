# WikiSearch Website

## Features
### Search
* Search through Wikipedia articles indexed in a B+Tree.
* Indexes are generated with this [script](../DataProcessing/genIndexServer.cpp) in Data Processing.
* Get a small description and keywords of each result.
* Search results are fetched from the [Crow search server](server.cpp).

### Autocomplete
* Powered by a pregenerated trie in JSON with the [JSON-Trie format](https://github.com/fluorine/JSON-Trie).
* JSON Tries are available in assets. More tries can be generated from a list of words with [this script](../DataProcessing/jsonTrie.cpp), which uses the [Trie class](../DataProcessing/include/Trie.h#L42).

### Ads
* [A Flask app](ads_server.py) scrapes Linio and gets the first n products.
* Links and prices to Linio products are displayed in WikiSearch along with Wikipedia results.

## Run
### Frontend
Install dependencies and start React app
```
npm i
npm run start
```
### Search server
Compile and start the server. Crow framework is required.
```
make
./server
```
### Ads server
Install Python 3 and Flask and start app
```
python3 ads_server.py
```

## Requirements
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
