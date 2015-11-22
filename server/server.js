var express = require("express");
// var Firebase = require('firebase');
// var firebase = new Firebase('https://tatertots.firebaseio.com/');

var port = process.env.PORT || 3000;

var app = express();

app.use(express.static(__dirname + '/../client'));

// URL syntax: http://host/path?id={device}&time={time}&key1={var1}&key2={var2}...
// Available variables: device, time, duplicate, snr, station, data, avgSignal, lat, lng, rssi, seqNumber
app.get('/data', function(req, res){
  var params = req.query;
  console.log(params)
  firebase.child("parkingSpots").push(params);

  res.send(JSON.stringify(params));
});



// window.setInterval(function(){

// }, 5000)

// firebase.child('data').child('ABC').on('child_added', function(DataSnapshot){
//   console.log(DataSnapshot);
//   console.log("changed!!");

// })

var server = app.listen(port, function () {
  var host = server.address().address;
  var port = server.address().port;

  console.log('Listening at http://%s:%s', host, port);
});
