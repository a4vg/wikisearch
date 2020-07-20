import React, { Component } from "react";

class Results extends Component {
  state = {
    articles: [
      {
        id: 1,
        description:
          "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Praesent eu mauris molestie, gravida augue vitae, semper tellus. Proin gravida justo vitae finibus tempus. Pellentesque aliquam efficitur magna. Etiam fringilla, diam ac fringilla suscipit, ante orci consequat eros, at placerat turpis augue nec risus. Vestibulum tincidunt in ex nec molestie. Praesent et mattis nisi. Phasellus efficitur efficitur fermentum. Proin pharetra volutpat purus at vehicula. Proin non lacus sit amet ipsum imperdiet sollicitudin ultrices quis urna. Phasellus nec lectus dui. Nulla commodo at lorem at ultricies. Maecenas eget nunc eu lectus mattis consectetur. Aenean dapibus, justo vitae suscipit condimentum, enim dui pretium.",
        title: "Título de artículo",
        keywords: ["ipsum", "etiam", "aenean"],
      },
      {
        id: 2,
        description:
          "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Praesent eu mauris molestie, gravida augue vitae, semper tellus. Proin gravida justo vitae finibus tempus. Pellentesque aliquam efficitur magna. Etiam fringilla, diam ac fringilla suscipit, ante orci consequat eros, at placerat turpis augue nec risus. Vestibulum tincidunt in ex nec molestie. Praesent et mattis nisi. Phasellus efficitur efficitur fermentum. Proin pharetra volutpat purus at vehicula. Proin non lacus sit amet ipsum imperdiet sollicitudin ultrices quis urna. Phasellus nec lectus dui. Nulla commodo at lorem at ultricies. Maecenas eget nunc eu lectus mattis consectetur. Aenean dapibus, justo vitae suscipit condimentum, enim dui pretium.",
        title: "Título de artículo",
        keywords: ["ipsum", "etiam", "aenean"],
      },
      {
        id: 3,
        description:
          "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Praesent eu mauris molestie, gravida augue vitae, semper tellus. Proin gravida justo vitae finibus tempus. Pellentesque aliquam efficitur magna. Etiam fringilla, diam ac fringilla suscipit, ante orci consequat eros, at placerat turpis augue nec risus. Vestibulum tincidunt in ex nec molestie. Praesent et mattis nisi. Phasellus efficitur efficitur fermentum. Proin pharetra volutpat purus at vehicula. Proin non lacus sit amet ipsum imperdiet sollicitudin ultrices quis urna. Phasellus nec lectus dui. Nulla commodo at lorem at ultricies. Maecenas eget nunc eu lectus mattis consectetur. Aenean dapibus, justo vitae suscipit condimentum, enim dui pretium.",
        title: "Título de artículo",
        keywords: ["ipsum", "etiam", "aenean"],
      },
    ],
  };

  getQuery = () => {
    let params = new URLSearchParams(this.props.location.search);
    return params.get("q");
  };

  render() {
    let { articles } = this.state;
    return (
      <div className="resultsContainer">
      {
        this.getQuery() &&
        articles.map((article) => (
          <div key={article.id} className="result">
            <a href="#">
              <h3>{this.getQuery()}</h3>
            </a>
            <div className="keywords">
              <b>Palabras clave:</b>
              {
                article.keywords.map((kw, kidx) => (
                  <span key={kidx}>{kw}</span>
                ))
              }
            </div>
            <p>{this.state.articles[0].description}</p>
          </div>
        ))
      }
      </div>
    );
  }
}

export default Results;
