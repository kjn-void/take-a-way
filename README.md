# Take-a-way proof-of-concept 

The PoC shows that it is possible to leak information between two separate address-spaces. 

Victim process goes through the input-string, one character at a time. The character is use in an offset calculation into an array in a way so different characters map to different cache-ways.

The attacker probe the latency against the cache-ways to figure out which character the victim is currently working with.

# Requirements

Linux-based operating system, only tested on Ubuntu 18.04 and AMD 3900X.

The victim and attacker processes must run on separate threads on the same physical core. The `run.sh` script will by default use logical core #0 and logical core #N, where `N` is equal to the number of physical CPU-cores.