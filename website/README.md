# WikiSearch Website

## Features
### Autocomplete
* Powered by a pregenerated trie in JSON with the [JSON-Trie format](https://github.com/fluorine/JSON-Trie).
* JSON Tries are available in assets. More tries can be generated from a list of words with [this script](../DataProcessing/jsonTrie.cpp), which uses the [Trie class](../DataProcessing/include/Trie.h#L42).

## Run
Install dependencies and start React app
```
npm i
npm run start
```

## Requirements
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
