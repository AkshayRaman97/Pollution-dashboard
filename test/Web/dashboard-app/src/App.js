import React, { Component } from 'react';
import './App.css';
import {Bar} from 'react-chartjs-2';
import * as firebase from 'firebase';

// Initialize Firebase
var config = {
    apiKey: "AIzaSyCYHafn5GJ8hhKD8bfmEyRSeu3EieWW4Mk",
    authDomain: "air-quality-dashboard.firebaseapp.com",
    databaseURL: "https://air-quality-dashboard.firebaseio.com",
    projectId: "air-quality-dashboard",
    storageBucket: "air-quality-dashboard.appspot.com",
    messagingSenderId: "1067693693969"
  };
firebase.initializeApp(config);

class App extends Component {
  constructor(){
    super();
    this.state = {
      test : [],
      data : {
        labels : ['gas1','gas2','gas3','gas4'],
        datasets : [
          {
            label : 'gas-data',
            backgroundColor: ['rgba(0,0,0,0.3','rgba(0,0,0,0.3','rgba(0,0,0,0.3','rgba(0,0,0,0.3'],
            borderWidth: 1,
            hoverBorderColor: 'rgba(0,0,0,1)',
            data: [0, 0, 0, 0]
          }
        ]
      }};
  }

  componentDidMount() {
    const rootRef = firebase.database().ref().child('entries');
    rootRef.on('value', snap => {
      this.setState({
        test : snap.val()
      });
    var keys = Object.keys(this.state.test);
    var l = [];
    var obj = this.state.test[keys[keys.length-1]];
    for(var gas in obj){
      l.push(obj[gas]);
    }
    console.log(l);
    var bgcolor = this.getBackgroundColor(l);
    console.log(bgcolor);

    this.setState({
        data : {
        labels : ['gas1','gas2','gas3','gas4'],
        datasets : [
          {
            label : 'Gas-data',
            backgroundColor: bgcolor,
            borderWidth: 1,
            data: l.slice(0,-1)
           }
          ]
        }
      });
    })
  }
  
getBackgroundColor(l){
    var bgc = [];
    for(var i=0;i<l.length;i++){
      var n = parseInt(l[i],10);
      if(n > 4){
        bgc.push("rgba(255,50,50,0.8)");
      }
      else if(n > 3){
        bgc.push("rgba(255,192,76,0.8)");
      }
      else if(n > 2){
        bgc.push("rgba(255,255,127,0.8)");
      }
      else if(n > 1){
        bgc.push("rgba(127,191,127,0.8)");
      }
      else {
        bgc.push("rgba(76,166,255,0.8)");
      }
      console.log(l[i] + ' : ' + bgc[i]);
    }
    return bgc;
  }
  render() {
    return (
      <div className="App">
      <div className="Header">
          <h1>Pollution Monitoring Dashboard</h1>
      </div>
      <div className="Chart">
        <Bar
            data={this.state.data}
            options={{
              maintainAspectRatio : false,
              scales: {
                yAxes: [{
                  ticks: {
                    beginAtZero: true,
                    min : 0,
                    max : 7
                  }
                }]
              }
            }}
      />
      </div>
      <p></p>
      </div>
    );
  }
}

export default App;
