# Εισαγωγή βιβλιοθηκών
from tkinter import *
from tkinter import messagebox
from tkinter import ttk
from tkcalendar import Calendar
import datetime as dt
import db

# Κλάση κουμποιού επιλογής κράτηση/ακύρωση
class Buttons_kratisi_akyrwsi():
    # Βοηθητικές μεταβλητές
    button_push_kratisi = 0 # Αν πατήθηκε το κουμπί κράτηση
    button_push_akirwsi = 0 # Αν πατήθηκε το κουμπί ακύρωση

    # Δημιουργία των κουμπιών κράτησης και ακύρωσης
    def __init__(self):
        # Κουμπί κράτησης
        self.btn_kratisi = Button(win,
                                  text='ΚΡΑΤΗΣΗ',
                                  height=5,
                                  width=20,
                                  bg='grey',
                                  fg='black',
                                  command=self.show_kratisi)
        # Κουμπί ακύρωσης
        self.btn_akyrwsi = Button(win,
                                  text='ΑΚΥΡΩΣΗ',
                                  height=5,
                                  width=20,
                                  bg='grey',
                                  fg='black',
                                  command=self.show_akyrvsi)
        # Εμφάνιση κουμπιών
        self.btn_kratisi.place(x=0, y=300)
        self.btn_akyrwsi.place(x=0, y=400)

    # Εμφάνιση διεπαφής για την κράτηση
    def show_kratisi(self):
        self.button_push_kratisi += 1
        self.button_push_akirwsi = 0
        A.destroy_Akyrwsi()
        if self.button_push_kratisi == 1:
            K.show_Kratisi()

    # Εμφάνιση διεπαφής για την ακύρωση
    def show_akyrvsi(self):
        self.button_push_kratisi = 0
        self.button_push_akirwsi += 1
        K.destroy_Kratisi()
        if self.button_push_akirwsi == 1:
            A.show_Akyrwsi()

