#include "web.h"

// ============================================================
// PAGINA WEB
// ============================================================

void handleRoot() {

  String estadoMPU =
    control_activo ? "MPU ON" : "MPU OFF";

  String estadoMotor1 =
    motor1_activo ? "MOTOR 1 ON" : "MOTOR 1 OFF";

  String estadoMotor2 =
    motor2_activo ? "MOTOR 2 ON" : "MOTOR 2 OFF";

  String estadoMotor3 =
    motor3_activo ? "MOTOR 3 ON" : "MOTOR 3 OFF";
  
  String estadoMotor4 =
    motor4_activo ? "MOTOR 4 ON" : "MOTOR 4 OFF";


  String html =
    "<!DOCTYPE html>"
    "<html>"
    "<head>"
    "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"

    "<style>"

    "body {"
    "  font-family: Arial, sans-serif;"
    "  text-align: center;"
    "  background: #111;"
    "  color: white;"
    "  margin: 0;"
    "  padding: 20px;"
    "}"

    "h2 {"
    "  margin-bottom: 25px;"
    "}"

    "button {"
    "  font-size: 18px;"
    "  padding: 12px 25px;"
    "  margin: 8px;"
    "  border-radius: 8px;"
    "  border: none;"
    "}"

    ".datos {"
    "  display: flex;"
    "  justify-content: center;"
    "  gap: 15px;"
    "  flex-wrap: wrap;"
    "}"

    ".dato {"
    "  background: #222;"
    "  border-radius: 10px;"
    "  padding: 15px;"
    "  width: 90px;"
    "}"

    ".titulo {"
    "  font-size: 14px;"
    "  color: #aaa;"
    "}"

    ".valor {"
    "  font-size: 25px;"
    "  margin-top: 8px;"
    "}"

    ".motor {"
    "  background: #222;"
    "  padding: 20px;"
    "  margin: 20px auto;"
    "  border-radius: 10px;"
    "  max-width: 400px;"
    "}"



    "</style>"
    "</head>"

    "<body>"

    "<h2>DRON ESP32-C3</h2>"

    "<button onclick=\"fetch('/stop')\" "
    "style=\"background:red;color:white;font-size:20px;padding:15px;\">"
    "🛑 PARAR TODOS LOS MOTORES"
    "</button>"


    "<button onclick=\"fetch('/motores/on')\">"
    "ENCENDER 4 MOTORES"
    "</button>"


    // ========================================================
    // MOTOR 1
    // ========================================================

    "<div class=\"motor\">"

    "<h3>MOTOR 1</h3>"

    "<p id=\"motor1Estado\">" 
    + estadoMotor1 +
    "</p>"

    "<button onclick=\"toggleMotor1()\">"
    "MOTOR 1 ON / OFF"
    "</button>"

    "<p>GPIO4 - PWM: "
    + String(motor1_pwm) +
    "/255"
    "</p>"

    "</div>"

    // ========================================================
    // MOTOR 2
    // ========================================================

    "<div class=\"motor\">"

    "<h3>MOTOR 2</h3>"

    "<p id=\"motor2Estado\">"
    + estadoMotor2 +
    "</p>"

    "<button onclick=\"toggleMotor2()\">"
    "MOTOR 2 ON / OFF"
    "</button>"

    "<p>GPIO3 - PWM: "
    + String(motor2_pwm) +
    "/255"
    "</p>"

    "</div>"


    // ========================================================
    // MOTOR 3
    // ========================================================

    "<div class=\"motor\">"

    "<h3>MOTOR 3</h3>"

    "<p id=\"motor3Estado\">"
    + estadoMotor3 +
    "</p>"

    "<button onclick=\"toggleMotor3()\">"
    "MOTOR 3 ON / OFF"
    "</button>"

    "<p>GPIO2 - PWM: "
    + String(motor3_pwm) +
    "/255"
    "</p>"

    "</div>"

   
    // ========================================================
    // MOTOR 4
    // ========================================================


     "<div class=\"motor\">"

    "<h3>MOTOR 4</h3>"

    "<p id=\"motor4Estado\">"
    + estadoMotor4 +
    "</p>"

    "<button onclick=\"toggleMotor4()\">"
    "MOTOR 4 ON / OFF"
    "</button>"

    "<p>GPIO1 - PWM: "
    + String(motor4_pwm) +
    "/255"
    "</p>"

    "</div>"

    // ========================================================
    // MPU
    // ========================================================

    "<div>"
    "<p id=\"estado\">"
    + estadoMPU +
    "</p>"

    "<button onclick=\"toggleMPU()\">"
    "ACTIVAR / DESACTIVAR MPU"
    "</button>"
    "</div>"

    "<br>"

    "<div class=\"datos\">"

    "<div class=\"dato\">"
    "<div class=\"titulo\">ROLL</div>"
    "<div class=\"valor\" id=\"roll\">--</div>"
    "</div>"

    "<div class=\"dato\">"
    "<div class=\"titulo\">PITCH</div>"
    "<div class=\"valor\" id=\"pitch\">--</div>"
    "</div>"

    "<div class=\"dato\">"
    "<div class=\"titulo\">YAW</div>"
    "<div class=\"valor\" id=\"yaw\">--</div>"
    "</div>"

    "</div>"

    "<br>"

    "<div class=\"datos\">"

    "<div class=\"dato\">"
    "<div class=\"titulo\">ACC X</div>"
    "<div class=\"valor\" id=\"ax\">--</div>"
    "</div>"

    "<div class=\"dato\">"
    "<div class=\"titulo\">ACC Y</div>"
    "<div class=\"valor\" id=\"ay\">--</div>"
    "</div>"

    "<div class=\"dato\">"
    "<div class=\"titulo\">ACC Z</div>"
    "<div class=\"valor\" id=\"az\">--</div>"
    "</div>"

    "</div>"

    "<br>"

    "<div class=\"datos\">"

    "<div class=\"dato\">"
    "<div class=\"titulo\">GYRO X</div>"
    "<div class=\"valor\" id=\"gx\">--</div>"
    "</div>"

    "<div class=\"dato\">"
    "<div class=\"titulo\">GYRO Y</div>"
    "<div class=\"valor\" id=\"gy\">--</div>"
    "</div>"

    "<div class=\"dato\">"
    "<div class=\"titulo\">GYRO Z</div>"
    "<div class=\"valor\" id=\"gz\">--</div>"
    "</div>"

    "</div>"

    "<br>"

    "<div>"
    "Temperatura: <span id=\"temp\">--</span> °C"
    "</div>"

    // ========================================================
    // JAVASCRIPT
    // ========================================================

    "<script>"

    "function actualizar() {"

    "fetch('/datos')"
    ".then(r => r.json())"
    ".then(d => {"

    "document.getElementById('roll').innerText = d.roll.toFixed(2);"
    "document.getElementById('pitch').innerText = d.pitch.toFixed(2);"
    "document.getElementById('yaw').innerText = d.yaw.toFixed(2);"

    "document.getElementById('ax').innerText = d.ax.toFixed(2);"
    "document.getElementById('ay').innerText = d.ay.toFixed(2);"
    "document.getElementById('az').innerText = d.az.toFixed(2);"

    "document.getElementById('gx').innerText = d.gx.toFixed(2);"
    "document.getElementById('gy').innerText = d.gy.toFixed(2);"
    "document.getElementById('gz').innerText = d.gz.toFixed(2);"

    "document.getElementById('temp').innerText = d.temp.toFixed(1);"

    "document.getElementById('estado').innerText = "
    "d.activo ? 'MPU ON' : 'MPU OFF';"

    "document.getElementById('motor1Estado').innerText = "
    "d.motor1 ? 'MOTOR 1 ON' : 'MOTOR 1 OFF';"

    "document.getElementById('motor2Estado').innerText = "
    "d.motor2 ? 'MOTOR 2 ON' : 'MOTOR 2 OFF';"

    "document.getElementById('motor3Estado').innerText = "
    "d.motor3 ? 'MOTOR 3 ON' : 'MOTOR 3 OFF';"
    

    "document.getElementById('motor4Estado').innerText = "
    "d.motor4 ? 'MOTOR 4 ON' : 'MOTOR 4 OFF';"
    
    "});"
    "}"

    "function toggleMotor1() {"
    "fetch('/motor1');"
    "}"

    "function toggleMotor2() {"
    "fetch('/motor2');"
    "}"

     "function toggleMotor3() {"
    "fetch('/motor3');"
    "}"

         "function toggleMotor4() {"
    "fetch('/motor4');"
    "}"


    "function toggleMPU() {"
    "fetch('/control');"
    "}"

    "setInterval(actualizar, 100);"

    "actualizar();"

    "</script>"

    "</body>"
    "</html>";

  server.send(200, "text/html", html);
}




