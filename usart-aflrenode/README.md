### Dependencies:
- sudo apt-get install xterm

- Renode (https://github.com/renode/renode)

### Execution:
```bash
./start.sh
```

### To display the transferred data via USART, enter the following command in a new terminal window, the script will also write all data to the output.log file
```bash
./showUsart.sh
```
### Additional Information:
- The LOG folder contains the results of fuzzing sessions, as well as any defects found

- The filter folder contains a tool for sorting output.log by Embox sessions (from crash to crash)

- embox - a regular binary with the configuration stm32f411_blackpill (419c7f96f3ba2f8d33b04447c4877edf7377a4a4)
  embox_update - updated binary (some timers and download log are disabled)
