# Pietro Mihelj SM3201407
from collections import defaultdict
#la classe lmc è ora in grado di eseguire piu codici di fila come se questi fossero l'unico codice presente sulla macchina
class LMC:
    def __init__(self):
        # userò oggetti base di python per rappresentare le componenti dell'LCM i controlli sulla correttezza dei dati verranno quindi esplicitamente fatti
        self.memoria = [0] * 100
        self.accumulatore = 0
        self.pc =  0
        self.flag = 0
        self.input_q = []
        self.output = []
        self.running = True
    
    def carica_dati(self,input,n_op):
        #la funzione prende in input una lista di numeri che contine nelle prima n posizioni cio che va in memoria e nelle
        # seguenti quelloche va nella coda di input
        #la funzione popola la memoria e la coda di input
        self.clean() #per esecuzioni di più codici sullo stesso LMC
        for i,elem in enumerate(input[:n_op]): #inserisco i valori in memoria partendo dalla prima pos libera
            if elem > 999:
                 raise ValueError("Numero in memoria tropo grande max 999\n")
            self.memoria[i] = elem

        for elem in input[n_op:]:
            if elem > 999:
                raise ValueError("Numero tropo grande. max 999\n")
            self.input_q.append(elem)


    def internal_state(self):
        print(f'Programm counter: {self.pc}')
        print(f'Accumulatore: {self.accumulatore}')
        print(f'Flag: {self.flag}')
        print(f'Memoria: {self.memoria}\n')

    def istr_corr (self,verbose):
        #prende in input la specifica del metodo di esecucione (tutto in uno o istruzione per istruzione)
        #se specificato dall'utente mostralo stato interno e permette una esecuzione operazione per operazione
        #restituisce l'istruzione corrente
        if verbose == '1':
             c = input()
             self.internal_state()
        istr = self.memoria[self.pc]
        return istr

    def decode_esec(self, istr):
        #prende in input l'istruzione corrente, la decodifica e la esedue
        #se i valori delle op superano i limiti imposta la flag a presente
        #solleva errori per input non forniti o codice operazione sconosciuti
        op_code = istr // 100
        xx = istr % 100
        if op_code == 1:
            if self.accumulatore + self.memoria[xx] > 999:
                self.flag = 1
            else: 
                self.flag = 0
            self.accumulatore = (self.memoria[xx] + self.accumulatore) %1000
        elif op_code == 2:
            if self.accumulatore - self.memoria[xx] < 0:
                self.flag = 1
            else:   
                self.flag = 0
            self.accumulatore = (self.accumulatore - self.memoria[xx]) %1000
        elif op_code == 3:
            self.memoria[xx] = self.accumulatore
        elif op_code == 5:
            self.accumulatore = self.memoria[xx]
        elif op_code == 6:
            self.pc = xx - 1
        elif op_code == 7:
            if self.accumulatore == 0 and self.flag == 0:
                self.pc = xx - 1
            pass
        elif op_code == 8:
            if self.flag == 0:
                self.pc = xx - 1
            pass
        elif op_code == 9:
            if xx == 1:
                if not self.input_q:
                    raise ValueError("Input non fornito.")
                self.accumulatore = self.input_q.pop(0)
            elif xx == 2:
                self.output.append(self.accumulatore)
                
        elif op_code == 0:
            self.running = False
        else:
             raise ValueError(f"Opcode sconosciuto: {op_code}")
        
    def run(self,verbose):
        #riceve in input il metodo di esecuzione
        #fin che il programma non si ferma con una istr di halt
        #prende l'istruzione 
        #la decodifica ed esegue
        #ed aumenta il valore del programm counter
        while self.running:
            istruzione = self.istr_corr(verbose)
            self.decode_esec(istruzione)
            self.pc += 1 
            if self.pc > 999: #se il pc supera 999 lo imposto a 0  
                self.pc = 0
        print(f'OUTPUT: {self.output}')
    
    def clean(self):
        #ripulisco lo stato inetrno del LMC per l'esecuzione successiva di codici diversi
        self.accumulatore = 0
        self.pc =  0
        self.flag = 0
        self.input_q = []
        self.output = []
        self.running = True

       

