#include "Timer.hpp"

#include "../utilities/core/Assert.hpp"

#include <fmt/format.h>
#include <fmt/chrono.h>  // Formatting for std::chrono
#include <fmt/color.h>   // For fmt::fg

namespace openstudio::workflow::util {

Timer::Timer(std::string message, unsigned level) : m_message(std::move(message)), m_level(level) {}

unsigned Timer::level() const {
  return m_level;
}

auto Timer::start() const {
  return m_start;
}

auto Timer::end() const {
  return m_end;
}

void Timer::tick() {
  m_end = TimePointType{};
  m_start = ClockType::now();
}

void Timer::tock() {
  m_end = ClockType::now();
}

std::string Timer::message() const {
  return m_message;
}

auto Timer::duration() const {
  OS_ASSERT(m_end != TimePointType{});
  return std::chrono::duration_cast<DurationType>(m_end - m_start);
}

std::string Timer::format() const {
  if (m_end == TimePointType{}) {
    return fmt::format("Timer {} started at {}", m_message, m_start);
  } else {
    return fmt::format("Timer {} took {} ms", m_message, duration());
  }
}

std::string Timer::formatRow(size_t message_len, size_t timepoint_len, size_t duration_len) const {
  if (m_level == 1) {
    return fmt::format(fmt::fg(fmt::color::light_gray), "| o {3:<{0}.{0}s} | {4:%T} | {5:%T} | {6:^{2}} |\n", message_len - 2, timepoint_len,
                       duration_len, m_message, m_start, m_end, duration());
  } else if (m_level == 2) {
    return fmt::format(fmt::fg(fmt::color::light_gray), "|   o {3:<{0}.{0}s} | {4:%T} | {5:%T} | {6:^{2}} |\n", message_len - 4, timepoint_len,
                       duration_len, m_message, m_start, m_end, duration());
  } else {
    return fmt::format("| {3:<{0}.{0}s} | {4:%T} | {5:%T} | {6:^{2}} |\n", message_len, timepoint_len, duration_len, m_message, m_start, m_end,
                       duration());
  }
}

Timer& TimerCollection::newTimer(std::string message, unsigned level) {
  m_timerIndices.push_back(m_timers.size());
  return m_timers.emplace_back(std::move(message), level);
}

void TimerCollection::tockCurrentTimer() {
  m_timers[m_timerIndices.back()].tock();
  m_timerIndices.pop_back();
}

// line_length is the maximum terminal width, fit = true will cause the table to be resized down as much as possible, fit = false means the table will take exactly line_length
std::string TimerCollection::timeReport(int line_length, bool fit) const {
  // Timer | start | end | duration
  std::string result;

  constexpr size_t timepoint_len = 8;  // 19 if not specifying %T
  constexpr size_t duration_len = 10;
  constexpr size_t ncols = 4;
  constexpr size_t extra_chars = 3 * (ncols - 1) + 2 * (ncols - 2);
  size_t message_len = line_length - (2 * timepoint_len + duration_len + extra_chars);
  if (fit) {
    size_t max_message_len = 0;
    for (const auto& timer : m_timers) {
      max_message_len = std::max(max_message_len, timer.message().size());
    }
    message_len = std::min(message_len, max_message_len + 4);
    // fmt::print("line_length={}, message_len={}, max_message_len={}\n", line_length, message_len, max_message_len);
  }

  // Use │ or | ?
  // result += fmt::format("┌{0:─^{1}}┐\n", "", total_len - 2);
  result +=
    fmt::format("| {3:^{0}} | {4:^{1}} | {5:^{1}} | {6:^{2}} |\n", message_len, timepoint_len, duration_len, "Timer", "start", "end", "duration");
  // result += fmt::format("|{0:─^{1}}|\n", "", total_len - 2);
  result += fmt::format("|:{3:-^{0}}|:{3:-^{1}}:|:{3:-^{1}}:|:{3:-^{2}}:|\n", message_len + 1, timepoint_len, duration_len, "");

  for (const auto& timer : m_timers) {
    result += timer.formatRow(message_len, timepoint_len, duration_len);
  }

  m_totalTimer.tock();
  result += fmt::format(fmt::fg(fmt::color::red), "| {3:^{0}.{0}s} | {4:%T} | {5:%T} | {6:^{2}} |\n", message_len, timepoint_len, duration_len,
                        m_totalTimer.message(), m_totalTimer.start(), m_totalTimer.end(), m_totalTimer.duration());

  return result;
};

}  // namespace openstudio::workflow::util
