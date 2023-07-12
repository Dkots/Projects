# Εισαγωγή βιβλιοθηκών
from tkinter import *
from tkinter import messagebox
from tkinter import ttk
from tkcalendar import Calendar
import datetime as dt
import db

# Κλάση απόδειξη
class Apodeixi():
    # Δημιουργία του περιβάλλοντος διεπαφής για την απόδειξη
    def __init__(self):
        ## Σημερινή ημερομηνία και ώρα ##
        date_now = dt.datetime.now()
        hour_now = '20:30'  #str(date_now.time())[0:5]
        cal_now = str(date_now.date())

        ## Ώρα ##
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

        ## id_trapezi// Τα τραπέζια της κράτησης
        self.lbl_id_tra = Label(win,
                            text='Τραπέζι',
                            font=("Arial Bold", 15),
                            bg='#a86f4c')
        n0 = StringVar()
        self.txt_id_tra = ttk.Combobox(win, width=25, textvariable=n0)
        # Αναζήτηση των τραπεζιών που δεν πάρει απόδειξη
        self.search_trapezia()

        ## Απόδειξη
        self.lbl_txt = Label(win,
                              text='Απόδειξη',
                              font=("Arial Bold", 15),
                              bg='#a86f4c')
        self.text = Text(win,
                         height=30,
                         width=50)

        ## Τρόπος πληρωμής
        self.lbl_tr_plir = Label(win,
                                  text='Τρόπος πληρωμής',
                                  font=("Arial Bold", 15),
                                  bg='#a86f4c')
        n2 = StringVar()
        self.txt_tr_plir = ttk.Combobox(win,
                                        width=25,
                                        textvariable=n2)
        self.txt_tr_plir['values'] = ['ΚΑΡΤΑ', 'ΜΕΤΡΗΤΑ']  # [---, ---]

        # Κουμπί δημιουργίας απόδειξης
        self.btn_create_apodeixi = Button(win,
                                  text='Δημιουργία',
                                  height=2,
                                  width=13,
                                  bg='grey',
                                  fg='black',
                                  command=self.create_apodeixi)

        # Κουμπί εκτύπωσης
        self.btn_print_apodeixi = Button(win,
                                    text='Εκτύπωση',
                                    height=2,
                                    width=13,
                                    bg='grey',
                                    fg='black',
                                    command=self.print_apodeixi)

        # Εμφάνιση Widgets
        self.place_widgets()

    # Συνάρτηση εμφάνιση Widgets
    def place_widgets(self):
        self.lbl_hour.place(x=0, y=0)
        self.txt_hour.place(x=0, y=30)
        self.lbl_id_tra.place(x=0, y=60)
        self.txt_id_tra.place(x=0, y=90)
        self.lbl_tr_plir.place(x=0, y=180)
        self.txt_tr_plir.place(x=0, y=210)
        self.lbl_cal.place(x=200, y=0)
        self.cal.place(x=200, y=30)
        self.lbl_txt.place(x=500, y=0)
        self.text.place(x=500, y=30)
        self.btn_create_apodeixi.place(x=0, y=250)
        self.btn_print_apodeixi.place(x=800, y=550)

    # Συνάρτηση εύρεσης τραπεζιών χωρίς απόδειξη
    def search_trapezia(self):
        sql_tr = '''select distinct K.id_trapezi, K.date_kratisi, K.wra_kratisi from KRATISI as K
                            inner join PARAGGELIA as P using(date_kratisi, wra_kratisi, wra_kratisi_end, id_trapezi)
                            where K.akirwsi = 0 and 
        	                K.date_kratisi = ''' + self.get_cal() + ''' and
        	                K.wra_kratisi <= ''' + self.get_time() + ''' and
        	                K.wra_kratisi_end >= ''' + self.get_time() + ''' and
        	                P.tropos_plirwmis is NULL;'''
        list_tr = d.executeSQL(sql_tr, show=False)  # [(---,), (---,)]
        self.txt_id_tra['values'] = [item[0] for item in list_tr]  # [---, ---]
        self.dict_tr = {}
        for item in list_tr:
            self.dict_tr[item[0]] = [item[1], item[2]]

    # Συνάρτηση δημιουργίας απόδειξης
    def create_apodeixi(self):
        # Αν δεν έχει επιλεχτεί τραπέζι
        if self.txt_id_tra.get() == '':
            messagebox.showwarning("Προειδοποίηση", "Δεν έχετε επιλέξει τραπέζι")
        # Αν δεν έχει επιλεχτεί τρόπος πληρωμής
        elif self.get_tr_plirwmis == '':
            messagebox.showwarning("Προειδοποίηση", "Δεν έχετε επιλέξει τρόπος πληρωμής")
        # Δημιουργία της απόδειξης
        else:
            # Αναζήτηση για τα πιάτα της παραγγελίας
            sql_total_piata = '''select p.onoma, count(*) as total_pos, p.timh*count(*) as axia
                                from PARAGGELIA as par
                                inner join PARAGGELIA_HAS_PIATA as php using(id_paraggelias)
                                inner join PIATO as p using(id_piato)
                                inner join KATHGORIA_PIATOY as kp using(id_katigorias)
                                where par.id_paraggelias in (select id_paraggelias
							                            from PARAGGELIA
							                            where date_kratisi = ''' + self.get_cal() + ''' and
								                        wra_kratisi <= ''' + self.get_time() + ''' and
								                        wra_kratisi_end >= ''' + self.get_time() + ''' and
								                        id_trapezi = ''' + self.get_id_tra() + ''' and
								                        tropos_plirwmis is NULL)
                                group by p.onoma
                                order by kp.id_katigorias;'''
            self.text.insert(INSERT, 'ΗΜΕΡΟΜΗΝΙΑ ΕΚΠΤΥΠΩΣΗΣ: ' + self.get_cal()+ '\n')
            self.text.insert(INSERT, 'ΩΡΑ ΕΚΠΤΥΠΩΣΗΣ: ' + self.get_time()+ '\n\n')
            list_total_piata = d.executeSQL(sql_total_piata, show=FALSE)
            total_axia = 0
            for tuple_item in list_total_piata:
                self.text.insert(INSERT, tuple_item[0] + '\tx' + tuple_item[1] + '\t-> ' + tuple_item[2] + '\n')
                total_axia += float(tuple_item[2])
            self.text.insert(INSERT, '\nΣΥΝΟΛΙΚΗ ΑΞΙΑ: ' + str(total_axia))

    # Συνάρτηση εκτύπωσης της απόδειξης
    def print_apodeixi(self):
        # Αν δεν έχει επιλεχτεί τραπέζι
        if self.txt_id_tra.get() == '':
            messagebox.showwarning("Προειδοποίηση", "Δεν έχετε επιλέξει τραπέζι")
        # Αν δεν έχει επιλεχτεί τρόπος πληρωμής
        elif self.get_tr_plirwmis == '':
            messagebox.showwarning("Προειδοποίηση", "Δεν έχετε επιλέξει τρόπος πληρωμής")
        else:
            messagebox.showinfo("Επιτυχία", "Το έγγραφο εκτυπώθηκε!")
            # Upadate της παραγγελίας στον τρόπο πληρωμής
            sql_update = '''update PARAGGELIA 
                            set tropos_plirwmis = '''+ "'"+self.get_tr_plirwmis()+"'" + '''
                            where date_kratisi = ''' + "'"+self.dict_tr[self.get_id_tra()][0]+"'" + ''' and
                                    wra_kratisi = ''' + "'"+self.dict_tr[self.get_id_tra()][1]+"'" + ''' and
                                    id_trapezi = ''' + self.get_id_tra() + ''';'''
            d.executeSQL(sql_update, show=FALSE)

        ## Αρχικοποίηση widgets
        # Τραπέζι
        self.txt_id_tra.delete(0, END)
        self.search_trapezia()
        # Τρόπος πληρωμής
        self.txt_tr_plir.delete(0, END)
        # Απόδειξη
        self.text.delete('1.0', END)


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
    # Παίρνουμε το id του τραπεζιού
    def get_id_tra(self):
        return self.txt_id_tra.get()
    # Παίρνουμε τον τρόπο πληρωμή
    def get_tr_plirwmis(self):
        return self.txt_tr_plir.get()

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
    win = Tk(className="Διεπαφή απόδειξη")
    center_window(1280, 700)
    win.configure(bg='#a86f4c')
    # Δημιουργία της κλάσης απόδειξης
    A = Apodeixi()
    win.mainloop()
    # Κλείσιμο της βάσης δεδομένων
    d.close()