// ============================================================
// DATOS
// ============================================================
void handleDatos() {

  sensors_event_t a;
  sensors_event_t g;
  sensors_event_t temp;

  mpu.getEvent(&a, &g, &temp);

  String json = "{";

  json += "\"roll\":" + String(roll, 2) + ",";
  json += "\"pitch\":" + String(pitch, 2) + ",";
  json += "\"yaw\":" + String(yaw, 2) + ",";

  json += "\"ax\":" + String(a.acceleration.x, 2) + ",";
  json += "\"ay\":" + String(a.acceleration.y, 2) + ",";
  json += "\"az\":" + String(a.acceleration.z, 2) + ",";

  json += "\"gx\":" + String(g.gyro.x, 2) + ",";
  json += "\"gy\":" + String(g.gyro.y, 2) + ",";
  json += "\"gz\":" + String(g.gyro.z, 2) + ",";

  json += "\"temp\":" + String(temp.temperature, 1) + ",";

  json += "\"activo\":"
       + String(control_activo ? "true" : "false")
       + ",";

  json += "\"motor1\":"
       + String(motor1_activo ? "true" : "false")
       + ",";

  json += "\"motor2\":"
       + String(motor2_activo ? "true" : "false")
       + ",";

  json += "\"motor3\":"
       + String(motor3_activo ? "true" : "false")
       + ",";

  json += "\"motor4\":"
       + String(motor4_activo ? "true" : "false");

  json += "}";

  server.send(200, "application/json", json);
}




