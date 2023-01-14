import csv
import unidecode
from googletrans import Translator

trans = Translator()
word = []
file = open('temp.txt', 'r')

obj = csv.reader(file, delimiter = ' ')
for row in obj:
    # Get destination and translate it in english
    lang = trans.translate(row[1], src = 'fr', dest = 'en').text
    # Translate the word
    word.append(unidecode.unidecode(trans.translate(row[0], src = 'fr', dest = lang).text))

# Write the word in temp.txt
file2 = open('temp.txt', 'w')
write = csv.writer(file2)
write.writerow(word)

# Close file
file.close()
file2.close()

