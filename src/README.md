Zum Kompilieren und Ausuehren des Programms sollte sich der Nutzer im Hauptverzeichnis (openGL_zauberwuerfel) befinden und dort 
entweder das Skript './exec.sh' oder manuell die Befehle:
1) ./cleancmake.sh
2) cmake .
3) make
4) ./src/openGL_demo

... ausfuehren.

Wenn das Programm erfolgreich erzeugt und gestartet wurde, wird in der Konsole gefragt, ob es im Texturen-Modus gestartet werden soll:
'y' -> Texturen
'n' -> klassischer, farbiger Zauberwuerfel

Nun sollte sich das Grafikfenster oeffnen, der Wuerfel befindet sich initial in der Frontalansicht (nur die Vorderseite ist sichtbar).

Mit der Maus kann der Wuerfel gedreht werden. Dafuer einfach neben, bzw. über/unter den Wuerfel klicken und halten. (Um den Vorgang des Loesens und Drehungen im Allgemeinen
gut sehen zu koennen, sollten mind. 2 Flaechen sichtbar sein.

Es koennen nun folgende Tasteninputs gegeben werden, um den Wuerfel zu steuern:

--Randomisierung--

Der Wuerfel kann mit 'r' als Input randomisiert werden. Das anschließende automatische Loesen funktioniert allerdings nur, wenn der Wuerfel vor und 
nach der Randomisierung nicht verdreht worden ist.

--Automatisches Loesen--

Die Taste 's' loest den Zauberwuerfel, allerdings nur wenn er zuvor mit 'r' randomisiert und anschließend auch nicht verdreht wurde.

Ist der Wuerfel geloest und man moechte ihn nochmals randomizen und loesen, muss das Programm neugestartet werden. 

--Einzelrotationen--

Zum Verdrehen zuerst die Achse und dann die Reihe (bzw. Richtung) mit den unten angegebenen Tastenbefehlen auswaehlen. 

y -> y-Achse
x -> x-Achse
c -> z-Achse

1 -> 1. Reihe 
2 -> 2. Reihe
3 -> 3. Reihe
4 -> 1. Reihe reverse
5 -> 2. Reihe reverse
6 -> 3. Reihe reverse


Bei Fragen oder Unklarheiten wenden Sie sich bitte an:
felix.baumann@stud.sbg.ac.at
jonas.winkler@stud.sbg.ac.at
ravinder.sangar@stud.sbg.ac.at
