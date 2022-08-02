from typing import Optional
import logging
import platform


if platform.system() == 'Windows':
    class ColorLogHandler(logging.StreamHandler):
        pass

else:
    class ColorLogHandler(logging.StreamHandler):
        def emit(self, record):
            color = {
                'WARNING': 'bright_yellow',
                'ERROR': 'bright_red'
            }.get(record.levelname)

            record.name = self.ansi_color(record.name, color)
            record.msg = self.ansi_color(record.msg, color)
            super().emit(record)

        @staticmethod
        def ansi_color(s: str, color: Optional[str]) -> str:
            """
            Colorize string `s` by wrapping in ANSI escape sequence for given `color`.
            This function does not consider whether escape sequences are functional or not; it is up to the caller to determine
            if its appropriate. Check global `ansi_color_enabled` value in this module.
            """
            if color is None:
                return s

            codes = {
                'black':          '30m',
                'bright_black':   '90m',
                'gray':           '90m',  # alias 'bright black'
                'blue':           '34m',
                'bright_blue':    '94m',
                'cyan':           '36m',
                'bright_cyan':    '96m',
                'green':          '32m',
                'bright_green':   '92m',
                'magenta':        '35m',
                'bright_magenta': '95m',
                'red':            '31m',
                'bright_red':     '91m',
                'white':          '37m',
                'bright_white':   '97m',
                'yellow':         '33m',
                'bright_yellow':  '93m',
            }
            return '\u001b[' + codes[color] + s + '\u001b[0m'
