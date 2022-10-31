---
layout: entry
---
<h1> <span style="color:white">10/27/2022 - Alex's Coding Journey</span> </h1>

<h2 class="attendance"> <span style="color:red"> Attendance</span> </h2>

<p> </p>

<label class="container" style="color:white">Alex
  <input type="checkbox" disabled checked="checked">
  <span class="checkmark"></span>
</label>

<label class="container" style="color:white">Malachi
  <input type="checkbox" disabled xchecked="xchecked">
  <span class="checkmark"></span>
</label>

<label class="container" style="color:white">Michael
  <input type="checkbox" disabled xchecked="xchecked">
  <span class="checkmark"></span>
</label>

<label class="container" style="color:white">Taven
  <input type="checkbox" disabled xchecked="xchecked">
  <span class="checkmark"></span>
</label>
<h2 style="color:white">Items Included in this Entry:</h2>
<ul>
  <li> Drive Base Program - Mecanum Wheels </li>
  <li> Flywheel RPM Guages </li>
  <li> Flywheel Data Logging </li>
  <li> Other Robot Code </li>
</ul>
<p style="color:white"> </p>
<h2 style="color:white">Drive Base Program - Mecanum Wheels:</h2>
<p style="color:white">While programming drive controls is usually a simple task within VexCode. Having Mecanum wheels on our robot requires manual writing of controls to enable straffing. For this, and many other reasons. Alex has decided to go with using PROS V5 to program our robot. This will allow for many more advanced programs (Like the RPM Guages Program Later in this entry). To enable strafing you have to utilize one more axis on the V5 Joystick. While a normal base (Either tank drive or split controls) only uses 2, usually one axis on the left and one on the right. We will be using 2 axis on the left thumbsick. Our control scheme will be shown in an image below. (Only Base Controls)</p>
<!-- Insert Image Of Control With Base Controls-->

<p style="color:white">Getting these controls to control the robot the way we need to is a bit challanging but can be done with a few math equations in our code. There are different patterns of addition and subtraction to get the values we want from the V5 controller. So we will set variables corresponding to it's thumbsick axis shown under //Strafe Control. Now that we have the raw controller values we need to bring it through these patterns of addition and subtraction. The code will be displayed below this in a code snipit.</p>
<pre>
  <code>
	//Strafe Control (Converts Joystick Input to Integer Values)
		int turn = master.get_analog(RIGHT_JOYSTICK_X);
		int power = master.get_analog(LEFT_JOYSTICK_Y);
		int strafe = master.get_analog(LEFT_JOYSTICK_X);
	//Math for Strafe Control (Complicated Math, Don't Touch :D)
		int turnreversed = turn;
		int straferevered = strafe;
		int fl = power + turnreversed + straferevered;
		int rl = power + turnreversed - straferevered;
		int fr = power - turnreversed - straferevered;
		int rr = power - turnreversed + straferevered;
  </code>
</pre>

<p style="color:white">Now that we have the final values fl, rl, fr, rr. All we have to do is tell the motors to spin according to these variables. (These variables have a maximum value of 127 according to v5 documentation) </p>

<pre>
  <code>
  //Set Motor Speeds (relative to controller input)
		left_front_mtr.move(fl);
		left_back_mtr.move(rl);
		right_front_mtr.move(fr);
		right_back_mtr.move(rr);
  </code>
</pre>

<p>After all of this we have working drive controls for our mecanum base!</p>

<h2 style="color:white">Flywheel Data Logging</h2>

<h3 style="color:white">Getting Usable Data</h3>

<p style="color:white">Now the second more complicated program for this robot is the FlyWheel. We decided that while going along with building a Dual Flywheel, we are going to Collect Data and Compare RPM Drops, Optimal RPM for shooting the disks, And optimal flywheel weight. Too much flywheel weight may cause the flywheels to be unstable or for the motors overheading. Also larger Flywheels will take longer to spin up to their max speed. To recieve any data we first need to install a rotational sensor onto our output shaft of the gear box.</p>
<!--Picture of Rotation Sensors-->

<p style="color:white">Now that we have some useable data the first thing we need to do is convert the output from the rotational sensors from centidegrees per second to rotations per minute. I have taken the output from the rotational sensor and set them to the variable CPSLEFT and CPSRIGHT. This value is then divided by 6 to recieve Rotations per minute. The Left rotational sensor is reversed because the flywheels are spinning in oposite directions and for this program we only want positive values. </p>

<pre>
  <code>
  	int CPSLEFT = -(left_fly_encoder.get_velocity());
		int CPSRIGHT = right_fly_encoder.get_velocity();
		int RPMLEFT = CPSLEFT / 6;
		int RPMRIGHT = CPSRIGHT / 6;
  </code>
</pre>

<h3 style="color:white">Outputting to Console</h3>

<p style="color:white">Next, we have to have a way to log time in order to get acriate data from our flywheels. We can't do much with just a bunch of numbers. Within PROS V5 library, there is the RTOS Facilities C++ API. This includes all of the special funtions for the backend of PROS. Included in this is a task named "millis". This will call back the time since PROS initualized(Robot Initualization) in milliseconds. we will take this and set it to the variable "time". But to get some more user friendly we will convert this to Seconds by dividing it by 1000. This creates our new variable "timesec</p>

<p style="color:white">The variables "RPMLEFT", "RPMRIGHT", and "timesec" are now useful data. This is what we will be logging. Unfortanetly there is no way of doing this wirelessly. We will be connecting my computer to the usb port on the V5 Brain. This will allow us to connect to the terminal serial connetion. Using the library known as "cout" we can print text to the terminal. To get repeated data we're going to put this in a loop. Now all we have to do is add our variables and we have the code displayed below</p>

<pre>
    <code>
        <xmp>
		if (RPMLEFT < 1 and RPMRIGHT < 1){ //Prevents Excessive Console Spam
			lv_gauge_set_value(rpm_gaugeleft, 0, RPMLEFT);
			lv_gauge_set_value(rpm_gaugeright, 0, RPMRIGHT);
		}

		else{
			//Updates RPM Gauge and Outputs RPM to Console for Data Collection
			lv_gauge_set_value(rpm_gaugeleft, 0, RPMLEFT);
			lv_gauge_set_value(rpm_gaugeright, 0, RPMRIGHT);
			cout<<"L: "<<RPMLEFT<<" "<<"R: "<<RPMRIGHT<<endl;
			delay(20);
		}
      </xmp>
    </code>
</pre>
<p>("<" = "%lt;")</p>

<h3 style="color:white"> Graphing Data </h3>

<p style="color:white"> Now that we have the data outputted to the console. We can copy this data from the console to a Google Spreadsheet and make a graph so we can visably view our flywheel RPMs. My first version of this program didn't include time so our graphs weren't originally useful data as show below</p>

<!-- Place This Redirect Button Underneath all other text and images on page-->
<a href="https://robotics.oavr.net/Directory">
<button class="return" type="button">Return to Directory</button>
</a>