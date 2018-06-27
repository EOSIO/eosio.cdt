/*
 * Copyright 2016 WebAssembly Community Group participants
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <cassert>
#include <cinttypes>
#include <cstdio>
#include <cstdlib>
#include <iostream>

#include "src/apply-names.h"
#include "src/binary-reader.h"
#include "src/binary-writer.h"
#include "src/binary-reader-ir.h"
#include "src/error-handler.h"
#include "src/feature.h"
#include "src/generate-names.h"
#include "src/ir.h"
#include "src/option-parser.h"
#include "src/stream.h"
#include "src/validator.h"
#include "src/wast-lexer.h"
#include "src/wat-writer.h"

using namespace wabt;

static int s_verbose;
static std::string s_infile;
static std::string s_outfile;
static Features s_features;
static WriteBinaryOptions s_write_binary_options;
static std::unique_ptr<FileStream> s_log_stream;

static const char s_description[] =
R"(  Read a file in the WebAssembly binary format, strip bss or any data segment that is only initialized to zeros.

  $ stripbss test.wasm -o test.stripped.wasm

  # or original replacement
  $ wasm2wat test.wasm 
)";

static void ParseOptions(int argc, char** argv) {
  OptionParser parser("stripbss", s_description);

  parser.AddOption('v', "verbose", "Use multiple times for more info", []() {
    s_verbose++;
    s_log_stream = FileStream::CreateStdout();
  });
  parser.AddHelpOption();
  parser.AddOption(
      'o', "output", "FILENAME",
      "Output file for the generated wast file, by default use stdout",
      [](const char* argument) {
        s_outfile = argument;
        ConvertBackslashToSlash(&s_outfile);
      });
  parser.AddArgument("filename", OptionParser::ArgumentCount::One,
                     [](const char* argument) {
                       s_infile = argument;
                       ConvertBackslashToSlash(&s_infile);
                     });
  parser.Parse(argc, argv);
}

void StripZeroedData( Module& mod ) {
   std::vector<DataSegment*> ds;
   for ( auto DS : mod.data_segments ) {
      bool isZeroed = true;
      for ( auto datum : DS->data ) {
         isZeroed &= datum == 0;
      }
      if (!isZeroed) {
         ds.push_back(DS);
      }
   }
   mod.data_segments = ds;
}

void WriteBufferToFile(string_view filename,
                       const OutputBuffer& buffer) {
  buffer.WriteToFile(filename);
}

int ProgramMain(int argc, char** argv) {
  Result result;
  
  InitStdio();
  ParseOptions(argc, argv);
  
  std::vector<uint8_t> file_data;
  bool stub = false;
  std::unique_ptr<FileStream> s_log_stream_s;
  result = ReadFile(s_infile.c_str(), &file_data);
  if (Succeeded(result)) {
    ErrorHandlerFile error_handler(Location::Type::Binary);
    Module module;
    const bool kStopOnFirstError = true;
    ReadBinaryOptions options(s_features, s_log_stream_s.get(),
                              stub, kStopOnFirstError,
                              stub);
    result = ReadBinaryIr(s_infile.c_str(), file_data.data(),
                          file_data.size(), &options, &error_handler, &module);

    if (Succeeded(result)) {
      StripZeroedData(module);
     if (Succeeded(result)) {
      MemoryStream stream(s_log_stream.get());
      result =
          WriteBinaryModule(&stream, &module, &s_write_binary_options);

      if (Succeeded(result)) {
        if (s_outfile.empty()) {
          s_outfile = s_infile;
        }
        WriteBufferToFile(s_outfile.c_str(), stream.output_buffer());
      }
    }
   } 

  }
  return result != Result::Ok;
}

int main(int argc, char** argv) {
  WABT_TRY
  return ProgramMain(argc, argv);
  WABT_CATCH_BAD_ALLOC_AND_EXIT
}
