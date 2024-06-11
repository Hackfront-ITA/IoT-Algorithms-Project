import sys, csv
import matplotlib.pyplot as plt

sample_s = []
sample_mA = []
sample_mW = []

# Read from csv file
filename = sys.argv[1]
csv_file = open(filename)
csv_reader = csv.reader(csv_file, delimiter=',')
line_count = 0
for row in csv_reader:
	if line_count != 0:
		sample_s.append(float(row[0]))
		# sample_mA.append(float(row[1]))
		sample_mW.append(float(row[2]))
	line_count += 1

fig, ax = plt.subplots()

mean_mW = 0
for value in sample_mW:
	mean_mW += value
mean_mW /= len(sample_mW)

plt.title(f'{filename[:-4]}')
plt.xlabel('sec')
plt.ylabel(f'mW (mean: {mean_mW:.0f})')

ax.plot(sample_s, sample_mW)
# ax.plot(sample_s, sample_mA);

plt.savefig(f'{filename[:-4]}.png', dpi=200)
plt.show()
