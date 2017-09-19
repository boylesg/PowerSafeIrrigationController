<?php 
		header('Content-Type: text/html; charset=utf8');
		session_start();
		include "Inventory.php";
		include "Browser.php";
?>
<!DOCTYPE html>
<html>

	<head>

		<meta charset="UTF-8">

		<meta content="text/html; charset=windows-1252" http-equiv="Content-Type" />
		<title>Irrigation Controller User Manual</title>

		<?php include "Style.css"; ?>
				
		<style type="text/css">
		.auto-style1 {
	border-width: 0px;
}
		</style>
		
		
		</head>
		
	<body>
	
		<div class="container">
		
			<?php include "Header.htm"; ?>
		
			<div class="columns">
			
				<?php include "NavigationStructure.htm"; ?>
			    
			    <div class="content" id="content">

			        <p class="PageTitle">
			        	<script type="text/javascript">
			        		document.write(document.title);
			        	</script>
			        </p>
			        
<!--********************************************************************************************************************************************
	********************************************************************************************************************************************
	Start main content
	********************************************************************************************************************************************
    ********************************************************************************************************************************************-->
               	    
				<hr>
				<h3><a name="Contents">Table of contents</a></h3>
				<p>
					&nbsp;&nbsp;&nbsp;&nbsp;<a href="#Installing_your_solenoid_valves">Installing your solenoid valves</a><br>
					&nbsp;&nbsp;&nbsp;&nbsp;<a href="#Connecting_solenoid_valves">Connecting it to the solenoid valves</a><br>
					&nbsp;&nbsp;&nbsp;&nbsp;<a href="#Compatible_solenoid_valves">Compatible solenoid valves</a><br>
					&nbsp;&nbsp;&nbsp;&nbsp;<a href="#Installation_example">Installation example</a><br>
					&nbsp;&nbsp;&nbsp;&nbsp;<a href="#Uploading_your_wifi_network_name_and_key">Uploading your wifi network name and key</a><br>
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="#Manually">Manually</a><br>
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="#Using_your_mobile_phone">Using your mobile phone</a><br>
					&nbsp;&nbsp;&nbsp;&nbsp;<a href="#Setting_up_your_ADSL_modem">Setting up your ADSL modem</a><br>
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="#Find_your_ADSL_modem">Find your ADSL modem</a><br>
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="#Irrigation_controller_IP">Irrigation controller IP address</a><br>
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="#Port_forwarding">Port forwarding</a><br>
					&nbsp;&nbsp;&nbsp;&nbsp;<a href="#Dynamic_IP_addresses">Dyamic IP addresses</a><br>
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="#freedns.afraid.org">freedns.afraid.com</a><br>
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="#Dynamic_DNS_on_your_ADSL_modem">Dynamic DNS on your ADSL modem</a><br>
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="#IP_address_update_app">IP address update app</a><br>
					&nbsp;&nbsp;&nbsp;&nbsp;<a href="#Irrigation_Controller_User_Interface">Irrigation Controller User Interface</a><br>
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="#System_Clock">System Clock</a><br>
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="#Manual_Control">Manual Control</a><br>
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="#Irrigation_Programs">Irrigation Programs</a><br>
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="#Station_Description">Station Description</a><br>
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="#Persistence_of_station_details">Persistence of station details</a><br>
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="#Uploading_to_Irrigation_Controller">Uploading to Irrigation Controller</a><br>
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="#Correct_Procedure">Correct Procedure</a><br>
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="#Turning_a_station_off">Turning a station 
					off</a><br>
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="#Wired_Soil_Moisture_Alarms">Wired Soil Moisture Alarms</a><br>
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="#Making_a_moisture_probe">Making a moisture probe</a><br>
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="#Email_Settings">Email Settings</a><br>
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="#Changing_the_Backup_Battery">Changing the Backup Battery</a><br>
					&nbsp;&nbsp;&nbsp;&nbsp;<a href="#Irrigation_Controller_Data_Files">Irrigation Controller Data Files (on SD card)</a><br>
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="#program.txt">program.txt</a><br>
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="#alarms.txt">alarms.txt</a><br>
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="#wifi.txt">wifi.txt</a><br>
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="#email.txt">email.txt</a><br>
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="#timezone.txt">timezone.txt</a><br>
				</p>
					<hr><br>
				
					<h3><a name="Installing_your_solenoid_valves">Installing 
					your solenoid valves</a></h3>
					<ul>
						<li>The first task is to select a spot in the garden to 
						install your solenoid valve pit.</li>
					</ul>
					<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					<img alt="" height="205" src="images/SolenoidValvePit.jpg" width="329"></p>
					<ul>
						<li>You will have a far easier job if you position your 
						valve pit close to both a tap <strong>AND</strong> close to 
						an external power point.</li>
						<li>You can install an ordinary brass tap splitter on 
						the tap and then use <strong>plumbing </strong>
						irrigation polypipe and fittings to connect one outlet 
						of the tap splitter to your manifold with solenoid 
						valves.</li>
					</ul>
					<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					<img alt="" height="178" src="images/BrassTapSplitter.png" width="220">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					<img alt="" height="161" src="images/PlumbingPolypipe.png" width="159">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					<img alt="" height="183" src="images/PlumbingPolypipeFittings.png" width="192"></p>
					<ul>
						<li>That way you won't risk damaging the hard copper 
						plumbing inside the wall or brickwork.</li>
						<li>The device on the right will screw on to the outlet 
						of the tap splitter while the other end will accept the 
						plumbing polypipe.</li>
						<li>Don't be tempted to use the irrigation polypipe and 
						fittings (without the blue stripe) because that is not 
						capable or permanetly withstanding mains pressure - it 
						will fail and you will have a flood on your hands.</li>
						<li>With plumbing polypipe you can leave the tap 
						splitter outlet (and the tap iteself) permanently turned on and you also have 
						a quick and easy fix if you ever accidently damage a 
						pipe.</li>
						<li>So, to be clear, you connect the following manifold 
						to the tap splitter via the plumbing polypipe:</li>
					</ul>
					<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					<img alt="" height="205" src="images/SolenoidValvePit.jpg" width="329"></p>
					<h3><a name="Connecting_solenoid_valves">
					Connecting it to the solenoid valves</a></h3>
					<ul>
						<li>Connecting electrical wires between the power supply 
						and the irrigation controller and between the irrigation 
						controller and the solenoid valves should be ket to a 
						minimum.</li>
						<li>Just be aware that if the connecting wires get too 
						long then they may start presenting enough electrical 
						resitance for voltage levels to drop below the minimum 
						levels required by either the irrigation controller or 
						the solenoid valves to operate normally - the shorter 
						they are the better.</li>
						<li>Try and keep the length of the conecting wires to 10 
						meters or less.</li>
						<li>You can use broken ethernet cables, offcuts of 
						ethernet cable or speaker wire to connect your 
						irrigation controller to your solenoid valves.</li>
					</ul>
					<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<img alt="" height="180" src="images/EthernetCable.png" width="279">&nbsp;</p>
					<ul>
						<li>Ethernet cable contains 4 pairs of wires - 
						orange/orange &amp; white, brown/brown &amp; white, blue/blue &amp; 
						white, green/green &amp; white, so one length of ethernet cable can accomodate 4 
						solenoid valves - 2 wires are needed for each solenoid 
						valve.</li>
						<li>Just be aware the insulation material covering most 
						electrical wires are not UV stabilised and will 
						disintegrate over time and cause an electrical short 
						circuit and damage to the irrigation controller and or 
						the power source.</li>
						<li>Any connecting wires that are exposed to sunlight 
						should be covered some sort of UV stabilised plastic 
						conduit:
							<ul>
								<li>Irrigation polypipe - whether you use speaker wire or ethernet cable you 
									will probably have to use 25mm polypipe and fittings in 
									order to comfortably accomdate all the connecting wires 
									required.</li>
								<li>Orange PVC electrical conduit.</li>
							</ul>
						</li>
					</ul>
					<h3><a name="Compatible_Solenoid_Valves">Compatible solenoid valves</a></h3>
               		<p>You can connect any standard 24VAC solenoid valve to the 
					controller.</p>

					<p>&nbsp;<img alt="" height="269" src="images/SolenoidValve1.png" width="323">
					<img alt="" height="302" src="images/SolenoidValve2.png" width="295"></p>

					<h3><a name="Installation_example">Installation example</a></h3>
					<p>
					<img alt="" height="442" src="images/InstallationExample.jpg" width="507"></p>
					<ul>
						<li>In the above example all the connecting wire lengths 
						are kept to a minimum, the minimum amount of plumbing 
						polypipe is required and there is no chance that the 
						irrigation controller can get rained on.</li>
					</ul>
					<h3><a name="Compatible_Solenoid_Valves">Uploading your wifi network name and key</a></h3>
					<h4><a name="Manually">Manually</a></h4>
					<ul>
						<li>If you remove the cover from the irrigation 
						controller enclosure you will see the Arduino 
						microcontroller:</li>
					</ul>
					<p>
					<img alt="" height="537" src="images/SDCardLocation.jpg" width="400"></p>
					<ul>
						<li>You should also be able to see a standard size SD 
						card.</li>
						<li>Carefully remove the SD card and insert it into the 
						SD card reader of your laptop or PC.</li>
						<li>If your laptop or PC does not have a card reader 
						then you can purchase a USB SD card reader from Ebay for 
						around $15.</li>
						<li>Carefully remove the SD card and insert it into the 
						SD card reader in your laptop or PC.</li>
						<li>Windows will present you with:</li>
					</ul>
					<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					<img alt="" height="327" src="images/SDAutoplayOptions.jpg" width="250">&nbsp;</p>
					<ul>
						<li>Click the 'Open folder to view files' option</li>
						<li>Windows will present you with:</li>
					</ul>
					<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					<img alt="" height="404" src="images/SDCardFiles.jpg" width="638">&nbsp;</p>
					<ul>
						<li>Locate the file named 'wifi.txt'.</li>
						<li>Right click on it and windows will present you with:</li>
					</ul>
					<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &nbsp;<img alt="" height="364" src="images/SDRightClickMenu.jpg" width="292"></p>
					<ul>
						<li>Move the mouse to the 'Open with' option and a new 
						popup menu will appear:</li>
					</ul>
					<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &nbsp;<img alt="" height="146" src="images/SDOpenWithMenu.jpg" width="358"></p>
					<ul>
						<li>Select the 'Notepad' option and Windows Notepad will 
						open:</li>
					</ul>
					<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &nbsp;<img alt="" height="167" src="images/SDWifi_txt.jpg" width="286"></p>
					<ul>
						<li>The first line of text in this file is simply the 
						name of your wifi network and the second line of text is 
						your network key.</li>
						<li>Simply edit these two lines of text to match your 
						wifi network name and key and then save the file.</li>
						<li>If Windows asks you if you are sure you want to 
						overwrite the existing file then select 'yes'.</li>
						<li>Insert the SD card back into the SD card reader slot 
						on the microcontroller.</li>
						<li>Press the reset button or turn the irrigation 
						controller power supply off and then on again.</li>
					</ul>
					<h4><a name="Using_your_mobile_phone">Using your mobile phone</a></h4>
					<ul>
						<li>First of all you will need an Android mobile phone.</li>
						<li>At present I am not able to create a version of the 
						app which will run on other mobile device operating 
						systems, e.g. Apple IPhones and Windows Phones.</li>
						<li>Next you will need to download the 
						<span><a href="IrrigationController/Irrigation.apk">irrigation app</a></span>
						and install it on your android phone.</li>
						<li>Next you will need to open 'Settings' on your mobile 
						phone and then 'Wireless and network' - the exact names 
						used may vary from phone to phone but they will be 
						similar to the afore mentioned ones.</li>
						<li>Here is an example:</li>
					</ul>
					<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					<img alt="" height="258" src="images/AndroidBluetooth.jpg" width="330">&nbsp;</p>
					<ul>
						<li>Next you need to go to the 'search for bluetooth 
						devices' function on your android phone.</li>
						<li>Here is an example:</li>
					</ul>
					<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					<img alt="" height="172" src="images/AndroidBluetoothDevices.jpg" width="332">&nbsp;</p>
					<ul>
						<li>Look for a bluetooth device named 'Irrigation' - 
						that is your irrigation controller.</li>
						<li>Tap 'Irrigation' and the Android phone will pair 
						with it, thus allowing the 'irrigation app' to connect 
						with it.</li>
						<li>You will be required to enter a PIN number - enter 
						1234.</li>
						<li>Now start the 'irrigation' app:</li>
					</ul>
					<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					<img alt="" height="511" src="images/AndroidBluetoothOpening.jpg" width="301">&nbsp;</p>
					<ul>
						<li>Notice the drop down or combo box at the top - if 
						some bluetooth device other than 'IRRIGATION' is 
						selected then touch it.</li>
					</ul>
					<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					<img alt="" height="507" src="images/AndroidBluetoothIrrigation.jpg" width="303"></p>
					<ul>
						<li>From the drop down box that appears select the 
						bluetooth device named 'IRRIGATION'.</li>
					</ul>
					<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					<img alt="" height="509" src="images/AndroidBluetoothOpening.jpg" width="304">&nbsp;</p>
					<ul>
						<li>The bluetooth device named 'IRRIGATION' should now 
						be selected in the combo box.</li>
						<li>Touch the 'CONNECT' button.</li>
					</ul>
					<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					<img alt="" height="504" src="images/AndroidBluetoothConnected.jpg" width="300">&nbsp;</p>
					<ul>
						<li>Now just touch the 'Network name' edit field and 
						enter your wifi network name and then touch the 'Network 
						key' edit field and enter your wifi network key.</li>
						<li>Then touch the 'Upload to irrigation controller' 
						button.</li>
						<li>You should receive a popup message stating that the 
						upload was successful.</li>
					</ul>

					<h3><a name="Setting_up_your_ADSL_modem">Setting up your 
					ADSL modem</a></h3>
					<h4><a name="Find_your_ADSL_modem">Find your ADSL modem</a></h4>
					<ul>
						<li>The first thing you need to find the web address of 
						your ADSL modem - this will be detailed in your user 
						manual and will be something like 
						'http://mygateway.gateway/'<ul>
							<li>If you can't find the user manual for your ADSL 
						modem, or can't find the web address of your ADSL modem 
						then follow these steps instead:</li>
							<li>Open a windows command prompt by clicking 'start' 
						and then typing 'cmd' in the search box.</li>
							<li>Type 'ipconfig' at the commnad prompt, press enter 
						and the following text will be displayed:</li>
						</ul>
						</li>
						<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; 
					Ethernet adapter Local Area Connection:<br><br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; 
					Connection-specific DNS Suffix . : gateway<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; 
					Link-local IPv6 Address . . . . . : 
					fe80::99a:6121:d5d:2450%11<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; 
					IPv4 Address. . . . . . . . . . . : 10.0.0.117<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; 
					Subnet Mask . . . . . . . . . . . : 255.255.255.0<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; 
					Default Gateway . . . . . . . . . : 10.0.0.138&nbsp;</p>
						<ul>
							<li>In the above example '10.0.0.138' is the IP 
							address of the ADSL modem and you can type this into 
							the address bar of your web browser in place of more 
							familiar web addresses.</li>
						</ul>
						<li>So type either the web or IP address of your ADSL 
						modem into the address bar of your web browser like this 
						and press enter:</li>
					</ul>
					<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					<a href="images/Browser1.jpg">
					<img alt="" height="457" src="images/Browser1.jpg" width="801" class="auto-style1"></a>&nbsp;</p>
					<ul>
						<li>You should see something like this:</li>
					</ul>
					<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					<a href="images/Browser2.jpg">
					<img alt="" height="453" src="images/Browser2.jpg" width="801" class="auto-style1"></a>&nbsp;</p>
					<h4><a name="Irrigation_controller_IP">Irrigation controller IP address</a></h4>
					<ul>
						<li>Next you need to determine the IP address of your 
						irrigation controller so have it near by.</li>
						<li>Connect the power supply and wait a minute or so for 
						it to boot up and initialise.</li>
						<li>In your web browser fit F5 a few times until a new 
						green tick appears in the 'Wi-Fi' column:</li>
					</ul>
					<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					<a href="images/Browser5.jpg">
					<img alt="" height="409" src="images/Browser5.jpg" width="801" class="auto-style1"></a></p>
					<ul>
						<li>In the above example the device at the top of the 
						'Wi-Fi' column has a green tick.</li>
						<li>Disconnect the power from the irrigation controller 
						and again hit F5 in your web browser a few times:</li>
					</ul>
					<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					<a href="images/Browser6.jpg">
					<img alt="" height="453" src="images/Browser6.jpg" width="800" class="auto-style1"></a>&nbsp;</p>
					<ul>
						<li>You will notice that, in the above example, the 
						green tick has disappeared from the device at the top of 
						the 'Wi-Fi' column.</li>
						<li>So you can be sure that the local IP address of your 
						irrigation controller is 10.0.0.79 in this example.</li>
						<li>Now this is a dynamic IP address that your ADSl 
						modem chooses at random from a range of possible IP 
						addresses.</li>
						<li>The local IP address of your irrigation controller 
						will not remain as 10.0.079 indefinitely, but there is a 
						way to give your irrigation controller a static or 
						permanent local IP address so that you don't need to keep 
						looking in your ADSL modem settings.</li>
					</ul>
					<h5>
					<a name="Giving_your_irrigation_controller_a_static_IP_address">
					Giving your irrigation controller a static IP address</a></h5>
					<ul>
						<li>The first thing to do here isto click on your device 
						- in this example the IP address of the irrigation 
						controller is 10.0.0.48</li>
					</ul>
					<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &nbsp;<a href="images/Browser10.jpg"><img alt="" height="436" src="images/Browser10.jpg" width="800" class="auto-style1"></a></p>
					<ul>
						<li>Either copy or write down the devices MAC address - 
						in this example it is '<span class=" simple-desc span3 text-left">5c:cf:7f:1b:0a:50'</span>.</li>
						<li>Now click 'Advanced'.</li>
					</ul>
					<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &nbsp;<a href="images/Browser11.jpg"><img alt="" height="427" src="images/Browser11.jpg" width="800" class="auto-style1"></a></p>
					<ul>
						<li>Click 'Local Network'.</li>
					</ul>
					<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &nbsp;<a href="images/Browser12.jpg"><img alt="" height="431" src="images/Browser12.jpg" width="801" class="auto-style1"></a></p>
					<ul>
						<li>You need to locate a section named 'Static leases' 
						or a similar name.</li>
						<li>Now hit the 'Add new lease' button.</li>
					</ul>
					<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &nbsp;<a href="images/Browser13.jpg"><img alt="" class="auto-style1" height="433" src="images/Browser13.jpg" width="800"></a></p>
					<ul>
						<li>In the 'MACaddress' edit field paste or type in the 
						MAC address of the irrigation controller.</li>
						<li>In the 'IP' edit field you can type the existing 
						local IP address of the irrigation controller, 10.0.0.48 
						in this example, or any IP address you prefer so long as 
						your chosen IP address is not currently being used by 
						another device connected to your network (either Wifi or 
						ethernet/wired).</li>
						<li>You can check that your chosen IP address is not 
						already being used by going back to the main page of 
						your ADSL modem settings and check each device that has 
						a green tick - if you click on a device with a green 
						tick then you will be able to see its IP address.</li>
						<li>In this example we will just use the dynamically 
						assigned IP address 10.0.0.48.</li>
						<li>In the 'Hostname' edit field you can type any name 
						that you want to call your irrigation controller, e.g. 
						'IrrigationController' (no space characters allowed)</li>
					</ul>
					<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					<a href="images/Browser14.jpg">
					<img alt="" class="auto-style1" height="427" src="images/Browser14.jpg" width="800"></a></p>
					<ul>
						<li>Now hit the small blue button with the plus '+' 
						symbol.</li>
					</ul>
					<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &nbsp;<a href="images/Browser15.jpg"><img alt="" class="auto-style1" height="423" src="images/Browser15.jpg" width="799"></a></p>
					<ul>
						<li>You will have now given your irrigation controller a 
						permanent or static local IP address and the name 
						'IrrigationController' will appear in the main settings 
						of your ADSL modem.</li>
					</ul>
					<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &nbsp;<a href="images/Browser16.jpg"><img alt="" class="auto-style1" height="439" src="images/Browser16.jpg" width="801"></a></p>
					<ul>
						<li>You can see in the main page of the ADSL modem 
						settings there is now a device with the name 
						'IrrigationController' with a local IP address of 
						10.0.0.48</li>
						<li>So while you are at home, you can connect to your 
						irrigation controller by typing this local IP address 
						into the address bar of your web browser.</li>
						<li>But this local IP address will not work away from 
						your home network. e.g. at the office or a friend's 
						house.</li>
					</ul>
						<h4><a name="Port_forwarding">Port forwarding</a></h4>
					<ul>
						<li>In order to access the irrigation controller away 
						from home you need to setup port forwarding.</li>
						<li>Reconnect the power to your irrigation controller 
						and make sure the green tick re-appears next to device 
						10.0.0.79</li>
						<li>Click 'User Settings:</li>
					</ul>
					<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					<a href="images/Browser3.jpg">
					<img alt="" height="439" src="images/Browser3.jpg" width="801" class="auto-style1"></a>&nbsp;</p>
					<ul>
						<li>Now click 'Port Forwarding':</li>
					</ul>
					<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					<a href="images/Browser4.jpg">
					<img alt="" height="455" src="images/Browser4.jpg" width="800" class="auto-style1"></a>&nbsp;</p>
					<ul>
						<li>Click 'Add new port mapping and select 'HTTP 
						Server':</li>
					</ul>
					<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					<a href="images/Browser7.jpg">
					<img alt="" height="444" src="images/Browser7.jpg" width="801" class="auto-style1"></a>&nbsp;</p>
					<ul>
						<li>A new blank item will appear:</li>
					</ul>
					<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					<a href="images/Browser8.jpg">
					<img alt="" height="441" src="images/Browser8.jpg" width="802" class="auto-style1"></a>&nbsp;</p>
					<ul>
						<li>The 'Name' field can contain any device name you 
						like - I would suggest just typing 'Irrigation 
						controller' in this field.</li>
						<li>Leave the 'Protocol' field as 'TCP'.</li>
						<li>Leave the 'LAN Port' field as '80'</li>
						<li>In the 'Destination IP' field you type the local IP 
						address of your irrigation controller - in this example 
						it was '10.0.0.79'.</li>
						<li>Now the 'WAN Port' field can contain any integer you 
						like - it is used if you have multiple network devices 
						that you want to access away from home.</li>
						<li>For example, if you were running two irrigation 
						controllers on your home network then you could give the 
						first one a WAN port of 84 and the second one a WAN port 
						of 85.</li>
						<li>In this example the public IP address of the ADSL 
						modem was '101.182.208.140' so, at the office you could 
						access the first irrigation controller by typing 
						'101.182.208.140<b>:84</b>' into the address bar of your web 
						browser - please note ':84'.</li>
						<li>The second irrigation controller would be accessed 
						by typing '101.182.208.140<b>:85</b>' into the address 
						bar of your web browser.</li>
						<li>If you have only one irrigation controller that you 
						want access from the office for example then you could 
						leave the 'WAN Port' field as '80' and you would then 
						access your irrigation controller at work bt typing 
						'101.182.208.140' into the address bar of your web 
						browser.</li>
						<li>Once you have finished you will need to click the 
						blue button in this example to save the changes you have 
						made.</li>
					</ul>
					<h3><a name="Dynamic_IP_addresses">Dynamic IP addresses</a></h3>
					<ul>
						<li>More than likely you will have a dynamic public IP 
						address asigned to you by your <strong>I</strong>nternet<strong> 
						S</strong>ervice <strong>P</strong>rovider.</li>
						<li>This means that your public IP address, that you 
						would use to access your irrigation controller from work 
						like this '101.182.208.140<b>:85</b>' or like this 
						'101.182.208.140', will change when ever your ADSL modem 
						reboots and somtimes on the fly at the discretion of 
						your ISP. .</li>
						<li>So tomorrow or next week you may have to access your 
						irrigation controller from work via '101.182.250.145' 
						rather than '101.182.208.140'.</li>
						<li>There are two ways around this:<ul>
							<li>Request a static IP address from you ISP - this 
							will incurr additional costs but it is the simplest 
							method of overcoming the above problem because your 
							public IP address will never change.</li>
							<li>Simply check what you latest public IP address 
							is before leaving home and hope that it does not 
							change while you are away from home.</li>
							<li>Use a DNS hosting service like '<span><a href="https://freedns.afraid.org">https://freedns.afraid.org</a></span>'.</li>
							<li>There are many others but I have used this one 
							so I will use it as an example here.</li>
							<li>Most of these web sites allow you to setup a 
							free account with the bare 
							basic features.</li>
						</ul>
						</li>
					</ul>
					<h4><a name="freedns.afraid.org">freedns.afraid.org</a></h4>
					<ul>
						<li>Setting up an account is quite simple and I won't go 
						into details here.</li>
						<li>Once you create you account click on the link named 
						'Subdomain'.</li>
					</ul>
					<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					<a href="images/FreeDNS1.jpg">
					<img alt="" class="auto-style1" height="448" src="images/FreeDNS1.jpg" width="800"></a>&nbsp;</p>
					<ul>
						<li>Next click the link named 'Add'.</li>
					</ul>
					<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					<a href="images/FreeDNS2.jpg">
					<img alt="" class="auto-style1" height="460" src="images/FreeDNS2.jpg" width="802"></a>&nbsp;</p>
					<ul>
						<li>The first thing you need to do is give your 
						subdomain a name in the 'Subdomain' edit field - in this 
						example I have simply used my surname 'boyles' as the 
						name of my sub-domain but you can choose any meaningful 
						name you like.</li>
						<li>Next you need to choose one of the fee domains from 
						the 'Domain' combox box below:</li>
					</ul>
					<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					<a href="images/FreeDNS3.jpg">
					<img alt="" class="auto-style1" height="138" src="images/FreeDNS3.jpg" width="216"></a>&nbsp;</p>
					<ul>
						<li>If you don't like any of those domains then click 
						'Many many more available':</li>
					</ul>
					<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					<a href="images/FreeDNS4.jpg">
					<img alt="" class="auto-style1" height="450" src="images/FreeDNS4.jpg" width="801"></a>&nbsp;</p>
					<ul>
						<li>From here click 'Shared domain registry':</li>
					</ul>
					<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					<a href="images/FreeDNS5.jpg">
					<img alt="" class="auto-style1" height="443" src="images/FreeDNS5.jpg" width="802"></a>&nbsp;</p>
					<ul>
						<li>Select any domain that takes your fancy and it will 
						then appear in the combo box back here:</li>
					</ul>
					<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					<a href="images/FreeDNS6.jpg">
					<img alt="" class="auto-style1" height="462" src="images/FreeDNS6.jpg" width="800"></a>&nbsp;</p>
					<ul>
						<li>Your current public IP address automatically appears 
						in the 'Destination' edit box.</li>
						<li>Leave the 'Type' combox box on 'A'.</li>
						<li>Now you just need to type the text in the 'captcha' 
						image into the edit box below - this is just a check to 
						make sure you are not a 'web robot' up to no good.</li>
						<li>Now click the 'Save!' button and you will have 
						created your subdomain.</li>
						<li>I have previously created the subdomain 
						'boylesg.homenet.org'</li>
					</ul>
					<h4><a name="Dynamic_DNS_on_your_ADSL_modem">Dynamic DNS on 
					your ADSL modem</a>&nbsp;</h4>
					<ul>
						<li>Now you need to go back to your ADSL modem settings 
						- type in the web address or the local IP address of 
						your ADSL modem and click the 'User settings' link</li>
					</ul>
					<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					<a href="images/Browser3.jpg">
					<img alt="" height="439" src="images/Browser3.jpg" width="801" class="auto-style1"></a>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; 
					&nbsp;</p>
					<ul>
						<li>From the 'Service name' combo box you need to select 
						the name of the free DNS service you have chosen to use 
						- in this example we have chosen 'freedns.afraid.org' so 
						select 'FREEDNS' in from the combo box.</li>
						<li>Back at 'https://freedns.afraid.org' 
						there is a link 'Router setup guide' - this explains how 
						to setup your ADSL modem to use the subdomian you have 
						just created in your 'freedns.afraid.org' 
						account.</li>
						<li>Essentially you need to click the link named 
						'Dynamic DNS'.</li>
						<li>Go down to the bottom of this page and you should 
						see your subdomain - in this example it is 
						'boylesg.homenet.org'</li>
					</ul>
					<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &nbsp;<a href="images/FreeDNS7.jpg"><img alt="" class="auto-style1" height="432" src="images/FreeDNS7.jpg" width="801"></a></p>
					<ul>
						<li>Click the link named 'Direct URL':</li>
						<li>Ignore the page contents - you need to look up in 
						the address bar for you web browser that will contain 
						text something like this 
						'https://freedns.afraid.org/dynamic/update.php?d1E0RmdTUGxDOFQ2YmRncllMWVkxQWxsOjE2MTg1Njk1'.</li>
						<li>Highlight all the text after the '?' character and 
						copy it to your clipboard.</li>
						<li>Now go back to here:</li>
					</ul>
					<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					<a href="images/Browser3.jpg">
					<img alt="" height="439" src="images/Browser3.jpg" width="801" class="auto-style1"></a>&nbsp;</p>
					<ul>
						<li>In the 'Domain' edit field you need to type your 
						FreeDNS sub-domain name - in this exaple it is 
						'boylesg.homenet.org'.</li>
						<li>Type a comma - ','.</li>
						<li>Now paste the text you copied to your clipboard.</li>
						<li>Into both the 'User Name' and 'Password' edit fields 
						type the word 'guest'.</li>
						<li>Now you will be able to type the web address 
						'boylesg.homenet.org' into the address field of your web 
						browser, at any location, and you will be able to view 
						the web pages from your irrigation controller.</li>
						<li>And your ADSL modem will automatically update your 
						public IP address at&nbsp; 'freedns.afraid.org' whenever 
						it changes.</li>
					</ul>
					<h4><a name="IP_address_update_app">IP address update app</a></h4>
					<ul>
						<li>If your ADSL modem does not have the above feature 
						then, when your public IP address changes, 
						'boylesg.homenet.org' would then have an invalid IP 
						address stored against it and you won't be able to 
						connect to your irrigation controller.</li>
						<li>In this situation you would normally have to go back 
						to this werb page and click the 
						'Direct URL' to make 'freedns.afraid.org' update your 
						public IP address:</li>
					</ul>
					<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &nbsp;<a href="images/FreeDNS7.jpg"><img alt="" class="auto-style1" height="432" src="images/FreeDNS7.jpg" width="801"></a></p>
					<ul>
						<li>However, conveninetly, the community has created a 
						whole bunch of apps that run in the background of your 
						Windows PC, Android device or Apple device and 
						automatically make 'freedns.afraid.org' update your 
						public IP address.</li>
						<li>The only restriction is that the device needs to 
						remain at home, running all the time and connected to 
						your home network.</li>
						<li>Now it is likely that you have more than one mobile 
						device, e.g. a tablet associated with your landline 
						phone or a spare mobile phone or what ever.</li>
						<li>These various apps are listed here for download and 
						installation:
						<span><a href="https://freedns.afraid.org/scripts/freedns.clients.php">
						https://freedns.afraid.org/scripts/freedns.clients.php</a></span>.</li>
						<li>Installing an Android or IPhone app is far less 
						prone to catastrophes.</li>
					</ul>
					<h3><a name="Irrigation_Controller_User_Interface">
					Irrigation Controller User Interface</a></h3>
					<ul>
						<li>Please remember the device is not as powerful as a laptop 
					or android device so it will take longer to download the web 
					pages from the device - please be patient.</li>
						<li>If the irriagtion controller fails to respond then 
						please wait a few minutes and hit the refresh button in 
						your web browser.</li>
						<li>If the irrigation controller is in the process of 
						turning stations on or off then this is probably why it 
						has failed to respond to your web page request.</li>
					</ul>
					<h4><a name="System_Clock">System Clock</a></h4>
					<p>
					<a href="images/SystemClock.jpg">
					<img alt="" height="416" src="images/SystemClock.jpg" width="701" class="auto-style1"></a></p>
					<ul>
						<li>The first thing you need to do is visit this page 
						and change your time zone to the appropriate offset from 
						Greenwich Mean Time (Universal Time).</li>
						<li>By default this is set to +10 hours (Australian 
						Eastern Standard Time).</li>
						<li>If you region does use daylight savings then uncheck 
						the 'Adjust time for daylight savings' check box.</li>
						<li>The system time of the device you are using will be 
						automatically detected, but if this is incorrect then 
						you can specifiy a system time and date in the relevant 
						edit fields.</li>
						<li>There is also an edit field 'Irrigation Controller 
						ID' that allows you to enter a name for the irrigation 
						controller, which is useful if you happen to be running 
						two or more controllers.</li>
						<li>The name you enter if this field appears at the top 
						of each web page.</li>
						<li>Hit the 'Upload to irrigation controller' and the 
						time in the Irrigation controller will be altered 
						accordingly.</li>
						<li>Please also note that the irrigation controller will 
						automatically synchronise its internal clock with the 
						Greenwich time server (plus time zone adjustment) on 
						boot up and once weekly.</li>
						<li>Also note this this page gives you an indication of 
						the backup battery voltage for the irrigation 
						controllers internal clock.</li>
						<li>The back battery consists of 2 x 1.5V AA cells and 
						these will keep the internal clock ticking should power 
						to the irrigation controller be lost.</li>
						<li>If you set an email address in the 'Email Settings' 
						below then the irrigation controller will email when the 
						back battery needs to be replaced.</li>
					</ul>
					<h4><a name="Manual_Control">Manual Control</a></h4>
					<p>
					<a href="images/ManorHouseDriveEppingPond.jpg">
					<img alt="" height="412" src="images/ManualControl.jpg" width="700" class="auto-style1"></a></p>
					<ul>
						<li>Simply select the station and type in a run time in 
						minutes and hit the upload button.</li>
						<li>If that station is already running then it you will 
						get a message stating this.</li>
					</ul>
					<h4><a name="Irrigation_Programs">Irrigation Programs</a></h4>
					<p><a href="images/IrrigationPrograms.jpg">
					<img alt="" class="auto-style1" height="312" src="images/IrrigationPrograms.jpg" width="701"></a></p>
					<h5><a name="Station_Description">Station Description</a></h5>
					<ul>
						<li>The first thing to note about this page is the edit 
						field, containing 'Station 1' in this example, immediately below the 
						'Settings for' (stations 1 - 8) combo box.</li>
						<li>This field allows you to enter a description for 
						each station so that you can easily remember what part 
						of the garden each 
						station is irrigating.</li>
						<li>Examples might be 'vege garden', 'side' or 'front'.</li>
					</ul>
					<h5><a name="Persistence_of_station_details">Persistence of station details</a></h5>
					<ul>
						<li>Another important thing to note is that, once you 
						enter irrigation details for the currently selected 
						'Station 1', the details are saved in the background.</li>
						<li>If you select a different station and then go back 
						to station 1 then you will find that all your settings 
						for station 1 are the same.</li>
						<li>Each station will be intially set to default once a 
						month watering.</li>
					</ul>
					<h5><a name="Uploading_to_Irrigation_Controller">Uploading to Irrigation Controller</a></h5>
					<ul>
						<li>Notice than in the above example the 'Upload to 
						irigation controller' button is disabled.</li>
						<li>In order for this button to become enabled the big 
						list box above that button must be filled for at least 
						one of the stations from 1 to 8.</li>
					</ul>
					<h5><a name="Correct_Procedure">Correct Procedure</a></h5>
					<ol>
						<li>Select the station to which you want to make changes 
						by selecting a station from the 'Settings fo' combo box at the 
						top of the form.</li>
						<li>Type or re-type the description you want to apply to 
						this station.</li>
						<li>If you wish to enter a period over which irrigation 
						for this station will be suspended then use the two edit 
						field labelled &quot;Suspend irrigation between&quot; - the top 
						edit box is the start date, the bottom edit box is the 
						end date and the data format should be of the form 1/12. 
						You can use this feature to suspend irrigation to an 
						area of your garden over winter for example.</li>
						<li>Next you need to choose a broad frequency via the 
						radio buttons labelled 'Monthly', 'Weekly' and 'Daily' - 
						clicking these will enable the drop down box beside it 
						and disable the other drop down boxes.</li>
						<li>Next you need to select a fine frequency via the 
						drop down box - if you have selected the 'Weekly' radio 
						button then you have the option of irrigating once per 
						week, once every 2 weeks or once every 3 weeks.</li>
						<li>Next you will need to select a station run time in 
						minutes from the 'Run time' edit box.</li>
						<li>Then select a run frequency from the 'Run every' 
						drop down box - options range from once every 2 hours to 
						once per 24 hours.</li>
						<li>You can also select a start and end times in 24 hour 
						clock format, e.g. 23:00. For example, if you have 
						selected 'once every 2 hours', and you enter start and 
						end times of 1:00 and 6:00 then the controller will turn 
						this station on every 2 hours between 1:00am and 6:00am. 
						If you don't enter an start and end times then the 
						controller will turn the station on every 2 hours 
						between 12:00am and the following 12:00am.</li>
						<li>You will notice that changing any of these settings 
						will result in the contents of the two list boxes being 
						updated automatically.</li>
						<li>The station will be turned on at these dates and at 
						these times during that day.</li>
						<li>You can hit the &quot;Upload to irrigation controller&quot; 
						button any time to upload your changes.</li>
					</ol>
					<h5><a name="Turning_a_station_off">Turning a station off</a></h5>
					<ul>
						<li>If you want to turn a station off then simply hit 
						the 'Station off' button.</li>
						<li>This will result in the two list boxes being emptied 
						and a '0'&nbsp; (minutes) will be placed in the 
						'Runtime' edit field.</li>
						<li>A station run time of 0 minutes indicates to the 
						irrigation controller that this station is not in use.</li>
					</ul>
					<h4><a name="Wired_Soil_Moisture_Alarms">Wired Soil Moisture 
					Alarms</a></h4>
					<p><a href="images/WiredAlarms.jpg">
					<img alt="" class="auto-style1" height="413" src="images/WiredAlarms.jpg" width="700"></a></p>
					<ul>
						<li>Here you can set system alarms that can email you 
						when the alarm is triggered and/or automatically turn a 
						station on to rectify the alarm condition - you can use 
						them to alert you to the fact that the irrigation 
						frequency you have set is inadequate or that a solenoid 
						valve has failed etc.</li>
						<li>The feature was designed with soil moisture probes 
						in mind, consisting of two metal contacts 0.5 to 1.0cm 
						apart, embedded in the soil and that measure soil 
						conductivity.</li>
						<li>But the feature can equally work with an electronic 
						leaf.</li>
						<li>For each station for which you have installed a soil 
						moisture probe, click the &quot;Installed' check box - this 
						will enable the other inputs.</li>
						<li>If you want the system to automatically turn the 
						station on if an alarm condition is detected then click 
						the 'Auto' check box.</li>
						<li>Next type in your allowed dry time in minutes - the 
						alarm will not be triggered until this number of minutes 
						has elapsed since the last alarm condition was detected.</li>
						<li>Next type in probe threshold value - the alarm will 
						not be triggered unless the value measured on the soil 
						proble is below this value for the number of minutes you 
						have specified in the previous step</li>
						<li>The default value of 600 corresponds to bone dry 
						soil while a measurement of over 2000 would correspond 
						to sopping wet soil - you will need to play with this 
						value until you get it about right for your particular 
						situation.</li>
						<li>Next you need to specify an on time in minutes - if 
						an alarm is dected and you have the 'Auto' checkb box 
						checked then the controller will turn this station on 
						for this number of minutes.</li>
					</ul>
					<h5><a name="Making_a_moisture_probe">Making a moisture 
					probe</a></h5>
					<ul>
						<li>Moisture probes are simply two metal contacts about 
						1cm apart - they are very easy to make if you are handy.</li>
						<li>Here is one I made:</li>
					<p><a href="images/MoistureProbe.jpg">
					<img alt="" src="images/MoistureProbe.jpg" height="440" width="578" ></a></p>
					<li>I made it from these two products available at Bunnings:</li>

				</ul>

				<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
				<img alt="" src="images/RiserStake.jpg" height="296" width="293">&nbsp;
				<img alt="" height="296" src="images/RiserTube.jpg" width="297"></p>

		        <ul>
					<li>The riser stakes in the left photo are ideal as they 
					are strong and made of stainless steel that can be 
					soldered.</li>
					<li>I simply cut down the riser stakes into short 
					lengths, cut a hort length of dual core speaker wire and 
					soldered each core of the speaker wire to two of the 
					pieces of riser stake.</li>
					<li>You need to put a small piece of heat shrink tube 
					over the solder jount to ensure that the soft copper 
					core of the speaker wire does not break off over time 
					due to flexing.</li>
					<li>I then cut short lengths of the plastic riser tube 
					in the right photo and slid them over the lenghts of 
					stainless steel such that a short length of each was 
					protruding.</li>
					<li>I then simply taped to two firmly together using 
					black electrical insulation tape and attache an 
					electrical terminal block to the other end of the 
					speaker wires - this allows for easy connecting of wire 
					that then goes to the irrigation controller.</li>
					<li>It is best to use metals that won't corrode in the 
					moist soil.</li>
					<li>Stainless steel is ideal because you can solder 
					copper core electrical wire to it.</li>
					<li>But you can use aluminium if you wish, bearing in 
					mind that you won't be able to solder copper core 
					electrical wire to it - some sort of stiff aluminium 
					wire, that you can attach the electrical terminal block 
					directly to, would be ideal.</li>
				</ul>
				<h4><a name="Email_Settings">Email Settings</a></h4>
				<p>
				<a href="images/EmailSettings.jpg">
				<img alt="" class="auto-style1" height="414" src="images/EmailSettings.jpg" width="700"></a></p>
				<ul>
					<li>On this web page you can enter your email address 
					that will received alarm notifications and your email 
					account settings.</li>
					<li>Then you will need to type in your outgoing mail 
					server, mail server username and password.</li>
					<li>I will use Microsoft Outlook 2007 as an example for 
					obtaining these email account settings if you don' have 
					them handy some where:<ul>
						<li>Click 'Tools' from the menu bar and select 
						'Account details' from the popup menu.</li>
					</ul>
					</li>
				</ul>
				<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
				<a href="images/OutlookToolsMenu.jpg">
				<img alt="" class="auto-style1" height="401" src="images/OutlookToolsMenu.jpg" width="701"></a> 
				&nbsp;</p>
				<ul>
					<ul>
						<li>You should see this:</li>
					</ul>
				</ul>
				<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
				<a href="images/OutlookToolsMenu.jpg">
				<img alt="" class="auto-style1" height="499" src="images/OutllookEmailAccounts.jpg" width="613"></a>&nbsp;</p>
				<ul>
					<ul>
						<li>Select the appropriate email address and hit the 
						'Change' button and you should then see this:</li>
					</ul>
				</ul>
				<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
				<a href="images/OutllookEmailAccountDetails.jpg">
				<img alt="" class="auto-style1" height="468" src="images/OutllookEmailAccountDetails.jpg" width="624"></a> 
				&nbsp;</p>
				<ul>
					<ul>
						<li>You will need to write down the contents of 
						'Outgoing mail server (SMTP)' and 'User Name'</li>
						<li>Unfortunately there is no way to reveal the 
						actual contents of&nbsp; 'Password' so, if you don't 
						know or can't remember the password for your email 
						account then you will need to login in to your 
						Bigpond (or other ISP) account and find it there.</li>
						<li>If you don't use Microsoft Outlook as your email 
						program then you will need to consult the online 
						user manual for the software you are using, but it 
						is likely that the process will be similar.</li>
					</ul>
				</ul>
				<h4><a name="Changing_the_Backup_Battery">Changing the Backup Battery</a></h4>
				<ul>
					<li>Simply remove the lid, locate the battery holder and 
					replace the two AA cells.</li>
					<li>Then cycle the power to the irrigation controller to 
					make it re-boot and synchronize its clock with the 
					Greenwich timer server.</li>
				</ul>
				<p><a href="images/BatteryHolder.png">
				<img alt="" class="auto-style1" height="68" src="images/BatteryHolder.png" width="102"></a></p>
				<h4><a name="Irrigation_Controller_Data_Files">Irrigation Controller Data Files</a></h4>
				<ul>
					<li>These data files are simple text files so you can 
					edit them directly with Windows Notepad or any other 
					simple text editor.</li>
					<li>But please ensure you maintain the format detailed 
					below for each file.</li>
				</ul>
				<h5><a name="program.txt">program.txt</a></h5>
				<ul>
					<li>This file has the following format</li>
				</ul>
				<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;station1<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;description:front<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;suspend:1/4-1/8<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;1/1=00:00-1,08:00-1,16:00-1<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;1/3=00:00-1,08:00-1,16:00-1<br>
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;30/4=00:00-1,08:00-1,16:00-1<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;29/6=00:00-1,08:00-1,16:00-1<br>
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;28/8=00:00-1,08:00-1,16:00-1<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;27/10=00:00-1,08:00-1,16:00-1<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;26/12=00:00-1,08:00-1,16:00-1<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;station2<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;description:2222<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;suspend:<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;1/1=00:00-10<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;1/3=00:00-10<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;30/4=00:00-10<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;29/6=00:00-10<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;28/8=00:00-10<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;27/10=00:00-10<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;26/12=00:00-10<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;station3<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;description:3333<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;suspend:<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;1/1=00:00-10<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;1/3=00:00-10<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;30/4=00:00-10<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;29/6=00:00-10<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;28/8=00:00-10<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;27/10=00:00-10<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;26/12=00:00-10<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;station4<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;description:4444<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;suspend:<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;1/1=00:00-10<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;1/3=00:00-10<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;30/4=00:00-10<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;29/6=00:00-10<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;28/8=00:00-10<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;27/10=00:00-10<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;26/12=00:00-10<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;station5<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;description:5555<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;suspend:<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;1/1=00:00-10<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;1/3=00:00-10<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;30/4=00:00-10<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;29/6=00:00-10<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;28/8=00:00-10<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;27/10=00:00-10<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;26/12=00:00-10<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;station6<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;description:6666<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;suspend:<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;1/1=00:00-10<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;1/3=00:00-10<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;30/4=00:00-10<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;29/6=00:00-10<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;28/8=00:00-10<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;27/10=00:00-10<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;26/12=00:00-10<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;station7<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;description:7777<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;suspend:<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;1/1=00:00-10<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;1/3=00:00-10<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;30/4=00:00-10<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;29/6=00:00-10<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;28/8=00:00-10<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;27/10=00:00-10<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;26/12=00:00-10<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;station8<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;description:8888<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;suspend:<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;1/1=00:00-10<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;1/3=00:00-10<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;30/4=00:00-10<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;29/6=00:00-10<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;28/8=00:00-10<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;27/10=00:00-10<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;26/12=00:00-10<br />
				</p>
				<ul>
					<li>The rules are as follows:<ul>
						<li>The key words 'station1' through to 'station8' 
						MUST appear in the file on seperate lines.</li>
						<li>Each station must be followed by the key words 
						'description:' and 'suspend:' on seperate lines and 
						in that order, but can be blank after the colon (<strong>:</strong>) 
						if you wish.</li>
						<li>The key word 'description' can followed by a 
						text string up to 24 characters long describing the 
						station, e.g. front, back or vegetables.</li>
						<li>The key word 'suspend' can be followed by 2 
						dates seperated by a dash (<strong>-</strong>) and the 
						dates must have the format day/month (no year 
						required). I.E. Irrigation on this station will be 
						suspended starting from the first date and ending 
						with the second date. The two dates can span the end 
						of the year, e.g. 2/10-2/2.</li>
						<li>Following these can be any number of dates on 
						seperate lines and each date must be have the format 
						day/month.</li>
						<li>Each date must also be follwed by an equal (<strong>=</strong>) 
						and at least one start time / run time pair, with 
						multiple start time / run time pairs seperated by
						comma (<strong>,</strong>). E.G. 1/1=1:00-10,2:00-10 
						or 1/1=1:00</li>
						<li>Start times must have the format h:mm.</li>
						<li>Run time is in minutes and must be a whole 
						number.</li>
						<li>The maximum number of start time / run time 
						pairs per date is 12 - any start time / run time 
						pairs beyond 12 will be ignored.</li>
						<li>You can use indenting (using tab characters or 
						space characters) to make the file easier to read.</li>
					</ul>
					</li>
					<li>The web page (html file) interface provides you with 
					a restricted range of irrigation frequencies etc, and 
					this is necessary to keep the html file size to a 
					minimum and therefore the download time of that web page 
					to a minumum.</li>
					<li>However by editing 'program.txt' directly you can 
					enter any dates and start times you want:<ul>
						<li>The dates you enter don't need to be regular.</li>
						<li>The start times don't need to be regular.</li>
						<li>And the run times for each start don't need to 
						be the same number of minutes.</li>
					</ul>
					</li>
				</ul>
				<h5><a name="alarms.txt">alarms.txt</a></h5>
				<ul>
					<li>This file has the following format:</li>
				</ul>
				<p>
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;station1<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;probe:<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;allowed_dry_time:0<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;probe_threshold_value:0<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;probe_auto:<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;auto_runtime:0<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;station2<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;probe:installed<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;allowed_dry_time:10<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;probe_threshold_value:1500<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;probe_auto:yes<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;auto_runtime:1<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;station3<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;probe:<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;allowed_dry_time:0<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;probe_threshold_value:0<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;probe_auto:<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;auto_runtime:0<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;station4<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;probe:<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;allowed_dry_time:0<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;probe_threshold_value:0<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;probe_auto:<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;auto_runtime:0<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;station5<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;probe:<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;allowed_dry_time:0<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;probe_threshold_value:0<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;probe_auto:<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;auto_runtime:0<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;station6<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;probe:<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;allowed_dry_time:0<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;probe_threshold_value:0<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;probe_auto:<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;auto_runtime:0<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;station7<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;probe:<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;allowed_dry_time:0<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;probe_threshold_value:0<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;probe_auto:<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;auto_runtime:0<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;station8<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;probe:<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;allowed_dry_time:0<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;probe_threshold_value:0<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;probe_auto:<br />
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;auto_runtime:0<br />
				</p>
				<ul>
					<li>The rules are as follows:<ul>
						<li>The key words 'station1' through to 'station8' 
						MUST appear in the file on seperate lines.</li>
						<li>Each station must be followed by the key words 
						'probe:', 'allowed_dry_time:', 
						'probe_threshold_value:', 'probe_auto:' and 
						'auto_runtime:' on seperate lines and 
						in that order, but can be blank after the colon (<strong>:</strong>) 
						if you wish.</li>
						<li>If you have a wired soil moisture probe 
						installed on any particular station then you must 
						have the key word 'installed' after 'probe:' for 
						that station.</li>
						<li>You should also 
						have values after the key words 'allowed_dry_time:' 
						and 
						'probe_threshold_value:'<ul>
							<li>The value after 'allowed_dry_time:' should 
							be a whole number corresponding the number of 
							minutes the soil is allowed to remain dry for 
							this station.</li>
							<li>The value after 'probe_threshold_value:' 
							should be a whole number corresponding to the 
							soil conductivity and therefore how wet it is - 
							600 corresponds to bone dry soil while 2000 
							corresponds to sopping wet soil and you will 
							need to play around with this value to get it 
							right for you particular situation.</li>
						</ul>
						</li>
						<li>So, if the controller measures soil conductivity 
						at or below the value for&nbsp; 
						'probe_threshold_value:' and for longer than the 
						number of minutes after 'allowed_dry_time:' then a 
						system alarm will be triggered and you will receive 
						an email.</li>
					</ul>
					</li>
					<ul>
						<li>Optionally you can have the controller 
						automatically turn the station on if an alarm is 
						triggered on that station.</li>
						<li>If you want this to happen then you must put the 
						key word 'yes' after the key word '&nbsp;probe_auto:'.</li>
						<li>You must also specify the number of minutes that 
						the station should be turned on for by typing a 
						whole number after the key word 'auto_runtime:'.</li>
					</ul>
					<li>This feature can also be used to warn you should any 
					of your solenoid valves or pipes fail.</li>
				</ul>
				<h5><a name="wifi.txt">wifi.txt</a></h5>
				<ul>
					<li>This file contains to lines of text:<ul>
						<li>Your wifi network name.</li>
						<li>Your wifi network key.</li>
					</ul>
					</li>
				</ul>
				<h5><a name="email.txt">email.txt</a></h5>
				<ul>
					<li>This file contains 4 lines ot text:<ul>
						<li>The email address that alarm notifcations will 
						be emailed to.</li>
						<li>Your outgoing mail server.</li>
						<li>Your mail server user name.</li>
						<li>Your mail server password.</li>
					</ul>
					</li>
				</ul>
				<h5><a name="timezone.txt">timezone.txt</a></h5>
				<ul>
					<li>This file contains 2 lines ot text:<ul>
						<li>Your time zone offset from Greenwich Mean Time - 
						this should be a whole number and can be negative.</li>
						<li>Either 'yes' or 'no' that specifies whether or 
						not to adjust the time for daylight savings.</li>
					</ul>
					</li>
				</ul>

<!--********************************************************************************************************************************************
	********************************************************************************************************************************************
	End main content
	********************************************************************************************************************************************
    ********************************************************************************************************************************************-->

			    </div>
			    			    			    
			</div>
			    
			<?php include "Footer.htm"; ?>
		
		</div>
		
	</body>

</html>
