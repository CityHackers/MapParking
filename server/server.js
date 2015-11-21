var express = require('express');
var cors = require('cors');
var bodyParser = require('body-parser');


var app = express();

app.use(cors());
app.use(bodyParser.json());

app.use(express.static(__dirname + '/../client'));

app.listen(process.env.PORT || 3000);
