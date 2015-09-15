var express = require('express');
var http = require('http');
var path = require('path');

var app = express();

app.set('views', path.join(__dirname, 'views'));
app.set('view engine', 'mustache');
app.engine('mustache', require('hogan-middleware').__express);
//app.use(express.static(path.join(__dirname, 'public')));

app.get('/', function (req, res) {
  
})


var server = app.listen(3000, function () {
  var host = server.address().address
  var port = server.address().port
  console.log('Example app listening at http://%s:%s', host, port)
})