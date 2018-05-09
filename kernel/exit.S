  .text
  .globl exit

# Function to save the return value of a process and exit
exit:
  push %eax
  call exitFunction
