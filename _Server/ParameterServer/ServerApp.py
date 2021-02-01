from flask import Flask

flaskApplication = Flask(__name__)

@flaskApplication.route('/')
def index():
    return "Hello, World!"
flaskApplication.add_url_rule('/index', 'index', index)

from FanControl import FanControl 
moduleFanControl = FanControl.FanControl('/FanControl', 'FanControl')
moduleFanControl.Route(flaskApplication)

if __name__ == '__main__':
    flaskApplication.run(host='0.0.0.0', port=int("8080"), debug=True)