# Linux Netfilter Hook Kernel Module

## Overview

This project implements a **Linux kernel module** that hooks into the **Netfilter framework** to inspect and optionally manipulate **incoming and outgoing IPv4 packets** at the kernel level.

The module registers **Netfilter hooks** for both inbound and outbound traffic, allowing deep packet inspection of protocols such as **ICMP** and **TCP** directly inside the Linux networking stack.

It is intended for **learning, research, and security experimentation**, including firewall logic, traffic monitoring, and protocol analysis.

---

## Features

* 📡 Hooks **incoming and outgoing IPv4 traffic**
* 🧠 Parses **IP, ICMP, and TCP headers**
* ⚙️ Uses Linux **Netfilter kernel API**
* 🔍 Demonstrates packet inspection at kernel level
* 🧩 Clean module load and unload lifecycle
* 📜 GPL-licensed kernel module

---

## Technical Summary

### Netfilter Integration

The module registers two Netfilter hooks:

| Hook                   | Direction | Purpose                             |
| ---------------------- | --------- | ----------------------------------- |
| `NF_INET_PRE_ROUTING`  | Incoming  | Inspect packets entering the system |
| `NF_INET_POST_ROUTING` | Outgoing  | Inspect packets leaving the system  |

These hooks receive a pointer to `struct sk_buff`, allowing full access to packet data.

---

### Protocols Handled

The code demonstrates parsing of:

* **IPv4 header** (`struct iphdr`)
* **ICMP header** (`struct icmphdr`)
* **TCP header** (`struct tcphdr`)

Helper macros are defined to simplify header extraction and checksum validation.

---

### Packet Processing Flow

1. Netfilter invokes the hook function
2. Packet headers are extracted from `sk_buff`
3. Protocol type is identified (ICMP / TCP)
4. Packet is logged or analyzed
5. Packet verdict is returned:

   * `NF_ACCEPT` → allow packet
   * (Can be extended to `NF_DROP`, `NF_STOLEN`, etc.)

---

## File Structure

```
.
├── hook.c          # Netfilter kernel module source
└── README.md       # Project documentation
```

---

## Build Instructions

### Requirements

* Linux kernel headers
* GCC compatible with your kernel
* Root privileges

### Example Makefile

```makefile
obj-m += hook.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
```

### Compile

```bash
make
```

---

## Usage

### Load the Module

```bash
sudo insmod hook.ko
```

### Verify It Is Loaded

```bash
lsmod | grep hook
```

### View Kernel Logs

```bash
dmesg | tail
```

### Unload the Module

```bash
sudo rmmod hook
```

---

## Safety & Warnings ⚠️

* This module runs in **kernel space**
* A bug can **crash the system**
* Do **not** use on production systems
* Always test in:

  * Virtual machines
  * Isolated lab environments

---

## Extending This Module

You can expand this code to:

* Drop packets (`NF_DROP`)
* Implement a basic firewall
* Detect scanning or flooding behavior
* Log packet metadata to user space
* Add connection tracking logic
* Integrate with Netlink or eBPF userland tools

---

## License

```
GPL (GNU General Public License)
```

---

## Author

**Yusdiel Rodriguez Amoros**
