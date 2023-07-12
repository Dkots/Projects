# Εισαγωγή βιβλιοθηκών
from tkinter import *
from tkinter import messagebox
from tkinter import ttk
from tkcalendar import Calendar
import datetime as dt
import db

# Κλάση παραγγελία
class Paraggelia():
    # Δημιουργία του περιβάλλοντος διεπαφής για την παραγγελία
    def __init__(self):
        ## Σημερινή ημερομηνία και ώρα ##
        date_now = dt.datetime.now()
        hour_now = '19:30'  #str(date_now.time())[0:5]
        cal_now = str(date_now.date())

        ## Ώρα παραγγελίας ##
        self.lbl_hour = Label(win,
                              text='Ωρα',
                              font=("Arial Bold", 15),
                              bg='#a86f4c')
        n3 = StringVar()
        self.txt_hour = Entry(win,
                              width=25)
        self.txt_hour.insert(0, hour_now)
        
        ## Ημερομηνία ##
        self.lbl_cal = Label(win,
                             text='Ημερομηνία Κρατήσης',
                             font=("Arial Bold", 15),
                             bg='#a86f4c')
        # Για την επιλογή της σημερινής ημερομηνίας
        '''self.cal = Calendar(win, selectmode='day',
                            year=int(cal_now[0:4]), month=int(cal_now[5:7]),
                            day=int(cal_now[8:10]))'''
        self.cal = Calendar(win, selectmode='day',
                            year=2022, month=1,
                            day=18)

        ## id_paraggelias
        self.lbl_id_par = Label(win,
                            text='id_paraggelias',
                            font=("Arial Bold", 15),
                            bg='#a86f4c')
        self.txt_id_par = Entry(win,
                            width=25)
        # Αναζήτηση του επόμενου id παραγγελίας
        sql_par = '''select count(*) from PARAGGELIA;'''
        list_par = d.executeSQL(sql_par, show=False)  # [(---,), (---,)]
        self.txt_id_par.insert(0, str(int(list_par[0][0]) + 1))

        ## id_trapezi// Τα τραπέζια της κράτησης
        self.lbl_id_tra = Label(win,
                            text='Τραπέζι',
                            font=("Arial Bold", 15),
                            bg='#a86f4c')
        n0 = StringVar()
        self.txt_id_tra = ttk.Combobox(win, width=25, textvariable=n0)
        self.txt_id_tra.bind("<<ComboboxSelected>>", self.search_atoma)
        # Αναζήτηση των τραπεζιών με την κράτηση
        sql_tr = '''select id_trapezi from KRATISI 
                    where akirwsi = 0 and 
	                date_kratisi = ''' + self.get_cal() + ''' and
	                wra_kratisi <= ''' + self.get_time() + ''' and
	                wra_kratisi_end >= ''' + self.get_time() + '''
	                except
	                select distinct id_trapezi
	                from PARAGGELIA 
	                where date_kratisi = ''' + self.get_cal() + ''' and
	                wra_kratisi <= ''' + self.get_time() + ''' and
	                wra_kratisi_end >= ''' + self.get_time() + '''and 
	                tropos_plirwmis is not NULL;'''
        list_tr = d.executeSQL(sql_tr, show=False)  # [(---,), (---,)]
        self.txt_id_tra['values'] = [item[0] for item in list_tr]  # [---, ---]

        ## Όνομα του εργαζομένου που παίρνει την παραγγελία
        self.lbl_er_onoma = Label(win,
                                text='Σερβιτόρος',
                                font=("Arial Bold", 15),
                                bg='#a86f4c')
        n1 = StringVar()
        self.txt_er_onoma = ttk.Combobox(win, width=25, textvariable=n1)
        # Αναζήτηση στην βάση τα στοιχεία του σερβιτόρου
        sql_serv = '''select firstname, lastname, amka
                        from ERGAZOMENOS inner join SERVITOROS on amka = id_ergazomenou
                        where posto = 'ΣΕΡΒΙΤΟΡΟΣ';'''
        list_serv = d.executeSQL(sql_serv, show=False)  # [(---,), (---,)]
        self.txt_er_onoma['values'] = [item[0] + ' ' + item[1] for item in list_serv]  # [---, ---]
        self.dict_serv = {}
        for item in list_serv:
            self.dict_serv[item[0] + ' ' + item[1]] = item[2]

        ## Άτομα στο τραπέζι
        self.lbl_atoma = Label(win,
                              text='Άτομα',
                              font=("Arial Bold", 15),
                              bg='#a86f4c')
        self.txt_atoma = Entry(win,
                              width=25)

        ## Συνολική αξία παραγγελίας
        self.lbl_axia = Label(win,
                            text='Συνολική Αξία',
                            font=("Arial Bold", 15),
                            bg='#a86f4c')
        self.txt_axia = Entry(win,
                                width=25)
        self.txt_axia.insert(0, 0)

        ## Σχόλια
        self.lbl_txt = Label(win,
                              text='Σχόλια',
                              font=("Arial Bold", 15),
                              bg='#a86f4c')
        self.text = Text(win,
                         height=5,
                         width=35)
        self.text.insert(INSERT, "NULL")

        ## Μενού
        self.lbl_menou = Label(win,
                                  text='Κατάλογος',
                                  font=("Arial Bold", 15),
                                  bg='#a86f4c')
        self.menou = ttk.Treeview(win,
                                  height=20)
        style = ttk.Style()
        style.configure("Treeview.Heading", font=(None, 15))
        # Δημιουργία του πίνακα του μενού
        self.menou['columns'] = ('όνομα', 'τιμή', 'κατηγορία', 'επαρκές')
        self.menou.column("#0", width=0, stretch=NO)
        self.menou.column("όνομα", anchor=W, width=250)
        self.menou.column("τιμή", anchor=CENTER, width=100)
        self.menou.column("κατηγορία", anchor=W, width=100)
        self.menou.column("επαρκές", anchor=W, width=100)
        self.menou.heading("#0", text="", anchor=CENTER)
        self.menou.heading("όνομα", text="Όνομα", anchor=CENTER)
        self.menou.heading("τιμή", text="Τιμή", anchor=CENTER)
        self.menou.heading("κατηγορία", text="Κατηγορία", anchor=CENTER)
        self.menou.heading("επαρκές", text="Επαρκές", anchor=CENTER)
        self.menou.bind('<ButtonRelease-1>', self.selectItem_add)
        # Συνάρτηση εισαγωγής μενού
        self.add_menou()

        ## Ποσότητες
        self.lbl_count = Label(win,
                               text='Ποσότητα',
                               font=("Arial Bold", 15),
                               bg='#a86f4c')
        self.count_piato = Spinbox(win,
                                from_=1,
                                to=10,
                                font=('sans-serif', 11),
                                width=12)

        ## Κουμπί προσθήκης πιάτου
        self.btn_add_piato = Button(win,
                                  text='Προσθήκη',
                                  height=2,
                                  width=13,
                                  bg='grey',
                                  fg='black',
                                  command=self.add_piato)

        ## Κουμπί διαγραφής πιάτου
        self.btn_delete_piato = Button(win,
                                    text='Διαγραφή',
                                    height=2,
                                    width=13,
                                    bg='grey',
                                    fg='black',
                                    command=self.delete_piato)

        ## Κουμπί αποστολή παραγγελίας
        self.btn_send_par = Button(win,
                                       text='Αποστολή',
                                       height=2,
                                       width=13,
                                       bg='grey',
                                       fg='black',
                                       command=self.send_paraggelia)

        ## Προσθήκη πιάτου σε πίνακα
        self.lbl_paraggelia = Label(win,
                               text='Παραγγελία',
                               font=("Arial Bold", 15),
                               bg='#a86f4c')

        self.paraggelia = ttk.Treeview(win, height=20)
        self.paraggelia['columns'] = ('όνομα', 'τιμή', 'κατηγορία', 'ποσότητα')
        self.paraggelia.column("#0", width=0, stretch=NO)
        self.paraggelia.column("όνομα", anchor=W, width=250)
        self.paraggelia.column("τιμή", anchor=CENTER, width=100)
        self.paraggelia.column("κατηγορία", anchor=W, width=100)
        self.paraggelia.column("ποσότητα", anchor=W, width=100)
        self.paraggelia.heading("#0", text="", anchor=CENTER)
        self.paraggelia.heading("όνομα", text="Όνομα", anchor=CENTER)
        self.paraggelia.heading("τιμή", text="Τιμή", anchor=CENTER)
        self.paraggelia.heading("κατηγορία", text="Κατηγορία", anchor=CENTER)
        self.paraggelia.heading("ποσότητα", text="Ποσότητα", anchor=CENTER)
        self.paraggelia.bind('<ButtonRelease-1>', self.selectItem_delete)
        # Αριθμός πιάτων που προσθήθονται στον πίνακα
        self.count_piata = 0

        ## Εμφάνιση Widgets
        self.place_widgets()

    # Συνάρτηση εμφάνιση Widgets
    def place_widgets(self):
        self.lbl_hour.place(x=0, y=0)
        self.txt_hour.place(x=0, y=30)
        self.lbl_cal.place(x=200, y=0)
        self.cal.place(x=200, y=30)
        self.lbl_id_par.place(x=0, y=60)
        self.txt_id_par.place(x=0, y=90)
        self.lbl_id_tra.place(x=0, y=120)
        self.txt_id_tra.place(x=0, y=150)
        self.lbl_er_onoma.place(x=700, y=0)
        self.txt_er_onoma.place(x=700, y=30)
        self.lbl_atoma.place(x=700, y=60)
        self.txt_atoma.place(x=700, y=90)
        self.lbl_axia.place(x=700, y=120)
        self.txt_axia.place(x=700, y=150)
        self.lbl_txt.place(x=900, y=0)
        self.text.place(x=900, y=30)
        self.lbl_menou.place(x=0, y=200)
        self.menou.place(x=0, y=230)
        self.count_piato.place(x=580, y=230)
        self.lbl_count.place(x=580, y=200)
        self.btn_add_piato.place(x=580, y=260)
        self.btn_delete_piato.place(x=580, y=300)
        self.btn_send_par.place(x=580, y=600)
        self.lbl_paraggelia.place(x=700, y=200)
        self.paraggelia.place(x=700, y=230)

    # Συνάρτηση εισαγωγής μενού
    def add_menou(self):
        # Αναζήτηση στην βάση για τα πιάτα και της κατηγορίες τους
        sql_menou = '''select P.onoma, P.timh, KP.onoma, P.id_piato 
                            from PIATO as P inner join KATHGORIA_PIATOY as KP using(id_katigorias);'''
        list_menou = d.executeSQL(sql_menou, show=False)  # [(---,), (---,)]
        list_menou = [list(item) for item in list_menou]
        self.dict_menou = {}
        for list_piato in list_menou:
            self.dict_menou[list_piato[0]] = int(list_piato[3])
        # Αναζήτηση των πιάτων που δεν μπορούν να σερβιριστούν λόγω έλειψης προιόντων
        sql_finished = '''select distinct P.onoma from PIATO as P 
                        inner join PIATO_HAS_PROIONTA as PP using(id_piato)
                        inner join PROIONTA as Pr using(id_proiontos)
                        where Pr.posotita<=1.0;'''
        list_finished = d.executeSQL(sql_finished, show=False)  # [(---,), (---,)]
        list_finished = [item[0] for item in list_finished]
        for i in range(0, len(list_menou)):
            if list_menou[i][0] in list_finished:
                list_menou[i][3] = 0
            else:
                list_menou[i][3] = 1
            self.menou.insert(parent='', index='end', iid=i, text='',
                              values=tuple(list_menou[i]))

    # Συνάρτηση εισαγωγής πιάτου στον πίνακα παραγγελίας
    def add_piato(self):
        # Επιλογή πιάτου εισαγωγής στην παραγγελία
        piato = self.menou.item(self.curItem_add)['values']
        # Δεν υπάρχει επαρκής ποσότητα
        if piato[3] == 0:
            messagebox.showwarning("Προειδοποίηση", "Δεν υπάρχει επαρκής ποσότητα")
        # Υπάρχει επαρκής ποσότητα εισαγωγή στον πίνακα
        else:
            piato.pop(3)
            pos = self.count_piato.get() # Ποσότητα πιάτου
            piato.append(int(pos))
            piato[1] = float(piato[1])
            self.paraggelia.insert(parent='', index='end', iid=self.count_piata, text='',
                              values=piato)
            self.count_piata += 1
            total_pos = piato[1]*piato[3] + float(self.txt_axia.get())
            self.txt_axia.delete(0, END)
            self.txt_axia.insert(0, total_pos)
            self.count_piato.delete(0, END)
            self.count_piato.insert(0, 1)

    # Συνάρτηση διαγραφής πιάτου από τον πίνακα παραγγελίας
    def delete_piato(self):
        piato = self.paraggelia.item(self.paraggelia.selection()[0])['values']
        piato[1] = float(piato[1])
        total_pos = float(self.txt_axia.get()) - piato[1] * piato[3]
        self.txt_axia.delete(0, END)
        self.txt_axia.insert(0, total_pos)
        self.count_piato.delete(0, END)
        self.count_piato.insert(0, 1)
        self.paraggelia.delete(self.paraggelia.selection()[0])
        self.count_piata -= 1

    # Συνάρτηση αναζήτησης ατόμων στο τραπέζι
    def search_atoma(self, a):
        sql_atoma = '''select atoma, date_kratisi, wra_kratisi, wra_kratisi_end, id_trapezi
                        from KRATISI
                        where date_kratisi = ''' + self.get_cal() + ''' and
        	            wra_kratisi <= ''' + self.get_time() + ''' and
        	            wra_kratisi_end >= ''' + self.get_time() + ''' and
        	            id_trapezi = ''' + self.get_id_tra() + ''';'''
        list_atoma = d.executeSQL(sql_atoma, show=False)  # [(---,), (---,)]
        self.stoixeia_kratisis = [list_atoma[0][i] for i in range(1, 5)]
        self.stoixeia_kratisis[3] = int(self.stoixeia_kratisis[3])
        self.txt_atoma.delete(0, END)
        self.txt_atoma.insert(0, list_atoma[0][0])
        for item in self.paraggelia.get_children():
            self.paraggelia.delete(item)
        self.txt_axia.delete(0, END)
        self.txt_axia.insert(0, 0)

    # Αποστολή παραγγελίας
    def send_paraggelia(self):
        # Αν δεν έχει επιλεχτεί τραπέζι
        if self.txt_id_tra.get() == '':
            messagebox.showwarning("Προειδοποίηση", "Δεν έχετε επιλέξει τραπέζι")
        # Αν δεν έχει επιλεχτεί το όνομα του εργαζομένου
        elif self.txt_er_onoma.get() == '':
            messagebox.showwarning("Προειδοποίηση", "Δεν έχετε επιλέξει όνομα σερβιτόρου")
        # Αποστολή παραγγελίας
        else:
            ## Λίστα με τα στοιχεία της παραγγελίας
            stoixeia_paraggelias = []
            stoixeia_paraggelias.extend(self.stoixeia_kratisis)
            stoixeia_paraggelias.append(self.get_amka_serv())
            stoixeia_paraggelias.append(int(self.get_id_par()))
            stoixeia_paraggelias.append(self.get_total_axia())
            stoixeia_paraggelias.append(self.get_sxolia())
            stoixeia_paraggelias.append(self.get_time()[1:-1])
            stoixeia_paraggelias.append(None)

            ## Εισαγωγή της παραγγελίας στην βάση δεδομένων
            sql_insert_paraggelia = '''insert into PARAGGELIA(date_kratisi,wra_kratisi,wra_kratisi_end,id_trapezi,id_ergazomenou,id_paraggelias,axia,sxolia,time_paraggelias,tropos_plirwmis)
                                    values(?,?,?,?,?,?,?,?,?,?) '''
            d.cursor.execute(sql_insert_paraggelia, stoixeia_paraggelias)

            ## Εισαγωγή της Paraggelias_has_piata στην βάση δεδομένων
            sql_count_par_h_piata = '''select count(*) from PARAGGELIA_HAS_PIATA;'''
            list_count_par_h_piata = d.executeSQL(sql_count_par_h_piata, show=False)  # [(---,), (---,)]
            count_par_h_piata = int(list_count_par_h_piata[0][0])
            insert_list_par_h_piata = []
            for line in self.paraggelia.get_children():
                piato = self.paraggelia.item(line)['values']  # ['ΣΑΛΑΤΑ ΤΟΥ ΚΑΙΣΑΡΑ', '9.0', 'ΟΡΕΚΤΙΚΟ', 1]
                for i in range(0, piato[3]):
                    count_par_h_piata += 1
                    par_h_piata = [count_par_h_piata] # id_paraggelia_has_piata
                    par_h_piata.append(int(self.get_id_par())) # id_paraggelias
                    id_piato = self.dict_menou[piato[0]]
                    par_h_piata.append(id_piato) # id_piato
                    par_h_piata.append(1) # time_serve
                    par_h_piata = tuple(par_h_piata)
                    insert_list_par_h_piata.append(par_h_piata)
            insert_list_par_h_piata_str = ''
            for tuple_item in insert_list_par_h_piata:
                insert_list_par_h_piata_str += str(tuple_item) + ','
            insert_list_par_h_piata_str = insert_list_par_h_piata_str[0:-1]
            sql_insert_par_has_piata = '''insert into PARAGGELIA_HAS_PIATA (id,id_paraggelias,id_piato,time_serve)
                                                values ''' + insert_list_par_h_piata_str + ''';'''
            d.insertRow(sql_insert_par_has_piata)

            ## Αφαίρεση ποσοτήτων προιοντων από την βαση
            list_piata_paraggelias = [item[2] for item in insert_list_par_h_piata] # λίστα με τα πιάτα της παραγγελίας
            dict_posotites_proiontos = {} # Λεξικό με κλειδί το id προιόντος και τιμή την παλιά ποσότητα
            # Αναζήτηση προιόντων με τις παλιές ποσότητες
            sql_posotites_proiontos = '''select id_proiontos, posotita from PROIONTA;'''
            list_posotites_proiontos = d.executeSQL(sql_posotites_proiontos, show=False)  # [(---,), (---,)]
            for item in list_posotites_proiontos:
                dict_posotites_proiontos[int(item[0])] = float(item[1])
            dict_posotites_piatou = {} # Λεξικό με κλειδί το id πιάτου και τιμή το id προιόντος και την ποσότητα που έχουν στο κάθε πιάτο
            # Αναζήτηση των ποσοτήτων των προιόντων που υπάρχουν στο κάθε πιάτο
            sql_posotites_piatou = '''select id_piato, id_proiontos, posotita from PIATO_HAS_PROIONTA;'''
            list_posotites_piatou = d.executeSQL(sql_posotites_piatou, show=False)  # [(---,), (---,)]
            for item in list_posotites_piatou:
                if int(item[0]) not in dict_posotites_piatou.keys():
                    dict_posotites_piatou[int(item[0])] = [[int(item[1]), float(item[2])]]
                else:
                    dict_posotites_piatou[int(item[0])].append([int(item[1]), float(item[2])])
            # Εισαγωγή των νέων ποσοτήτων
            for p in list_piata_paraggelias:
                list_piato_proionta = dict_posotites_piatou[p]
                for pro in list_piato_proionta:
                    dict_posotites_proiontos[pro[0]] -= pro[1]
                    if dict_posotites_proiontos[pro[0]] < 0:
                        dict_posotites_proiontos[pro[0]] = 0.0
            # Update των νέων ποσοτήτων
            for k in dict_posotites_proiontos.keys():
                sql_update_posotites = '''update PROIONTA set posotita = ''' + str(dict_posotites_proiontos[k]) + '''
                                        where id_proiontos = ''' + str(k) + ''';'''
                d.executeSQL(sql_update_posotites, show=False)

            ## Αρχικοποίηση τιμών
            #- id_paraggelias
            id_par_new = int(self.get_id_par()) + 1
            self.txt_id_par.delete(0, END)
            self.txt_id_par.insert(0, id_par_new)
            #-id trapezi
            #-servitoros
            self.txt_er_onoma.delete(0, END)
            #-atoma
            self.txt_atoma.delete(0, END)
            #-axia
            self.txt_axia.delete(0, END)
            self.txt_axia.insert(0, 0)
            #-sxolia
            self.text.delete("1.0", END)
            self.text.insert(INSERT, 'NULL')
            #-paraggelia
            for item in self.paraggelia.get_children():
                self.paraggelia.delete(item)
            #-count piata
            self.count_piata = 0
            #-menou update
            for item in self.menou.get_children():
                self.menou.delete(item)
            self.add_menou()
            # Μύνημα επιτυχούς αποστολής
            messagebox.showinfo(title='Μύνημα', message='Επιτυχής αποστολή')

    ## Συναρτήσεις get_widgets()
    # Παίρνουμε την ημερομηνία κράτησης από το self.cal
    def get_cal(self):
        datet = self.cal.get_date()
        year = "20" + datet[-2:]
        month = ''
        day = ''
        if datet[0].isdigit() and not datet[1].isdigit():
            month = '0' + datet[0]
            if datet[2].isdigit() and not datet[3].isdigit():
                day = '0' + datet[2]
            else:
                day = datet[2] + datet[3]
        elif datet[0].isdigit() and datet[1].isdigit():
            month = datet[0] + datet[1]
            if datet[3].isdigit() and not datet[4].isdigit():
                day = '0' + datet[3]
            else:
                day = datet[3] + datet[4]

        cal = "'" + year + '-' + month + '-' + day +"'"
        return cal
    # Παίρνουμε την ώρα κράτησης από το self.cal
    def get_time(self):
        time_ = "'" + self.txt_hour.get() + ":00'"
        return time_
    def get_time_end(self):
        hour = int(self.txt_hour.get()[0:2]) + 2
        if hour == 24: hour = '24'
        if hour == 25: hour = '01'
        else: hour = str(hour)
        time_ = "'" + hour + self.txt_hour.get()[2:] + ":00'"
        return time_
    # Παίρνουμε το id της παραγγελίας
    def get_id_par(self):
        return self.txt_id_par.get()
    # Παίρνουμε το id του τραπεζιού
    def get_id_tra(self):
        return self.txt_id_tra.get()
    # Παίρνουμε το id του εργαζομένου
    def get_amka_serv(self):
        return self.dict_serv[self.txt_er_onoma.get()]
    # Παίρνουμε την συνολική αξία
    def get_total_axia(self):
        return self.txt_axia.get()
    # Παίρνουμε τα σχόλια
    def get_sxolia(self):
        if self.text.get("1.0",END) == 'NULL\n':
            return None
        else:
            return self.text.get("1.0",END)

    # Όταν επιλέγεται μία γραμμή των πινάκων
    def selectItem_add(self, a):
        self.curItem_add = self.menou.focus()
    def selectItem_delete(self, a):
        pass

# Συνάρτηση που φέρνει το παράθυρο στο κέντρο της οθόνης
def center_window(w=300, h=200):
    # get screen width and height
    ws = win.winfo_screenwidth()
    hs = win.winfo_screenheight()
    # calculate position x, y
    x = (ws/2) - (w/2)
    y = (hs/2) - (h/2)
    win.geometry('%dx%d+%d+%d' % (w, h, x, y))

## Main - Κύριο πρόγραμμα
if __name__ == '__main__':
    # Σύνδεδη με την βάση δεδομένων
    dbfile = "Restaurant.db"
    d = db.DataModel(dbfile)
    # Δημιουργία του παραθύρου
    win = Tk(className="Διεπαφή παραγγελίας για εστιατόριο")
    center_window(1280, 700)
    win.configure(bg='#a86f4c')
    # Δημιουργία κλάσης
    P = Paraggelia()
    win.mainloop()
    # Κλείσιμο της βάσης δεδομένων
    d.close()
