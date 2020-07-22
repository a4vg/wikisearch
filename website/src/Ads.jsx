import React, { Component } from 'react';

const ads_server = "http://localhost:5000"

class Ads extends Component {
  state = {
    ads: []
  }

  updateAds = () => {
    let url = `${ads_server}/ads${this.props.query}`;
    fetch(url)
    .then(r => r.json())
      .then(data => {
        this.setState({ads: data});
      });
  }

  componentDidMount(){
    this.updateAds();
  }

  componentDidUpdate(prevpros){
    if (this.props.query === prevpros.query)
      return null;

    this.updateAds();
  }
  // http://i.linio.com/p/cc2d14d3d3aaf17ba3ee1788a475240b-catalog.jpg
  // http://i.linio.com/p/cc2d14d3d3aaf17ba3ee1788a475240b-catalog.jpg

  render() {
    return (
      <React.Fragment>
        <h3 className="ads-title">Ads</h3>
        <div className="ad-container">
        {
          this.state.ads.map( (ad, id) => (
            <div key={id} className="result ad">
              <a href={ad.link}>
                <h3>{ad.title}</h3>
              </a>
              <p>{ad.price}</p>
            </div>
          ))
        }
        </div>
      </React.Fragment>
    );
  }
}

export default Ads;