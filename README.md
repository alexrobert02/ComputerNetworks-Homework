# Computer Networks - Homework

Develop two applications (named "client" and "server") that communicate with each other based on a protocol with the following specifications:

- Communication is carried out by executing commands entered from the keyboard on the client side, and executed in child processes created by the server;
- Commands are character strings delimited by new lines;
- Responses are byte strings prefixed by the length of the response;
- The result obtained from executing any command will be displayed by the client;
- The child processes in the server do not communicate directly with the client, but only with the parent process;
- The minimal protocol includes the following commands:
  - `login : username` -  the existence of the username is validated using a configuration file that contains all users with access to the application's functionalities. The execution of the command will be handled in a child process on the server;
  - `get-logged-users` - displays information (username, hostname for remote login, time the entry was made) about users currently logged into the operating system (see "man 5 utmp" and "man 3 getutent"). This command cannot be executed unless the user is authenticated in the application. The execution will be handled in a child process on the server;
  - `get-proc-info : pid` - displays information (name, state, ppid, uid, vmsize) about the specified process (source of information: the file /proc/<pid>/status). This command cannot be executed unless the user is authenticated in the application. The execution will be handled in a child process on the server;
  - `logout`;
  - `quit`.
- In implementing the commands, no functions from the "exec()" family (or similar, e.g., popen(), system(), etc.) will be used for executing system commands that offer the respective functionalities;
- Communication between processes must use at least once each of the following inter-process communication mechanisms: pipes, FIFOs, and socketpairs.
