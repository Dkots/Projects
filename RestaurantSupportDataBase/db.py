import sqlite3

class DataModel():
    def __init__(self, filename):
        self.filename = filename
        try:
            self.con = sqlite3.connect(filename)
            self.con.row_factory = sqlite3.Row  # ώστε να πάρουμε τα ονόματα των στηλών του πίνακα
            self.cursor = self.con.cursor()
            print("Επιτυχής σύνδεση στη βάση δεδομένων", filename)
            sqlite_select_Query = "select sqlite_version();"
            self.cursor.execute(sqlite_select_Query)
            record = self.cursor.fetchall()
            for rec in record:
                print("SQLite Database Version is: ", rec[0])
        except sqlite3.Error as error:
            print("Σφάλμα σύνδεσης στη βάση δεδομένων sqlite", error)
    
    def close(self):
        self.con.commit()
        self.con.close()

    def insertRow(self, query):
        try:
            for statement in query.split(";"):
                self.cursor.execute(statement)
            self.con.commit()
            return True
        except sqlite3.Error as error:
            print(f"Σφάλμα εκτέλεσης εντολής εισαγωγής SQL", error)
            return False
    
    def executeSQL(self, query, show=False):
        try:
            for statement in query.split(";"):
                if statement.strip():
                    self.cursor.execute(statement)
            if show:
                for row in self.cursor.fetchall():
                    print(", ".join([str(item)for item in row]))
            self.list_data = []
            for row in self.cursor.fetchall():
                row_data = tuple([str(item)for item in row])
                self.list_data.append(row_data)
            self.con.commit()
            return self.list_data
        except sqlite3.Error as error:
            print(f"Σφάλμα εκτέλεσης εντολής SQL", error)
            return False

    def updateSQL(self, query):
        try:
            for statement in query.split(";"):
                self.cursor.execute(statement)
            self.con.commit()
            return True
        except sqlite3.Error as error:
            print(f"Σφάλμα εκτέλεσης εντολής αναβάθμισης SQL", error)
            return False

    def readTable(self, table):
        '''Φόρτωμα ενός πίνακα, όταν το προαιρετικό όρισμα machine πάρει τιμή, τότε επιστρέφει μόνο 
        τις εγγραφές που αφορούν τη συγκεκριμένη μηχανή'''
        try:
            query = f'''SELECT * FROM {table};'''
            self.cursor.execute(query)
            records = self.cursor.fetchall()
            result = []
            for row in records:
                result.append(dict(row))
            return result
        except sqlite3.Error as error:
            print(f"Σφάλμα φόρτωσης πίνακα {table}", error)
    
    def _insertIntoTable(self, table, row_dict):
        ''' Εισαγωγή εγγραφής σε μορφή λεξικού σε πίνακα'''
        try:
            query_param = f"""INSERT INTO {table} ({",".join(row_dict.keys())}) VALUES ({", ".join((len(row_dict)-1) * ["?"])}, ?);"""
            data = tuple(row_dict.values())
            self.cursor.execute(query_param, data)
            self.con.commit()
            return True
        except sqlite3.Error as error:
            print(f"Σφάλμα εισαγωγής στοιχείων στον πίνακα {table}", error)
            return False