#pragma once

#include <future>
#include <pp_common/error.hpp>

struct xReportResult {
	bool     Success    = false;
	uint32_t RetryTimes = 0;
};

std::future<xReportResult> ReportError(const std::string & ServerHost, eErrorType ErrorType, const std::string & ErrorMessage);
