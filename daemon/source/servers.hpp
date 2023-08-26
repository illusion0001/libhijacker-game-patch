#pragma once

#include "server.hpp"

class ElfServer : public TcpServer {

	static constexpr uint16_t PORT = 9029;

	void run(TcpSocket &sock) override;

	public:
		ElfServer() noexcept : TcpServer(PORT) {}

		ElfServer(const ElfServer&) = delete;

		ElfServer &operator=(const ElfServer&) = delete;

		ElfServer(ElfServer &&rhs) noexcept = default;

		ElfServer &operator=(ElfServer &&rhs) noexcept = default;

		~ElfServer() noexcept override = default;
};

class KlogServer : public TcpServer {

	static constexpr uint16_t PORT = 9081;

	void run(TcpSocket &sock) override;

	public:
		KlogServer() noexcept : TcpServer(PORT) {}

		KlogServer(const KlogServer&) = delete;

		KlogServer &operator=(const KlogServer&) = delete;

		KlogServer(KlogServer &&rhs) noexcept = default;

		KlogServer &operator=(KlogServer &&rhs) noexcept = default;

		~KlogServer() noexcept override = default;
};

class CommandServer : public TcpServer {

	static constexpr uint16_t PORT = 9028;

	void run(TcpSocket &sock) override;

	public:
		CommandServer() noexcept : TcpServer(PORT) {}

		CommandServer(const CommandServer&) = delete;

		CommandServer &operator=(const CommandServer&) = delete;

		CommandServer(CommandServer &&rhs) noexcept = default;

		CommandServer &operator=(CommandServer &&rhs) noexcept = default;

		~CommandServer() noexcept override = default;
};

class AbortServer : public TcpServer {

	static constexpr uint16_t PORT = 9048;

	void run(TcpSocket &sock) override;

	void start() noexcept override {
		TcpSocket sock = accept();
		if (sock) {
			run(sock);
		}
	}

	public:
		AbortServer() noexcept : TcpServer(PORT) {}

		AbortServer(const AbortServer&) = delete;

		AbortServer &operator=(const AbortServer&) = delete;

		AbortServer(AbortServer &&rhs) noexcept = default;

		AbortServer &operator=(AbortServer &&rhs) noexcept = default;

		~AbortServer() noexcept override = default;
};
