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

### Python Backend
Il setup richiede 3 computer fisici, di cui uno è il Server (d'ora in poi chiamato 'S'), il quale gestisce la comunicazione tra due client ('C1', 'C2'), che eseguono ognuno un backend python, un frontend Ofx e un'istanza di supercollider. Ci si assicuri che il firewall di ogni computer tenga aperte le corrette porte come da schema sotto per la rete locale (1255 per S, 5555 per C1 e C2, 5501 e 5511 per C1, 5502 e 5522 per C2).

![OSC Communication Scheme](/documentation/osc_communication_scheme.png)

Compilare congruamente il file info.json con gli ip e, per i due client, con il proprio numero (1 o 2). Se si è sicuri della correttezza degli ip inseriti e dell'apertura delle porte saltare il prossimo punto e andare a quello dopo.

Per testare in modo conveniente l'apertura delle porte sono forniti uno script Python what_is_my_ip.py (estrae l'ip locale del computer che lo esegue) e due script Processing in osc/processing_osc, SimpleOSCSender e SuperBaseOSCReceiver, che rispettivamente sono in grado di mandare e ricevere messaggi OSC verso e da ip e porte definite chiaramente nel codice.

Avviare server_main.py su S. In console viene stampato a intervalli periodici lo stato di rete dei due client, che a questo punto dovrebbe essere OFFLINE.

Avviare client_main.py su C1 e C2. Se tutte le operazioni prima di avviare il server sono state eseguite correttamente, sulla console del server figureranno come ONLINE.

A questo punto avviare Ofx e Supercollider come da sezione seguente.

[TUTORIAL TODO]

Per iniziare l'esecuzione dell'installazione, dare in input al server alt+w.

Per fare in modo che supercollider cominci a suonare, dare in input al server alt+c.

Per terminare l'esecuzione, dare in input al server alt+q.

## Libraries
[Mediapipe](https://google.github.io/mediapipe/)
python-osc
