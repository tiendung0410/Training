#!/bin/bash

# Debuglog permission
if [ "$(id | awk -F'[()]' '{print $2}')" != "root" ]; then
    echo "Please run the tool with sudo permission!"
	exit
fi

debug_dir=/home/practicelog
log_storage_dir=/home/log_storage
if [ ! -d "$debug_dir" ]; then
	sudo mkdir $debug_dir
fi

if [ ! -d "$log_storage_dir" ]; then
	sudo mkdir $log_storage_dir
fi

main_log=$debug_dir/main.log

getTime()
{
	echo "---------------------------TIME-----------------------------"
	time_file=$debug_dir/time_log.log

	timedatectl > $time_file

	count=1
	while read LINE; do
	if [ $count -gt 4 ]; then 
		break
	fi
	((count++))
	echo $LINE
	done < $time_file
}

getOsInfo()
{
	echo "--------------------------OS Info---------------------------"
	osInfo_file=$debug_dir/osInfo.log
	lsb_release -a > $osInfo_file 2>/dev/null
	while read LINE; do
		echo $LINE
	done < $osInfo_file
}

getDeviceInfo()
{
	echo "------------------------Device Info--------------------------"
	#get cpu info
	cpuInfo_file=$debug_dir/cpuInfo.log
	cat /proc/cpuinfo > $cpuInfo_file
	echo $(cat $cpuInfo_file | grep "model name" | head -1) 

	#get RAM info
	memInfo_file=$debug_dir/memInfo.log
	cat /proc/meminfo > $memInfo_file
	echo $(cat $memInfo_file | grep "MemTotal")

	#get numer of processes
	process_file=$debug_dir/process.log
	ps aux --no-heading > $process_file
	echo "Number of running processes: $(cat $process_file | wc -l)"

	#get space disk on
	diskSpace_file=$debug_dir/diskSpace.log
	du -h --max-depth=1 / > $diskSpace_file 2>/dev/null
	echo "Disk Space on root(depth=1):"
	while read LINE; do
		echo $LINE
	done < $diskSpace_file
}

getNetworkInfo()
{
	echo "------------------------Network Info--------------------------"
	networkInfo_file=$debug_dir/network.log
	nmcli device show > $networkInfo_file
	grep -v '^IP6\.' "$networkInfo_file" | grep -v -- '--'
}

#Call function
echo "" > $main_log
getTime >> $main_log
getOsInfo >> $main_log
getDeviceInfo >> $main_log
getNetworkInfo >> $main_log
cat $main_log	

#Compress log files
current_date_time="`date +%Y%m%d%H%M%S`";
sudo tar -czf $log_storage_dir/log$current_date_time.tar.gz $debug_dir > /dev/null 2>&1
sudo rm -rf $debug_dir

#command to make script can be called from anywhere
#alias practicelog='$(pwd)/practicelog.sh'