// ============================================================
// MPU ON / OFF
// ============================================================

void handleControl() {

  control_activo = !control_activo;

  if (control_activo) {

    sensors_event_t a;
    sensors_event_t g;
    sensors_event_t temp;

    mpu.getEvent(&a, &g, &temp);

    float ax = -a.acceleration.x;
    float ay = -a.acceleration.y;
    float az = -a.acceleration.z;

    acc_roll_offset =
      atan2(
        ay,
        sqrt(ax * ax + az * az)
      )
      * 180.0 / PI;

    acc_pitch_offset =
      atan2(
        -ax,
        sqrt(ay * ay + az * az)
      )
      * 180.0 / PI;

    gyro_roll_offset = g.gyro.x;
    gyro_pitch_offset = g.gyro.y;
    gyro_yaw_offset = g.gyro.z;

    roll = 0;
    pitch = 0;
    yaw = 0;

    lastTime = millis();

    Serial.println("MPU ACTIVADO");
    Serial.println("Calibracion realizada");

  } else {

    Serial.println("MPU DESACTIVADO");
  }

  server.send(
    200,
    "text/plain",
    control_activo ? "MPU ON" : "MPU OFF"
  );
}




// ============================================================
// Encender motores
// ============================================================

void handleEncenderMotores()
{
    encender_motores();

    server.send(200, "text/plain", "Motores encendidos");
}

