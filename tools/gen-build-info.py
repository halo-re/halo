import subprocess
from datetime import datetime

def main():
	git_rev = "UNKNOWN"
	try:
		run = subprocess.run(["git", "describe", "--always", "--dirty"], stdout=subprocess.PIPE)
		if run.stdout.strip() != "":
			git_rev = run.stdout.strip().decode("utf-8")
	except:
		pass
	print(f'''
#define BUILD_REV "{ git_rev }"
#define BUILD_DATE "{ datetime.utcnow().isoformat() }"
const char *build_rev = BUILD_REV;
const char *build_date = BUILD_DATE;
const char *build_ui_widget_text = "HDP " BUILD_REV;
''')

if __name__ == '__main__':
	main()

