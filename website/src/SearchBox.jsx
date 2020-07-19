import React, { Component } from "react";
import trieJson from "./assets/trie-es-50K.json";

const maxSuggestions = 50;

const Icon = (
  <svg viewBox="0 0 20 20">
    <path d="M18.125,15.804l-4.038-4.037c0.675-1.079,1.012-2.308,1.01-3.534C15.089,4.62,12.199,1.75,8.584,1.75C4.815,1.75,1.982,4.726,2,8.286c0.021,3.577,2.908,6.549,6.578,6.549c1.241,0,2.417-0.347,3.44-0.985l4.032,4.026c0.167,0.166,0.43,0.166,0.596,0l1.479-1.478C18.292,16.234,18.292,15.968,18.125,15.804 M8.578,13.99c-3.198,0-5.716-2.593-5.733-5.71c-0.017-3.084,2.438-5.686,5.74-5.686c3.197,0,5.625,2.493,5.64,5.624C14.242,11.548,11.621,13.99,8.578,13.99 M16.349,16.981l-3.637-3.635c0.131-0.11,0.721-0.695,0.876-0.884l3.642,3.639L16.349,16.981z"></path>
  </svg>
);

const separateLastWord = (str) => {
  let words = str.split(" ");
  let last = words.pop();
  return [words.join(" "), last];
};

class SearchBox extends Component {
  state = {
    previousInput: "",
    input: "",
    currentTrie: trieJson,
    inputDifference: "",
  };

  resetTrieInput = () => {
    this.setState({ currentTrie: trieJson, inputDifference: "", input: "" });
  };

  setNewInput = (newInput) => {
    let previousInput;
    [previousInput, newInput] = separateLastWord(newInput);
    let oldInput = this.state.input;
    let regexMatches = newInput.match(RegExp(`^${oldInput}(.*)`));

    /* Update previous input if the total input has more
       than one word (previousInput is "") or if the stored
       previous input needs to be reseted
    */
    if (previousInput || this.state.previousInput)
      this.setState({ previousInput: previousInput });

    this.setState({ input: newInput });
    if (!regexMatches) {
      this.setState({ currentTrie: trieJson });
      this.setState({ inputDifference: newInput });
      this.updateTrie(newInput, trieJson);
      return;
    }

    this.setState({ inputDifference: regexMatches[1] });
    /* Remember: setState is asynchronous */
    this.updateTrie(regexMatches[1]);
  };

  updateTrie = (inputDifference, trie = null) => {
    if (!trie) trie = this.state.currentTrie;
    for (let idx = 0; trie && idx < inputDifference.length; idx++)
      trie = trie[inputDifference.charAt(idx)];

    this.setState({ currentTrie: trie });
  };

  wordsFromTrie = (trie, words, prefix) => {
    if (words.length>maxSuggestions) return;
    if (trie.end) words.push(prefix);
    for (const [key, value] of Object.entries(trie))
      if (key != "end") this.wordsFromTrie(value, words, prefix + key);
  };

  suggestions = () => {
    if (!this.state.currentTrie) return [];
    // let prefix = this.state.inputDifference? this.state.input : "";

    /* js doesn't pass by reference, var creates a global variable instead */
    var words = [];
    var { currentTrie } = this.state;
    this.wordsFromTrie(currentTrie, words, this.state.input);

    return words;
  };

  render() {
    return (
      <div className="searchbox">
        <input
          onChange={(e) => this.setNewInput(e.target.value)}
          value={this.state.previousInput + " " + this.state.input}
          type="text"
          placeholder="Busca artículos de Wikipedia..."
        />
        <button onClick={() => console.log(trieJson)}>{Icon}</button>
        {this.state.input && (
          <div className="searchbox-autocomplete">
            {
              this.suggestions().map((w, i) => (
              <div
                onClick={() =>
                  this.setNewInput(this.state.previousInput + " " + w)
                }
                key={i}
              >
                {this.state.previousInput + " " + w}
              </div>
              ))
            }
          </div>
        )}
      </div>
    );
  }
}

export default SearchBox;
