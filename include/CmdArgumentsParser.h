#pragma once

namespace cmd {
class CmdArgumentsParser {
  public:
    CmdArgumentsParser(int argc, char* argv[]);
    ~CmdArgumentsParser();
    void PrintHelp() const;
    void PrintVersion() const;
    void Parse();

  private:
    int m_argc;
    char** m_argv;
};
}  // namespace cmd