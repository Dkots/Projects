# Εισαγωγή βιβλιοθηκών
from tkinter import *
from tkinter import messagebox
from tkinter import ttk
from tkcalendar import Calendar
import datetime as dt
import db

# Κλάση λίστα αγορών
class Shopping_List():
    # Συνάρτηση Δημιουργίας
    def __init__(self):
        ## Λίστα αγορών
        self.lbl_txt = Label(win,
                             text='Λίστα Αγορών',
                             font=("Arial Bold", 18),
                             bg='#a86f4c')
        self.text = ttk.Treeview(win,
                                  height=20)
        style = ttk.Style()
        style.configure("Treeview.Heading", font=(None, 15))
        # Δημιουργία του πίνακα των προιόντων
        self.text['columns'] = ('id_proiontos', 'όνομα', 'κατηγορία', 'ποσότητα')
        self.text.column("#0", width=0, stretch=NO)
        self.text.column("id_proiontos", anchor=W, width=250)
        self.text.column("όνομα", anchor=W, width=100)
        self.text.column("κατηγορία", anchor=CENTER, width=100)
        self.text.column("ποσότητα", anchor=W, width=100)
        self.text.heading("#0", text="", anchor=CENTER)
        self.text.heading("id_proiontos", text="Κωδικός Προιόντος", anchor=CENTER)
        self.text.heading("όνομα", text="Όνομα", anchor=CENTER)
        self.text.heading("κατηγορία", text="Κατηγορία", anchor=CENTER)
        self.text.heading("ποσότητα", text="Ποσότητα", anchor=CENTER)
        # Γέμισμα αποθήκης
        self.btn_foul_storage = Button(win,
                                  text='Γέμισμα αποθήκης',
                                  height=2,
                                  width=13,
                                  bg='grey',
                                  fg='black',
                                  command=self.foul_storage)
        # Θέση των widgets
        self.lbl_txt.place(x=0, y=0)
        self.text.place(x=0, y=40)
        self.btn_foul_storage.place(x=700, y=40)
        # Αναζήτηση για τα προιόντα που είναι σε έλλειψη
        sql_proionta = '''select id_proiontos, P.onoma, K.onoma, posotita from PROIONTA as P
                            inner join KATHGORIA_PROIONTOS as K using(id_katigorias)
                            where posotita <= 10.0;'''
        list_proionta = d.executeSQL(sql_proionta, show=FALSE)
        i=0
        # Εισαγωγή στον πίνακα
        for tuple_item in list_proionta:
            self.text.insert(parent='', index='end', iid=i, text='',
                              values=tuple_item)
            i+=1
        
    # Γέμισμα αποθήκης
    def foul_storage(self):
        sql_update = '''update PROIONTA 
                            set posotita = 20;'''
        d.executeSQL(sql_update, show=FALSE)
        messagebox.showinfo("Επιτυχία", "Η αποθήκη γέμισε!")


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
    win = Tk(className="Λίστα αγορών")
    center_window(1280, 700)
    win.configure(bg='#a86f4c')
    # Δημιουργία της κλάσης λίστα αγορών
    SL = Shopping_List()
    win.mainloop()
    # Κλείσιμο της βάσης δεδομένων
    d.close()