class Assembler:
    def __init__(self):
        self.input_lmc = [] #lista completa che andra direttamente in input dell'LMC
        self.istr = [] #lista delle sole istruzioni "pure", cioè senza riferimenti alle celle di memoria 
        self.xx_pos = [] #lista di coppie di celle di memoria e delle loro posizioni nell'assembly
        self.dati = [] #lista dei dati forniti in input dall'utente
        self.label = defaultdict(int) #dizionario con label e valore numerico della cella corrispondente
        self.lab_pos = [] #lista di coppie label posizione di dove viene usata una label
        self.istr_type = {'add':100,'sub':200,'sta':300,'lda':500,'bra':600,'brz':700,'brp':800,'inp':901,'out':902,'hlt':0,'dat':0} #dizionario dei codici delle istruzioni puri 

    def is_number(self,stringa):
        #prende in input una stringa e controlla se questa è un numero
        try:
            float(stringa)  
            return True
        except ValueError:
            return False

    def lettura_file(self, nome):
        #prende in input il nome del file da leggere.
        #restituisce una lista che contiene liste corrispondenti alle righe del file. ogni sottolista contiene le parole della riga corrispondente
        #per poter permettere l'esecuzione di più codici in sequenza sulla stessa macchina 
        # devo ripulire anche l'assembler ogni volta che vado a leggere un file nuovo
        self.input_lmc = [] 
        self.istr = [] 
        self.xx_pos = [] 
        self.dati = [] 
        self.label = defaultdict(int) 
        self.lab_pos = []
        lista = []
        try:
            with open(nome, "r") as file:
                for i, line in enumerate(file):
                    lista.append(line.strip().split("//")[0].strip()) #rimuovo spazi e commenti tra le righe
        except FileNotFoundError:
            print('file non presente o path errato')
            return 0
             


        for i,row in enumerate(lista):
            lista[i] = row.split()  #per ogni riga separo le perole
        
        lista_pulita = [sublista for sublista in lista if sublista] #elimino le righe di solo commenti che sono ora liste vuote
        return lista_pulita

    def traduci(self, nome):
        #prende in input il nome del file e genera la lista di parole chiamando la funzione apposita
        #poi itera su queste assegnando ognuna alla lista corrispondente
        #infine somma alle istruzione pure le cella di memoria a cui si riferiscono.
        #restituisce una lista composta dalla lista di ciò che deve finire in memoria seguita da ciò che deve finire nella coda di input
        #e il numero di elementi che finiranno in memoria

        word_list = self.lettura_file(nome) #se il path è errato vado a passare una lista con solo uno zero in maniera che l'LMC faccai solo una 
        if word_list == 0:                  #operazione di halt e esca senza dare errori
             return [0],0
        #itero su tutte le parole del codice assembly
        for j,line in enumerate(word_list):
            for i,word in enumerate(line):
                    if i==0 and word.lower() not in self.istr_type: #se la parola è la prima della riga e non è una istruzione sarà una label
                            self.label[word.lower()] = j                    #la aggiungo quindi al dizionario
                    elif word.lower() in self.istr_type:            #se invece è una istruzione in qualsiasi posizione si trovi
                            self.istr.append(self.istr_type[word.lower()]) #finisce codificata in linguaggio macchina nella lista delle istruzioni
                            if word.lower() == 'inp':               #se l'istruzione è di input richiedo all'utente di inserire i dati di input
                                    temp = eval(input("Inserisci l'input corretto come [...], se sono piu nueri inserirli separati da virgole: "))
                                    for elem in temp:               #inserisco gli elementi della lista di dati fornita in input nella lista di dati
                                         self.dati.append(elem)
                    elif i>0 and self.is_number(word):              #se la parola è un numero e non sta in prima posizione la considero una cella di memoria
                            self.xx_pos.append([j,int(word)])
                    else:   
                         self.lab_pos.append([j,word.lower()])              #se la parola non è un numero e non sta in prima posizione la considero una label
                    
        #sostituisco le label con i loro valori e li sommo alle corrispondenti istruzioni
        for j,lab in self.lab_pos:      
                if lab not in self.label:
                     raise ValueError(f'la parola inserita a riga {j} non è una label esistente')
                self.istr[j] += self.label[lab] 

        #sommo le celle di memoria alle corrispondenti istruzioni
        for i,xx in self.xx_pos:
             self.istr[i] += xx 
        
        self.input_lmc = self.istr + self.dati
        return self.input_lmc, len(self.istr)

assembler = Assembler()
lmc = LMC()
nome = input("inserisci il path al file assembly da eseguire: ")
while nome != 'Stop':     
    verbose = input("per esecuzione completa digiti 0 altrimenti 1: ")
    if verbose == '1':
         print('esecuzione che deve esere mandata avanti volta per volta con un invio da tastiera.\ninoltre verra mostrato lo stato interno ad ogni operazione')
    inp, lung = assembler.traduci(nome)
    lmc.carica_dati(inp,lung)
    lmc.run(verbose)
    nome = input("insere il path al file assembly da eseguire. per fermarsi inserire Stop: ")