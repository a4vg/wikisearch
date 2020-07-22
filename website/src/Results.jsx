import React, { Component } from "react";
import Ads from "./Ads";
import { Link } from "react-router-dom";

const server = "http://localhost:18080"

class Results extends Component {
  state = {
    query: "",
    articles: [], 
    time: 0
  };

  updateState = () => {
    let {search: q} = this.props.location;
    if (q === this.state.query)
      return null;

    let newState = { query: q };

    let url = `${server}/search${q}`;
    fetch(url)
      .then(r => r.json())
      .then(data => {

        newState.articles = data.articles;
        newState.time = data.time;
        this.setState(newState);
      });
  }

  componentDidMount(){
    this.updateState();
  }

  componentDidUpdate(){
    this.updateState();
  }

  render() {
    let { articles } = this.state;
    let { time } = this.state;
    return (
      <div className="resultsContainer">
      {
        this.state.query && articles.length
        ?<React.Fragment>
        <h3>Se han encontrado { articles.length } resultados en {time} milisegundos</h3>
        <Ads query={this.state.query}/>
        { articles.map((article) => (
          <div key={article.id} className="result">
            <a href={`${server}/article/${article.id}`}>
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
        </React.Fragment>
        :<h3>No hay resultados</h3>
      }
      </div>
    );
  }
}

export default Results;
