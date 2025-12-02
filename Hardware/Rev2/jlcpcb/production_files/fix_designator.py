import csv

FILE_NAME = 'BOM-Rev2.csv'

parts: dict[str, dict] = {}

with open(FILE_NAME, 'r', newline='') as f:
    reader = csv.DictReader(f)
    for line in reader:
        mpn = line['LCSC']
        if mpn not in parts:
            parts[mpn] = {
                'comment': line['Comment'],
                'designator': set(),
                'footprint': line['Footprint']
            }
        parts[mpn]['designator'] |= set(line['Designator'].split(','))

with open(FILE_NAME, 'w', newline='') as f:
    fields = ['Comment', 'Designator', 'Footprint', 'LCSC']
    writer = csv.DictWriter(f, fieldnames=fields)
    writer.writeheader()
    for mpn, info in parts.items():
        for d in parts[mpn]['designator']:
            writer.writerow({
                'Comment': info['comment'],
                'Designator': d,
                'Footprint': info['footprint'],
                'LCSC': mpn
            })

