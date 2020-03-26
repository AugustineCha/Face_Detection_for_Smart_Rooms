# Design using C++ - Project Proposal
##### Augustine Cha (ac4612) - ac4612@columbia.edu
##### Lawan Rahim (lr2965) - lr2965@columbia.edu
##### Jin Yan (jy2913) - jy2913@columbia.edu
##### March 26, 2020
## SmartRooms
### Brief project description
We develop a tool called ”SmartRooms” which allows the user to regulate accessto rooms or areas using facial recognition software.  Prioritized design criteria arelatency, ease of use, reliability and maintainability.  For demonstration purposesthe software will run on a notebook.  Through a graphical user interface the usercan oversee the operations.

### Roadmap:
As a first step we will integrate the facial recognition algorithm from the publiclyavailable computer vision library OpenCV into our software.  After setting up thefacial recognition system, we move on to create a simple interface with which theuser can easily train the model on recognizing certain faces which could e.g.  be ofpeople or pets living in the household.  Along the same lines, the user should beable to select certain areas or rooms and allocate accesses to different individuals.All of this interaction should be hidden behind a user-friendly graphical interface.

### Measurements:
The measurements will center around the prioritized design criteria, i.e.  we willmeasure the latency and precision with which the facial recognition system works.Moreover, we will try to quantify the memory capacity needed since in practicalimplementations, our software has to run on a microcontroller.

### Tests:
Like the measurements, our tests will also center around the design criteria.  Wewill test the facial recognition system using multiple positive and negative testsamples.  Moreover, we will design test cases for multiple possible functionalitiesaccessible through the GUI. Apart from that, the time delay between the appear-ance of a detectable face in the camera and the response, i.e.  either permissionto access the room or area or denial, will be examined.
### R0.8:
This release will contain a fully working facial recognition system implementedin C++ using simple images of faces.
### R1.0:
On top of the R0.8 functionality, R1.0 should allow for area and person specifi-cation and permission allocations.  Moreover, the facial recognition should workwith live camera stream.
### R1.2:
On top of the previous features, R1.2 incorporates a user-friendly graphical inter-face allowing all the desired functionality in an easy and quick way.  The alarmnotifications should also be implemented.

#### References
[GeorgeSeif/Face-Recognition](https://github.com/GeorgeSeif/Face-Recognition)  
[Jin's Cloud Computing Project](https://designincplusplus.slack.com/files/U010P4VMBRS/F010XK3LLVD/_spring_2020__cloud_computing_and_big_data_-_hw_2.pdf)
