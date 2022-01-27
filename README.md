# Shaihulud: A LD Preload rootkit for linux

## 1. Goals 
		a. Hide files
		b. Hide processes
		c. Remote backdoor
		d. SSH Worm
## 2. Progress
		a. Files with the keyword "shaihulud" get hidden
		b. Not yet attempted
		c. Have tried accept() and write backdoor, but no luck yet. Currently the only remote back door that works is from a '''dirent''' hook.
		d. Need to find a more stable way to get this working, but as a proof of concept it is somewhat usable


