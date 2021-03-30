/*
   File: inter.c
   Course: CENG251
   Author: Lindsey Hatch
   Date: Saturday Mar 20, 2021   16:40 PM
*/

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <fcntl.h>
#include <string.h>
#include <pwd.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <grp.h>

char *prompt = "Enter your command: ";

   char * hello(char ** cmdArgs) {
      char *user = getenv("USER");
      fprintf(stdout, "Hello, %s\n", user);
      prompt = strcat(user, prompt);
      return "Hello command";
   }

   char * password(char ** cmdArgs) {
      struct passwd *pw;
      struct stat st;
      int status;
      int i = 0;
      while(cmdArgs[i]) {
         pw = getpwnam(cmdArgs[i]);
	 fprintf(stdout,"User: %s\n", cmdArgs[i]);
         fprintf(stdout,"\tName: %s \n", pw->pw_gecos);
         fprintf(stdout,"\tGroup ID: %d \n", pw->pw_gid);
         fprintf(stdout,"\tHome Directory: %s \n", pw->pw_dir);
	 stat("inter.c", &st);
         fprintf(stdout,"Last Access: %s\n", ctime(&st.st_atime));
	 i++;
      }
      return "Password command";
   }

   char * compare(char ** cmdArgs) {
      int i=0;
      char *longest = cmdArgs[0];
      while(cmdArgs[i]) {
	 if(strlen(cmdArgs[i])>strlen(longest)); {
		  longest = cmdArgs[i];
	 }
		  i++;
      }
      fprintf(stdout, "Longest String: %s\n", longest);

      // Checks which word comes first
      if(strcmp(cmdArgs[0], cmdArgs[1])<0) {
	 fprintf(stdout, "%s comes before %s\n", cmdArgs[0], cmdArgs[1]);
	    } else {
	 fprintf(stdout, "%s comes before %s\n", cmdArgs[1], cmdArgs[0]);
	    }

      // Checks first 4 chars and tests if strings are the same
      if(strncmp(cmdArgs[0], cmdArgs[1],4)==0) {
	 fprintf(stdout, "The first four characters are equal\n");
      } else {
	 fprintf(stdout, "The first four characters are not equal\n");
      }      
      return "Compare command";
   }

   char * group(char ** cmdArgs) {
      struct stat buf;
      struct group *grp;
      int status;
      int i = 0;
      while(cmdArgs[i]) {
	 fprintf(stdout, "File Name: %s\n", cmdArgs[i]);
         status = stat(cmdArgs[i], &buf);	 
         fprintf(stdout, "\tGroup ID: %-8d\n", buf.st_gid);
	 grp = getgrgid(buf.st_gid);
	 fprintf(stdout, "\tGroup Name: %-8.8s\n", grp->gr_name);
	 i++;
      }
      return "Group command";
   }

   char * display(char ** cmdArgs) {
      FILE *fp;
      char test[50];
      fp = fopen("mydb.txt", "r");
      fseek(fp,92,SEEK_CUR);
      char c = fgetc(fp);
      while(c != EOF)
      {
      	fprintf(stdout, "%c", c);
	c = fgetc(fp);
      }
      fclose(fp);
      return "Display command";
   }

   void ctrlCHandler(int signum) {
      write_history(".inter.history");
      system("echo this is your session history; cat -n .inter.history");
      fprintf(stderr, "Command server ended \n");
      exit(1);
   }

   char *interpret(char ** tokens) {
      char *cmd;
      cmd = tokens[0];
      if(strcasecmp(cmd, "hello")==0) return hello(&tokens[1]);
      if(strcasecmp(cmd, "password")==0) return password(&tokens[1]);
      if(strcasecmp(cmd, "compare")==0) return compare(&tokens[1]);
      if(strcasecmp(cmd, "group")==0) return group(&tokens[1]);
      if(strcasecmp(cmd, "display")==0) return display(&tokens[1]);
      if(strcasecmp(cmd, "replace")==0) return replace(&tokens[1]);
      else return "command not found\n";
   }

int main(int argc, char * argv[],char * envp[])
{
   //char *prompt = "Enter your command: ";
   char cmd[100];
   char *cmdLine;
   int nBytes;
   char *expansion;
   char **tokens;

   signal(SIGINT, ctrlCHandler);
   read_history(".inter.history");
   history_comment_char = '#';

   while(1) {
      cmdLine = readline(prompt);
      if(!cmdLine) break;

      history_expand(cmdLine, &expansion);
      tokens = history_tokenize(expansion);
      add_history(expansion);
      if(!tokens) continue;

      char *response = interpret(tokens);
      fprintf(stdout, "%s\n", response);
   }
   raise(SIGINT);
   return 0;
}


   
