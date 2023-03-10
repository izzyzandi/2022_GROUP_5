Group members: Isabelle Zandi, Adbul Rehman, Haneen Suliman, Siya Shetty, Khalifah Kh J E Aljasem

This project aims to incorporate virtual reality with an interface that loads the formula student car and allows you to alter aspects such as colour, visibility and name.The graphical user interface (GUI) allows users to load the formula car CAD models and make modifications to them. The software provides a standard 3D view of the model, as well as tools for changing part color, adding and removing parts, and applying special effects to parts.

System Requirements

You need to have the following software installed:
•	CMake (minimum 3.5)
•	Visual Studio 2019 version or later
In addition to that OpenVR and VTK directories must be in your local disk. 


Installation
To install the software, follow these steps:
1.	Clone the GitHub repository to your local machine.
2.	Open CMake and select the repository directory as the source code directory.
3.	Choose a build directory and click "Configure".
4.	Select your desired generator (e.g Visual Studio 16 2019) and click "Finish".
5.	After CMake has finished configuring, click "Generate" to generate the project files.
6.	Open the project  in Visual Studio and build the project.

Known Issues
• CMAKE will not find OPENVR and VTK so the path must be added, in order to do that group and advanced must be ticked. 


