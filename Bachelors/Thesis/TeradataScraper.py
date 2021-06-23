import pandas as pd
import pyodbc


class TeradataScraper:
    """
    Class for executing commands in terradata and retrieving it as dataframe
    """

    def __init__(self, cfg):
        self.conn_string = """DSN=%s; Database=default; AuthMech=3;
                UseSASL=1; UID=%s; PWD=%s; SSL=1;
                AllowSelfSignedServerCert=1""" % \
                           (cfg['DSN'], cfg['username'], cfg['password'])
        self.conn = pyodbc.connect(self.conn_string, autocommit=True)
        self.cursor = self.conn.cursor()
        self.session_opened = True

    def open_session(self):
        # Opens teradata session
        assert self.session_opened == False
        self.conn = pyodbc.connect(self.conn_string, autocommit=True)
        self.cursor = self.conn.cursor()

    def query_to_df_gen(self, query, chunksize=100000):
        # Creates a generator for terrdata query
        for chunk in pd.read_sql_query(query, self.conn, chunksize=chunksize):
            yield chunk

    def query_to_df(self, query):
        # Saves query to df
        return pd.read_sql_query(query, self.conn, chunksize=None)

    def query_to_csv(self, query, csv_name, chunksize=100000):
        # Saves query to csv
        if chunksize is None:
            df = pd.read_sql_query(query, self.conn)
            df.to_csv(csv_name, mode='w')
        else:
            for chunk in pd.read_sql_query(query, self.conn, chunksize=chunksize):
                chunk.to_csv(csv_name, mode='a', sep=',', encoding='utf-8')

    def query(self, query):
        # Execute query
        return self.cursor.execute(query)

    def query_from_file(self, file):
        f = open(file, "r")
        try:
            self.query(f.read())
            print('Success')
        except Exception as e:
            print(f'An error occured: {e}')

    def restart_session(self):
        self.dispose()
        self.open_session()

    def dispose(self):
        self.conn.close()
        self.session_opened = False

