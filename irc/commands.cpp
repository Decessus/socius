/**
  This is the IRC Commands source file, all commands are defined in irc.h
  This code is for the parsing of IRC commands i.e.: /me /say as well as
    command-line function calls such as $calc()

  Also, Linux users will be able to run programs and scripts and pass commands
    to the terminal directly from the IRC module. For security purposes, until
    I come up with a security policy framework, this will only be possible from
    the text input. Socius and IRC scripts will not have access to this
    functionality just yet.
  **/
