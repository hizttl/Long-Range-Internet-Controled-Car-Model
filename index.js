const express = require("express");
const app = express();
const port = 3000;
const bodyParser = require("body-parser");
const lodash = require('lodash');
var testVar = "";
var router = express.Router();
var name1 = "";
var datatostring = "";
var ledColor = "";
var counterr = 0;
var counterr2 = 0;
var mysql = require("mysql");
// var timeout = require('connect-timeout');
var db = mysql.createPool({
  host: "localhost",
  user: "root",
  password: "",
  port: "3306",
  database: "iee3031project"
})

// function haltOnTimedout (req, res, next) {
//   if (!req.timedout) next()
// }


// app
app.use(bodyParser.urlencoded({extended: true}))

//static files
app.use(express.static('public'))
app.use('/CSS', express.static(__dirname + 'public/CSS'))
app.use('/img', express.static(__dirname + 'public/img'))
app.use('/js', express.static(__dirname + 'public/js'))

//set engines
app.set('views', './views')
app.set('view engine', 'ejs')

app.get('/', (req,res) => {
  res.render('index');
});

app.get('/led', (req, res) => {
  res.render('led');
});

app.post('/led/data', (req, res) => {
  ledColor=  req.body.ledColor;

  console.log(ledColor);

  res.status(204).send();
});

app.get('/led/data', (req, res) => {
  var led = {
    color: ledColor
  };
  res.send(JSON.stringify(led));
});

app.post('/form1', (req,res) => {
  name1 = req.body.name1;
  if (name1==="Mobil1"){
    counterr = 0;
  } else {
    counterr2 = 0;
  }
  // db.query("INSERT INTO Mobil(IdMobil) VALUES (?)",[name1] , function(err, rs) {
  //   if (err) throw err;
  //   // console.log("1 record inserted");
  // });
  db.query("UPDATE Mobil SET Movement = ? WHERE IdMobil = ?",[0, name1], function(err,rs) {
    if (err) throw err;
    // console.log("Sequence updated")
  });
  db.query("UPDATE Mobil SET Sequence = ? WHERE IdMobil = ?",[0, name1], function(err,rs) {
    if (err) throw err;
    // console.log("Sequence updated")
  });
  res.status(204).send();
  // res.redirect('/');
});

app.post('/formnext', (req,res) => {
  var correctorwrong = req.body.status[0];
  var currentName = req.body.status.slice(2,8);
  console.log(currentName);
  if (currentName === "Mobil1") {
    counterr = counterr + 1;
    console.log(counterr);
    db.query("UPDATE Mobil SET Movement = ? WHERE IdMobil = ?",[correctorwrong, "Mobil1"] , function(err, rs) {
      if (err) throw err;
    });
    db.query("UPDATE Mobil SET Sequence = ? WHERE IdMobil = ?",[counterr, "Mobil1"], function(err,rs) {
      if (err) throw err;
    });
    res.status(204).send();
  } else {
    counterr2 = counterr2 + 1;
    console.log(counterr2);
    db.query("UPDATE Mobil SET Movement = ? WHERE IdMobil = ?",[correctorwrong, "Mobil2"] , function(err, rs) {
      if (err) throw err;
    });
    db.query("UPDATE Mobil SET Sequence = ? WHERE IdMobil = ?",[counterr2, "Mobil2"], function(err,rs) {
      if (err) throw err;
    });
    res.status(204).send();
  }
});

app.get('/form1/:idMobil', (req,res) => {
  var requestedName = req.params.idMobil;
  var data = {
    IdMobil: requestedName
  }
  res.send(JSON.stringify(data));
})

app.get('/formnext/:idMobil', (req, res) => {
  var requested = req.params.idMobil;
  console.log(requested);
  // requested = lodash.camelCase(requested);
  db.query("SELECT * FROM Mobil Where IdMobil = ?", [requested], function(err,rs) {
    if (err) throw err;
    console.log(rs);
    datatostring = JSON.stringify(rs);
  })
  res.send(datatostring);
});

app.get('/test', (req,res) => {
  if (db!=null) {
    res.send("success");
  } else {
    res.send("fail");
  }
});

app.get('/show', function(req,res,next)  {
  db.query('SELECT * FROM Mobil WHERE IdMobil = ?',[name1], function(err,rs){
    if(err) {
      console.log(err);
    } else {
      console.log(rs);
    }
  });
});


app.listen(process.env.PORT || 3000, function(){
  console.log("Server started on port 3000");
});
