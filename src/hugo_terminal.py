#!/usr/bin/env python
from terminal_kivy.terminal import Terminal
from terminal_kivy.argument_processor import ArgumentProcessor
import sys

def main():
  args = ArgumentProcessor.process_cmd_arguments()
  if not args:
    return False

  if not args.flash and not args.monitor:
    return True

  terminal = Terminal(args.remote_control, args.mac_addr, args.verbose, args.source_dir)
  return terminal.run(args)

if not main():
  sys.exit(1)
