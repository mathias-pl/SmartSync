# SmartSync
***All-in-one smart home solution at a little price***

## Introduction
The Roman poet Virigil said, *“The greatest wealth is health”* and we agree. In this challenge we decided to tackle the third sustainable development goal, **“Good Health and Well-Being.”** Everyone has a right to good health and we wanted to create a product that helps our planet get healthier. 

After some research, we found a shocking statistic, *“the World Health Organization estimates 3.8 million people worldwide die every year from illnesses attributable to indoor conditions”*. We dove deeper and found that indoor homes, that most of the population lives in, have various aspects that have been silently harming its inhabitants. We found that air contaminants, water contaminants, humidity levels, amongst others were all contributing to a decline in the health of its inhabitants if they weren’t properly regulated and monitored. 

That’s where SmartSync Homes come in. We have built a smart home system that can detect dangerous air contaminants and water contaminants and alert you when humidity and temperature values get out of a safe range in real time with accuracy. With our product, any homeowner can make sure that their home is healthy and if not, have the information to be able to restore their home as a healthy environment. Our product is compatible with every home whether it be a house, apartment or another housing style 

***

## Technical aspects
### Software
The most commonly used software used for Arduino development is the default Arduino IDE. We used this IDE for most of the development, with some use of VS Code when writting long blocks of code and wanted to use the autoformat feature. Due to the time constraint, the code is from times to times unoptimized at the profit of faster writing time.

The first part of the code is used to setup all the components connected to the board, and to instantiate the functions that are going to be called. Once everything is ready, we enter the main loop and at each 4 seconds, we retrieve data from all the sensors and display them on the LCD display. The display is still able to update in real time to the button press.

### Hardware
We had a restricted list of permitted items during the competition. Those are the components that have been used:
#### I/O
- SH-D1602: LCD 16x2 Display
- LEDs: 1x Red, 1x Blue, 1x Green, 1x Yellow
- Push buttons: Toggle between the menus of the system
- Potentiometer: Change the brightness of the display
  
#### Sensors
- DHT11: Temperature & humidity sensor
- Solu SL067: Water level sensor (serves as water contaminant detector)
- HC-SR501: PIR motion sensor (serves as air contaminant detector)

***

## Business aspects
- Marketed towards homeowners age 25 and up.
- Advertisement will be done over social medias such as Instagram, TikTok and Facebook.
- There's no such product on the market that regroups all the functionalities offered by this product under the same brand.
- As the water and air detection kits as interchangeable, this is a plus regarding the sustainability plan.
- Based on a fictive budget, we estimate the production cost to be around 300$. With a MSRP of 626$, the profit margin is high at 52% of retail cost (326$).

***

## Conclusion
During this hackathon project, not only that we learned many crucial soft skills such as communication, teamwork, leadership and curiosity, but we also developed other more technical skills. First of all, creating the prototype under 24 hours while setting up a whole presentation and business report proves that we were able to work under pressure and deliver a satisfactory final product. We learned more about how to make all the sensors work given that some were blocking and others non-blocking. On the business side of things, it was mostly learning to speak in front of a jury and answer clearly and concisely their questions, no matter if they were technical or business-oriented.

<p align="center">
  <i><b>Be Smart, Sync with Us at SmartSync Homes</b></i>
</p>
