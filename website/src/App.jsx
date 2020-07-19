import React, { Component} from "react";
import {hot} from "react-hot-loader";
import SearchBox from "./SearchBox";
import Results from "./Results";
import "./App.css";

class App extends Component{
  render(){
    return(
      <div className="App">
        <div className="searchbar">
          <img src="https://upload.wikimedia.org/wikipedia/en/thumb/8/80/Wikipedia-logo-v2.svg/1920px-Wikipedia-logo-v2.svg.png"/>
          <SearchBox/>
        </div>
        <Results/>
      </div>
    );
  }
}

export default hot(module)(App);