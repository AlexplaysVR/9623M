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
  <li> Drive Base Program - Mecanum Wheels
  <li> Flywheel RPM Guages
  <li> Flywheel Data Logging
  <li> Other Robot Code
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

<p style="color:white">Now the second more complicated program for this robot is the FlyWheel. We decided that while going along with building a Dual Flywheel, we are going to Collect Data and Compare RPM Drops, Optimal RPM for shooting the disks, And optimal flywheel weight. Too much flywheel weight may cause the flywheels to be unstable or for the motors overheading. Also larger Flywheels will take longer to spin up to their max speed</p>
<!-- Place This Redirect Button Underneath all other text and images on page-->
<a href="https://robotics.oavr.net/Directory">
<button class="return" type="button">Return to Directory</button>
</a>