# Κλάση κράτηση
class Kratisi():
    # Εμφάνιση διεπαφής για την κράτηση
    def show_Kratisi(self):
        ## Τίτλος πελάτης
        self.lbl_pelatis = Label(win,
                                 text='Πελάτης',
                                 font=("Arial Bold", 18),
                                 bg='#a86f4c')
        # Επιλογή γνωτού/αγνώστου πελάτη
        self.Checkbutton1 = IntVar()
        self.Checkbutton1.set(0)
        self.known_pelatis = Checkbutton(win,
                                         text="Γνωστός",
                                         variable=self.Checkbutton1,
                                         onvalue=1,
                                         offvalue=0,
                                         height=2,
                                         width=10,
                                         bg='#a86f4c',
                                         command=self.checkbutton1_choice)

        ## Στοιχεία Πελάτη ##
        # Όνομα Πελάτη
        self.lbl_name = Label(win,
                              text='Ονοματεπώνυμο',
                              font=("Arial Bold", 15),
                              bg='#a86f4c')
        n0 = StringVar()
        self.pelatischoosen = ttk.Combobox(win,
                                           width=25,
                                           textvariable=n0)
        # Τηλέφωνο Πελάτη
        self.lbl_thle = Label(win,
                              text='Τηλέφωνο',
                              font=("Arial Bold", 15),
                              bg='#a86f4c')
        self.txt_thle = Entry(win,
                              width=28)
        # id_pelati
        self.lbl_id = Label(win,
                              text='id_pelati',
                              font=("Arial Bold", 15),
                              bg='#a86f4c')
        self.txt_id = Entry(win,
                              width=28)

        ## Τοποθεσία Τραπέζιών ##
        self.lbl_topoth = Label(win,
                                text='Τοποθεσία Τραπεζιού',
                                font=("Arial Bold", 15),
                                bg='#a86f4c')
        n1 = StringVar()
        self.topochoosen = ttk.Combobox(win, width=25, textvariable=n1)

        # Αναζήτηση στην βάση για την τοποθεσία των τραπεζιών
        sql_topothesia = '''select onoma from TOPOTHESIA;'''
        list_onoma = d.executeSQL(sql_topothesia, show=False)  # [(---,), (---,)]
        # Εισαγωγή στο Combobox self.topochoosen
        self.topochoosen['values'] = [item[0] for item in list_onoma]  # [---, ---]

        ## Άτομα για την κράτηση ##
        self.lbl_xwr = Label(win,
                             text='Άτομα',
                             font=("Arial Bold", 15),
                             bg='#a86f4c')
        n2 = StringVar()
        self.xwritchoosen = ttk.Combobox(win, width=25, textvariable=n2)
        self.xwritchoosen['values'] = [i for i in range(1, 9)]

        ## Σημερινή ημερομηνία και ώρα ##
        date_now = dt.datetime.now()
        hour_now = str(date_now.time())[0:5]
        cal_now = str(date_now.date())

        ## Ώρα κράτησης ##
        self.lbl_hour = Label(win,
                              text='Ωρα',
                              font=("Arial Bold", 15),
                              bg='#a86f4c')
        n3 = StringVar()
        self.txt_hour = Entry(win,
                            width=28)
        self.txt_hour.insert(0, hour_now)

        ## Επιλογή ημέρας κράτησης ##
        self.lbl_cal = Label(win,
                             text='Ημερομηνία Κρατήσης',
                             font=("Arial Bold", 15),
                             bg='#a86f4c')
        self.cal = Calendar(win, selectmode='day',
                            year=int(cal_now[0:4]),
                            month=int(cal_now[5:7]),
                            day=int(cal_now[8:10]))
        ## Κουμπί αναζήτηση τραπεζιών
        self.btn_search = Button(win,
                                 text='Αναζήτηση',
                                 height=2,
                                 width=13,
                                 bg='grey',
                                 fg='black',
                                 command=self.search_trapezi)

        # Λίστα για τα έλευθερα τραπέζια
        self.listBox_trapezia = Listbox(win,
                                        height=15,
                                        width=80,
                                        font=15)

        ## Κουμπί επιβεβαίωση κράτησης
        self.btn_kratisi = Button(win,
                                  text='Κράτηση',
                                  height=2,
                                  width=13,
                                  bg='grey',
                                  fg='black',
                                  command=self.do_kratisi)

        ## Θέση widgets
        self.place_widgets()

    # Συνάρτηση Τοποθέτησης widgets
    def place_widgets(self):
        self.lbl_pelatis.place(x=200, y=0)
        self.known_pelatis.place(x=300, y=0)
        self.lbl_name.place(x=200, y=30)
        self.pelatischoosen.place(x=200, y=60)
        self.lbl_thle.place(x=200, y=90)
        self.txt_thle.place(x=200, y=120)
        self.lbl_id.place(x=200, y=150)
        self.txt_id.place(x=200, y=180)
        self.lbl_topoth.place(x=400, y=30)
        self.topochoosen.place(x=400, y=60)
        self.lbl_xwr.place(x=400, y=90)
        self.xwritchoosen.place(x=400, y=120)
        self.lbl_hour.place(x=400, y=150)
        self.txt_hour.place(x=400, y=180)
        self.lbl_cal.place(x=700, y=0)
        self.cal.place(x=700, y=30)
        self.btn_search.place(x=1000, y=60)
        self.listBox_trapezia.place(x=200, y=225)
        self.btn_kratisi.place(x=1000, y=625)

    # Διαγραφή των widgets της κράτησης
    def destroy_Kratisi(self):
        try:
            self.lbl_pelatis.destroy()
            self.known_pelatis.destroy()
            self.lbl_name.destroy()
            self.pelatischoosen.destroy()
            self.lbl_thle.destroy()
            self.txt_thle.destroy()
            self.lbl_id.destroy()
            self.txt_id.destroy()
            self.lbl_topoth.destroy()
            self.topochoosen.destroy()
            self.lbl_xwr.destroy()
            self.xwritchoosen.destroy()
            self.lbl_hour.destroy()
            self.txt_hour.destroy()
            self.lbl_cal.destroy()
            self.cal.destroy()
            self.btn_search.destroy()
            self.listBox_trapezia.destroy()
            self.btn_kratisi.destroy()
        except AttributeError:
            pass

    # Διεπαγή Γνωστού Πελάτη
    def checkbutton1_choice(self):
        self.dict_pelatis = {} # Λεξικό με κλειδί το ονοματεπώνυμο του πελάτη και τιμές το id και το τηλεφωνό του
        # Αν η επιλογή self.Checkbutton1 γίνει ένα έχουμε γνωστό πελάτη
        if self.Checkbutton1.get() == 1:
            # Αναζήτηση στην βάση τους γνωστούς πελάτες
            sql_pelatis = '''select id_pelath, firstname, lastname, thlefwno from PELATHS order by lastname;'''
            list_pelatis = d.executeSQL(sql_pelatis, show=False)  # [(---,), (---,)]
            # Εισαγωγή των δεδομένων στο Combobox self.pelatischoosen και στο λεξικό self.dict_pelatis
            self.pelatischoosen['values'] = [item[1]+' '+item[2] for item in list_pelatis]  # [---, ---]
            for item in list_pelatis:
                self.dict_pelatis[item[1]+' '+item[2]] = [item[0], item[3]]
        # Αν η επιλογή self.Checkbutton1 γίνει μηδέν έχουμε άγνωστο πελάτη
        elif self.Checkbutton1.get() == 0:
            # Δημιουργία ξανά του widget self.pelatischoosen
            self.pelatischoosen.destroy()
            n0 = StringVar()
            self.pelatischoosen = ttk.Combobox(win, width=25, textvariable=n0)
            self.pelatischoosen.place(x=200, y=60)

    # Αναζήτηση τραπεζιών
    def search_trapezi(self):
        # Παίρνουμε τα στοιχεία του πελάτη και τα στοιχεία του επιθυμητού τραπεζιού
        name_pelati = self.get_name()
        thle_pelati = self.get_thle()
        topo_trapezi = self.get_topoth()
        xwrit_trapezi = self.get_xwrit()
        time_krat = self.txt_hour.get()
        # Έλεγχος όνομα πελάτη
        if name_pelati=='' or len(name_pelati.split()) > 2 or (len(name_pelati.split()) == 1 and name_pelati != 'NULL'):
            messagebox.showwarning("Προειδοποίηση", "Δεν έχετε σε σωστή μορφή το όνομα του πελάτη")
        # Έλεγχος τηλέφωνο πελάτη
        elif (thle_pelati=='' or len(thle_pelati)!=10 or not thle_pelati.isdigit()) and self.Checkbutton1.get() == 0:
            messagebox.showwarning("Προειδοποίηση", "Δεν έχετε σε σωστή μορφή το τηλέφωνο του πελάτη")
        # Έλεγχος τοποθεσία τραπεζιού
        elif topo_trapezi=='':
            messagebox.showwarning("Προειδοποίηση", "Δεν έχετε τοποθετήσει χώρο τραπεζιού")
        # Έλεγχος χωριτικότητα τραπεζιού
        elif xwrit_trapezi=='':
            messagebox.showwarning("Προειδοποίηση", "Δεν έχετε τοποθετήσει άτομα")
        # Έλεγχος ώρας
        elif time_krat == '' or not time_krat[0:2].isdigit() or not time_krat[3:5].isdigit() or not time_krat[2] == ':':
            messagebox.showwarning("Προειδοποίηση", "Δεν έχετε τοποθετήσει σωστά την ώρα")
        # Αναζήτηση ελεύθερων τραπεζιών
        else:
            # Γνωστός πελάτης εισάγουμε στο id και τηλέφωνο τα στοιχεία του
            if self.Checkbutton1.get() == 1:
                thle_pelati = self.dict_pelatis[name_pelati][1]
                self.txt_thle.insert(0, thle_pelati)
                id_pelati = self.dict_pelatis[name_pelati][0]
                self.txt_id.insert(0, id_pelati)
            # Άγνωστος πελάτης αναζήτηση και καινούργιου id πελάτη
            elif self.Checkbutton1.get() == 0:
                sql_count_pelatis = '''select count(*) from PELATHS;'''
                self.count_pelatis = d.executeSQL(sql_count_pelatis, show=False)  # [(---,), (---,)]
                id_pelati = str(int(self.count_pelatis[0][0])+1)
                self.txt_id.insert(0, id_pelati)

            # Αναζήτηση στην βάση τα ελεύθερα τραπέζια
            sql_trapezi = '''select id_trapezi, onoma, xwritikothta from TRAPEZI 
                            inner join TOPOTHESIA using(id_topothesia)
                            where id_trapezi not in (select id_trapezi from KRATISI 
							                        where akirwsi = 0 and 
							                        date_kratisi = ''' + self.get_cal() + ''' and 
							                        wra_kratisi < ''' + self.get_time_end() + ''' and
                                                    ''' + self.get_time() + ''' < wra_kratisi_end) and 
	                        onoma = ''' + "'" + self.get_topoth() + "'" + ''' and
	                        xwritikothta >= ''' + self.get_xwrit() + ''';'''
            self.list_free_trapezia = d.executeSQL(sql_trapezi, show=False)  # [(---,), (---,)]
            # Εισαγωγή στο listbox self.listBox_trapezia
            self.listBox_trapezia.delete(0, END)
            n = 0
            self.dict_trapezi = {} # Λεξικό με κλειδί την σειρά του listbox και τιμή το id τραπεζιού
            for item in self.list_free_trapezia:
                item_listBox = 'ΤΡΑΠΕΖΙ: ' + str(item[0]).ljust(5) + 'ΑΤΟΜΑ:'.ljust(7) + str(item[2]).ljust(4)
                self.listBox_trapezia.insert(END, item_listBox)
                self.dict_trapezi[n] = str(item[0])
                n += 1

    # Συνάρτηση για την κράτηση του τραπεζιού
    def do_kratisi(self):
        ## Διμιουργίαλίστας για την συλλογή των στοιχείων του τραπεζιού
        stoixeia_trapezi = []
        stoixeia_trapezi.append(self.get_cal()[1:-1]) # date_kratisi
        stoixeia_trapezi.append(self.get_time()[1:-1]) # wra_kratisi
        stoixeia_trapezi.append(self.get_time_end()[1:-1]) # wra_kratisi_end
        stoixeia_trapezi.append(int(self.get_id()))# id_pelati
        stoixeia_trapezi.append(int(self.get_id_trapezi()))  # id_trapezi
        stoixeia_trapezi.append(int(self.get_xwrit()))  # atoma
        stoixeia_trapezi.append(0)  # akyrwsi

        ## Εισαγωγή στην βάση
        # Γνωστός πελάτης
        if self.Checkbutton1.get() == 1:
            # Εισαγωγή τραπεζιού
            stoixeia_trapezi_str = str(tuple(stoixeia_trapezi))+';' # (--,--,--);
            sql_insert_kratisi = '''insert into KRATISI (date_kratisi,wra_kratisi,wra_kratisi_end,id_pelath,id_trapezi,atoma,akirwsi)
                                    values ''' + stoixeia_trapezi_str
            d.insertRow(sql_insert_kratisi)
        # Άγνωστος πελάτης
        elif self.Checkbutton1.get() == 0:
            # Εισαγωγή πελάτη
            stoixeia_pelati = []
            stoixeia_pelati.append(int(self.get_id())) # id_palath
            name = self.get_name().split()
            stoixeia_pelati.append(name[0]) # firstname
            stoixeia_pelati.append(name[1]) # lastname
            stoixeia_pelati.append(self.get_thle()) # thlefwno
            stoixeia_pelati_str = str(tuple(stoixeia_pelati)) + ';'  # (--,--,--);
            sql_pelatis = '''insert into PELATHS (id_pelath,firstname,lastname,thlefwno)
                            values ''' + stoixeia_pelati_str
            d.insertRow(sql_pelatis)

            # Εισαγωγή τραπεζιού
            stoixeia_trapezi_str = str(tuple(stoixeia_trapezi)) + ';'  # (--,--,--);
            sql_insert_kratisi = '''insert into KRATISI (date_kratisi,wra_kratisi,wra_kratisi_end,id_pelath,id_trapezi,atoma,akirwsi)
                                    values ''' + stoixeia_trapezi_str
            d.insertRow(sql_insert_kratisi)
        # Μύνημα επιτυχούς κράτησης
        messagebox.showinfo(title='Εισαγωγή', message='Επιτυχής κράτηση')

        ## Αρχικοποίηση
        # Γνωστός/Αγνωστος πελάτης
        self.Checkbutton1.set(0)
        # Όνομα Πελάτη
        self.pelatischoosen.destroy()
        n0 = StringVar()
        self.pelatischoosen = ttk.Combobox(win, width=25, textvariable=n0)
        self.pelatischoosen.place(x=200, y=60)
        # Τηλέφωνο
        self.txt_thle.delete(0, END)
        # id_pelati
        self.txt_id.delete(0, END)
        # Τοποθεσία
        self.topochoosen.delete(0, END)
        # Άτομα
        self.xwritchoosen.delete(0, END)


    ## Συναρτήσεις get_widgets()
    # Παίρνουμε το όνομα του πελάτη από το self.txt_name
    def get_name(self):
        return self.pelatischoosen.get().upper()
    # Παίρνουμε το τηλέφωνο του πελάτη από το self.txt_thle
    def get_thle(self):
        return self.txt_thle.get()
    # Παίρνουμε το id_pelath από το self.txt_id
    def get_id(self):
        return self.txt_id.get()
    # Παίρνουμε την τοποθεσία του τραπεζιού από το self.topochoosen
    def get_topoth(self):
        return self.topochoosen.get()
    # Παίρνουμε την χωριτικότητα του τραπεζιού από το self.xwritchoosen
    def get_xwrit(self):
        return self.xwritchoosen.get()
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
    # Παίρνουμε το id_trapezi από το self.listBox_trapezia
    def get_id_trapezi(self):
        return self.dict_trapezi[self.listBox_trapezia.curselection()[0]]

