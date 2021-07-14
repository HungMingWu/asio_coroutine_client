#include <asio/co_spawn.hpp>
#include <asio/use_awaitable.hpp>
#include <asio/this_coro.hpp>
#include <asio/ts/net.hpp>
#include <asio/experimental/as_tuple.hpp>
#include <asio/experimental/awaitable_operators.hpp>

constexpr auto use_nothrow_awaitable = asio::experimental::as_tuple(asio::use_awaitable);

asio::awaitable<void> timeout(std::chrono::seconds seconds)
{
	asio::steady_timer timer(co_await asio::this_coro::executor);
	timer.expires_after(seconds);
	co_await timer.async_wait(use_nothrow_awaitable);
}

asio::awaitable<std::error_code> session(asio::ip::tcp::socket socket,
	asio::ip::tcp::endpoint endpoint)
{
	using namespace asio::experimental::awaitable_operators;
	auto connect_result = co_await(
		socket.async_connect(endpoint, use_nothrow_awaitable) ||
		timeout(std::chrono::seconds(5))
	);
	if (connect_result.index() == 1) {
		co_return asio::error::timed_out; // timed out
	}
	auto [e1] = std::get<0>(connect_result);
	if (e1) {
		co_return e1;
	}
	co_return std::error_code{};
}

std::error_code connect_to(const asio::ip::tcp::endpoint &endpoint)
{
	asio::io_context io_context;
	auto connect_future = asio::co_spawn(io_context.get_executor(),
		session(asio::ip::tcp::socket(io_context), endpoint), asio::use_future);
	io_context.run();
	return connect_future.get();
}

int main()
{
	asio::ip::tcp::endpoint target(asio::ip::make_address("192.168.190.69"), 123);
	auto error = connect_to(target);
	if (error)
		printf("%s\n", error.message().c_str());
	else
		printf("Success\n");
	return 0;
}