// IACore-OSS; The Core Library for All IA Open Source Projects
// Copyright (C) 2026 IAS (ias@iasoft.dev)
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <IACore/Logger.hpp>

#include <chrono>
#include <fstream>
#include <iostream>

namespace IACore {
Mut<Logger::LogLevel> Logger::m_log_level = Logger::LogLevel::Info;
Mut<std::ofstream> Logger::m_log_file;

static auto get_seconds_count() -> f64 {
  static Const<std::chrono::time_point<std::chrono::steady_clock>> start_time =
      std::chrono::steady_clock::now();
  Const<std::chrono::time_point<std::chrono::steady_clock>> now =
      std::chrono::steady_clock::now();
  Const<std::chrono::duration<f64>> duration = now - start_time;
  return duration.count();
}

auto Logger::initialize() -> void {}

auto Logger::terminate() -> void {
  if (m_log_file.is_open()) {
    m_log_file.flush();
    m_log_file.close();
  }
}

auto Logger::enable_logging_to_disk(Const<Const<char> *> file_path)
    -> Result<void> {
  if (m_log_file.is_open()) {
    m_log_file.flush();
    m_log_file.close();
  }

  m_log_file.open(file_path);

  if (!m_log_file.is_open()) {
    return fail("Failed to open log file: {}", file_path);
  }

  return {};
}

auto Logger::set_log_level(Const<LogLevel> log_level) -> void {
  m_log_level = log_level;
}

auto Logger::flush_logs() -> void {
  std::cout.flush();
  if (m_log_file.is_open()) {
    m_log_file.flush();
  }
}

auto Logger::log_internal(Const<Const<char> *> prefix, Const<Const<char> *> tag,
                          ForwardRef<String> msg) -> void {
  Const<f64> seconds = get_seconds_count();
  Const<String> out_line =
      std::format("[{:>8.3f}]: [{}]: {}", seconds, tag, msg);

  std::cout << prefix << out_line << console::RESET << '\n';

  if (m_log_file.is_open()) {
    m_log_file.write(out_line.data(),
                     static_cast<std::streamsize>(out_line.size()));
    m_log_file.put('\n');
    m_log_file.flush();
  }
}
} // namespace IACore