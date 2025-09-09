# practicelog.sh

## Description

`practicelog.sh` is a Bash script for Linux that collects system information and archives it for review and troubleshooting. It is designed for learning and practical use.

## Features

- **Time Information:** Captures local time, RTC time, universal time, and timezone.
- **OS Details:** Logs Linux kernel version and distribution release 
- **Device Info:** Records CPU model, RAM size, number of running processes, and disk space usage on `/` (depth=1).
- **Network Info:** Collects Ethernet and WiFi interface details.
- **Separate Log Files:** Each data point is saved in its own `.log` file.
- **Archiving:** Compresses all log files into a single `.tar.gz` archive.
- **Invocation:** Can be run from anywhere on the system.
- **Scheduled Execution:** Supports automatic hourly runs via cron.

## Installation

1. **Copy the Script:**  
   Place `practicelog.sh` in desired directory.

2. **Make Executable:**  
   ```bash
   chmod +x /path/to/practicelog.sh
   ```

3. **Run Manually (with sudo):**  
   ```bash
   sudo /path/to/practicelog.sh
   ```

4. **Invoke from Anywhere:**  
   Add an alias to shell configuration file:  
   ```bash
   alias practicelog='/path/to/practicelog.sh'
   ```

5. **Schedule Automatic Runs:**  
   Add to crontab to run every hour:  
   ```bash
   sudo crontab -e
   # Add the following line:
   @hourly /path/to/practicelog.sh
   ```

## Output

- Log files are created in `/home/practicelog/` for each data point.
- All logs are compressed into `/home/log_storage/log<timestamp>.tar.gz`.
- The `/home/practicelog/` directory is deleted after archiving.


## Notes

- Script must be run with `sudo` for proper permissions.
- Tested on Ubuntu; may require adjustments for other distributions.