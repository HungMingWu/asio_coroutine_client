#pragma once
#include <string_view>
#include <system_error>
struct client {
public:
	std::error_code connect_to(std::string_view host, unsigned short port);
};