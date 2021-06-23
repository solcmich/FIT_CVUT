### Please note that data are not available as it was data about real KB clients. ####

To get a specific report in html, go to experiments/{desired_section}/{A/B}_report.html
The reports are also available withing each .ipynb

In the experiments folder, there are results obtained by 10 fold stratified cross validation with 
undersampling in csv format with generated reports for each dataset separately.

In the results folder are results obtained by 5 fold stratified cross validation with undersampling
in csv format done for hyperparameters tuning.

The html folder contains .ipynb notebooks exported in html without reports.

The text folder contains pdf version of the thesis.

utils.py contains implementation of functions used in .ipynb notebooks. 

TeradataScraper.py is a python class which I used to obtain the data from wihin the warehouse.

The .ipynb files are orignal files, where experimeting was done. 
