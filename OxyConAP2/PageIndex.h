const char MAIN_page[] PROGMEM = R"=====(
<html>
  <head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
      html {
        font-family: Arial;
        display: inline-block;
        margin: 0px auto;
        text-align: center;
      }
      h1 { font-size: 2.0rem; }
      p { font-size: 2.0rem; }
      .units { font-size: 1.2rem; }
      .oxy-labels{
        font-size: 1.5rem;
        vertical-align:middle;
        padding-bottom: 10px;
      }
    </style>
  </head>
  <body>
    <h1>OxyThes Monitoring Dashboard</h1>
    <p>
    <img src="data:image/png;base64,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" align="middle" style="padding-bottom: 20px;">
      <span class="oxy-labels">SpO2 : </span> 
      <span id="SpO2Value" style="color:#ffaa56;">0</span>
      <sup class="units">%</sup>
    </p>
    <p>
    <img src="data:image/png;base64,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" align="middle" style="padding-bottom: 20px;">
      <span class="oxy-labels">O2 : </span> 
      <span id="O2Value" style="color:#ffaa56;">0</span>
      <sup class="units">%</sup>
    </p>
    <p>
    <img src="data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAEAAAABACAYAAACqaXHeAAAABHNCSVQICAgIfAhkiAAAAAlwSFlzAAAB2AAAAdgB+lymcgAAABl0RVh0U29mdHdhcmUAd3d3Lmlua3NjYXBlLm9yZ5vuPBoAAAO0SURBVHic7dtPaB1VFMfxz4tJK1iLSavW+o8qsSK4ENGiVMX6BxTdVKu4UPwvLtzUjUs3gm5cFO3CPwsXbqwWUUQREasUN6Ig6sIKVURri6QpDVbTpHFxX3Bm3jS89zJz74uZL1wylzcz93fPzL1zzr0nNDQ0NCxjWqkFLILTcQOuxTkYxRH8hn34AIeSqauRK/EmpjG3QJnB57gjjczqWY1XccLCHS8re3BpfMnVsVZ4rXvteLZM4rbYwqviYZ0dOoKXcSc2CHPABbgOzyk32Azuiqy9Eq7GrNCJWbwoDImFGMYTOCxvhClcUZvSGtkiPNmrerxuI36UN8I+rKhU3YBzIQ7KG+GppIoScLu8AQ5iJKmiBHwib4RbTkmrJzp/4Z5M/WgqIalYhX/89wZ8OZRWT3SmhFhhnvXLzQDwR+b47OVogNHM8eRw++CizA/T+EHwuIpsFMZRlezHRMX3PBlDODdTP0Dwroo+816dntLrJedVUaaEuD4Gmwttv0UIKsqEXZO58DT9haHdljdq6W4nOwvt3g+flgiaFELQLN+XnFdVieGWXiz/CZzG2DC24hGc2T7xGHbhz8INtuAhnFGxsG+0X8UaGRGGcHZYvybe3JOUFcISWvaNO4p1KUXFYgO+0DnkHk0pqm6GsAk78LfOzu/InjxcvLpLWngaNy3iHnVwFs6Xd3ayvITtVTR0t/q+CHX5Go+XdaRfV3ipTCAzeAXj7b+VsVrwH+YXKgelHBb8ld3CJ3v+097Q0FDO/O5wNhyOyYywOntICLaSUBYOxy7H8L6wBbay3u52crJwOFX5GffV2eEiZeHwIJSdImxctITwNhsOx2Sl4FRtxnklv78trOPPxRSVghZuxlc634Rn08mKz4iQBZI1wHFLPLujV1p4R94Iu+tsrBuG8IywetttAHUCn+F5vY/hUfyEsXZ9Vpgrist00bhX/7P5tj7bfKFwnwf7Vr8A3T7NNYtoo7i63C3vFepJ01pW4SNhQur2yR9vX9PvTtLawv129S9/aXKqvAE+rKORQd4cXV+oH6ijkUE2wPWF+i9JVCRkj/wQ2JRWTly2ynf+V4P9ti7IiJDO3i2XC5uzWQM8WYOuKIwLOTlzeFfnxFZkm871ie8M1uZLTxS9uSkhwflWwRhrcJmwabFXpy8xgUuiq66QB/TvQk+IlzlSK9uFBMVeOv+tMHz+N4wLCQplu7XZsh+PIVoGa+x/mhrDjUL+0Toh9+h34TP3Mb6OrKehoaFhefMvW3o8ZD0fU6IAAAAASUVORK5CYII=" align="middle" style="padding-bottom: 20px;">
      <span class="oxy-labels">Flow : </span>
      <span id="LPMValue" style="color:#4da5fc;">0</span>
      <sup class="units">LPM</sup>
    </p>
    <img src="data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAEAAAABACAQAAAAAYLlVAAAABGdBTUEAALGPC/xhBQAAACBjSFJNAAB6JgAAgIQAAPoAAACA6AAAdTAAAOpgAAA6mAAAF3CculE8AAAAAmJLR0QA/4ePzL8AAAAJcEhZcwAACxMAAAsTAQCanBgAAAAHdElNRQfmBQ8QFB8jyrTJAAAFzElEQVRo3u2Za0xURxTHf/sAWUB5iSAi+ABFLNWI1qrR+Kia0NKX1S+tSfuhD5vQD7bV1gSjNVZN2lRjjI2apja11VbwCdamVIMJakWl1VIsFd9SBcEH4Oqyu/3Q2cPC3rt3F8WkKf/5cO/e+c89/3tmzsyZWehGN7rRjf87TIaMcPqSTDzhOGngKpdpxKXD7ckgMhhILKHYuU4NVZzH3lkBVh4jh8kMJY4wTICDO1yinH2UcqPDe9J5nhwyicGqnrlppY4KdrOXK8F6xkw2G6nFhVuj2DnM60QLO4kl1ODU5Lpx8BtvExWM+V4s4LLO6zzlHsWMBUxMpUzXuKfcZxePB9oFyaxiNiHql4tGLnKVO1iII4V+2IR5gUWEspwkcXoDVZyhFjs2khnGEHqJjdPkcdD461Mo8tJ9hPfIJo4QzFiw0Z8c1nt55xa31Z2Lv1jGGHqKQRMxTGQNV4RdzSQj8zFsE3oNefTW4FgYyWZa2rm4hU1kaPrTzGgKua94J8nwP/SW0KqopYz2w7SRR72Yb2S+V8f4IoqV3FXcb4jQJ07muqL9TJqBr8zMVRKayMNiwLaxCofy1Sv6pAJlvlJ7xBLLBNLE0Wbe4iyXWEwPjBHNDvX2Q8RpU6ZxU2mcq1mfxC6aqCZHnlgYQJrEixGe4KqaRWZrE9YqhUVEatT2YK2alor89rc+zGJhs1aX9aFChd7Lms1fo0k1v052pwTAFBW0lfT1rRynOqCaVI2mY6jxCroPOikgjuNq9hjf5hYP0ukJQCW1Pg3jWcZAr98zFTdYNHIGgAj6+wpIUvc13O/QzMq7TAfAiROAkYzolACXWhUtbXHQJsDzTQ0+zWYxT/EKKAMgmhmqLlxzwOrjrrqG+grwoON0msUSegFwnIUUqqcziMXKsxRQxItBCPDEj3jZKlW31LX9JBHDR2r2rief8xzgbxKBTHIZzhtEAbEcDTDhsJAMgJN6XwGXcWIBhhBPKxYsmDHzJs8A0Mpn7AfOcIxcIJLVRKrWg8niGgmYJEpcmncu4hgGQBMXfAW04MACTKAYEyFYsRJKgmLsYh0uwE4xT2PG5JUP2chlFtMw4cTlpzgJU950tI0BDyLY4yefqWS4MDO46FPfLGtooGVvxzWxD6d1yTfbzd0hbFXPr7GFhiANt31SQvsuqGc7qUSoGHBwG5dy6G02stNLgIMNjKIf5SzjJHsYJzV26gAzJkyY5PrvHYRKBLho5jvPMGwLOhtjWcwUABpZQBluWmmlmfoO+wATafTlD+qANbwjz7eyCDB3KBbMQDiL1GTWwgpKqJAZoR2mc0O5qIRBAQXWHEm27vKCLiuMhdxRvC2E67/OwseSYJfqJCXtMUSGZDl9dDgm8rmnWBUqEHXRm0IZKKWkGwoIo1ixl+pypktefI6pxt80gP0i4QBDDfnzsOOmRnd5msyf6m31PBeAT4FU9np5IdOAHcGrrGCSzi5zIlUyVywwTF0FKeyRSbSMrECb+WA8v8u+IT+g1FWQzA6RcLSTq/9YTkmELCUs2OZJbBcJ5YwK2vxolWO6sbO8c2lsItskKE8wJqi2oziBZxe9yl/k+0cCW0TCrzwZcLsRHJPt7af+tmLGiOcrkXCKCQG1yeKImF/TyfTVC3F8IYttpfH2mkzK8JyMrFPJ3AMilg1qY+mmSi1XesjgkGK28nlwxzL+EM16kVDNU7q8dA6K+U3EPCzzAFGslXXvLDM1OYMpUQwnXxL7MM0D9GK1SDjntUP2YCA/ivmvNU9WHhiRfCLL6gVy29Wlsk+y3291F+YHRgQrsStDl7wSkP6yfLn4nsSuMg8QznI57bnCSwD0Y6eYL5Qjuy6DjaVyQlbLHJIokDVjt9r5dDHCyKdZJByWaaqIlEdhHqAHH8p5iaf8wIBHZR4glPclz3Xj5qcAs+iHiBDmyzHtgQDS1y6AlTzqcFASQOqqC+N/TPxLyCaRXzROlbrx38E/cxv37ihZMhEAAAAldEVYdGRhdGU6Y3JlYXRlADIwMjItMDUtMTVUMTY6MjA6MzErMDA6MDCsx7+gAAAAJXRFWHRkYXRlOm1vZGlmeQAyMDIyLTA1LTE1VDE2OjIwOjMxKzAwOjAw3ZoHHAAAAABJRU5ErkJggg==" align="middle" style="padding-bottom: 20px;">
      <span class="oxy-labels">HR : </span>
      <span id="HRValue" style="color:#4da5fc;">0</span>
      <sup class="units">BPM</sup>
    </p>
    <p>
    <img src="data:image/png;base64,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" width="20" height="20" align="middle" style="padding-bottom: 12px;">
      <span style="font-size:1.0rem;">Time </span>
      <span id="time" style="font-size:1.0rem;"></span>
    
    <img src="data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAEAAAABACAYAAACqaXHeAAAACXBIWXMAAAsTAAALEwEAmpwYAAAA1ElEQVR4nO3bwQqCQBgA4TZ6Rh/Sl6y7QiIlX+nMUXEZhnUPPzpuGzyn+fnu/pinsbXGkXzqd/+uzv9RAC2gGVvv0Nm5/A4ogBbQFEALaAqgBTQF0AKaAmgBTQG0gKYAWkDzWF7QM76jWc4/Lr8DCqAFNKszYIu9M8TlmaKfX3L5HVAALaApgBbQFEALaAqgBTQF0AKa1fcBzQMuRgG0gKZ5wJ7FzkgBtICmAFpAUwAtoCmAFtAUQAtomgcokV+hAFpAUwAtoCmAFtD036AW0BRAC2heXrU4ekB4r6wAAAAASUVORK5CYII=" width="20" height="20" align="middle" style="padding-bottom: 12px;">
      <span style="font-size:1.0rem;">Date </span>
      <span id="date" style="font-size:1.0rem;"></span>
    </P>
  <p>

    <script>
      setInterval(function() {
        // Call a function repetatively with 1 Second interval
        getSpO2Data();
        getO2Data();
        getLPMData();
        getHRData();
      }, 1000); 
      
      setInterval(function() {
        // Call a function repetatively with 1 Second interval
        Time_Date();
      }, 1000); 

      function getSpO2Data() {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
          if (this.readyState == 4 && this.status == 200) {
            document.getElementById("SpO2Value").innerHTML =
            this.responseText;
          }
        };
        xhttp.open("GET", "readSpO2", true);
        xhttp.send();
      }

      function getO2Data() {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
          if (this.readyState == 4 && this.status == 200) {
            document.getElementById("O2Value").innerHTML =
            this.responseText;
          }
        };
        xhttp.open("GET", "readO2", true);
        xhttp.send();
      }

      function getLPMData() {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
          if (this.readyState == 4 && this.status == 200) {
          document.getElementById("LPMValue").innerHTML =
          this.responseText;
          }
        };
        xhttp.open("GET", "readLPM", true);
        xhttp.send();
      }

      function getHRData() {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
          if (this.readyState == 4 && this.status == 200) {
          document.getElementById("HRValue").innerHTML =
          this.responseText;
          }
        };
        xhttp.open("GET", "readHR", true);
        xhttp.send();
      }
      
      function Time_Date() {
        var t = new Date();
        document.getElementById("time").innerHTML = t.toLocaleTimeString();
        var d = new Date();
        const dayNames = ["Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday","Saturday"];
        const monthNames = ["January", "February", "March", "April", "May", "June","July", "August", "September", "October", "November", "December"];
        document.getElementById("date").innerHTML = dayNames[d.getDay()] + ", " + d.getDate() + "-" + monthNames[d.getMonth()] + "-" + d.getFullYear();
      }
    </script>
  </body>
</html>
)=====";