# Κλάση ακύρωση
class Akyrwsi():
    # Εμφάνιση διεπαφής για την ακύρωση
    def show_Akyrwsi(self):
        ## Τίτλος ακύρωση
        self.lbl_akyrvsi = Label(win,
                                 text='Ακύρωση',
                                 font=("Arial Bold", 18),
                                 bg='#a86f4c')

        ## Σημερινή ημερομηνία και ώρα ##
        date_now = dt.datetime.now()
        hour_now = str(date_now.time())[0:5]
        cal_now = str(date_now.date())

        ## Ώρα κράτησης ##
        self.lbl_hour = Label(win,
                              text='Ωρα',
                              font=("Arial Bold", 15),
                              bg='#a86f4c')
        n3 = StringVar()
        self.txt_hour = Entry(win,
                              width=25)
        self.txt_hour.insert(0, hour_now)

        ## Επιλογή ημέρας κράτησης ##
        self.lbl_cal = Label(win,
                             text='Ημερομηνία Κρατήσης',
                             font=("Arial Bold", 15),
                             bg='#a86f4c')
        self.cal = Calendar(win, selectmode='day',
                            year=int(cal_now[0:4]), month=int(cal_now[5:7]),
                            day=int(cal_now[8:10]))

        ## Κουμπί αναζήτησης
        self.btn_search = Button(win,
                                 text='Αναζήτηση',
                                 height=2,
                                 width=13,
                                 bg='grey',
                                 fg='black',
                                 command=self.akyrwsi_trapezi)

        # Λίστα για τα κρατημένα τραπέζια
        self.listBox_trapezia = Listbox(win,
                                        height=20,
                                        width=100,
                                        font=15)

        ## Κουμπί ακύρωσης κράτησης
        self.btn_akyrvsi = Button(win,
                                  text='Ακύρωση',
                                  height=2,
                                  width=13,
                                  bg='grey',
                                  fg='black',
                                  command=self.do_akyrvsi)

        ## Θέση widgets
        self.place_widgets()

    # Συνάρτηση Τοποθέτησης widgets
    def place_widgets(self):
        self.lbl_akyrvsi.place(x=200, y=0)
        self.lbl_hour.place(x=200, y=30)
        self.txt_hour.place(x=200, y=60)
        self.lbl_cal.place(x=400, y=0)
        self.cal.place(x=400, y=30)
        self.btn_search.place(x=1000, y=60)
        self.listBox_trapezia.place(x=200, y=225)
        self.btn_akyrvsi.place(x=1000, y=625)

    # Διαγραφή των widgets της ακύρωσης
    def destroy_Akyrwsi(self):
        try:
            self.lbl_akyrvsi.destroy()
            self.lbl_hour.destroy()
            self.txt_hour.destroy()
            self.lbl_cal.destroy()
            self.cal.destroy()
            self.btn_search.destroy()
            self.listBox_trapezia.destroy()
            self.btn_akyrvsi.destroy()
        except AttributeError:
            pass

    # Συνάρτηση εύρεσης των κρατημένων τραπεζιών
    def akyrwsi_trapezi(self):
        time_krat = self.txt_hour.get()
        # Έλεγχος ώρας
        if time_krat == '' or not time_krat[0:2].isdigit() or not time_krat[3:5].isdigit() or not time_krat[2] == ':':
            messagebox.showwarning("Προειδοποίηση", "Δεν έχετε τοποθετήσει σωστά την ώρα")
        # Αναζήτηση στην βάση για τα κρατημένα τραπέζια
        else:
            sql_trapezi = '''select id_pelath, firstname, lastname, id_trapezi, atoma from PELATHS
                                    inner join KRATISI using(id_pelath)
                                    inner join TRAPEZI using(id_trapezi)
                            where akirwsi = 0 and 
                                date_kratisi = ''' + self.get_cal() + '''  and 
                                wra_kratisi >= '''+ self.get_time() + ''';'''
            self.list_trapezia = d.executeSQL(sql_trapezi, show=False)  # [(---,), (---,)]
            # Εισαγωγή των δεδομένων στο self.listBox_trapezia
            self.listBox_trapezia.delete(0, END)
            n = 0
            self.dict_trapezi = {} # Λεξικό με κλειδί την σειρά στο listbox και τα στοιχεία της κράτησης
            for item in self.list_trapezia:
                item_listBox = item[1] + ' ' + item[2] + ' ΤΡΑΠΕΖΙ: '.rjust(25) + item[3] + ' ΑΤΟΜΑ: ' + item[4]
                self.listBox_trapezia.insert(END, item_listBox)
                self.dict_trapezi[n] = [item[0], item[1], item[2], item[3], item[4]]
                n += 1

    # Συνάρτηση που γίνεται η ακύρωση
    def do_akyrvsi(self):
        # Τα στοιχεία της κράτησης για την ακύρωση του τραπεζιού
        n = self.listBox_trapezia.curselection()
        li = self.dict_trapezi[n[0]]
        # update στον πίνακα KRATISI για την ακύρωση του τραπεζιού
        sql_akirwsi = '''update KRATISI
                        set akirwsi = 1
                        where date_kratisi = ''' + str(self.get_cal()) + ''' and 
                        wra_kratisi >= ''' + str(self.get_time()) + ''' and 
                        id_pelath = ''' + str(li[0]) + ''' and 
                        id_trapezi = ''' + str(li[3]) + ''';'''
        d.updateSQL(sql_akirwsi)
        # Μύνημα επιτυχούς ακύρωσης
        messagebox.showinfo(title='Αναβαθμισή', message='Επιτυχής ακύρωση')
        # Διαγραφή του πελάτη από την λίστα
        self.listBox_trapezia.delete(ANCHOR)

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

        cal = "'" + year + '-' + month + '-' + day + "'"
        return cal
    # Παίρνουμε την ώρα κράτησης από το self.cal
    def get_time(self):
        time_ = "'" + self.txt_hour.get() + ":00'"
        return time_

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
    win = Tk(className="Διεπαφή κράτησης για εστιατόριο")
    center_window(1280, 700)
    win.configure(bg='#a86f4c')
    # Δημιουργία κλάσεων
    B = Buttons_kratisi_akyrwsi()
    K = Kratisi()
    A = Akyrwsi()
    win.mainloop()
    # Κλείσιμο της βάσης δεδομένων
    d.close()
