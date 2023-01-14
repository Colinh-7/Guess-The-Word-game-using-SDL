import csv
import unidecode
from googletrans import Translator

x = Translator()
language = []
trans = []
csvtext = []
rownb = 0
nbcolumns = 0

file = open('data/loading-files/data.csv', 'r')
obj = csv.reader(file, delimiter=';')

# Read CSV line by line
for row in obj:

    # Stock all languege available
    if (rownb == 0):
        for i in range(len(row)):
            if (i > 1):
                language.append(x.translate(row[i], src='fr', dest='english').text)
            nbcolumns += 1
    # Translate all words
    else:
        if (len(row) < nbcolumns):
            i = len(row)-2 # Column number where we need to start the translation
            j = 0
            for lang in language:
                # Start the translation a column i
                if (j >= i):
                    trans.append(unidecode.unidecode(x.translate(row[1], src='fr', dest = lang).text))
                j += 1
            print(trans)
            for word in trans:
                # Add to row all translations
                row.append(word)
            trans = []
    rownb+=1
    # Stock the row
    csvtext.append(row)

print(language)

file2 = open('data/loading-files/data.csv', 'w')
writer = csv.writer(file2, delimiter=';')
# Write all rows in file
for row in csvtext:
    writer.writerow(row)

# Close file
file.close()
file2.close()
