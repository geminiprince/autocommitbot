#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <signal.h>
#include <pwd.h>
#include <stdarg.h>
#include <sys/wait.h>

// Configuration
#define REPO_DIR "dailycommit"
#define GIT_EMAIL "geminiprinceee@gmail.com"
#define GIT_NAME "geminiprince"
#define COMMIT_INTERVAL 300 // 5 minutes

// Array of quotes
const char* quotes[] = {
    "The only way to do great work is to love what you do. - Steve Jobs",
    "Innovation distinguishes between a leader and a follower. - Steve Jobs",
    "The future belongs to those who believe in the beauty of their dreams. - Eleanor Roosevelt",
    "Success is not final, failure is not fatal: It is the courage to continue that counts. - Winston Churchill",
    "The only limit to our realization of tomorrow will be our doubts of today. - Franklin D. Roosevelt"
};
const int num_quotes = sizeof(quotes) / sizeof(quotes[0]);

// Global flag for signal handling
static volatile sig_atomic_t running = 1;

// Signal handler for graceful shutdown
void handle_signal(int sig) {
    (void)sig;  // Explicitly mark parameter as unused
    running = 0;
}

// Execute a command in the specified directory
int run_command(const char* dir, const char* format, ...) {
    char cmd[1024];
    va_list args;
    va_start(args, format);
    vsnprintf(cmd, sizeof(cmd), format, args);
    va_end(args);
    
    char cwd[1024];
    if (!getcwd(cwd, sizeof(cwd))) {
        perror("getcwd() error");
        return -1;
    }
    
    if (chdir(dir) != 0) {
        perror("chdir() error");
        return -1;
    }
    
    int result = system(cmd);
    chdir(cwd);
    return WEXITSTATUS(result);
}

// Initialize git repository
void init_repo(const char* repo_path) {
    struct stat st = {0};
    
    // Create repo directory if it doesn't exist
    if (stat(repo_path, &st) == -1) {
        printf("Creating repository directory: %s\n", repo_path);
        mkdir(repo_path, 0755);
    }
    
    // Initialize git repository if it doesn't exist
    char git_dir[1024];
    snprintf(git_dir, sizeof(git_dir), "%s/.git", repo_path);
    
    if (stat(git_dir, &st) == -1) {
        printf("Initializing git repository...\n");
        run_command(repo_path, "git init");
        run_command(repo_path, "git config user.email \"%s\"", GIT_EMAIL);
        run_command(repo_path, "git config user.name \"%s\"", GIT_NAME);
        
        // Create README if it doesn't exist
        char readme_path[1024];
        snprintf(readme_path, sizeof(readme_path), "%s/README.md", repo_path);
        if (stat(readme_path, &st) == -1) {
            FILE* f = fopen(readme_path, "w");
            if (f) {
                fprintf(f, "# Daily Commit Bot\n\n");
                fprintf(f, "This repository is automatically updated by the Daily Commit Bot.\n");
                fclose(f);
                run_command(repo_path, "git add README.md");
                run_command(repo_path, "git commit -m \"Initial commit\"");
            }
        }
    }
}

// Create a new commit with a random quote
void create_commit(const char* repo_path) {
    // Pull latest changes
    printf("Pulling latest changes...\n");
    run_command(repo_path, "git pull origin main");
    
    // Generate a random quote
    srand(time(NULL));
    const char* quote = quotes[rand() % num_quotes];
    
    // Create a new file with the quote
    char filename[128];
    time_t now = time(NULL);
    struct tm* timeinfo = localtime(&now);
    strftime(filename, sizeof(filename), "quote_%Y-%m-%d_%H-%M-%S.txt", timeinfo);
    
    char filepath[1024];
    snprintf(filepath, sizeof(filepath), "%s/%s", repo_path, filename);
    
    printf("Creating new quote file: %s\n", filename);
    FILE* f = fopen(filepath, "w");
    if (f) {
        fprintf(f, "%s\n", quote);
        fclose(f);
        
        // Add and commit the file
        run_command(repo_path, "git add %s", filename);
        
        char commit_msg[1024];
        snprintf(commit_msg, sizeof(commit_msg), "Add new quote: %.*s...", 30, quote);
        run_command(repo_path, "git commit -m \"%s\"", commit_msg);
        
        // Push changes
        printf("Pushing changes...\n");
        run_command(repo_path, "git push origin main");
    } else {
        perror("Failed to create quote file");
    }
}

int main() {
    // Set up signal handlers
    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);
    
    // Get home directory
    const char* home = getenv("HOME");
    if (!home) {
        home = getpwuid(getuid())->pw_dir;
    }
    
    // Set up repository path
    char repo_path[1024];
    snprintf(repo_path, sizeof(repo_path), "%s/%s", home, REPO_DIR);
    
    // Initialize repository
    init_repo(repo_path);
    
    printf("Starting AutoCommitBot...\n");
    printf("Repository: %s\n", repo_path);
    printf("Commit interval: %d seconds\n", COMMIT_INTERVAL);
    printf("Press Ctrl+C to stop\n\n");
    
    // Main loop
    while (running) {
        time_t now = time(NULL);
        char time_str[64];
        strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", localtime(&now));
        printf("[%s] Checking for changes...\n", time_str);
        
        create_commit(repo_path);
        
        printf("Waiting %d seconds until next commit...\n\n", COMMIT_INTERVAL);
        
        // Sleep in smaller chunks to be responsive to signals
        for (int i = 0; i < COMMIT_INTERVAL && running; i++) {
            sleep(1);
        }
    }
    
    printf("\nShutting down AutoCommitBot...\n");
    return 0;
}
