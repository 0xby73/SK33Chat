#pragma once
static void Receive_Messages(const int clientsocket) noexcept;

static std::string Username_Receive(const int clientsocket) noexcept;
static std::string Client_Username(std::string message) noexcept;
static void Client_Assign(const int clientsocket) noexcept;
static void Client_Join(const int clientsocket) noexcept;
