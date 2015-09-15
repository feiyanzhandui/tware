var http = require('http'); // this is new
var express = require('express');
var app = express();
var anyDB = require('any-db');
var engines = require('consolidate');
var server = http.createServer(app);

app.engine('html', engines.hogan); // tell Express to run .html files through Hogan
app.set('views', __dirname + '/templates'); // tell Express where to find templates

app.use('/public', express.static(__dirname + '/public'));
app.use(express.bodyParser()); // definitely use this feature

app.get("/", function(req, res) {
    res.render('index.html');
});

app.get("/cluster", function(req, res) {
    res.render('cluster.html');
});

app.get("/job", function(req, res) {
    res.render('job.html');
});

server.listen(8080);

