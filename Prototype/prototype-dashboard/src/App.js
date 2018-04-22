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
      temperature : 0,
      humidity : 0,
      data : {
        labels : ['CH4','CO','CO2','Smoke'],
        datasets : [
          {
            label : 'Gas-Data',
            backgroundColor: ['rgba(127,255,127,0.5)','rgba(128,128,128,0.5)','rgba(0,0,0,0.7)'],
            borderWidth: 1,
            hoverBorderColor: 'rgba(0,0,0,1)',
            data: [0, 0, 0, 0]
          }
        ]
      }};
  }

  componentDidMount() {
    const rootRef = firebase.database().ref().child('proto_entries');
    rootRef.on('value', snap => {
      var temp = snap.val();
      console.log(temp);
      var temp_list = Object.keys(temp);
      temp = temp[temp_list[temp_list.length-1]];
      console.log(temp);
      var keys = Object.keys(temp);
      console.log(keys);
      var l = [];
      for(var i=0;i<keys.length-1;i++){
        l.push(temp[keys[i]]);
      }
      console.log(l);
      this.setState({
        temperature : temp["temperature"],
        humidity : temp["humidity"],
        data : {
          labels : ['CH4','CO','CO2','Smoke'],
          datasets : [
            {
              label : 'Gas-Data',
              backgroundColor: ['rgba(127,255,127,0.7)','rgba(255,51,51,0.7)','rgba(128,128,128,0.7)','rgba(0,0,0,0.7)'],
              borderWidth: 1,
              hoverBorderColor: 'rgba(0,0,0,1)',
              data: l
            }
          ]
        }
      });
    })
  }
  
  render() {
    return (
      <div className="App">
        <div className="Header">
            <h1>Pollution Monitoring Dashboard</h1>
        </div>
        <div className = "HandT">
          <div className = "Temp">
              Temperature : {this.state.temperature} &#8451;
          </div>
          <div className = "Humi">
              Humidity : {this.state.humidity} %
          </div>
        </div>
        <div className="Chart">
          <Bar
              data={this.state.data}
              options={{
                maintainAspectRatio : false,
                scales: {
                  yAxes: [{
                    scaleLabel: {
                      display : true,
                      labelString : 'Concentration (ppm)',
                    },
                    ticks: {
                      beginAtZero: true,
                      min : 0,
                      max : 500
                    }
                  }],
                  xAxes: [{
                    scaleLabel: {
                      display: true,
                      labelString: 'Gases',
                    }
                  }],
                }
              }}
            />
        </div>
      </div>
    );
  }
}

export default App;
