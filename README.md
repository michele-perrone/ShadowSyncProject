# Shadow Sync Project

Shadow Sync Project (SSP) is an interactive installation 

## Page index
  1. [Documents](#documents)
  2. [Installation](#installation)
  3. [Virtual Enviroment](#virtual-enviroment)
  4. [Libraries](#libraries)

## Documents
[Abstract](https://www.overleaf.com/3486219427fydybqrpkgxq)

[PPT Presentation](https://docs.google.com/presentation/d/1LFyTLhim1t9SbvFDJJ7-GJlO1ojOpn4Hszm1eV8Ltk4/edit?usp=sharing)

[OSC Communication Scheme](https://docs.google.com/drawings/d/1_bteFoAQ-XNcbMFUPh2aHLeAcxxXcW8jXVOBckXudQc/edit?usp=sharing)

## Installation

The instructions and the commands are meant for Linux and macOS. On Windows, it is suggested to install a POSIX shell or Windows Subsystems for Linux (WSL).   
Download the repository first:

```
git clone https://github.com/Jacopo-brz/Shadow_sync_project.git
cd Shadow_sync_project
```

SSP needs a Python installation in order to work. Please verify whether you have Python 2 or Python 3 installed by running the following command:

```
python -V
>>> Python 3.8.X
```

If the output is NOT the form`3.X.Y`, call the python scripts with the `python` command. Otherwise, use the `python3` command.

### Virtual environment

It is suggested to make use of a virtual environment in order to obtain the necessary dependencies.

Conda:
```
conda create -n yourenvname python=x.x
conda activate myenv
conda install --file requirements.txt 
```

Pip:
```
python3 -m venv venv
source venv/bin/activate
python -m pip install -r requirements.txt
```

### Python Backend
The setup requires 3 physical computers, one of which is the Server (henceforth called 'S'), which handles communication between two clients ('C1', 'C2'), each running a python backend, an Ofx frontend and a supercollider instance. Make sure that the firewall of each computer keeps the correct ports open as per the diagram below for the local network (1255 for S, 5555 for C1 and C2, 5501 and 5511 for C1, 5502 and 5522 for C2).

![OSC Communication Scheme](/documentation/osc_communication_scheme.png)

1. Fill the info.json file with the IPs and, for the two clients, with their number (1 or 2). If you are sure that the entered IPs are correct and the ports are open, skip step 2 and go to step 3.

2. To conveniently test if the correct ports are opened, we provide a Python script, called called `what_is_my_ip.py` (it extracts the local IP of the computer running it), and two Processing scripts in `osc/processing_osc`, `SimpleOSCSender and SuperBaseOSCReceiver`, which respectively are able to send and receive OSC messages to and from IPs and ports defined in the code.

3. Start `server_main.py` on S. The console periodically prints the network status of the two clients, which at this point should be OFFLINE.

4. Start `client_main.py` on C1 and C2. If all the operations before starting the server have been done correctly, they will appear as ONLINE on the server console.

5. At this point start Ofx and Supercollider as per the following section.

[TUTORIAL TODO]

To start the installation, give as input to the server `alt`+`w`.

To make supercollider start playing, give the server `alt`+`c` as input.

To stop the execution, give input to the server `alt`+`q`.

## Libraries
- [Mediapipe](https://google.github.io/mediapipe/)
- [python-osc](https://pypi.org/project/python-osc/)
