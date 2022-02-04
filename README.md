# Shaihulud: A LD Preload rootkit for linux

## 1. Goals
		a. Hide files
		b. Hide processes
		c. Remote backdoor
		d. SSH Worm
## 2. Progress
		a. Files with the keyword "shaihulud" get hidden
		b. Not yet attempted
		c. The accept and write backdoors work
		d. Need to find a more stable way to get this working, but as a proof of concept it is somewhat usable

## 3. Usage
		After backdooring, the backdoors can be accessed via two methods.
		Method 1: accept hook
			Compares source port in order to start reverse shell.
		 	nc ip port -p source_port
			This will trigger the reverse shell
		Method 2: write hook
			Uses syslog to trigger reverse shell. The trigger lies in the ssh username field, the username gets written which will activate the write hook and start the reverse shell
			ssh shaihulud@ip will start the reverse shell
		
