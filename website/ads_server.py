from flask import Flask, jsonify, Response, request
app = Flask(__name__)

import requests
import json
from bs4 import BeautifulSoup

MAX_ADS = 3

def getProduct(div, domain):
    product = {}

    product["title"] = div.a["title"]
    product["link"] = domain + div.a["href"]
    product["image"] = div.find("img", {"class": "image"})["data-lazy"].strip("/")
    product["price"] = div.find("div", {"class": "lowest-price"}).span.get_text(strip=True)

    return product

def getAds(query):
    domain = "https://www.linio.com.pe"
    endpoint = "/search"

    url = domain + endpoint
    ads = [] # title, href, image, price
    
    p = {"q": query}

    r = requests.get(url, params=p)
    soup = BeautifulSoup(r.text, "html.parser")

    products_div = soup.findAll("div", {"class":"catalogue-product row"})

    for idx, product_div in enumerate(products_div):
        if idx>MAX_ADS:
            break

        product = getProduct(product_div, domain)
        if not any(product["title"] == p["title"] for p in ads):
            ads.append(product)

    return ads


@app.route('/ads')
def ads():
    query = request.args.get('q')
    if not query:
        return jsonify({"error": "Missing query parameter"})

    ads = getAds(query)
    return Response(json.dumps(ads),  mimetype='application/json')

@app.after_request
def apply_cors(response):
    response.headers["Access-Control-Allow-Origin"] = "*"
    return response

if __name__ == '__main__':
    app.run()





