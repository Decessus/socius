#ifndef COMMANDS_H
#define COMMANDS_H

#include "utilities.h"
#include "session.h"

QMap<QString,QString> command_aliases;
QMap<QString,int(*function)(IRC_Session,irc_event_data)> commands;

void commandParser(IRC_Session* session,QString command,QString data);
void cmd_init();
void cmd_message(IRC_Session* session,QString target,QString message);
void cmd_action(IRC_Session* session,QString target,QString message);
void cmd_join(IRC_Session* session,QString target);
void cmd_part(IRC_Session* session,QString target);
void cmd_part(IRC_Session* session,QString target,QString message);
void cmd_query(IRC_Session* session,QString target,QString message);
void cmd_query(IRC_Session* session,QString target);
#endif // COMMANDS_H
