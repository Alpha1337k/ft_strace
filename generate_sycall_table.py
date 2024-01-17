import csv

# special so https://chromium.googlesource.com/chromiumos/docs/+/master/constants/syscalls.md#x86_64-64_bit

all_parsers = set()

with open('x86_syscalls.csv', newline='\n') as csvFile:
	reader = csv.DictReader(csvFile)
	arg_keys = ['arg0', 'arg1', 'arg2', 'arg3', 'arg4', 'arg5']
	print("#include \"strace.h\"")
	print("const t_header SYSCALLS_x64[1024] = {")
	for row in reader:
		args = [];
		for arg in arg_keys:
			if (row[arg] == '-'): continue

			row[arg] = row[arg].replace('*', 'ptr ')

			if (row[arg][:-1] != '*'):
				row[arg] = 'parse_' + ' '.join(row[arg].split(' ')[:-1]);
			else:
				row[arg] = 'parse_' + row[arg];
			if row[arg] == 'parse_': row[arg] = "parse_unknown"
			item = row[arg].replace('*', 'ptr ').replace('const ', '').replace('const', '').replace('?', 'unknown').strip().replace(' ', '_');
			all_parsers.add(item)
			args.append(item)
		arg_str =  ','.join(args).replace('"', '')
		print(f'\t {{ "{row["syscall name"]}", {{{arg_str }}}}},')
		# print(row);
	print('};')

def make_func(i):
	return "char\t*"+ i +"(void *data);"
	
vals = map(make_func, all_parsers);

print('\n'.join((list(vals))));