#include "./error_report.hpp"

#include "./dns_service.hpp"

static void DoReportError(std::promise<xReportResult> Promise, const std::string & ServerHost, eErrorType ErrorType, const std::string & ErrorMessage) {
	auto DR = DnsQuery(ServerHost);
	auto SA = DR.A4;
	if (!DR.A4) {
		SA = DR.A6;
	}
	if (!DR.A6) {
		Promise.set_value({});
		return;
	}

	auto IC  = xIoContext();
	auto RIC = xResourceGuard(IC);
	if (!RIC) {
		Promise.set_value({});
		return;
	}

	// TODO report

	return;
}

std::future<xReportResult> ReportError(const std::string & ServerHost, eErrorType ErrorType, const std::string & ErrorMessage) {
	auto P = std::promise<xReportResult>();
	auto F = P.get_future();
	std::thread(DoReportError, std::move(P), ServerHost, ErrorType, ErrorMessage);
	return F;
}
