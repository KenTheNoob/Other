#webscraping test
from bs4 import BeautifulSoup
import requests
#from pymongo import MongoClient

url = 'https://finance.yahoo.com/most-active'
page = requests.get(url)
soup = BeautifulSoup(page.text, 'html.parser')
table = soup.find('table')
titles = table.find_all('th')
title_list = [title.text.strip() for title in titles][0:4] + [[title.text.strip() for title in titles][5]]
print(title_list)
columns = table.find_all('tr')[1:]
for row in columns:
    row_data = row.find_all('td')
    row_strip = [data.text.strip() for data in row_data][0:4] + [[data.text.strip() for data in row_data][5]]
    print(row_strip)