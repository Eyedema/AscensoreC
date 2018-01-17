#Laboratorio di Sistemi Operativi
##Progetto A.A. 2015/2016– Ascensore2016
Obiettivo del progetto è la realizzazione di un programma che descrive il
comportamento di un ascensore.
L'ascensore opera su quattro piani, portando differenti tipologie di persone da
piano a piano.
Nel dettaglio, le specifiche dell'ascensore, dell'edificio e delle persone da
trasportare sono le seguenti:
- Si considera un edificio di quattro piani, numerati in ordine crescente
{0, 1, 2,3}, dove 0 è il piano terra e 3 è il piano più alto.
- Nell'edificio è presente un unico ascensore, che accetta un carico massimo
di 300 KG.
- L'ascensore si avvia dal piano terra (piano 0), ed aspetta la prima persona
da servire a tale piano. In altre parole, la prima persona che sale
sull’ascensore è al piano 0.
Appena la prima persona è presente nell’ascensore, questo si sposta verso il
piano successivo (piano 1). Da ora in avanti, il sistema si comporta come di
seguito.
- L'ascensore impiega 3 secondi per spostarsi da un piano al successivo o
precedente. A titolo di esempio, impiega 3 secondi per ciascuno dei seguenti
spostamenti:
- andare dal piano 0 al piano 1
- andare dal piano 1 al piano 2
- Quando l'ascensore raggiunge un piano, sosta per 3 secondi. L’ascensore
sosta sempre 3 secondi, anche se ci sono persone che devono salire o
scendere a quel piano.
- Quando l'ascensore sosta ad un piano, le persone che attendono
l'ascensore a quel piano salgono a bordo, senza però superare il carico
massimo permesso (ovvero, 300 KG).
O N.B. prima scendono le persone che hanno indicato quel piano come
destinazione, quindi salgono le nuove persone.
- Ogni persona che sale sull'ascensore ha associato un piano di
destinazione.
- Le seguenti 3 tipologie di persone possono attendere l’ascensore a ciascun
piano:
o Bambini(anche indicati dalla lettera B). Ciascun bambino pesa 40 KG.
O Adulti(anche indicati dalla lettera A). Ciascun adulto pesa 80 KG.
O Addetti alla consegna (anche indicati dalla lettera C). Ciascun addetto alla
consegna pesa 90 KG.
2- La sequenza temporale di arrivo (generazione) delle persone al piano, la
loro tipologia, e il piano di destinazione sono riportati nei file contenuti
all’interno di
laboratorio_file_piano.tar.gz:
o Per il piano 0: file piano0
o Per il piano 1: file piano1
o Per il piano 2: file piano2
o Per il piano 3: file piano3
Ciascuno dei quattro file sopra menzionati è composto da una serie di triple,
dove:
o Il primo valore indica la persona (B, A,C)
o Il secondo valore indica l’istante di tempo (in secondi dall’avvio del
sistema) in cui la persona è generata ed entra nel sistema
o Il terzo valore indica il piano di destinazione della persona.
-Tutte le persone attendono l'ascensore al rispettivo piano secondo una fila
first-in/first-out.
Questo vuol dire che nessuno salta la fila.
-Quando l'ascensore sosta ad un piano, le persone a bordo che hanno
indicato quel piano come destinazione scendono e "scompaiono" dal sistema.
-L’ascensore si muove nel seguente modo:
o Parte dal piano 0
o Appena la prima persona è disponibile al piano 0, sale a bordo e quindi
l’ascensore parte.
O L’ascensore visita i piani iterando il seguente ordine:
 dal piano 0 al piano 1
 dal piano 1 al piano 2
 dal piano 2 al piano 3
 dal piano 3 al piano 2
 dal piano 2 al piano 1
 dal piano 1 al piano 0
Ad ogni piano, le persone possono scendere e salire. Ovviamente si devono
sempre rispettare i tempi di percorrenza e di sosta sopra definiti.
Da notare, non esiste il concetto di “chiamata” dell’ascensore da parte di
persone in attesa ad altri piani
. Inoltre, non esiste il concetto di “indicare il piano di destinazione
all’ascensore”
. L’ascensore, anche vuoto, si sposta sempre al piano successivo (o al
precedente), tramite la sequenza mostrata sopra.
