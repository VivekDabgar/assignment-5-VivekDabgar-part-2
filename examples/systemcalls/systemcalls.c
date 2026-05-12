#include "systemcalls.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>


/**
 * @param cmd the command to execute with system()
 * @return true if the command in @param cmd was executed
 *   successfully using the system() call, false if an error occurred,
 *   either in invocation of the system() call, or if a non-zero return
 *   value was returned by the command issued in @param cmd.
*/
bool do_system(const char *cmd)
{
	if (cmd == NULL)
	{
	return false;
	}

	int status = system(cmd);
	//system call itself failed
	if (status == -1)
	{
	return false;
	}

	// the command did not exit normally
	if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
	{
	return true;
	}

	return false;
}

/**
* @param count -The numbers of variables passed to the function. The variables are command to execute.
*   followed by arguments to pass to the command
*   Since exec() does not perform path expansion, the command to execute needs
*   to be an absolute path.
* @param ... - A list of 1 or more arguments after the @param count argument.
*   The first is always the full path to the command to execute with execv()
*   The remaining arguments are a list of arguments to pass to the command in execv()
* @return true if the command @param ... with arguments @param arguments were executed successfully
*   using the execv() call, false if an error occurred, either in invocation of the
*   fork, waitpid, or execv() command, or if a non-zero return value was returned
*   by the command issued in @param arguments with the specified arguments.
*/

bool do_exec(int count, ...)
{
    va_list args;
    va_start(args, count);
    char * command[count+1];
    int i;
    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;
    // this line is to avoid a compile warning before your implementation is complete
    // and may be removed
    command[count] = command[count];
    va_end(args);

	int status;
    	fflush(stdout);

    	int pid = fork();
	if (pid == -1)
    	{
	perror("fork");
        return false;
    	}

   	if (pid == 0)
   	{
   	// Child process
	execv(command[0], command);
	perror("execv");
	_exit(EXIT_FAILURE);
   	}

	if (waitpid(pid, &status, 0) < 0)
	{
	perror("waitpid");
	return false;
	}

	if (!WIFEXITED(status))
	{
	return false;
	}

	if (WEXITSTATUS(status) != 0)
	{
	return false;
	}

    return true;
}

/**
* @param outputfile - The full path to the file to write with command output.
*   This file will be closed at completion of the function call.
* All other parameters, see do_exec above
*/
bool do_exec_redirect(const char *outputfile, int count, ...)
{
    va_list args;
    va_start(args, count);
    char * command[count+1];
    int i;
    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;
    // this line is to avoid a compile warning before your implementation is complete
    // and may be removed
    command[count] = command[count];
    va_end(args);

	int status;
	fflush(stdout);

	int pid = fork();
	if (pid < 0)
	{
	return false;
	}

	if (pid == 0)
	{
	// child process
	int fd = open(outputfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0){
	perror("failed to open redirect file");
	_exit(EXIT_FAILURE);
	}

	// Redirect standard output (fd 1) to our opened file descriptor
	if (dup2(fd, STDOUT_FILENO) < 0){
	perror("dup2 failed");
	_exit(EXIT_FAILURE);
	}
	close(fd); // we do not need the original fd anymore after dup2

	//Execute the command
	execv(command[0], command);

	// if execv returns. it failed
	perror("execv failed");
	exit(EXIT_FAILURE);
	}

	// Parent process: wait for the child
	if (waitpid(pid, &status, 0) == -1) {
	return false;
	}

	if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
	return true;
	}

    return false;
}
