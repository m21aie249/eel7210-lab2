Project name: Simple RISCV CPU design in SystemC
===========================================================================================================================

EL7210 Course: Hardware Software Co-Design: Lab2, SystemC_RISCV
Department of Electrical Engineering
Indian Institute of Technology, Jodhpur

Original code forked from repo: https://github.com/binodkumar23/IITJ_IDDEDA/tree/main/SystemC_RISCV
Original Code Authors: Vipul Sharma(B19CSE099) and Darsh Patel(B19CSE115)
Mentor: Dr. Binod Kumar (binod@iitj.ac.in)

Modified code repo: https://github.com/m21aie249/eel7210-lab2
Modified by: Rohit Mathur (M21AIE249)
Mentor: Dr. Binod Kumar (binod@iitj.ac.in)
Modifications: Add 5 new instructions ,cleaned up code, added some comments

Added code marked with "M21AIE249"

To run: 

(0) Ensure that SystemC is installed at path, /usr/local/systemc-2.3.2/

(1) Get cpu.cpp and cpu_testbench.cpp to the same path

(2) At that path run the following commands, 
    (a) export SYSTEMC_HOME=/usr/local/systemc-2.3.2/
    
    (b) g++ -I. -I$SYSTEMC_HOME/include -L. -L$SYSTEMC_HOME/lib-linux64 -Wl,-rpath=$SYSTEMC_HOME/lib-linux64 -o cpu_testbench cpu_testbench.cpp -lsystemc -lm
    
    (c) ./cpu_testbench

To make changes:

(1) Refer the opcode, and funct values used in cpu.cpp ( They can be changed as well, as long as they follow the standards)
(2) Edit the instruction in instruction memory accordingly, in the set of 32 bits

===================================================================================
