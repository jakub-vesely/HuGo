import argparse
import sys

class ArgumentProcessor():
  @staticmethod
  def process_cmd_arguments():
    gui_default = len(sys.argv) == 1

    parser = argparse.ArgumentParser(description='This tool provides a BLE communication interface for HuGo module kit')

    parser.add_argument('--flash', '-f', action='store_true', help=f'Uploads content of folder specified by -s argument. When the -s argument is not entered "upload" folder will be used. Deprecated content will be removed.')
    parser.add_argument('--gui', '-gui', default=gui_default,  action='store_true', help='GUI is launched')
    parser.add_argument('--mac_addr', '-a', help='mac address of the main HuGo module')
    parser.add_argument('--monitor', '-m', action='store_true', help='Stay connected to receive log messages.')
    parser.add_argument('--remote_control', '-rc', action='store_true', help='read pressed keys and provide them as a remore control.')
    parser.add_argument('--verbose', '-v', action='store_true', help='debug messages are printed')
    parser.add_argument('--source_dir', '-s', default="upload", help='defines path to directory containing events.py which will be uploaded. If the source dir is not defined, the"upload" folder will be used.')
    parser.add_argument('--extra_reboot', '-xr', action='store_true', help='perform an extra reboot after flashing. It is usefull when a core scripts that are already loaded are changed')

    args = parser.parse_args()
    if not args.gui and not args.flash and not args.monitor:
        parser.print_help()
        return None

    sys.argv = [sys.argv[0]]
    return args
