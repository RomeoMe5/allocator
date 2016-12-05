#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>

#include <signal.h>
#include <time.h>
#include <linux/input.h>

#define LOG_FILE "/tmp/keyLoggerCount.log"
#define DEVICE "/dev/input/eventX"
#define TIME_INTERVAL 5*30

#define uint unsigned int
//#define DEBUG

int main(void) 
{
        
        /* Our process ID and Session ID */
        pid_t pid, sid;
        
        /* Fork off the parent process */
        pid = fork();
        if (pid < 0) 
		{
			perror("Fork ERROR");
            exit(EXIT_FAILURE);
        }
        /* If we got a good PID, then
           we can exit the parent process. */
        if (pid > 0) 
		{
            exit(EXIT_SUCCESS);
        }

        /* Change the file mode mask */
        umask(0);
                
        /* Open any logs here */        
                
        /* Create a new SID for the child process */
        sid = setsid();
        if (sid < 0) 
		{
                /* Log the failure */
				perror("Setsid ERROR");
                exit(EXIT_FAILURE);
        }
        
        /* Change the current working directory 
		
        if ((chdir("/")) < 0) {
                /* Log the failure 
				perror("ChDir ERROR");
                exit(EXIT_FAILURE);
        }*/
        
        /* Close out the standard file descriptors */
        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);
        
        /* Daemon-specific initialization goes here */
		
		// Open device
		int keyBoard= open(DEVICE, O_RDONLY);
		if (keyBoard < 0) 
		{
			perror("Device inintialization/open failure!");
			exit(EXIT_FAILURE);
		}
		
		FILE* log_file = fopen(LOG_FILE, "w");
		if (log_file == NULL) 
		{
			perror("Open log file ERROR");
			close(keyBoard);
			exit(EXIT_FAILURE);
		}
		
		time_t saveTime, now;
		time(&saveTime);
		uint cntClick=0;
		
        /* The Big Loop */
		//@TODO: Надо предусмотреть выход из вечного цикла и закрытие файла / дескриптора устройства
        while (true) 
		{
           /* Do some task here ... */
		    struct input_event kbEvent;					// не знаю как, но это работает.
		    int n_read = read(keyBoard, &kbEvent, sizeof(struct input_event));
			if (n_read != sizeof(struct input_event)) 
			{
				perror("Read EVENT error");	//continue BIG LOOP
				continue;
			}
			if ((kbEvent.type == EV_KEY) && (kbEvent.value == 0)) 
			{
				cntClick++;										//считаем клики
			}	
				
		    time(&now);
            if(difftime(saveTime,now)>=TIME_INTERVAL)			//прошел заданный интервал времени
		    {
				fseek(log_file, 0, SEEK_SET);
				fprintf(log_file, "Timestamp: %s; Click count: %d", ctime(&now), cntClick); 
				fflush(log_file);
				saveTime=now;
				cntClick=0;
		    }	
        }
		
		close(keyBoard);
		fclose(log_file);
		exit(EXIT_SUCCESS);
}