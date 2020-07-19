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
