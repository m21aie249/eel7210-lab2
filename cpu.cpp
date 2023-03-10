/*

Code Function: Main Implementation of CPU for SystemC RISCV Project

EL7210 Course: Hardware Software Co-Design: Lab2, SystemC_RISCV
Department of Electrical Engineering
Indian Institute of Technology, Jodhpur

Original code forked from repo: https://github.com/binodkumar23/IITJ_IDDEDA/tree/main/SystemC_RISCV
Original Code Authors: Vipul Sharma(B19CSE099) and Darsh Patel(B19CSE115)
Mentor: Dr. Binod Kumar

Modified code repo: https://github.com/m21aie249/eel7210-lab2
Modified by: Rohit Mathur (M21AIE249)
Mentor: Dr. Binod Kumar
Modifications: Add 5 new instructions, cleaned up code, added some comments

Added code marked with "M21AIE249"

References:
RISC-V Instructions: https://github.com/jameslzhu/riscv-card/blob/master/riscv-card.pdf
RISC-V Instruction Encoder/Interpreter, LupLab @ University of California, Davis: https://luplab.gitlab.io/rvcodecjs/

For simulating this module, follow the instructions in Readme.md

*/

#include "systemc.h"

SC_MODULE (cpu) {
  sc_in_clk clock;      // Clock input of the design
  sc_out<bool> cpu_out;

  sc_in <sc_bv<32>> instruction;
  sc_inout <int> Program_Counter;

  sc_in <sc_bv<32*32>> instruction_memory;
  sc_inout <sc_bv<32*32>> data_memory;
  
  //------------Local Variables Here---------------------
  sc_uint<4>	count;

  // 32 CPU registers
  sc_int<32> register_memory[32];

    /*
    R-type: register-register
    I-type: short immediates and loads
    S-type: stores
    B-type: conditional branches, a variation of S-type
    U-type: long immediates
    J-type: unconditional jumps, a variation of U-type
    */

// M21AIE249: Corrected a few entries in original code:

  sc_bv<7>  R_type_opcode = "0110011";
  sc_bv<7>  I_type_opcode = "0000011";
  sc_bv<7>  S_type_opcode = "0100011";
  sc_bv<7>  B_type_opcode = "1100011";
  sc_bv<7>  J_type_opcode = "1101111";

  // R_type grp1 
  sc_bv<7>  R_type_add_func7 = "0000000";
  sc_bv<3>  R_type_add_func3 = "000";
    
  // R_type grp2
  sc_bv<7>  R_type_and_func7 = "0000000";
  sc_bv<7>  R_type_or_func7 = "0000000";
  sc_bv<7>  R_type_xor_func7 = "0000000";
  
  sc_bv<3>  R_type_and_func3 = "111";
  sc_bv<3>  R_type_or_func3 = "110";
  sc_bv<3>  R_type_xor_func3 = "100";
  
  // M21AIE249: Added sll and srl instructions  
  sc_bv<7>  R_type_sll_func7 = "0000000";
  sc_bv<7>  R_type_srl_func7 = "0000000";

  sc_bv<3>  R_type_sll_func3 = "001";
  sc_bv<3>  R_type_srl_func3 = "101";

  // R_type grp4
  sc_bv<7>  R_type_sub_func7 = "0100000";
  sc_bv<3>  R_type_sub_func3 = "000";

  // B type functs
  sc_bv<3>  B_type_beq_func3 = "000";
  sc_bv<3>  B_type_bne_func3 = "001";
  
  // M21AIE249: Added blt, bge instructions
  sc_bv<3>  B_type_blt_func3 = "100";
  sc_bv<3>  B_type_bge_func3 = "101";
  
  // M21AIE249 added lw and lb instructions
  sc_bv<3>  I_type_lw_func3 = "010";
  sc_bv<3>  I_type_lb_func3 = "000";
  
  //------------Code Starts Here-------------------------
  // Below function implements actual cpu logic
  
  void select_line () {

    // At every rising edge of clock we read the instruction memory
    // from the location pointed by Program Counter
    sc_bv<32> temp = instruction_memory.read()( (32*32) -1 - Program_Counter ,  (32*32) -32 - Program_Counter );

    cout<<"\nProgram Counter :"<<Program_Counter<<"\n";
    Program_Counter.write(Program_Counter+4*8);
    
    cout<<"Current Instruction "<<temp<<"\n";
    cout<<"Data Memory Location: "<<(data_memory.read()( (32*32) -1 - (0)*32 ,  (32*32) -32 - (0 )*32 ) )<<"\n";

	//R-Type
    if(temp.range(6,0)==R_type_opcode) {
	// Add
      if(temp.range(31,25)==R_type_add_func7 && temp.range(14,12)==R_type_add_func3){
        cout<<"Add ";
        sc_int<32> loc_operand1  = (temp.range(24,20)).to_int();
        sc_int<32> loc_operand2  = (temp.range(19,15)).to_int();
        sc_int<32> loc_dest  = (temp.range(11,7)).to_int();

        register_memory[loc_dest] = register_memory[loc_operand1] + register_memory[loc_operand2];
        
        cout<<register_memory[loc_dest]<<" "<<register_memory[loc_operand1]<<" "<<register_memory[loc_operand2]<<"\n";
        
        cout<<"@" << sc_time_stamp() <<"\n";
      }
      // Subtract
      else if(temp.range(31,25)==R_type_sub_func7 && temp.range(14,12)==R_type_sub_func3){
      cout<<"Sub ";
      sc_int<32> loc_operand1  = (temp.range(24,20)).to_int();
      sc_int<32> loc_operand2  = (temp.range(19,15)).to_int();
      sc_int<32> loc_dest  = (temp.range(11,7)).to_int();

      register_memory[loc_dest] = register_memory[loc_operand1] - register_memory[loc_operand2];
      
      cout<<register_memory[loc_dest]<<" "<<register_memory[loc_operand1]<<" "<<register_memory[loc_operand2]<<"\n";
      
      cout<<"@" << sc_time_stamp() <<"\n";
      }
	// AND
      else if(temp.range(31,25)==R_type_and_func7 && temp.range(14,12)==R_type_and_func3){
        cout<<"AND ";
        sc_int<32> loc_operand1  = (temp.range(24,20)).to_int();
        sc_int<32> loc_operand2  = (temp.range(19,15)).to_int();
        sc_int<32> loc_dest  = (temp.range(11,7)).to_int();

        register_memory[loc_dest] = register_memory[loc_operand1] & register_memory[loc_operand2];
        
        cout<<register_memory[loc_dest]<<" "<<register_memory[loc_operand1]<<" "<<register_memory[loc_operand2]<<"\n";
        
        cout<<"@" << sc_time_stamp() <<"\n";
      }
	// XOR
      else if(temp.range(31,25)==R_type_xor_func7 && temp.range(14,12)==R_type_xor_func3){
        cout<<"XOR ";
        sc_int<32> loc_operand1  = (temp.range(24,20)).to_int();
        sc_int<32> loc_operand2  = (temp.range(19,15)).to_int();
        sc_int<32> loc_dest  = (temp.range(11,7)).to_int();

        register_memory[loc_dest] = register_memory[loc_operand1] ^ register_memory[loc_operand2];
        
        cout<<register_memory[loc_dest]<<" "<<register_memory[loc_operand1]<<" "<<register_memory[loc_operand2]<<"\n";
        
        cout<<"@" << sc_time_stamp() <<"\n";
      }
      // OR
      else if(temp.range(31,25)==R_type_or_func7 && temp.range(14,12)==R_type_or_func3){
        cout<<"OR ";
        sc_int<32> loc_operand1  = (temp.range(24,20)).to_int();
        sc_int<32> loc_operand2  = (temp.range(19,15)).to_int();
        sc_int<32> loc_dest  = (temp.range(11,7)).to_int();

        register_memory[loc_dest] = register_memory[loc_operand1] | register_memory[loc_operand2];
        
        cout<<register_memory[loc_dest]<<" "<<register_memory[loc_operand1]<<" "<<register_memory[loc_operand2]<<"\n";
        
        cout<<"@" << sc_time_stamp() <<"\n";
      }
	// M21AIE249: Handling sll
      else if(temp.range(31,25)==R_type_sll_func7 && temp.range(14,12)==R_type_sll_func3){
        cout<<"SLL ";
        sc_int<32> loc_operand1  = (temp.range(24,20)).to_int();
        sc_int<32> loc_operand2  = (temp.range(19,15)).to_int();
        sc_int<32> loc_dest  = (temp.range(11,7)).to_int();

        register_memory[loc_dest] = register_memory[loc_operand1] << register_memory[loc_operand2];
        
        cout<<register_memory[loc_dest]<<" "<<register_memory[loc_operand1]<<" "<<register_memory[loc_operand2]<<"\n";
        
        cout<<"@" << sc_time_stamp() <<"\n";
      }
	// M21AIE249: Handling srl 
      else if(temp.range(31,25)==R_type_srl_func7 && temp.range(14,12)==R_type_srl_func3){
        cout<<"SRL ";
        sc_int<32> loc_operand1  = (temp.range(24,20)).to_int();
        sc_int<32> loc_operand2  = (temp.range(19,15)).to_int();
        sc_int<32> loc_dest  = (temp.range(11,7)).to_int();

        register_memory[loc_dest] = register_memory[loc_operand1] >> register_memory[loc_operand2];
        
        cout<<register_memory[loc_dest]<<" "<<register_memory[loc_operand1]<<" "<<register_memory[loc_operand2]<<"\n";
        
        cout<<"@" << sc_time_stamp() <<"\n";
      }
    }
     // I - type       
    else if(temp.range(6,0)==I_type_opcode){
    // Implementing load word
        cout<<"Load ";
        sc_int<32> offset   = (temp.range(31,20)).to_int();
        sc_int<32> loc_operand  = (temp.range(19,15)).to_int();
        sc_int<32> funct3  = (temp.range(14,12)).to_int();
        sc_int<32> loc_dest  = (temp.range(11,7)).to_int();
        
        if(funct3==I_type_lw_func3.to_int()){
		cout<<"Register at "<< loc_dest<<" value "<<register_memory[loc_dest]<<"\n";
		register_memory[loc_dest] = (data_memory.read()( (32*32) -1 - (register_memory[loc_operand]+offset)*32 ,  (32*32) -32 - (register_memory[loc_operand]+offset )*32 ) ).to_int();
		    
		cout<<(data_memory.read()( (32*32) -1 - (register_memory[loc_operand]+offset)*32 ,  (32*32) -32 - (register_memory[loc_operand]+offset )*32 ) )<<"\n";
		
		cout<<"Register at "<< loc_dest<<" value "<<register_memory[loc_dest]<<"\n";
	        
	        cout<<"@" << sc_time_stamp() <<"\n";
	}
	else if(funct3==I_type_lb_func3.to_int()){
		cout<<"Register at "<< loc_dest<<" value "<<register_memory[loc_dest]<<"\n";
		register_memory[loc_dest] = (data_memory.read()( (32*32) -1 - (register_memory[loc_operand]+offset)*32 ,  (32*32) -16 - (register_memory[loc_operand]+offset )*32 ) ).to_int();    
		
		cout<<(data_memory.read()( (32*32) -1 - (register_memory[loc_operand]+offset)*32 ,  (32*32) -16 - (register_memory[loc_operand]+offset )*32 ) )<<"\n";
		
		cout<<"Register at "<< loc_dest<<" value "<<register_memory[loc_dest]<<"\n";
	        
	        cout<<"@" << sc_time_stamp() <<"\n";
	}        
    }
    else if(temp.range(6,0)==S_type_opcode){
       // Implementing store word      
        cout<<"Store ";
        sc_bv<12> offset_bits;
        offset_bits.range(11,5) = temp.range(31,25);
        offset_bits.range(4,0) = temp.range(11,7);
        sc_int<32> offset   = (offset_bits).to_int();
        
        sc_int<32> loc_base  = (temp.range(19,15)).to_int();
        sc_int<32> funct3  = (temp.range(14,12)).to_int();
        sc_int<32> loc_src  = (temp.range(24,20)).to_int();

        sc_bv<32*32> temp = data_memory;
        
        temp.range( (32*32) -1 - (register_memory[loc_base]+offset)*32 ,  (32*32) -32 - (register_memory[loc_base]+offset )*32 ) = register_memory[loc_src];
        cout<<"Storing "<<temp.range( (32*32) -1 - (register_memory[loc_base]+offset)*32 ,  (32*32) -32 - (register_memory[loc_base]+offset )*32 )<<"\n";
        data_memory.write(temp.range((32*32) -1, 0));
        
        cout<<"@" << sc_time_stamp() <<"\n";
    }
    // Branch
    else if(temp.range(6,0)==B_type_opcode){
      cout<<"Branch ";
      sc_bv<13> offset_bits;
      offset_bits.range(11,11) = temp.range(7,7);
      offset_bits.range(4,1) = temp.range(11,8);
      offset_bits.range(10,5) = temp.range(30,25);
      offset_bits.range(12,12) = temp.range(31,31);
      offset_bits.range(0,0) = "0";

      sc_int<32> funct3  = (temp.range(14,12)).to_int();
      sc_int<32> loc_operand1  = (temp.range(24,20)).to_int();
      sc_int<32> loc_operand2  = (temp.range(19,15)).to_int();
      cout<<register_memory[loc_operand1]<<" "<<register_memory[loc_operand2]<<"\n";
      // Branch if not equal
      if(funct3==B_type_bne_func3.to_int()){
        cout<<"BNE ";
        if(register_memory[loc_operand1]!=register_memory[loc_operand2] ){
          Program_Counter.write(Program_Counter + 2*32*offset_bits.to_int());
        }
      }
      // Branch if equal        
      else if(funct3==B_type_beq_func3.to_int()){
        cout<<"BEQ ";
        if(register_memory[loc_operand1]==register_memory[loc_operand2] ){
          Program_Counter.write(Program_Counter + 2*32*offset_bits.to_int());
          }
      }
	// M21AIE249: Handling blt  
      else if(funct3==B_type_blt_func3.to_int()){
        cout<<"BLT ";
        if(register_memory[loc_operand1]<register_memory[loc_operand2] ){
          Program_Counter.write(Program_Counter + 2*32*offset_bits.to_int());
        }
      }
	// M21AIE249: Handling bge
      else if(funct3==B_type_bge_func3.to_int()){
        cout<<"BGE ";
        if(register_memory[loc_operand1]>=register_memory[loc_operand2] ){
          Program_Counter.write(Program_Counter + 2*32*offset_bits.to_int());
        }
      }
      cout<<"@" << sc_time_stamp() <<"\n";
    }
    else if(temp.range(6,0)==J_type_opcode){
      cout<<"Unconditional Branch\n";
      sc_bv<21> offset_bits;
      offset_bits.range(0,0) = "0";
      offset_bits.range(10,1) = temp.range(30,21);
      offset_bits.range(11,11) = temp.range(20,20);
      offset_bits.range(20,20) = temp.range(31,31);
      offset_bits.range(19,12) = temp.range(19,12);
      cout<<Program_Counter + 2*32*offset_bits.to_int()<<"\n";
      Program_Counter.write(Program_Counter + 2*32*offset_bits.to_int());
      cout<<"@" << sc_time_stamp() <<"\n";
    }
    else {
      cout<<"@" << sc_time_stamp()<<" "<<instruction <<" Null  \n";
    }    
  } // End of function select_line()

  // Constructor for the counter
  // Since this counter is a positive edge trigged one,
  // We trigger the below block with respect to positive
  // edge of the clock and also when ever reset changes state
  
  SC_CTOR(cpu) {
  // Load some registers with some values
    register_memory[0] = 11;
    register_memory[1] = 11;
    register_memory[2] = 0;
    register_memory[3] = 3;
    register_memory[5] = 1;

    cout<<"\nExecuting...."<<endl;
    SC_METHOD(select_line);
    sensitive << clock.pos();
    //sensitive << reset; M21AIE249: Why should this be commented out?
  } // End of Constructor
}; // End of Module
