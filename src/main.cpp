#ifdef __unix__
    // no unix support yet
    #define Windows 0
#elif defined(_WIN32) || defined(WIN32)
    #include <stdio.h>
    #include <chrono>
    #include <Windows.h>
    #include <string>
    #define Windows 1
#endif

using namespace std::chrono;

std::string make_command(int argc, char *argv[]);
void get_help();
void in_milliseconds(std::string cmd);
void in_microseconds(std::string cmd);

enum TIME_UNIT {
    MILLISECONDS,
    MICROSECONDS
};

int main(int argc, char *argv[]) {   
    if (Windows) {
        if (argc != 1) {
            std::string cmd_to_run = "";

            for (int i = 1; i < argc; i++)
            {   
                if ((std::string)argv[i] == "-help") {
                    get_help();

                } else if ((std::string)argv[i] == "-m") {
                    in_milliseconds(make_command(argc, argv)); return 0;

                } else if ((std::string)argv[i] == "-u") {
                    in_microseconds(make_command(argc, argv)); return 0;

                } else {
                    // run default command
                    in_milliseconds(make_command(argc, argv)); return 0;
                }
            }
        } else {
            get_help();
        }

    } else {
        printf("\n\nNO SUPPORT FOR GNU/Linux YET\n");
    }

    return 1;
}

void print_out(const char *units, uint16_t time) {
    printf("\n----------------------------------");
    printf("\n Program took:\t\t%d %s\n", time, units);
    printf("----------------------------------");
}

std::string make_command(int argc, char *args[]) {
    std::string cmd = "";
    
    if (argc >= 3){
        for (int i = 1; i < argc; i++) {
            if (args[i][0] != '-') {
                cmd += args[i];
                cmd += " ";
            }
        }

        return cmd;
    } else if (argc == 2) {
        return cmd += args[1]; 
    }

    get_help();
    return "";
}

uint64_t get_time(TIME_UNIT time_type) {
    if (time_type == TIME_UNIT::MILLISECONDS) {
        return time_point_cast<milliseconds>(high_resolution_clock::now()).time_since_epoch().count();
    } else if (time_type == TIME_UNIT::MICROSECONDS) {
        return time_point_cast<microseconds>(high_resolution_clock::now()).time_since_epoch().count();
    }

    return 0;
}

uint32_t time_cmd(std::string cmd, TIME_UNIT time_type) {
    system("");

    // runs again to time the system call
    uint64_t process_start_time = get_time(time_type);
    system("");
    uint64_t process_start_end = get_time(time_type);

    // actual runs and times the command
    uint64_t start_time = get_time(time_type);
    system(cmd.c_str());
    uint64_t end_time = get_time(time_type);

    return end_time - start_time - (process_start_end - process_start_time);
}

void in_milliseconds(std::string cmd) {
    // runs once to enable optimizations
    print_out("ms", time_cmd(cmd, TIME_UNIT::MILLISECONDS));
}

void in_microseconds(std::string cmd) {
    // runs once to enable optimizations
    print_out("us", time_cmd(cmd, TIME_UNIT::MICROSECONDS));
}

void get_help() {
    printf("\nUSAGE:\n");
    printf("timeit {arguments} {program to run with args}\n\n");
    printf("ARGUMENTS:\n");
    printf("-help\tshows this menu\n");
    printf("-m\treturns time in milliseconds (default)\n");
    printf("-u\treturns time in microseconds\n");
    printf("\nNOTE: -m and -u are mutually exclusive\n");

    exit(0);
}