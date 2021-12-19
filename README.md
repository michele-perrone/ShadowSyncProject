# Shadow_sync_project

* [Shadow_sync_project](#shadow_sync_project)
  * [Documents](#documents)
  * [Installation](#installation)
   * [Virtual Enviroment](#virtual-enviroment)
  * [Libraries](#libraries)

## Documents
[Abstract](https://www.overleaf.com/3486219427fydybqrpkgxq)

[Presentation](https://docs.google.com/presentation/d/1LFyTLhim1t9SbvFDJJ7-GJlO1ojOpn4Hszm1eV8Ltk4/edit?usp=sharing)

[OSC Communication Scheme](https://docs.google.com/drawings/d/1_bteFoAQ-XNcbMFUPh2aHLeAcxxXcW8jXVOBckXudQc/edit?usp=sharing)
![OSC Communication Scheme](/documentation/osc_communication_scheme.png)

## Installation


Scaricare la repo.

```
git clone https://github.com/Jacopo-brz/Shadow_sync_project.git
cd Shadow_sync_project
```

Verificare se usare python o python3. Eseguire il seguente comando

```
python -V
>>> Python 3.8.X
```

Verificare che l'output è della forma `3.X.Y`. Altrimenti ai comandi successivi utilizzare `python3` invece che `python`.

### Virtual enviroment

#### Windows
[Tutorial](https://www.youtube.com/watch?v=ThU13tikHQw)

#### Linux: 
Conda:
```
conda create -n yourenvname python=x.x
conda activate myenv
```

Pip:
```
python3 -m venv venv
source venv/bin/activate
```
Installare le dipendenze di python.

Conda:

Pip:
```
python -m pip install -r requirements.txt
```
#### Mac
conda:

pip:


## Libraries
[Mediapipe](https://google.github.io/mediapipe/)
python-osc
