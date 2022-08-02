import subprocess
from datetime import datetime

def main():
	print(f'''
#define BUILD_REV "0000000"
#define BUILD_DATE "{ datetime.utcnow().isoformat() }"
const char *build_rev = BUILD_REV;
const char *build_date = BUILD_DATE;
const char *build_ui_widget_text = "HDP " BUILD_REV;
''')

if __name__ == '__main__':
	main()
