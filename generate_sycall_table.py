import csv

with open('x64_syscalls.csv', newline='\n') as csvFile:
	reader = csv.DictReader(csvFile)
	arg_keys = ['arg0', 'arg1', 'arg2', 'arg3', 'arg4', 'arg5']
	for row in reader:
		args = [];
		for arg in arg_keys:
			if (row[arg] == '-'): continue
			row[arg] = 'parse_' + row[arg];
			args.append(row[arg].replace('*', 'ptr ').replace(' ', '_').replace('?', 'unknown'))
		arg_str =  ','.join(args).replace('"', '')
		print(f'SYSCALLS_x64[{ row["%rax"] }] = {{ "{row["syscall name"]}", {{{arg_str }}}}};')
		# print(row);
	