# AutoCommitBot

A simple bot that automatically commits inspirational quotes to a Git repository at regular intervals.

## Features

- Automatically commits a new quote every 5 minutes
- Pushes changes to a remote repository
- Runs as a systemd service
- Simple configuration

## Requirements

- Git
- C compiler (gcc or clang)
- systemd (for running as a service)

## Installation

1. Clone the repository:
   ```bash
   git clone https://github.com/geminiprince/autocommitbot.git
   cd autocommitbot
   ```

2. Build the application:
   ```bash
   make
   ```

3. Install (run with sudo if needed):
   ```bash
   sudo make install
   ```

## Configuration

The bot is pre-configured to use the following settings:
- Repository: `~/dailycommit`
- Git user: `geminiprince`
- Git email: `geminiprinceee@gmail.com`
- Commit interval: 5 minutes

To change these settings, edit the `src/autocommitbot.c` file and rebuild.

## Usage

### Start the service
```bash
systemctl --user start autocommit.service
```

### Stop the service
```bash
systemctl --user stop autocommit.service
```

### Enable auto-start on boot
```bash
systemctl --user enable autocommit.service
```

### View logs
```bash
journalctl --user -u autocommit.service -f
```

## Uninstallation

To remove the bot:

```bash
sudo make uninstall
```

## License

MIT

The configuration is stored in `~/.config/autocommitbot/repos.conf` with the following format:

```
# name|path|branch|remote
myproject|/home/user/projects/myproject|main|origin
another_repo|/path/to/repo|develop|upstream
```

## Example Commit Message

```
auto: 2023-09-05 10:30:00

"The only way to do great work is to love what you do." - Steve Jobs
```

## Requirements

- Linux system with systemd
- Git
- GCC (for compilation)
- Basic understanding of Git repositories

## Troubleshooting

- If the bot doesn't start, check the service logs:
  ```bash
  journalctl --user -u autocommit.service -f
  ```

- Make sure the repositories you add exist and are Git repositories
- Ensure the user running the service has write access to the repositories
