import React, { Component } from "react";
import { hot } from "react-hot-loader";
import SearchBox from "./SearchBox";
import Results from "./Results";
import { BrowserRouter as Router, Route } from "react-router-dom";
import "./App.css";

class App extends Component {
  render() {
    return (
      <div className="App">
        <Router exact path="/">
          <div className="searchbar">
            <img src="https://upload.wikimedia.org/wikipedia/en/thumb/8/80/Wikipedia-logo-v2.svg/1920px-Wikipedia-logo-v2.svg.png" />
            <SearchBox />
          </div>
          <Route exact path="/" component={Results} />
        </Router>
      </div>
    );
  }
}

export default hot(module)(App);
