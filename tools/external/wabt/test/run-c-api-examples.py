#!/usr/bin/env python
#
# Copyright 2020 WebAssembly Community Group participants
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

import argparse
import os
import subprocess
import sys

import find_exe

ALL_EXAMPLES = [
    'callback',
    'finalize',
    'global',
    'hello',
    'hostref',
    'memory',
    'multi',
    'reflect',
    'serialize',
    'start',
    'table',
    'threads',
    'trap',
]

# We don't currently yet support shared modules which is required for threads.
SKIP_EXAMPLES = [
    'threads',  # We don't yet support threads
    'finalize',  # This test is really slow
    'hostref',  # The wasm module is currently invalid (needs subtyping changes)
]

IS_WINDOWS = sys.platform == 'win32'


def run_test(test_exe):
    print('Running.. %s' % test_exe)
    proc = subprocess.Popen([test_exe], stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    stdout, _ = proc.communicate()
    if proc.returncode == 0:
        return 0
    print("Failed with returncode=%d" % proc.returncode)
    print(stdout)
    print('FAIL(%d): %s' % (proc.returncode, test_exe))
    return 1


def error(msg):
    print(msg)
    sys.exit(1)


def check_for_missing(upstream_examples):
    # Check that all the expected examples are found

    upstream_examples = set(upstream_examples)
    all_examples = set(ALL_EXAMPLES)
    unexpected = upstream_examples - all_examples
    if unexpected:
        error('Unexpected examples found: %s' % str(unexpected))
    missing = all_examples - upstream_examples
    if missing:
        error('Missing example binaries not found: %s' % str(missing))


def main(args):
    print('Running c-api examples..')
    parser = argparse.ArgumentParser()
    parser.add_argument('--bindir', metavar='PATH',
                        default=find_exe.GetDefaultPath(),
                        help='directory to search for all executables.')

    options = parser.parse_args(args)
    script_dir = os.path.dirname(os.path.abspath(__file__))
    root = os.path.dirname(script_dir)
    upstream_dir = os.path.join(root, 'third_party', 'wasm-c-api', 'example')
    upstream_examples = [f for f in os.listdir(upstream_dir) if os.path.splitext(f)[1] == '.wasm']
    upstream_examples = [os.path.splitext(f)[0] for f in upstream_examples]
    check_for_missing(upstream_examples)

    def should_skip(e):
        return any(e.startswith(skip) for skip in SKIP_EXAMPLES)

    to_run = [e for e in upstream_examples if not should_skip(e)]

    os.chdir(options.bindir)
    count = 0
    fail_count = 0
    for f in to_run:
        if IS_WINDOWS:
            f += '.exe'
        exe = os.path.join(options.bindir, 'wasm-c-api-' + f)
        if not os.path.exists(exe):
            error('test executable not found: %s' % exe)

        count += 1
        if run_test(exe) != 0:
            fail_count += 1

    if fail_count:
        print('[%d/%d] c-api examples failed' % (fail_count, count))
        return 1
    return 0


if __name__ == '__main__':
    sys.exit(main(sys.argv[1:]))
