  .text
  .globl ret_exit

# Function to save the return value of a process and exit
ret_exit:
  push %eax
  call exitFunction
