#include <math.h>
#include <uWS/uWS.h>
#include <iostream>
#include <string>
#include "json.hpp"
#include "PID.h"
#include "twiddle.h"

// for convenience
using nlohmann::json;
using std::string;
using std::vector;

// For converting back and forth between radians and degrees.
constexpr double pi() { return M_PI; }
double deg2rad(double x) { return x * pi() / 180; }
double rad2deg(double x) { return x * 180 / pi(); }

// Checks if the SocketIO event has JSON data.
// If there is data the JSON object in string format will be returned,
// else the empty string "" will be returned.
string hasData(string s) {
  auto found_null = s.find("null");
  auto b1 = s.find_first_of("[");
  auto b2 = s.find_last_of("]");
  if (found_null != string::npos) {
    return "";
  }
  else if (b1 != string::npos && b2 != string::npos) {
    return s.substr(b1, b2 - b1 + 1);
  }
  return "";
}

int main() {
  uWS::Hub h;

  PID steer_pid;
  PID speed_pid;

  bool enableTwiddle = false;
  /**
   * TODO: Initialize the steer_pid variable.
   */
  
  //Intial start

  vector<double> init_coef = {0.14,0.0,6.8};
  vector<double> del_init_coef = {0.1,0.1,0.2};

  steer_pid.Init(init_coef[0],init_coef[1],init_coef[2]);
  speed_pid.Init(0.1,0.0,0.5);


  float tolerance = 0.2;
  
  Twiddle twiddle = Twiddle(tolerance,init_coef,del_init_coef);

  //only P
  //steer_pid.Init(1.0,0.0,0.0);

  //Only I
  //steer_pid.Init(0.0,1.0,0.0);

  //Only D
  //steer_pid.Init(0.0,0.0,1.0);

  double MAX_CTE = 2.6;
  double cte_error = 0;

  static unsigned min_start = 100u;
  static unsigned max_iter = 4000u;
  double min_speed = 3.0;
  double max_angle = 25.0;
  double max_throttle = 0.3;

  h.onMessage([&](uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length,
                     uWS::OpCode opCode) {
    // "42" at the start of the message means there's a websocket message event.
    // The 4 signifies a websocket message
    // The 2 signifies a websocket event
    if (length && length > 2 && data[0] == '4' && data[1] == '2') {
      auto s = hasData(string(data).substr(0, length));

      if (s != "") {
        auto j = json::parse(s);

        string event = j[0].get<string>();

        if (event == "telemetry") {
          // j[1] is the data JSON object
          double cte = std::stod(j[1]["cte"].get<string>());
          double speed = std::stod(j[1]["speed"].get<string>());
          double angle = std::stod(j[1]["steering_angle"].get<string>());
          double steer_value,throttle_value;
          /**
           * TODO: Calculate steering value here, remember the steering value is
           *   [-1, 1].
           * NOTE: Feel free to play around with the throttle and speed.
           *   Maybe use another PID controller to control the speed!
           */
          
          static unsigned error = 0u;

          if(enableTwiddle == true)
          {
            if(error > min_start && (abs(cte) > MAX_CTE  || speed < min_speed || error > max_iter))
            {

              vector<double> params = steer_pid.getCoefficients();
            
              bool isTwiddle = twiddle.Optimize(cte_error,params);

              const string msg = "42[\"reset\",{}]";
              ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);

              vector<double> new_params = twiddle.getCoefficients();
              steer_pid.Init(new_params[0],new_params[1],new_params[2]);
              std::cout << "Kp: " << new_params[0] << " , Ki: " << new_params[1] << " , Kd: " << new_params[2] << std::endl;
            
              error = 0u;
            }
          }

          cte_error += pow(cte,2);
          error++;

          steer_pid.UpdateError(cte);
          steer_value = steer_pid.TotalError();
          steer_value = steer_value > 1.0 ? 1.0 : steer_value < -1.0 ? -1.0 : steer_value;

          double target_speed = 25.0 + 5*(1-fabs(angle/max_angle));
          speed_pid.UpdateError(speed - target_speed);
          
          throttle_value = speed_pid.TotalError();
          throttle_value = throttle_value > max_throttle ? max_throttle : throttle_value  < -max_throttle ? -max_throttle : throttle_value;

          /*
          
          // DEBUG
          std::cout << "CTE: " << cte << " Steering Value: " << steer_value 
                    << std::endl;
          */
          
          json msgJson;
          msgJson["steering_angle"] = steer_value;
          msgJson["throttle"] = throttle_value;
          
          
          auto msg = "42[\"steer\"," + msgJson.dump() + "]";
          //std::cout << msg << std::endl;
          
          ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);

          
        }  // end "telemetry" if
      } else {
        // Manual driving
        string msg = "42[\"manual\",{}]";
        ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
      }
    }  // end websocket message if
  }); // end h.onMessage

  h.onConnection([&h](uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req) {
    std::cout << "Connected!!!" << std::endl;
  });

  h.onDisconnection([&h](uWS::WebSocket<uWS::SERVER> ws, int code, 
                         char *message, size_t length) {
    ws.close();
    std::cout << "Disconnected" << std::endl;
  });

  int port = 4567;
  if (h.listen(port)) {
    std::cout << "Listening to port " << port << std::endl;
  } else {
    std::cerr << "Failed to listen to port" << std::endl;
    return -1;
  }
  
  h.run();
}