import React, { Component } from "react";

const server = "http://localhost:18080"

class Results extends Component {
  state = {
    query: "",
    articles: []
  };

  componentDidUpdate(props, state){
    let {search: q} = props.location;
    if (q === state.query)
      return null;

    let newState = { query: q };

    let url = `${server}/search${q}`;
    fetch(url)
      .then(r => r.json())
      .then(data => {
        newState.articles = data;
        this.setState(newState);
      });
  }

  render() {
    let { articles } = this.state;
    return (
      <div className="resultsContainer">
      {
        this.state.query && articles.length &&
        articles.map((article) => (
          <div key={article.id} className="result">
            <a href="#">
              <h3>{article.title}</h3>
            </a>
            <div className="keywords">
              <b>Palabras clave:</b>
              {
                article.keywords.map((kw, i) => (
                  <span key={i}>{kw}</span>
                ))
              }
            </div>
            <p>{article.description}</p>
          </div>
        ))
      }
      {!articles.length && "No hay resultados"}
      </div>
    );
  }
}

export default Results;
